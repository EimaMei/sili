/*
siarg.h - v0.0.0 - an argument-parsing library for making CLIs.
===========================================================================
	- YOU MUST DEFINE 'SIARG_IMPLEMENTATION' in EXACTLY _one_ C file that includes
	this header, BEFORE the include like this:

		#define SIARG_IMPLEMENTATION
		#include "siarg.h"

	- All other files should just include the library without the #define macro.


===========================================================================
LICENSE:
	- This software is licensed under the zlib license (see the LICENSE at the
	bottom of the file).

WARNING
	- Siarg, like sili, is designed to be a fast, modern, but also an experimental
	library and because of it some unwarranted results may occur or be present when
	using the library, those being:
		1) Features may not work as expected
		2) Functions may not be documented or only contain incomplete documentation
		3) API breaking changes between releases
		4) Little to no security checks for malicious code attempting to explode
		sections of the code

*/

#ifndef SIARG_INCLUDE_SI_H
#define SIARG_INCLUDE_SI_H

#if defined(__cplusplus)
extern "C" {
#endif

#ifndef SI_INCLUDE_SI_H
	#include <sili.h>
#endif

#if SI_COMPILER_HAS_PRAGMA_ONCE
	#pragma once
#endif

#ifndef SIARG_VERSION_MAJOR
	#define SIARG_VERSION_MAJOR 0
	#define SIARG_VERSION_MINOR 0
	#define SIARG_VERSION_PATCH 0
#endif
#define SIARG_VERSION_CURRENT SI_VERSION(SIARG_VERSION_MAJOR, SIARG_VERSION_MINOR, SIARG_VERSION_PATCH)


/* */
SI_ENUM(u32, siOptionErrorType) {
	/* No error was encountered. */
	SI_OPTION_ERROR_NONE = 0,
	/* User-specified option either uses a wrong prefix or doesn't use one at all.
	 * NOTE: 'ctx.error.option' it set to nil. */
	SI_OPTION_ERROR_PREFIX,
	/* User-specified option doesn't exist.
	 * NOTE: 'ctx.error.option' is set to nil. */
	SI_OPTION_ERROR_OPTION,
	/* An option is specified more than once by the user. */
	SI_OPTION_ERROR_MULTIPLE,
	/* User-specified option requires a value, however nothing was provided. */
	SI_OPTION_ERROR_NO_VALUE,
	/* User-specified value does not conform to the option's required type.
	 * Eg. Entering an invalid number like "0xQD" for an integer option. */
	SI_OPTION_ERROR_INVALID_VALUE,
	/* User-specified value isn't in the range/list of valid choices. */
	SI_OPTION_ERROR_INVALID_CHOICE,
	/* User did not use the required separator. */
	SI_OPTION_ERROR_SEPARATOR,
	/* User did not specify a required option. */
	SI_OPTION_ERROR_REQUIRED
};


/* */
SI_ENUM(i32, siOptionType) {
	SI_OPTION_TYPE_STRING = 1,
	SI_OPTION_TYPE_INT,
	SI_OPTION_TYPE_UINT,
	SI_OPTION_TYPE_FLOAT,
	SI_OPTION_TYPE_BOOL,
};

/* */
SI_ENUM(u32, siOptionConfig) {
	SI_OPTION_CONFIG_REQUIRED = SI_BIT(0),
	SI_OPTION_CONFIG_NO_SEPARATOR = SI_BIT(1),
	SI_OPTION_CONFIG_POSITIONAL = SI_BIT(2),
	SI_OPTION_CONFIG_IS_ACTION = SI_BIT(3),

	SI_OPTION_CONFIG_BITS = SI_OPTION_CONFIG_REQUIRED | SI_OPTION_CONFIG_NO_SEPARATOR
							| SI_OPTION_CONFIG_POSITIONAL | SI_OPTION_CONFIG_IS_ACTION,
};
/* */
SI_ENUM(u32, __siInternOptionState) {
	SI__INTERN_OPTION_IS_SET = SI_BIT(0),
	SI__INTERN_OPTION_FLAG = SI_BIT(1),
	SI__INTERN_OPTION_CHOICES = SI_BIT(2),
	SI__INTERN_OPTION_HAS_DEFAULT = SI_BIT(3),
};

/* */
typedef union siOptionValue {
	siString string;
	u64 uinteger;
	i64 integer;
	f64 floatingPoint;
	b32 boolean;
} siOptionValue;


/* */
typedef struct {
	/* */
	siOptionType type;
	/* */
	siOptionConfig config;

	/* */
	siString name;
	siUtf8Char nameShort;
	i32 codepoint;

	/* */
	__siInternOptionState __state;

	/* */
	siString description;
	/* */
	siOptionValue defaultValue;

	union {
		siBuffer list;
		i64 rangeI[2];
		f64 rangeF[2];
	} choices;

	usize outCapacity;
	usize outLen;
	rawptr outPtr;
} siArgvOption;


/* */
typedef struct siOptionError {
	siOptionErrorType type;
	siArgvOption* option;
	siString value;
} siOptionError;

/* */
typedef struct {
	siArgvOption* options;
	usize len;
	usize capacity;

	siString executable;

	siString prefix;
	siString prefixShort;
	siString separator;

	siString description;
	siString usage;

	u32 paddingFront;
	u32 maxOptionLen;
	usize maxColumnLen;

	siOptionError error;
} siOptionContext;


/* */
SIDEF siOptionContext si_argvMake(cstring prefix, siArgvOption* options, usize len);
/* */
SIDEF u32 si_argvParse(siOptionContext* ctx, int argc, char** argv);
/* */
SIDEF void si_argvHelp(siOptionContext ctx);
/* */
SIDEF void si_argvError(siOptionContext ctx);


/* */
SIDEF siOptionContext si_argvContextMake(cstring prefix, siArgvOption* options, usize len);
/* */
SIDEF void si_argvContextSetPrefix(siOptionContext* ctx, cstring prefix);
/* */
SIDEF void si_argvContextSetPrefixShort(siOptionContext* ctx, cstring prefixShort);
/* */
SIDEF void si_argvContextSetSeparator(siOptionContext* ctx, cstring separator);
/* */
SIDEF void si_argvContextSetDescription(siOptionContext* ctx, cstring usage);
/* */
SIDEF void si_argvContextSetUsage(siOptionContext* ctx, cstring usage);

/* */
SIDEF siArgvOption* si_argvOptionMakeStr(siOptionContext* ctx, cstring name,
		siString* outPtr);
/* */
SIDEF siArgvOption* si_argvOptionMakeInt(siOptionContext* ctx, cstring name, i64* outPtr);
/* */
SIDEF siArgvOption* si_argvOptionMakeUint(siOptionContext* ctx, cstring name, u64* outPtr);
/* */
SIDEF siArgvOption* si_argvOptionMakeBool(siOptionContext* ctx, cstring name, b32* outPtr);
/* */
SIDEF siArgvOption* si_argvOptionMakeEx(siOptionContext* ctx, siOptionType type,
		cstring name, rawptr outPtr);
/* */
SIDEF siArgvOption* si_argvFlagMake(siOptionContext* ctx, cstring name, b32 isAction,
		b32* outPtr);

/* */
SIDEF void si_argvOptionSetDescription(siArgvOption* option, cstring description);
/* */
SIDEF void si_argvOptionSetShortName(siArgvOption* option, i32 codepoint);
/* */
SIDEF void si_argvOptionSetConfig(siArgvOption* option, siOptionConfig config);
/* */
SIDEF void si_argvOptionSetChoices(siArgvOption* option, siBuffer choices);
/* */
SIDEF void si_ArgvOptionSetCapacity(siArgvOption* option, usize capacity);
/* smth about the function. */
#define si_argvOptionSetDefault(/*siArgvOption**/ option, /*TYPE*/ type, /*VALUE*/...) \
	si__ArgvOptionSetDefault(option, sizeof(type), &(type){__VA_ARGS__})
/* TODO(EimaMei): Rework these functions. */
#define si_argvOptionSetRange(/*siArgvOption**/ option, /*TYPE*/ type, /*VALUE*/ value1, \
		/*VALUE*/ value2) si__ArgvOptionSetRange(option, sizeof(type), &(type){value1}, &(type){value2})

#if 1
SIDEF void si__ArgvOptionSetDefault(siArgvOption* option, usize item_sizeof,
		const rawptr ptr);
SIDEF void si__ArgvOptionSetRange(siArgvOption* option, usize item_sizeof,
		const rawptr ptr1, const rawptr ptr2);
#endif


#if defined(SIARG_IMPLEMENTATION)

#define SI__OPTION_ERROR(ctx, id, opt, str) \
	ctx->error.type = id; \
	ctx->error.option = opt; \
	ctx->error.value = str; \
	return false

force_inline
b32 si__argvOptionSet(siOptionContext* ctx, siArgvOption* option, siString substr) {
	b32 res;

	switch (option->type) {
		case SI_OPTION_TYPE_STRING: {
			siString* out = (siString*)option->outPtr + option->outLen;
			res = true;

			if ((option->__state & SI__INTERN_OPTION_CHOICES) == 0) {
				*out = substr;
			}
			else {
				b32 found = false;

				for_range (i, 0, option->choices.list.len) {
					siString choice = si_cast(siString*, option->choices.list.data)[i];
					if (si_stringEqual(substr, choice)) {
						*out = choice;
						found = true;
						break;
					}
				}

				if (!found) {
					SI__OPTION_ERROR(ctx, SI_OPTION_ERROR_INVALID_CHOICE, option, substr);
				}
			}
			break;
		}
		case SI_OPTION_TYPE_UINT:
		case SI_OPTION_TYPE_INT: {
			i64* out = (i64*)option->outPtr + option->outLen;
			i64 value = si_stringToIntEx(substr, &res);

			if ((option->__state & SI__INTERN_OPTION_CHOICES) == 0) {
				*out = value;
			}
			else {
				if (si_between(u64, value, option->choices.rangeI[0], option->choices.rangeI[1])) {
					*out = value;
				}
				else {
					SI__OPTION_ERROR(ctx, SI_OPTION_ERROR_INVALID_CHOICE, option, substr);
				}
			}
			break;
		}
		case SI_OPTION_TYPE_BOOL: {
			b32* out = (b32*)option->outPtr + option->outLen;
			*out = si_stringToBool(substr);
			res = (*out != UINT32_MAX);
			break;
		}

		default: res = false;
	}


	if (!res) {
		SI__OPTION_ERROR(ctx, SI_OPTION_ERROR_INVALID_VALUE, option, substr);
	}

	option->outLen += 1;
	option->__state |= SI__INTERN_OPTION_IS_SET;
	return true;
}

force_inline
void si__argvOptionDefaultSet(siArgvOption* option) {
	switch (option->type) {
		case SI_OPTION_TYPE_STRING: {
			siString* out = (siString*)option->outPtr;
			*out = option->defaultValue.string;
			break;
		}
		case SI_OPTION_TYPE_UINT:
		case SI_OPTION_TYPE_INT: {
			i64* out = (i64*)option->outPtr;
			*out = option->defaultValue.integer;
			break;
		}
		case SI_OPTION_TYPE_BOOL: {
			b32* out = (b32*)option->outPtr;
			*out = option->defaultValue.boolean;
			break;
		}
		case SI_OPTION_TYPE_FLOAT: {
			b32* out = (b32*)option->outPtr;
			*out = option->defaultValue.boolean;
			break;
		}
		default: SI_PANIC();
	}

	option->outLen += 1;
	option->__state |= SI__INTERN_OPTION_IS_SET;
}

force_inline
siString si__argvType(siOptionType type) {
	siString buf[] = {SI_STR("string"), SI_STR("integer"), SI_STR("float"), SI_STR("boolean")};
	return buf[type - 1];
}

SIDEF
siOptionContext si_argvMake(cstring prefix, siArgvOption* options, usize len) {
	SI_ASSERT_NOT_NULL(options);

	siOptionContext context;
	context.options = options;
	context.len = 0;
	context.capacity = len;
	context.executable = (siString){0};
	context.separator = SI_STR(" ");
	context.usage = (siString){0};
	context.description = (siString){0};

	context.paddingFront = 4;
	context.maxOptionLen = 35;
	context.maxColumnLen = 120;

	context.prefix = SI_CSTR(prefix);
	context.prefixShort = SI_STR("-");
	SI_ASSERT(context.prefix.len != 0);

	return context;
}
SIDEF
b32 si_argvParse(siOptionContext* ctx, int argc, char** argv) {
	SI_ASSERT_NOT_NULL(ctx);
	SI_ASSERT_NOT_NULL(argv);

	ctx->executable = SI_CSTR(argv[0]);

	for_range (i, 1, argc) {
		siString value = SI_CSTR(argv[i]);

		b32 regularPrefix = true;
		if (si_memcompareStr(value.data, ctx->prefix) != 0) {
			if (si_memcompareStr(value.data, ctx->prefixShort) != 0) {
				b32 continueLoop = false;

				siArgvOption* option;
				for_range (j, 0, ctx->len) {
					option = &ctx->options[j];
					SI_STOPIF((option->config & SI_OPTION_CONFIG_POSITIONAL) == 0, continue);

					if (option->outLen >= option->outCapacity) {
						continue;
					}

					b32 res = si__argvOptionSet(ctx, option, value);
					SI_STOPIF(!res, return false);
					continueLoop = true;
				}

				SI_STOPIF(continueLoop, continue);
				SI__OPTION_ERROR(ctx, SI_OPTION_ERROR_PREFIX, nil, value);
				return false;
			}
			regularPrefix = false;
			value = si_stringSubToEnd(value, ctx->prefixShort.len);
		}
		else {
			value = si_stringSubToEnd(value, ctx->prefix.len);
		}

		usize optionLen = 0;
		siArgvOption* option;
		for_range (j, 0, ctx->len) {
			option = &ctx->options[j];

			if (regularPrefix) {
				if (si_memcompareStr(value.data, option->name) == 0) {
					optionLen = option->name.len;
					break;
				}
			}
			else {
				siUtf8Char* shrt = &option->nameShort;
				if (shrt->len != 0 && si_memcompare(value.data, shrt->codepoint, shrt->len) == 0) {
					optionLen = shrt->len;
					break;
				}
			}
		}

		if (optionLen != 0) {
			if (option->outLen >= option->outCapacity) {
				SI__OPTION_ERROR(ctx, SI_OPTION_ERROR_MULTIPLE, option, value);
			}
			value = si_stringSubToEnd(value, optionLen);
		}
		else {
			SI__OPTION_ERROR(ctx, SI_OPTION_ERROR_OPTION, nil, value);
		}

		if (option->__state & SI__INTERN_OPTION_FLAG) {
			b32* outPtr = (b32*)option->outPtr + option->outLen;
			*outPtr = true;

			option->outLen += 1;
			option->__state |= SI__INTERN_OPTION_IS_SET;
			SI_STOPIF(option->config & SI_OPTION_CONFIG_IS_ACTION, return option->codepoint);
			continue;
		}

		if ((option->config & SI_OPTION_CONFIG_REQUIRED) == 0) {
			if (i + 1 >= argc || si_memcompareStr(argv[i + 1], ctx->prefix) == 0) {
				si__argvOptionDefaultSet(option);
				SI_STOPIF(option->config & SI_OPTION_CONFIG_IS_ACTION, return option->codepoint);
				continue;
			}
		}


		if (si_stringAtFront(ctx->separator) == ' ' && ctx->separator.len == 1) {
			if (i + 1 >= argc || si_memcompareStr(argv[i + 1], ctx->prefix) == 0) {
				SI__OPTION_ERROR(ctx, SI_OPTION_ERROR_NO_VALUE, option, value);
			}

			i += 1;
			value = SI_CSTR(argv[i]);

			b32 res = si__argvOptionSet(ctx, option, value);
			SI_STOPIF(!res, return false);
			SI_STOPIF(option->config & SI_OPTION_CONFIG_IS_ACTION, return option->codepoint);
			continue;
		}

		if ((option->config & SI_OPTION_CONFIG_NO_SEPARATOR) == 0
			&& si_memcompareStr(value.data, ctx->separator) != 0
		) {
			value = (value.len != 0) ? value : SI_STR(" ");
			SI__OPTION_ERROR(ctx, SI_OPTION_ERROR_SEPARATOR, option, value);
		}
		value = si_stringSubToEnd(value, ctx->separator.len);

		if (*(u8*)value.data == '\0') {
			SI__OPTION_ERROR(ctx, SI_OPTION_ERROR_NO_VALUE, option, value);
		}

		b32 res = si__argvOptionSet(ctx, option, value);
		SI_STOPIF(!res, return false);
		SI_STOPIF(option->config & SI_OPTION_CONFIG_IS_ACTION, return option->codepoint);
	}

	for_range (i, 0, ctx->len) {
		siArgvOption* option = &ctx->options[i];
		SI_STOPIF(option->__state & SI__INTERN_OPTION_IS_SET, continue);

		if (option->config & SI_OPTION_CONFIG_REQUIRED) {
			SI__OPTION_ERROR(ctx, SI_OPTION_ERROR_REQUIRED, option, (siString){0});
		}

		si__argvOptionDefaultSet(option);
	}

	return true;
}

SIDEF
void si_argvHelp(siOptionContext ctx) {
	if (ctx.usage.data == nil) {
		si_fprintf(
			si_stderr,
			"Usage: %S%S",
			ctx.executable, (ctx.len) ? SI_STR(" [options]") : SI_STR("")
		);

		for_range (i, 0, ctx.len) {
			siArgvOption* option = &ctx.options[i];
			SI_STOPIF((option->config & SI_OPTION_CONFIG_POSITIONAL) == 0, continue);

			si_fprintf(
				si_stderr,
				(option->config & SI_OPTION_CONFIG_REQUIRED)
					? " %S"
					: " [%S]",
				option->name
			);
		}

		si_fprint(si_stderr, "\n");
	}
	else {
		si_fprintf(si_stderr, "%S\n", ctx.usage);
	}

	if (ctx.description.len != 0) {
		si_fprintf(si_stderr, "%S\n", ctx.description);
	}

	si_fprintf(si_stderr, "Options:\n", ctx.usage);

	for_range (i, 0, ctx.len) {
		siArgvOption* option = &ctx.options[i];
		siString type = si__argvType(option->type);
		b32 noSeparator = (option->config & SI_OPTION_CONFIG_NO_SEPARATOR)
			|| option->__state & SI__INTERN_OPTION_FLAG;
		char argLetterStart, argLetterEnd;

		char buf[SI_KILO(4)];
		usize bufI = 0;

		if (option->config & SI_OPTION_CONFIG_REQUIRED)  {
			argLetterStart = '<';
			argLetterEnd = '>';
		}
		else {
			argLetterStart = '[';
			argLetterEnd = ']';
		}

		bufI += si_snprintf(buf, sizeof(buf), "%*s", ctx.paddingFront, "");

		if (option->nameShort.len != 0) {
			bufI += si_snprintf(
				&buf[bufI], sizeof(buf) - bufI,
				"%S%.*s, ",
				ctx.prefixShort, option->nameShort.len, option->nameShort.codepoint
			) - 1;
		}

		bufI += si_snprintf(
			&buf[bufI], sizeof(buf) - bufI,
			"%S%S%S",
			ctx.prefix, option->name, (!noSeparator) ? ctx.separator : SI_STR("")
		) - 1;

		if (option->__state & SI__INTERN_OPTION_CHOICES) {
			switch (option->type) {
				case SI_OPTION_TYPE_STRING: {
					siString* arr = (siString*)option->choices.list.data;
					usize len = option->choices.list.len - 1;

					bufI += si_snprintf(&buf[bufI], sizeof(buf) - bufI, "%c", argLetterStart) - 1;
					for_range (j, 0, len) {
						bufI += si_snprintf(&buf[bufI], sizeof(buf) - bufI, "%S|", arr[j]) - 1;
					}
					bufI += si_snprintf(&buf[bufI], sizeof(buf) - bufI, "%S%c", arr[len], argLetterEnd) - 1;
					break;
				}
				case SI_OPTION_TYPE_INT:
					bufI += si_snprintf(
						&buf[bufI], sizeof(buf) - bufI, "%c%li-%li%c",
						argLetterStart,
						option->choices.rangeI[0], option->choices.rangeI[1],
						argLetterEnd
					) - 1;
					break;
				case SI_OPTION_TYPE_UINT:
					bufI += si_snprintf(
						&buf[bufI], sizeof(buf) - bufI, "%c%lu-%lu%c",
						argLetterStart,
						(u64)option->choices.rangeI[0], (u64)option->choices.rangeI[1],
						argLetterEnd
					) - 1;
					break;
			}
		}
		else if (!noSeparator) {
			bufI += si_snprintf(
				&buf[bufI], sizeof(buf) - bufI, "%c%S%c",
				argLetterStart, type, argLetterEnd
			) - 1;
		}


		if (option->description.len != 0) {
			if (bufI > ctx.maxOptionLen) {
				bufI += si_snprintf(
					&buf[bufI], sizeof(buf) - bufI,
					"\n%*s%*s",
					ctx.paddingFront, "", ctx.maxOptionLen - ctx.paddingFront - 1, ""
				);
			}
			else {
				bufI += si_snprintf(
					&buf[bufI], sizeof(buf) - bufI,
					"%*s",
					ctx.maxOptionLen - bufI, ""
				);
			}

			if (bufI + option->description.len <= ctx.maxColumnLen) {
				bufI += si_snprintf(&buf[bufI], sizeof(buf) - bufI, "%S", option->description);
			}
			else {
				siString desc = option->description;
				usize startingLen = 0;
				usize prevLen, prevUtf8Len;
				usize limit = ctx.maxColumnLen - ctx.maxOptionLen;
				isize res;
back:
				prevLen = 0;
				prevUtf8Len = 0;
				while (true) {
					isize utf8Index;
					res = si_stringFindUtf8(desc, (u8*)" ", &utf8Index);
					SI_STOPIF(res == -1, break);
					SI_STOPIF(utf8Index + prevUtf8Len > limit, break);

					prevLen += res + 1;
					prevUtf8Len += utf8Index + 1;
					desc = si_stringSubToEnd(desc, res + 1);
				}

				if (res == -1) {
					desc = si_stringSubToEnd(option->description, startingLen);
					bufI += si_snprintf(&buf[bufI], sizeof(buf) - bufI, "%S", desc);
				}
				else {
					desc = si_stringSub(option->description, startingLen, prevLen);
					startingLen += prevLen;

					bufI += si_snprintf(
						&buf[bufI],
						sizeof(buf) - bufI, "%S\n%*s%*s", desc,
						ctx.paddingFront, "", ctx.maxOptionLen - ctx.paddingFront - 1, ""
					);

					desc = si_stringSubToEnd(option->description, startingLen);
					goto back;
				}
			}
		}

		if (option->__state & SI__INTERN_OPTION_HAS_DEFAULT) {
			switch (option->type) {
				case SI_OPTION_TYPE_STRING:
					si_fprintf(si_stderr, "%.*s (Default: '%S')\n", bufI, buf, option->defaultValue.string);
					break;
				case SI_OPTION_TYPE_INT:
					si_fprintf(si_stderr, "%.*s (Default: '%li')\n", bufI, buf, option->defaultValue.integer);
					break;
				case SI_OPTION_TYPE_UINT:
					si_fprintf(si_stderr, "%.*s (Default: '%lu')\n", bufI, buf, option->defaultValue.integer);
					break;
				case SI_OPTION_TYPE_BOOL:
					si_fprintf(si_stderr, "%.*s (Default: '%B')\n", bufI, buf, option->defaultValue.boolean);
					break;
				case SI_OPTION_TYPE_FLOAT:
					si_fprintf(si_stderr, "%.*s (Default: '%f')\n", bufI, buf, option->defaultValue.floatingPoint);
					break;
			}
		}
		else {
			si_fprintf(si_stderr, "%.*s\n", bufI, buf);
		}
	}
}

SIDEF
void si_argvError(siOptionContext ctx) {
	siPrintColor red = si_printColor3bitEx(siPrintColor3bit_Red, true, false),
				 bold = si_printColor3bitEx(siPrintColor3bit_White, true, true);
	siArgvOption* option = ctx.error.option;
	siString type = si__argvType(option->type);

	switch (ctx.error.type) {
		case SI_OPTION_ERROR_PREFIX:
			si_fprintf(
				si_stderr,
				"%C%S:%C %Cerror:%C A none valid prefix was used for %C'%S'%C (all options must be prefixed with %C'%S'%C).\n",
				bold, ctx.executable, &red,
				bold, ctx.error.value,
				bold, ctx.prefix
			);
			break;

		case SI_OPTION_ERROR_OPTION:
			si_fprintf(
				si_stderr,
				"%C%S:%C %Cerror:%C Unknown option %C'%S%S'%C was specified.\n",
				bold, ctx.executable, red,
				bold, ctx.prefix, ctx.error.value
			);
			break;

		case SI_OPTION_ERROR_MULTIPLE:
			si_fprintf(
				si_stderr,
				"%C%S:%C %Cerror:%C Option %C'%S%S%S<%S>'%C was specified more than %C'%lu'%C %S.\n",
				bold, ctx.executable, red,
				bold, ctx.prefix, option->name, ctx.separator, type,
				bold, option->outCapacity,
				(option->outLen == 1) ? SI_STR("time") : SI_STR("times")
			);
			break;

		case SI_OPTION_ERROR_NO_VALUE:
			si_fprintf(
				si_stderr,
				"%C%S:%C %Cerror:%C No value was specified for the %C'%S%S%S<%S>'%C option.\n",
				bold, ctx.executable, red,
				bold, ctx.prefix, option->name, ctx.separator, type
			);
			break;

		case SI_OPTION_ERROR_INVALID_VALUE:
			si_fprintf(
				si_stderr,
				"%C%S:%C %Cerror:%C Invalid %S %C'%S'%C was specified for the %C'%S%S%S<%S>'%C option.\n",
				bold, ctx.executable, red, type,
				bold, ctx.error.value,
				bold, ctx.prefix, option->name, ctx.separator, type
			);
			break;

		case SI_OPTION_ERROR_INVALID_CHOICE: {
			switch (option->type) {
				case SI_OPTION_TYPE_STRING: {
					si_fprintf(
						si_stderr,
						"%C%S:%C %Cerror:%C Invalid choice %C'%S'%C was specified for the %C'%S%S%S[",
						bold, ctx.executable, red,
						bold, ctx.error.value,
						bold, ctx.prefix, option->name, ctx.separator
					);

					siString* arr = (siString*)option->choices.list.data;
					usize len = option->choices.list.len - 1;
					for_range (i, 0, len) {
						siString choice = arr[i];
						si_fprintf(si_stderr, "%S|", choice);
					}
					si_fprintf(si_stderr, "%C%S]'%C option.\n", bold, arr[len]);
					break;
				}
				case SI_OPTION_TYPE_INT:
					si_fprintf(
						si_stderr,
						"%C%S:%C %Cerror:%C Invalid choice %C'%S'%C was specified for the %C'%S%S%S[%li-%li]'%C option.\n",
						bold, ctx.executable, red,
						bold, ctx.error.value,
						bold, ctx.prefix, option->name, ctx.separator,
						option->choices.rangeI[0], option->choices.rangeI[1]
					);
					break;
				case SI_OPTION_TYPE_UINT:
					si_fprintf(
						si_stderr,
						"%C%S:%C %Cerror:%C Invalid choice %C'%S'%C was specified for the %C'%S%S%S[%lu-%lu]'%C option.\n",
						bold, ctx.executable, red,
						bold, ctx.error.value,
						bold, ctx.prefix, option->name, ctx.separator,
						(u64)option->choices.rangeI[0], (u64)option->choices.rangeI[1]
					);
					break;
			}
			break;
		}

		case SI_OPTION_ERROR_SEPARATOR:
			si_fprintf(
				si_stderr,
				"%C%S:%C %Cerror:%C Invalid separator %C'%S'%C was specified at the %C'%S%S'%C option (it should be %C'%S'%C).\n",
				bold, ctx.executable, red,
				bold, ctx.error.value,
				bold, ctx.prefix, option->name,
				bold, ctx.separator
			);
			break;

		case SI_OPTION_ERROR_REQUIRED:
			si_fprintf(
				si_stderr,
				"%C%S:%C %Cerror:%C Required option '%C%S%S%S<%S>%C' wasn't specified.\n",
				bold, ctx.executable, red,
				bold, ctx.prefix, option->name, ctx.separator, si__argvType(option->type)
			);
			break;

		default: si_fprint(si_stderr, "N/A error\n");
	}
}

inline
void si_argvContextSetPrefix(siOptionContext* ctx, cstring prefix) {
	SI_ASSERT_NOT_NULL(ctx);
	SI_ASSERT_NOT_NULL(prefix);
	ctx->prefix = SI_CSTR(prefix);
}
inline
void si_argvContextSetPrefixShort(siOptionContext* ctx, cstring prefixShort) {
	SI_ASSERT_NOT_NULL(ctx);
	SI_ASSERT_NOT_NULL(prefixShort);
	ctx->prefixShort = SI_CSTR(prefixShort);
}
inline
void si_argvContextSetSeparator(siOptionContext* ctx, cstring separator) {
	SI_ASSERT_NOT_NULL(ctx);
	SI_ASSERT_NOT_NULL(separator);
	ctx->separator = SI_CSTR(separator);
}
inline
void si_argvContextSetDescription(siOptionContext* ctx, cstring description) {
	SI_ASSERT_NOT_NULL(ctx);
	SI_ASSERT_NOT_NULL(description);
	ctx->description = SI_CSTR(description);
}
inline
void si_argvContextSetUsage(siOptionContext* ctx, cstring usage) {
	SI_ASSERT_NOT_NULL(ctx);
	SI_ASSERT_NOT_NULL(usage);
	ctx->usage = SI_CSTR(usage);
}


inline
siArgvOption* si_argvOptionMakeStr(siOptionContext* ctx, cstring name, siString* outPtr) {
	return si_argvOptionMakeEx(ctx, SI_OPTION_TYPE_STRING, name, outPtr);
}
inline
siArgvOption* si_argvOptionMakeInt(siOptionContext* ctx, cstring name, i64* outPtr) {
	return si_argvOptionMakeEx(ctx, SI_OPTION_TYPE_INT, name, outPtr);
}
inline
siArgvOption* si_argvOptionMakeUint(siOptionContext* ctx, cstring name, u64* outPtr) {
	return si_argvOptionMakeEx(ctx, SI_OPTION_TYPE_UINT, name, outPtr);
}
inline
siArgvOption* si_argvOptionMakeBool(siOptionContext* ctx, cstring name, b32* outPtr) {
	return si_argvOptionMakeEx(ctx, SI_OPTION_TYPE_BOOL, name, outPtr);
}

SIDEF
siArgvOption* si_argvOptionMakeEx(siOptionContext* ctx, siOptionType type, cstring name,
		rawptr outPtr) {
	SI_ASSERT_NOT_NULL(outPtr);
	SI_ASSERT(ctx->len < ctx->capacity);

	siArgvOption* option = &ctx->options[ctx->len];
	option->type = type;
	option->name = SI_CSTR(name);
	option->nameShort = (siUtf8Char){0};
	option->codepoint = 0;
	option->description = (siString){0};
	option->config = 0;
	option->__state = 0;
	option->outPtr = outPtr;
	option->outLen = 0;
	option->outCapacity = 1;

	ctx->len += 1;
	return option;
}

inline
siArgvOption* si_argvFlagMake(siOptionContext* ctx, cstring name, b32 isAction,
		b32* outPtr) {
	siArgvOption* option = si_argvOptionMakeEx(ctx, SI_OPTION_TYPE_BOOL, name, outPtr);
	option->__state = SI__INTERN_OPTION_FLAG;
	option->defaultValue.boolean = false;

	if (isAction) {
		option->config |= SI_OPTION_CONFIG_IS_ACTION;
	}
	return option;
}

inline
void si_argvOptionSetDescription(siArgvOption* option, cstring description) {
	SI_ASSERT_NOT_NULL(option);
	SI_ASSERT_NOT_NULL(description);

	option->description = SI_CSTR(description);
}

inline
void si_argvOptionSetShortName(siArgvOption* option, i32 codepoint) {
	SI_ASSERT_NOT_NULL(option);

	option->nameShort = si_utf8Encode(codepoint);
	option->codepoint = codepoint;
}

inline
void si_argvOptionSetConfig(siArgvOption* option, siOptionConfig config) {
	SI_ASSERT_NOT_NULL(option);
	SI_ASSERT_MSG((option->config & ~SI_OPTION_CONFIG_BITS) == 0, "Invalid specified config.");
	SI_ASSERT_MSG((option->__state & SI__INTERN_OPTION_FLAG) == 0, "You cannot set the configurations for a flag.");
	SI_ASSERT_MSG(
		(option->config & SI_OPTION_CONFIG_POSITIONAL) == 0
		|| (option->config & SI_OPTION_CONFIG_IS_ACTION) == 0,
		"You cannot use the 'SI_OPTION_CONFIG_IS_ACTION' config for positionals."
	);
	option->config = config;
}

inline
void si_argvOptionSetChoices(siArgvOption* option, siBuffer choices) {
	SI_ASSERT_NOT_NULL(option);
	SI_ASSERT_NOT_NULL(choices.data);
	option->__state |= SI__INTERN_OPTION_CHOICES;
	option->choices.list = choices;
}

inline
void si_ArgvOptionSetCapacity(siArgvOption* option, usize capacity) {
	SI_ASSERT_NOT_NULL(option);
	SI_ASSERT(capacity != 0);
	option->outCapacity = capacity;
}

inline
void si__ArgvOptionSetDefault(siArgvOption* option, usize item_sizeof, const rawptr ptr) {
	SI_ASSERT_NOT_NULL(option);
	SI_ASSERT_NOT_NULL(ptr);

	switch (option->type) {
		case SI_OPTION_TYPE_STRING:
			SI_ASSERT(item_sizeof == sizeof(siString));
			option->defaultValue.string = *(siString*)ptr;
			break;

		case SI_OPTION_TYPE_UINT:
		case SI_OPTION_TYPE_INT:
			si_memcopy(&option->defaultValue.integer, ptr, item_sizeof);
			break;

		case SI_OPTION_TYPE_FLOAT:
			si_memcopy(&option->defaultValue.floatingPoint, ptr, item_sizeof);
			break;

		case SI_OPTION_TYPE_BOOL:
			si_memcopy(&option->defaultValue.boolean, ptr, item_sizeof);
			option->defaultValue.boolean &= true;
			break;
	}

	option->__state |= SI__INTERN_OPTION_HAS_DEFAULT;
}

inline
void si__ArgvOptionSetRange(siArgvOption* option, usize item_sizeof, const rawptr ptr1,
		const rawptr ptr2) {
	SI_ASSERT_NOT_NULL(option);
	SI_ASSERT(option->type != SI_OPTION_TYPE_STRING && option->type != SI_OPTION_TYPE_BOOL);
	option->__state |= SI__INTERN_OPTION_CHOICES;

	if (option->type == SI_OPTION_TYPE_INT || option->type == SI_OPTION_TYPE_UINT) {
		si_memcopy(&option->choices.rangeI[0], ptr1, item_sizeof);
		si_memcopy(&option->choices.rangeI[1], ptr2, item_sizeof);
	}
	else {
		si_memcopy(&option->choices.rangeF[0], ptr1, item_sizeof);
		si_memcopy(&option->choices.rangeF[1], ptr2, item_sizeof);
	}
}

#undef SI__OPTION_ERROR

#endif /* SIARG_IMPLEMENTATION */

#if defined(__cplusplus)
}
#endif

#endif /* SIARG_INCLUDE_SI_H */

/*
------------------------------------------------------------------------------
Copyright (C) 2024 EimaMei

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
