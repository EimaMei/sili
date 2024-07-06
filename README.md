# Sili
![VM workflow Linux](https://github.com/EimaMei/sili-toolchain/actions/workflows/linux.yml/badge.svg)
![VM workflow Windows](https://github.com/EimaMei/sili-toolchain/actions/workflows/windows.yml/badge.svg)
![VM workflow MacOS](https://github.com/EimaMei/sili-toolchain/actions/workflows/macos.yml/badge.svg)

Sili is a cross-platform standard library for modern C99 programming, providing
many new features, improvements and fixes to the language's standard libraries,
while still adhering to the main principles and appeals of C.

# Main goals
## Modernize C
The project aims to modernize C by implementing a proper standard library that is
more feature-rich and understandable to read and write, while also containing a
lot of quality of life changes for better readability and safety. It serves
as a lighter alternative to the bloated C++ STL.
## Performance & flexibility
Sili strives to strike a balance between having great performance and flexibility
that provide many options for developers when using the library.
## Portability
`sili.h`  is a single-header library, allowing for easy integration into any project.
Furthermore, sili is designed to be cross-platform, supporting any OS that has a
working C99 compiler.

# Some of the features present in the library
- Architecture/OS/Compiler/Standard indentification macros (`SI_CPU_X86`,
`SI_SYSTEM_WINDOWS`, `SI_COMPILER_CLANG`, `SI_STANDARD_C99`, `SI_STANDARD_ANSI`
etc).
- Useful macros (`typeof()`, `SI_ASSERT`/`SI_STATIC_ASSERT`, `SI_BIT`, `for_range`/`for_eachArr` etc).
- Robust string implementation (`siString`, fully compatible with `char*`).
- Better arrays (`siArray(<type>)`, compatible with other C arrays).
- `char` specific functions (`si_charToLower`, `si_charIsSpace`, `si_hexDigitToInt` etc).
- Hashtable functions (`si_hashtableMakeReserve`, `si_hashtableGetWithHash`, `si_hashtableSet`)
- Path related functions (`si_pathExists`, `si_pathCopy`, `si_pathGetFullname` etc).
- A cross-platform file IO functions (`si_fileOpen`, `si_fileReadContents`, `si_fileWriteLineAt` etc).
- `siOptional` type similar to `std::optional`.
- Cross-platform multi-thread support (`si_threadMake`, `si_threadRun`, `si_threadJoin`).
- Easy to use benchmarking functions to check performance (`si_benchmarkRunsPerLoop`,
`si_benchmarkExecutesPerMs`, `si_benchmarkLoopsAvg` etc).
- An allocator system, making allocations 3x faster than the traditional `malloc`/`free`.
- A general-use hash table.
- Unicode functions (`si_utf8Decode`, `si_utf8ToUtf16String`, `si_utf16ToUtf8String`).
- Bit manipulation funcionts (`si_numCountBitsU32`, `si_numRotateLeft`, `si_bytesToNumArr`,
`SI_NUM_BIT_SET` etc).
- DLL function loading (`si_dllLoad`, `si_dllProcAddress`).
- Custom `printf` implementation, which includes C23 and new specifiers, as well
as better cross-platform support (`%ll` and `%l` both equal to `i64`).

# Examples
Examples of most (but definitely not all) sili's features are utilized in the
`examples` folder.

# Release model
Sili uses the standard major, minor and patch (e.g., `v1.2.5`) for its `main` 
branch releases, which are meant to be stable and eligible for development. 
They're guaranteed to pass all of the tests, have working examples and compiled
binary releases available for Windows, MacOS and Linux.

If you're using the `dev` branch of sili, then you must be aware that they follow 
the versioning system of `v<current.main.version>-dev-<commit count for the release>`. 
For example, if 3 commits have been released for the `dev` branch, while the `main`
branch version is `v0.5.3`, then those are going to be referred as `v0.5.3-dev-1`, 
`v0.5.3-dev-2` and `v0.5.3-dev-3` respectively. `dev` releases are less likely to
have fully working examples or tests are compile and work as intended on every 
platform. 

The `experimental` version of sili is not meant for any stable use and is reserved 
for the most cutting-edge release of sili. The branch follows the versioning system
of `v<current.main.version>-experimental-<commit ID>`.