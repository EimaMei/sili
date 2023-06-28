# Sili Toolchain
![vm workflow linux](https://github.com/EimaMei/sili-toolchain/actions/workflows/linux.yml/badge.svg)
![vm workflow windows](https://github.com/EimaMei/sili-toolchain/actions/workflows/windows.yml/badge.svg)
![vm workflow macos](https://github.com/EimaMei/sili-toolchain/actions/workflows/macos.yml/badge.svg)

Sili Toolchain is a cross-platform software toolchain for modern C programming, providing many new features, improvements and fixes to the language's barebones STL. It is specifically designed for C99 and above but can also be used in C++ codebases.

# Main goals
## Modernize C
The toolchain aims to modernize C by implementing a proper STL that is more feature-rich, fast, safe and understandable. It serves as an alternative to the bloated C++ STL.
## Performance & Utility
Sili Toolchain strives to strike a balance between having exceptional performance and while providing more options to end-developers.
## Portability
`sili.h`  is a single-header library, allowing for easy integration into any project. Furthermore, Sili Toolchain is designed to be cross-platform, supporting all major desktop operating systems.

# Features
- Architecture/OS/Compiler/Standard indentification macros (`SI_CPU_X86`, `SI_SYSTEM_WINDOWS`, `SI_COMPILER_CLANG`, `SI_STANDARD_C99`, `SI_STANDARD_ANSI` etc).
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
Examples of most sili's features are utilized in the `examples` folder.

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
- Make thread priority values cross-platform.