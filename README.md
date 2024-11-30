Sili
====

<b><i>„Miau miau miau a miau'au miau a miau miau miau, ma miau miau miau miau miau miau miau miau miau miau miau miau miau miau miau miau miau miau miau'll miau miau miau a miau.“</i></b> - [katė](.github/cat.png)

---

<p align="center">
Sili is a collection of cross-platform single-header libraries for general
programming in C99/C11. It focuses on providing a more modern developer experience
whilst adhering to the main principles and appeals of C: simplicity, performance and portability.
</p>

<p align="center">
	<a href="https://github.com/EimaMei/sili-toolchain/actions/workflows/linux.yml">
		<img src="https://github.com/EimaMei/sili-toolchain/actions/workflows/linux.yml/badge.svg">
	</a>
	<a href="https://github.com/EimaMei/sili-toolchain/actions/workflows/windows.yml">
		<img src="https://github.com/EimaMei/sili-toolchain/actions/workflows/windows.yml/badge.svg">
	</a>
	<a href="https://github.com/EimaMei/sili-toolchain/actions/workflows/macos.yml">
		<img src="https://github.com/EimaMei/sili-toolchain/actions/workflows/macos.yml/badge.svg">
	</a>
</p>



# Libraries
|  Name                | Version | Description |
|----------------------|:-------:|:-----------:|
|  [sili.h](sili.h)    | v0.1.0  | A general-purpose programming library. An alternative to the C/C++ standard libraries.
|  [sigar.h](sigar.h)  | v0.0.0  | A cross-platform library for audio playback and recording.
|  [siarg.h](siarg.h)  | v0.0.0  | An argument-parsing library for making CLIs.

# Contact
You can join the [semi-offical Discord server](https://discord.gg/MSfSmNanF2) for 
any questions related to Sili.

# Versioning systems
## `main` branch
- Format: `v<main>.<minor>.<patch>`.
- Descrpition: A `main` release guarantees that the library will have working examples 
and tests that fufill their checks. Complete documentation is guaranteed, as well 
as compiled releases being available for all platforms.

## `dev` branch
- Format: `v<main-branch-version>-dev-<commit-number>`.
- Descrpition: A `dev` release is less likely to pass all of the tests and have
working examples on every platform. Some functions may have outdated or even no
documentation at all.

## `experimental` branch
- Format: `v<main-branch-version>-experimental-<commit-ID>`.
- Descrpition: An `experimental` release is not meant for any stable use and is
reserved for the most cutting-edge version. Errors and warnings are to
be expected.
