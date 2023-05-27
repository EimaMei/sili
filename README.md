# sili.h STL
A public domain STL alternative to C/C++ standard libraries, featuring many improvements and fixes for better development. Made specifically for C99 and up, but can also be used in C++.

# Features
- Standard types (`u8`, `i32`, `u64` etc).
- Macros that define what architecture/OS/compiler/standard you are on (`SI_CPU_X86`, `SI_SYSTEM_WINDOWS`, `SI_COMPILER_CLANG`, `SI_STANDARD_C99`, `SI_STANDARD_ANSI` etc).
- Useful macros (`typeof()`, `SI_ASSERT`/`SI_STATIC_ASSERT`, `SI_BIT`, `foreach` etc).
- Robust string implementation (`siString`, fully compatible with `char*`).
- Better arrays (`siArray(<type>)`, compatible with other C arrays`).
- Pair support like `std::pair` (`siPair(<type1>, <type2>)`).
- `char` specific functions (`si_char_to_lower`, `si_char_is_space`, `si_hex_digit_to_int` etc).
- Path IO related functions (`si_path_exists`, `si_path_copy`, `si_path_get_fullname` etc).
- A cross-platform file management functions (`si_file_open`, `si_file_read`, `si_file_write_line_at` etc).
- `siOptional` type similar to `std::pair`.
- ... and more to come!

# Examples
Go to [example.c](example.c) to see examples for almost every feature in the STL. You can also unexpand the brackets to hide the examples you're not interested in viewing with your IDE of choice.

# License
`sili.h` is dual-licensed to the public domain and under the MIT license, meaning it can be used in any want you'd like to. Credit is appreciated but unneeded

# TODO
## Features
- !! Documentation (ideally everything should be in the header).
- Directory IO.
- `siTime`.
- Threading.
- `siVariant` from `std::variant`.
- Named operators.
- `siRandom`

# Improvements
- Improve si_array_make.
- Possible better debugging improvements.