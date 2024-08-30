Sili
====
<center>
<br>
	<img src=".github/lonicat-neris.png" alt="Lonicat" width="150">
<br>
<b><i>„Miau miau miau a miau'au miau a miau miau miau, ma miau miau miau miau miau miau miau miau miau miau miau miau miau miau miau miau miau miau miau'll miau miau miau a miau.“ - katė</i></b>

---

</center>

<p align="center">
Sili is a collection of cross-platform and single-header libraries for general
programming in C99/C11. Many new fixes and exciting additions are made to the C 
libraries for a more modern developer experience while still adhering to the main 
principles and appeals of C: simplicity, performance and portability.
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
|  [sili.h](sili.h)    | v0.1.0  | A general-purpose programming library to replace the C/C++ standard libraries.
|  [sigar.h](sigar.h)  | v0.0.0  | A cross-platform library for audio playback and recording.
|  [siarg.h](siarg.h)  | v0.0.0  | An argument-parsing library for making CLIs.

# Examples
Examples of most (but definitely not all) sili's features are utilized in the
`examples` folder.

# Versioning systems
## `main` branch
- Format: `v<main>.<minor>.<patch>`.
- Descrpition: A `main` release guarantees that all of the `sili` libraries pass
their respective tests and have their examples working as intended, as well as 
compiled releases being available for all platforms.   

## `dev` branch
- Format: `v<main-branch-version>-dev-<commit-number>`.
- Descrpition: A `dev` release is less likely to pass all of the tests and have
working examples on every platform.  

## `experimental` branch
- Format: `v<main-branch-version>-experimental-<commit-ID>`.
- Descrpition: An `experimental` release is not meant for any stable use and is 
reserved  for the most cutting-edge release of sili. Errors and warnings are to 
be expected.