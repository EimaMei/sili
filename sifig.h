/*
sifig.h - v0.0.0 - a sili library for reading various configuration files.
===========================================================================
	- YOU MUST DEFINE 'SIFIG_IMPLEMENTATION' in EXACTLY _one_ C file that 
	includes this header, BEFORE the include like this:

		#include "sili.h"
		#define SIFIG_IMPLEMENTATION
		#include "sifig.h"

	- All other files should just include the library without the #define macro.
===========================================================================
SUPPORT
	- [~] .ini
	- [-] .json
	- [-] .xml

===========================================================================
DOCUMENTATION
	- Refer to sili.h's "Documentation" section.

===========================================================================z
LICENSE
	- This software is licensed under the zlib license (see the LICENSE at the
	bottom of the file).

WARNING
	- Sili and its supplementary libraries are designed to be fast, modern, but 
	also experimental. As a result some unwarranted results may occur during use, 
	such as:
		1) Features not working as expected;
		2) Functions having no or incompleted documentation;
		3) API breaking changes between releases (especially before v1.0.0 release);
		4) Little to no security checks for malicious code that attempt to exploit
		parts of the code.

*/

#if SI_COMPILER_HAS_PRAGMA_ONCE
	#pragma once
#endif

#ifndef SIFIG_INCLUDE_SI_H
#define SIFIG_INCLUDE_SI_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SI_INCLUDE_SI_H
	#include <sili.h>
#endif

#ifndef SIFIG_VERSION_MAJOR
	#define SIFIG_VERSION_MAJOR 0
	#define SIFIG_VERSION_MINOR 0
	#define SIFIG_VERSION_PATCH 0
#endif

#ifndef SIFIG_VERSION_CURRENT
	#define SIFIG_VERSION_CURRENT SI_VERSION(SIFIG_VERSION_MAJOR, SIFIG_VERSION_MINOR, SIFIG_VERSION_PATCH)
#endif 

#ifndef SIFIG_NO_INI

/* TODO */
typedef siMap(siString) siIniSection;
/* TODO */
typedef siMap(siIniSection) siIniFile;

typedef struct siIniOptions {
	siString comment;
	b32 lowerCase;
} siIniOptions;


typedef struct siIniIterator {
	siString section;
	siString key, value;

	siString src;
} siIniIterator;



/* TODO */
SIDEF siIniFile sifig_iniMake(siString path, siAllocator alloc);
/* TODO */
SIDEF siIniFile sifig_iniMakeStr(siString content, siAllocator alloc);
/* TODO */
SIDEF siIniFile sifig_iniMakeEx(siString content, siIniOptions options, siAllocator alloc);

/* TODO */
SIDEF siIniIterator sifig_iniIterator(siString content);

/* TODO */
SIDEF b32 sifig_iniIterate(siIniIterator* it);
/* TODO */
SIDEF b32 sifig_iniIterateEx(siIniIterator* it, siString comment);

/* TODO */
SIDEF void sifig_iniFree(siIniFile ini);


#ifdef SIFIG_IMPLEMENTATION
	#ifndef SIFIG_IMPLEMENTATION_INI
		#define SIFIG_IMPLEMENTATION_INI 1
	#endif 
#endif


#ifdef SIFIG_IMPLEMENTATION_INI

inline 
siIniFile sifig_iniMake(siString path, siAllocator alloc) {
	siString tmp = si_pathReadContents(path, si_allocatorHeap());
	siIniFile res = sifig_iniMakeStr(tmp, alloc);
	si_mfree((void*)tmp.data);

	return res;
}

inline 
siIniFile sifig_iniMakeStr(siString content, siAllocator alloc) {
	return sifig_iniMakeEx(content, (siIniOptions){SI_STR(";"), false}, alloc);
}

SIDEF 
siIniFile sifig_iniMakeEx(siString content, siIniOptions options, siAllocator alloc) {
	siIniFile ini = si_mapMakeReserve(siIniSection, 16, alloc);
	siIniSection* curIni;

	siString curSection = SI_STR_EMPTY;
	siIniIterator it = sifig_iniIterator(content);

	while (sifig_iniIterateEx(&it, options.comment)) {
		if (curSection.data != it.section.data) {
			curIni = si_mapSet(&ini, si_stringCopy(it.section, alloc), &curIni);
			*curIni = si_mapMakeReserve(siString, 32, alloc);

			curSection = it.section;
		}

		siString key = (options.lowerCase)
			? si_stringLower(it.key, alloc)
			: si_stringCopy(it.key, alloc);
		siString value = si_stringCopy(it.value, alloc);
		si_mapSet(curIni, key, &value);
	}

	return ini;
}

inline
siIniIterator sifig_iniIterator(siString content) {
	return (siIniIterator){SI_STR_EMPTY, SI_STR_EMPTY, SI_STR_EMPTY, content};
}

inline 
b32 sifig_iniIterate(siIniIterator* it) {
	return sifig_iniIterateEx(it, SI_STR(";"));
}

SIDEF 
b32 sifig_iniIterateEx(siIniIterator* it, siString comment) {
	siString line;

	while (si_stringSplitLinesIterate(&it->src, &line)) {
		line = si_stringStrip(line);
		SI_STOPIF(line.len == 0, continue);

		if (line.data[0] == '[') {
			isize len = si_stringFindByte(line, ']');
			it->section = si_substr(line, 1, (len != -1) ? len : line.len);
			continue;
		}

		if (si_stringHasPrefix(line, comment)) {
			continue;
		}

		isize end = si_stringFindByte(line, '=');
		SI_STOPIF(end == -1, return false);

		it->key = si_stringStripRight(si_substrTo(line, end));

		it->value = si_substrFrom(line, it->key.len + 1);
		it->value = si_stringUnquote(si_stringStrip(it->value));

		return true;
	}

	return false;
}

/* TODO */
SIDEF void sifig_iniFree(siIniFile ini);

#endif /* SIFIG_IMPLEMENTATION_INI */

#endif /* SIFIG_NO_INI */


#ifdef __cplusplus
}
#endif

#endif /* SIFIG_INCLUDE_SI_H */

/*
------------------------------------------------------------------------------
Copyright (C) 2025 EimaMei

This software is provided 'as-is', without any express or implied warranty. In
no event will the authors be held liable for any damages arising from the use of
this software.

Permission is granted to anyone to use this software for any purpose, including
commercial applications, and to alter it and redistribute it freely, subject to
the following restrictions:

1. The origin of this software must not be misrepresented; you must not
	 claim that you wrote the original software. If you use this software
	 in a product, an acknowledgment in the product documentation would be
	 appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
	 misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
------------------------------------------------------------------------------
*/
