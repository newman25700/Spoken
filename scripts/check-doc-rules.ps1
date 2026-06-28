<#
.SYNOPSIS
    Validate Markdown docs against the project authoring rules in _mydocs/002_rules.md.

.DESCRIPTION
    Enforces the conventions defined for this repo's documentation:
      R1  Append-only: existing tracked content must not be modified or deleted
          (the committed HEAD version must remain an exact prefix of the file).
          Checked only with -CheckAppendOnly (requires git).
      R3  Max line length is 120 characters.
      R4  Markdown tables are aligned with spaces, never tabs (no tab anywhere).
      R5  Table edges/columns use the '|---|' style: every table block has border
          rows (cells made of dashes) and all rows share identical '|' positions.

    Rules R2 (add new files, don't edit existing) and R8 (blank separator rows) are
    authoring intentions; R2 is partly covered by -CheckAppendOnly, R8 is advisory.

.PARAMETER Path
    One or more files or directories to scan. Directories are searched recursively
    for *.md files. Defaults to the 'docs' folder next to the repo root.

.PARAMETER MaxLineLength
    Maximum allowed characters per line. Default 120.

.PARAMETER CheckAppendOnly
    Also verify each tracked file is append-only vs. its committed (HEAD) version.

.PARAMETER WarnAsError
    Treat warnings (e.g. missing table border) as failures too.

.EXAMPLE
    pwsh ./scripts/check-doc-rules.ps1
    pwsh ./scripts/check-doc-rules.ps1 -Path docs -CheckAppendOnly
    pwsh ./scripts/check-doc-rules.ps1 -Path docs/003_new.md -WarnAsError

.NOTES
    Exit code 0 = clean, 1 = violations found. Safe/read-only: never edits files.
#>
[CmdletBinding()]
param(
    [string[]] $Path = @("docs"),
    [int]      $MaxLineLength = 120,
    [switch]   $CheckAppendOnly,
    [switch]   $WarnAsError
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

# --- Finding model ----------------------------------------------------------
$script:Findings = New-Object System.Collections.Generic.List[object]

function Add-Finding {
    param(
        [string] $File,
        [int]    $Line,
        [ValidateSet("ERROR", "WARN")] [string] $Level,
        [string] $Rule,
        [string] $Message
    )
    $script:Findings.Add([pscustomobject]@{
        File    = $File
        Line    = $Line
        Level   = $Level
        Rule    = $Rule
        Message = $Message
    })
}

# --- File discovery ---------------------------------------------------------
function Get-TargetFiles {
    param([string[]] $Inputs)
    $files = New-Object System.Collections.Generic.List[string]
    foreach ($p in $Inputs) {
        if (Test-Path -LiteralPath $p -PathType Container) {
            Get-ChildItem -LiteralPath $p -Recurse -Filter *.md -File |
                ForEach-Object { $files.Add($_.FullName) }
        }
        elseif (Test-Path -LiteralPath $p -PathType Leaf) {
            $files.Add((Resolve-Path -LiteralPath $p).Path)
        }
        else {
            Write-Warning "Path not found, skipping: $p"
        }
    }
    return $files
}

# --- R3 / R4: line length and tabs -----------------------------------------
function Test-LinesAndTabs {
    param([string] $File, [string[]] $Lines)
    for ($i = 0; $i -lt $Lines.Count; $i++) {
        $lineNo = $i + 1
        $line   = $Lines[$i]

        if ($line.Contains([char]9)) {
            Add-Finding -File $File -Line $lineNo -Level ERROR -Rule "R4" `
                -Message "Tab character found; use spaces for alignment."
        }
        if ($line.Length -gt $MaxLineLength) {
            Add-Finding -File $File -Line $lineNo -Level ERROR -Rule "R3" `
                -Message ("Line is {0} chars (max {1})." -f $line.Length, $MaxLineLength)
        }
    }
}

# --- R5: markdown table alignment and borders ------------------------------
function Test-Tables {
    param([string] $File, [string[]] $Lines)

    # A table row is a (trimmed) line that begins and ends with '|' and has >=2 pipes.
    $isTableRow = {
        param($l)
        $t = $l.Trim()
        $pipeCount = $t.Length - $t.Replace('|', '').Length
        return ($t.StartsWith("|") -and $t.EndsWith("|") -and ($pipeCount -ge 2))
    }
    # A border row's cells are made only of dashes (e.g. |-----|-----|).
    $isBorderRow = {
        param($l)
        $t = $l.Trim()
        if (-not $t.StartsWith("|")) { return $false }
        $cells = $t.Trim('|') -split '\|'
        foreach ($c in $cells) {
            if ($c.Trim().Length -eq 0) { return $false }
            if ($c.Trim() -notmatch '^[-]+$') { return $false }
        }
        return $true
    }
    # Positions of '|' within a line (0-based), used to verify column alignment.
    $pipeIndexes = {
        param($l)
        $idx = New-Object System.Collections.Generic.List[int]
        for ($k = 0; $k -lt $l.Length; $k++) { if ($l[$k] -eq '|') { $idx.Add($k) } }
        return $idx.ToArray()
    }

    $i = 0
    while ($i -lt $Lines.Count) {
        if (-not (& $isTableRow $Lines[$i])) { $i++; continue }

        # Gather a contiguous block of table rows.
        $block = New-Object System.Collections.Generic.List[object]
        while ($i -lt $Lines.Count -and (& $isTableRow $Lines[$i])) {
            $block.Add([pscustomobject]@{ No = $i + 1; Text = $Lines[$i] })
            $i++
        }

        # Reference column layout = pipe positions of the first row in the block.
        $refPipes = @(& $pipeIndexes $block[0].Text)
        $hasBorder = $false
        foreach ($row in $block) {
            if (& $isBorderRow $row.Text) { $hasBorder = $true }
            $pipes = @(& $pipeIndexes $row.Text)
            $mismatch = $false
            if ($pipes.Count -ne $refPipes.Count) {
                $mismatch = $true
            } else {
                for ($p = 0; $p -lt $pipes.Count; $p++) {
                    if ($pipes[$p] -ne $refPipes[$p]) { $mismatch = $true; break }
                }
            }
            if ($mismatch) {
                Add-Finding -File $File -Line $row.No -Level ERROR -Rule "R5" `
                    -Message ("Table column '|' positions do not align with the table's first row (line {0})." -f $block[0].No)
            }
        }
        if (-not $hasBorder) {
            Add-Finding -File $File -Line $block[0].No -Level WARN -Rule "R5" `
                -Message "Table block has no '|---|' border row (dashes-only)."
        }
    }
}

# --- R1: append-only vs committed HEAD -------------------------------------
function Test-AppendOnly {
    param([string] $File)

    $git = Get-Command git -ErrorAction SilentlyContinue
    if (-not $git) {
        Write-Warning "git not found; skipping append-only check."
        return
    }

    # Path relative to the repo root, forward slashes for git.
    $repoRoot = (& git rev-parse --show-toplevel) 2>$null
    if (-not $repoRoot) { Write-Warning "Not a git repo; skipping append-only check."; return }
    $rel = (Resolve-Path -LiteralPath $File).Path.Substring($repoRoot.Length).TrimStart('\','/').Replace('\','/')

    # Is it tracked at HEAD? Untracked/new files are exempt (creating is allowed).
    & git cat-file -e "HEAD:$rel" 2>$null
    if ($LASTEXITCODE -ne 0) { return }

    $headText = (& git show "HEAD:$rel") -join "`n"
    $curText  = (Get-Content -LiteralPath $File -Raw)
    # Normalize line endings for the prefix comparison.
    $headNorm = $headText -replace "`r`n", "`n"
    $curNorm  = $curText  -replace "`r`n", "`n"

    if (-not $curNorm.StartsWith($headNorm)) {
        Add-Finding -File $File -Line 1 -Level ERROR -Rule "R1" `
            -Message "File is not append-only: committed content was modified or removed (HEAD is not a prefix)."
    }
}

# --- Main -------------------------------------------------------------------
$targets = @(Get-TargetFiles -Inputs $Path)
if ($targets.Count -eq 0) {
    Write-Host "No Markdown files found to check." -ForegroundColor Yellow
    exit 0
}

foreach ($file in $targets) {
    $lines = @(Get-Content -LiteralPath $file)
    if ($null -eq $lines) { $lines = @() }
    Test-LinesAndTabs -File $file -Lines $lines
    Test-Tables       -File $file -Lines $lines
    if ($CheckAppendOnly) { Test-AppendOnly -File $file }
}

# --- Report -----------------------------------------------------------------
$errors   = @($script:Findings | Where-Object Level -eq "ERROR")
$warnings = @($script:Findings | Where-Object Level -eq "WARN")

if ($script:Findings.Count -eq 0) {
    Write-Host ("OK  {0} file(s) checked, no issues." -f $targets.Count) -ForegroundColor Green
    exit 0
}

$script:Findings |
    Sort-Object File, Line |
    Group-Object File |
    ForEach-Object {
        Write-Host ""
        Write-Host $_.Name -ForegroundColor Cyan
        foreach ($f in ($_.Group | Sort-Object Line)) {
            $color = if ($f.Level -eq "ERROR") { "Red" } else { "Yellow" }
            Write-Host ("  {0,5}  {1,-5} [{2}] {3}" -f $f.Line, $f.Level, $f.Rule, $f.Message) -ForegroundColor $color
        }
    }

Write-Host ""
Write-Host ("Summary: {0} error(s), {1} warning(s) across {2} file(s)." -f $errors.Count, $warnings.Count, $targets.Count)

$fail = ($errors.Count -gt 0) -or ($WarnAsError -and $warnings.Count -gt 0)
exit ([int]$fail)
