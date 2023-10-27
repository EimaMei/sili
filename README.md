# Sili
![vm workflow linux](https://github.com/EimaMei/sili-toolchain/actions/workflows/linux.yml/badge.svg)
![vm workflow windows](https://github.com/EimaMei/sili-toolchain/actions/workflows/windows.yml/badge.svg)
![vm workflow macos](https://github.com/EimaMei/sili-toolchain/actions/workflows/macos.yml/badge.svg)

Sili is a cross-platform standard library for modern C99 programming, providing many new features, improvements and fixes to the language's barebone libraries, while still adhering to the main principles and apeals of C.

# Main goals
## Modernize C
The project aims to modernize C by implementing a proper standard library that is more feature-rich and understandable to read and write, while also containing a lot of QoL changes for readability and safety, all while not sacrificing the speed of execution. It also serves as an alternative to the (bloated) C++ STL.
## Performance & Flexibility
Sili strives to strike a balance between having exceptional performance and flexibility to provide more options for developers using the library.
## Portability
`sili.h`  is a single-header library, allowing for easy integration into any project. Furthermore, sili is designed to be cross-platform, supporting any OS that has a working C99 compiler.

# Features
- Architecture/OS/Compiler/Standard indentification macros (`SI_CPU_X86`, `SI_SYSTEM_WINDOWS`, `SI_COMPILER_CLANG`, `SI_STANDARD_C99`, `SI_STANDARD_ANSI` etc).
- Useful macros (`typeof()`, `SI_ASSERT`/`SI_STATIC_ASSERT`, `SI_BIT`, `foreach` etc).
- Robust string implementation (`siString`, fully compatible with `char*`).
- Better arrays (`siArray(<type>)`, compatible with other C arrays).
- Pair support like `std::pair` (`siPair(<type1>, <type2>)`).
- `char` specific functions (`si_charToLower`, `si_charIsSpace`, `si_hexDigitToInt` etc).
- Path related functions (`si_pathExists`, `si_pathCopy`, `si_pathGetFullname` etc).
- A cross-platform file IO functions (`si_fileOpen`, `si_fileReadContents`, `si_fileWriteLineAt` etc).
- `siOptional` type similar to `std::optional`.
- Cross-platform multi-thread support (`si_threadCreate`, `si_threadStart`, `si_threadJoin`).
- Easy to use benchmarking functions to check performance (`si_benchmarkRunsPerLoop`, `si_benchmarkExecutesPerMs`, `si_benchmarkLoopsAvg` etc).
- An allocator system, making allocations 3x faster than the traditional `malloc`/`free`.
- A general-use hash table.
- Unicode functions (`si_utf8Decode`, `si_utf8ToUtf16String`, `si_utf16ToUtf8String`).
- Bit manipulation funcionts (`si_numCountBitsU32`, `si_numRotateLeft`, `si_bytesToNumArr`, `SI_NUM_BIT_SET` etc).
- Custom `printf` implementation, which introduces some new and C23-only specifiers and QoL improvements. 
- ... and more to come!

# Examples
Examples of most (but definitely not all) sili's features are utilized in the `examples` folder.