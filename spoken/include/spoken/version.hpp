#pragma once                                  // process this header once per translation unit

#include <cstdint>                            // fixed-width integer types (std::uint32_t)

namespace spoken {                            // library root namespace
inline namespace v1 {                         // inline namespace: names appear as spoken::x, room for a v2 later

// Compile-time version components; 'inline' gives each one a single definition across all TUs.
inline constexpr std::uint32_t version_major = 1;   // major version number
inline constexpr std::uint32_t version_minor = 0;   // minor version number
inline constexpr std::uint32_t version_patch = 0;   // patch version number

inline constexpr const char* version_string = "1.0.0";  // human-readable version for help/banner

}  // namespace v1                            // close the inline namespace
}  // namespace spoken                        // close the root namespace
