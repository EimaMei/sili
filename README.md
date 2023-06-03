# Sili Toolchain
Sili Toolchain is a cross-platform public domain software toolchain specialized for C development, featuring many features, improvements and fixes to the language. Made specifically for C99 and up, but can also be used in C++.

# Main goals of Sili Toolchain
## Modernize C
Implement a proper STL for C that is much more feature-rich, safe, readable and just generally better, while also being an alternative to the C++ one.
## Be fast & useful
Keep a balance between having exceptional performance and giving more options to the end-user.
## Be portable
`sili.h` is a single-header library, meaning it can be drag 'n dropped to any project. Furthermore, Sili Toolchain is cross-platform, meaning it'll work on all major desktop OSses without issue.

# Features
- Standard types (`u8`, `i32`, `u64` etc).
- Macros that define what architecture/OS/compiler/standard you are on (`SI_CPU_X86`, `SI_SYSTEM_WINDOWS`, `SI_COMPILER_CLANG`, `SI_STANDARD_C99`, `SI_STANDARD_ANSI` etc).
- Useful macros (`typeof()`, `SI_ASSERT`/`SI_STATIC_ASSERT`, `SI_BIT`, `foreach` etc).
- Robust string implementation (`siString`, fully compatible with `char*`).
- Better arrays (`siArray(<type>)`, compatible with other C arrays`).
- Pair support like `std::pair` (`siPair(<type1>, <type2>)`).
- `char` specific functions (`si_char_to_lower`, `si_char_is_space`, `si_hex_digit_to_int` etc).
- Path related functions (`si_path_exists`, `si_path_copy`, `si_path_get_fullname` etc).
- A cross-platform file IO functions (`si_file_open`, `si_file_read`, `si_file_write_line_at` etc).
- `siOptional` type similar to `std::optional`.
- Cross-platform multi-thread support (`si_thread_create`, `si_thread_start`, `si_thread_join`).
- Easy to use benchmarking functions to check performance (`si_performance_loop`, `si_performance_exec_per_ms`), while logging how many allocations/frees were done when the `SI_MEMORY_LOGGING` macro is enabled.
- ... and more to come!

# Examples
Go to [example.c](example.c) to see examples for almost every feature in the toolchain. You can also unexpand the brackets to hide the examples you're not interested in viewing with your IDE of choice.

# License
`sili.h` is dual-licensed to the public domain and under the MIT license, meaning it can be used in any want you'd like to. Credit is appreciated but unneeded.

# TODO
## Features
- !! Documentation (ideally documentation should be all in the header).
- Directory IO.
- `siTime`.
- `siVariant` from `std::variant`.
- `siRandom`.
- `sort()` for siArray.
- `insert()`/`remove()` for siString/siArray.

# Improvements
- Improve si_array_make.
- Possible better debugging improvements.
- Make thread priority values cross-platform.