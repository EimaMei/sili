# Sili
![vm workflow linux](https://github.com/EimaMei/sili-toolchain/actions/workflows/linux.yml/badge.svg)
![vm workflow windows](https://github.com/EimaMei/sili-toolchain/actions/workflows/windows.yml/badge.svg)
![vm workflow macos](https://github.com/EimaMei/sili-toolchain/actions/workflows/macos.yml/badge.svg)

Sili is a cross-platform STL for modern C programming, providing many new features, improvements and fixes to the language's barebones STL.

# Main goals
## Modernize C
The project aims to modernize C by implementing a proper STL that is more feature-rich, fast, safe and understandable. It also serves as an alternative to the bloated C++ STL.
## Performance & Flexibility
Sili strives to strike a balance between having exceptional performance and flexibility to provide more options to end-developers.
## Portability
`sili.h`  is a single-header library, allowing for easy integration into any project. Furthermore, sili is designed to be cross-platform, supporting any OS that has a C99 compiler.

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
- Easy to use benchmarking functions to check performance (`si_performance_loop`, `si_performance_exec_per_ms`).
- An allocator system in place, removing the need to do `free(<ptr>)` every so lines. Not only that, but it's 3x times faster than the traditional separate mallocing/freeing system.
- Bit manipulation funcionts (`si_num_count_bit`, `si_num_rotate_left`, `si_bytes_to_num`, `si_num_change_endian` etc).
- ... and more to come!

# Examples
Examples of most sili's features are utilized in the `examples` folder.
