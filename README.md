# sili.h STL
A public domain STL alternative to C/C++ standard libraries, featuring many improvements and fixes for better development.

# Features
- Standard types (`u8`, `i32`, `u64` etc).
- Macros that define what architecture/OS/compiler/standard you are on (`SI_CPU_X86`, `SI_SYSTEM_WINDOWS`, `SI_COMPILER_CLANG`, `SI_STANDARD_C99`, `SI_STANDARD_ANSI` etc).
- Useful macros (`typeof()`, `SI_ASSERT`/`SI_STATIC_ASSERT`, `SI_BIT`, `foreach` etc).
- Robust string implementation (`siString`, fully compatible with `char*`).
- Better arrays (`siArray(<type>)`, compatible with other C arrays`).
- Pair support like `std::pair` on C++ except better (`siPair(<type1>, <type2>)`).
- `char` specific functions (`si_char_to_lower`, `si_char_is_space`, `si_hex_digit_to_int` etc).
- ... and more to come

# Examples
Go to [example.c](example.c) and enable/disable whatever example you'd like to. You can also unexpand the brackets to hide the examples you're not interested in viewing with your IDE of choice.

# License
`sili.h` is in the public domain, meaning you can do whatever you want with it and use it any shape of form you like.

# TODO
I'll fill this out when I am not lazy.