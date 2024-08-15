/*
sigar.h - v0.0.0 - a cross-platform audio library for playback and recording.
===========================================================================
	- YOU MUST DEFINE 'SIGAR_IMPLEMENTATION' in EXACTLY _one_ C file that includes
	this header, BEFORE the include like this:

		#define SIGAR_IMPLEMENTATION
		#include "sigar.h"

	- All other files should just include the library without the #define macro.

===========================================================================
UNITS OF TIMES
	- TODO

===========================================================================
SUPPORTED & PLANNED BACKENDS
	- Windows:
		1. WASAPI (-)
		2. DirectSound (-)
		3. WinMM (-)

	- Apple:
		1. CoreAudio (+)

	- Linux/Unix:
		1. ALSA (+)
		2. PulseAudio (-)
		3. JACK (-)
		4. Pipewire (-)

	- Support for Android (AAudio/OpenSL), Web Audio, sndio, OSS and custom backends
	might be considered.

===========================================================================
MACROS
	- For any of the macros to work, you must _always_ define it before including
	the library. Example:
	```c
		#define SIGAR_IMPLEMENTATION
		#define SIGAR_NO_ALSA
		#include "sigar.h"
	```

	- SIGAR_NO_DEFAULT_MIXING - disables the default mixing done in sigar as well 
	as undefines the implementation of 'sigar__callbackOutputDefault', which will 
	result in a crash if a new implementation isn't made and the 'callback' setting 
	in the device config isn't changed.

===========================================================================
DOCUMENTATION
	- Some functions might report audio specific errors, in which case a list of
	errors labelled under the "ERRORS:" label will be in its declaration's above
	comment. Usually the format is as follows:
		'- <generic siAudioError enum name> - <explanation as to why/where in
		the function it happened>.'

	It's also worth noting that for audio device functions, the error will usually
	be written in '<device>.status' and attempting to continue using the device
	under an error state will result in an assertion firing off.

	- Other than that, all sigar functions, structures, macros, etc generally
	follow the same documentation style of 'sili.h'. In such cases, Please refer
	to the original file's "DOCUMENTATION" section if you haven't already as this
	library assumes that you are somewhat aware on how things are documented.

===========================================================================
CREDITS
	- David Reid's 'miniaudio' (https://github.com/mackron/miniaudio) - inspired
	me to consider making sigar, as well as certain design choices were used
	here for some functionality.

LICENSE:
	- This software is licensed under the zlib license (see the LICENSE at the
	bottom of the file).

WARNING
	- Sigar, like sili, is designed to be a fast, modern, but also an experimental
	library and because of it some unwarranted results may occur or be present when
	using the library, those being:
		1) Features may not work as expected
		2) Functions may not be documented or only contain incomplete documentation
		3) API breaking changes between releases.
		4) Little to no security checks for malicious code attempting to explode
		sections of the code.

	- Sigar is also NOT responsible for anything if garbage data gets played, be
	it through incorrect audio format inputs or wrong mixing done by the user
	or sili.

*/

#ifndef SIGAR_INCLUDE_SI_H
#define SIGAR_INCLUDE_SI_H

#if defined(__cplusplus)
extern "C" {
#endif

#ifndef SI_INCLUDE_SI_H
#include <sili.h>
#endif

#if SI_COMPILER_CLANG || SI_COMPILER_CHECK_MIN(MSVC, 4, 2, 0) || SI_COMPILER_CHECK_MIN(GCC, 3, 4, 0)
	#pragma once
#endif

#ifndef SIGAR_VERSION_MAJOR
	#define SIGAR_VERSION_MAJOR 0
	#define SIGAR_VERSION_MINOR 0
	#define SIGAR_VERSION_PATCH 0
#endif
#define SIGAR_VERSION_CURRENT SI_VERSION(SIGAR_VERSION_MAJOR, SIGAR_VERSION_MINOR, SIGAR_VERSION_PATCH)


#if SI_SYSTEM_IS_APPLE
	#include <CoreAudio/CoreAudio.h>
	#include <AudioToolbox/AudioToolbox.h>

#elif SI_SYSTEM_IS_UNIX
	#include <alsa/asoundlib.h>

	#include <poll.h>
	#include <sys/eventfd.h>
#endif


/* The default sample format. Good for mixing. */
#ifndef SIGAR_DEFAULT_FORMAT
#define SIGAR_DEFAULT_FORMAT SIGAR_FORMAT_F32
#endif

/* The default sample rate. */
#ifndef SIGAR_DEFAULT_SAMPLE_RATE
#define SIGAR_DEFAULT_SAMPLE_RATE 44100
#endif

/* The default channel count. */
#ifndef SIGAR_DEFAULT_CHANNELS
#define SIGAR_DEFAULT_CHANNELS 2
#endif

/* The default period count. Higher number reduces  latency, but also increases
 * the workload on the CPU. */
#ifndef SIGAR_DEFAULT_PERIODS
#define SIGAR_DEFAULT_PERIODS 3
#endif

/* The default frame count. Higher number reduces CPU workload, but also increases
 * latency. Latency formula (in ms): t = (1000 * <frameCount>) / <sample rate>.
 * A 512 frame count is good default choice for low-latency (~11 ms). */
#ifndef SIGAR_DEFAULT_FRAME_COUNT
#define SIGAR_DEFAULT_FRAME_COUNT 1024
#endif

/* x - UINT
 * Converts seconds into miliseconds. */
#define SIGAR_SEC(x) ((x) * 1000)
/* x - UINT
 * Converts minutes into miliseconds. */
#define SIGAR_MIN(x) (SIGAR_SEC(x) * 60)
/* x - UINT
 * Converts hours into miliseconds */
#define SIGAR_HRS(x) (SIGAR_MIN(x) * 60)
/* hours - UINT | minutes - UINT | seconds - UINT
 * Converts a 'hours:minutes:seconds' timestamp into miliseconds. */
#define SIGAR_TIMESTAMP(hours, minutes, seconds) \
	(SIGAR_HRS(hours) + SIGAR_MIN(minutes) + SIGAR_SEC(seconds))


SI_ENUM(i32, siAudioError) {
	/* 'SIGAR_SUCCESS' gets only set by sigar functions, meaning it's safe to
	 * assume no errors were encountered. */
	SIGAR_SUCCESS = true,

	/* A returned '0' musn't be trusted as things being correct, so treat it as
	 * an error (most likely a structure is fully zeroed out). */
	SIGAR_UNKNOWN = 0,
	/* Generic error. */
	SIGAR_ERROR = -1,

	SIGAR_ERROR_DEVICE = -2,
    SIGAR_ERROR_FORMAT = -3,
	SIGAR_ERROR_THREAD = -4,
	SIGAR_ERROR_POLL = -5,
};

SI_ENUM(u32, siSampleFormat) {
	SIGAR_FORMAT_U8 = 1,

	SIGAR_FORMAT_I16,
	SIGAR_FORMAT_I24,
	SIGAR_FORMAT_I32,
	SIGAR_FORMAT_F32,

	/* NOTE(EimaMei): Non-native endian playback is not supported for mixing! */
#if SI_ENDIAN_IS_LITTLE
	SIGAR_FORMAT_I16BE,
	SIGAR_FORMAT_I24BE,
	SIGAR_FORMAT_I32BE,
	SIGAR_FORMAT_F32BE,

	SIGAR_FORMAT_I16LE = SIGAR_FORMAT_I16,
	SIGAR_FORMAT_I24LE = SIGAR_FORMAT_I24,
	SIGAR_FORMAT_I32LE = SIGAR_FORMAT_I32,
	SIGAR_FORMAT_F32LE = SIGAR_FORMAT_F32,
#else
	SIGAR_FORMAT_I16LE,
	SIGAR_FORMAT_I24LE,
	SIGAR_FORMAT_I32LE,
	SIGAR_FORMAT_F32LE,

	SIGAR_FORMAT_I16BE = SIGAR_FORMAT_I16,
	SIGAR_FORMAT_I24BE = SIGAR_FORMAT_I24,
	SIGAR_FORMAT_I32BE = SIGAR_FORMAT_I32,
	SIGAR_FORMAT_F32BE = SIGAR_FORMAT_F32,
#endif
};

SI_ENUM(u32, siAudioDeviceFormat) {
    /* Unknown format. */
    SIGAR_DEVICE_FORMAT_UNKNOWN = 0,

    /* Signed 8-bit. */
    SIGAR_DEVICE_FORMAT_S8 = SI_BIT(0),
    /* Unsigned 8-bit. */
    SIGAR_DEVICE_FORMAT_U8 = SI_BIT(1),

    /* Signed 16-bit little endian. */
    SIGAR_DEVICE_FORMAT_S16LE = SI_BIT(2),
    /* Signed 16-bit big endian. */
    SIGAR_DEVICE_FORMAT_S16BE = SI_BIT(3),

    /* Unsigned 16-bit little endian. */
    SIGAR_DEVICE_FORMAT_U16LE = SI_BIT(4),
    /* Unsigned 16-bit big endian. */
    SIGAR_DEVICE_FORMAT_U16BE = SI_BIT(5),

    /* Signed 24-bit little endian in 3 bytes (tightly packed). */
    SIGAR_DEVICE_FORMAT_S24LE = SI_BIT(6),
    /* Signed 24-bit big endian in 3 bytes (tightly packed). */
    SIGAR_DEVICE_FORMAT_S24BE = SI_BIT(7),

    /* Unsigned 24-bit little endian. */
    SIGAR_DEVICE_FORMAT_U24LE = SI_BIT(8),
    /* Unsigned 24-bit big endian. */
    SIGAR_DEVICE_FORMAT_U24BE = SI_BIT(9),

    /* Signed 32-bit little endian. */
    SIGAR_DEVICE_FORMAT_S32LE = SI_BIT(10),
    /* Signed 32-bit big endian. */
    SIGAR_DEVICE_FORMAT_S32BE = SI_BIT(11),

    /* Unsigned 32-bit little endian. */
    SIGAR_DEVICE_FORMAT_U32LE = SI_BIT(12),
    /* Unsigned 32-bit big endian. */
    SIGAR_DEVICE_FORMAT_U32BE = SI_BIT(13),

    /* 32-bit floating point little endian. */
    SIGAR_DEVICE_FORMAT_F32LE = SI_BIT(14),
    /* 32-bit floating point big endian. */
    SIGAR_DEVICE_FORMAT_F32BE = SI_BIT(15),

    /* 64-bit floating point little endian. */
    SIGAR_DEVICE_FORMAT_F64LE = SI_BIT(16),
    /* 64-bit floating point big endian. */
    SIGAR_DEVICE_FORMAT_F64BE = SI_BIT(17),

    /* Signed 24-bit little endian in 4 bytes. */
    SIGAR_DEVICE_FORMAT_S24LE_32 = SI_BIT(18),
    /* Signed 24-bit big endian in 4 bytes. */
    SIGAR_DEVICE_FORMAT_S24BE_32 = SI_BIT(19),

    /* Unsigned 24-bit little endian in 4 bytes. */
    SIGAR_DEVICE_FORMAT_U24LE_32 = SI_BIT(20),
    /* Unsigned 24-bit big endian in 4 bytes. */
    SIGAR_DEVICE_FORMAT_U24BE_32 = SI_BIT(21),

	SIGAR_DEVICE_FORMAT_START = SIGAR_DEVICE_FORMAT_S8,
	SIGAR_DEVICE_FORMAT_END = SIGAR_DEVICE_FORMAT_U24BE_32

};

SI_ENUM(u32, siAudioDeviceAccess) {
    /* Unknown access. */
	SIGAR_DEVICE_ACCESS_UNKNOWN = 0,

    /* Interleaved access. */
    SIGAR_DEVICE_ACCESS_INTERLEAVED = SI_BIT(0),
    /* Non-interleaved access. */
    SIGAR_DEVICE_ACCESS_NONINTERLEAVED = SI_BIT(1),

	SIGAR_DEVICE_ACCESS_START = SIGAR_DEVICE_ACCESS_INTERLEAVED,
	SIGAR_DEVICE_ACCESS_END = SIGAR_DEVICE_ACCESS_NONINTERLEAVED
};

SI_ENUM(u32, siAudioDeviceType) {
	/* TODO(EimaMei): Implement */
	SIGAR_DEVICE_TYPE_PLAYBACK = SI_BIT(0),
	/* TODO(EimaMei): Implement */
	SIGAR_DEVICE_TYPE_CAPTURE = SI_BIT(1),
	/* TODO(EimaMei): Implement */
	SIGAR_DEVICE_TYPE_DUPLEX = SIGAR_DEVICE_TYPE_PLAYBACK | SIGAR_DEVICE_TYPE_CAPTURE,
};

SI_ENUM(u32, siAudioState) {
	SIGAR_CLOSED = 0,

	SIGAR_RUNNING,
	SIGAR_PAUSED,

	/* A special property only specific to 'siAudioDevice' that signals an exit
	 * of the current playing thread, but doesn't disallocate the objects within
	 * the device structure, meaning it can be played and started again without
	 * the overhead of setting the same old parameters or rellocating the same
	 * buffers. However, if you require to change the device settings, note that
	 * a stopped device cannot utilize that functionality. */
	SIGAR_STOPPED
};

/* */
struct siAudioDevice;

/* Type definition for sample rate. */
typedef u32 siSampleRate;
/* Function callback that gets called every frame during playback
	- device: The audio device that's being processed.

	- output: The buffer to write into during playback. Is a null pointer if
	capture mode is turned on.

	- input: The buffer to read from during capture for recorded data. Is a null
	pointer if playback mode is turned on.

	- length: The total size of one frame in bytes. Equivalent to:
	'device->config.frameCount * device->config.channels' */
typedef void SI_FUNC_PTR(siAudioCallback, (struct siAudioDevice* audio,
	siByte* restrict output, const siByte* restrict input, usize frameSize));


/* */
typedef union {
	char alsa[256];
	char coreaudio[256];
} siAudioDeviceID;

/* */
typedef struct siAudioDeviceInfo {
    /* Identifier. */
    siAudioDeviceID identifier;

    /* Supported audio formats. */
    siAudioDeviceFormat formats;
    /* Supported access types. */
    siAudioDeviceAccess accesses;
    /* Number of channels (min and max). */
    u32 channels[2];
    /* Supported sample rates (min and max). */
    siSampleRate sampleRates[2];
} siAudioDeviceInfo;

/* */

/* */
typedef struct siAudioDeviceConfig {
	/* */
	siAudioDeviceType type;
	/* Boolean indicating if the configurations were set by the user. */
	b32 isSet;

	/* The set format for the device. */
	siSampleFormat format;
	/* The set sample rate for the device. */
	siSampleRate rate;
	/* The set channels for the device. */
	u32 channels;
	/* */
	u32 periodCount;
	/*  */
	u32 frameCount;

	/* Function callback that gets called every frame during playback. */
	siAudioCallback callback;
	/* */
	rawptr callbackArg;
} siAudioDeviceConfig;

/* Struct representing an audio device. */
typedef struct siAudioDevice {
	/* Current error status of the device. */
	siErrorInfo status;
	/* */
	siAudioDeviceConfig config;

	/* */
	siAudioState state;

#if SI_SYSTEM_IS_APPLE
	/* Apple-specific audio object. */
	AudioObjectID id;
#elif SI_SYSTEM_IS_UNIX
	u32 numberOfSources;
	/* Alsa specific audio object. */
	snd_pcm_t* id;

	siThread thread;
	pthread_mutex_t mutex;
	pthread_cond_t cond;

	siAllocator alloc;
	struct siAudio* audioHead;
	struct pollfd pfd[2];
#endif
} siAudioDevice;


typedef struct siAudio {
	/* The device that the audio unit is connected to. */
	siAudioDevice* device;

	/* User-specified buffer. */
	const siByte* buffer;
	/* User-specified buffer's length (in bytes). */
	usize length;

#if SI_SYSTEM_IS_APPLE
	/* Apple-specific audio object identifier for the audio unit. */
	AudioUnit id;
#elif SI_SYSTEM_IS_UNIX
	/* */
	struct siAudio* nextUnit;
#endif

	/* == Audio Settings == */
	/* */
	siSampleFormat format;
	/* */
	siSampleRate rate;
	/* */
	u32 channels;


	/* === User-configurable settings === */
	/* */
	/* Number of times the audio should loop after the initial run. outValue negative
	 * value loops the unit indefinitely, zero closes the audio unit, and if the
	 * member is still a natural number, it gets decremented and the unit gets
	 * looped. */
	i32 loops;


	/* === User-readable settings === */
	/* NOTE: Editing any of the following might result in unwarranted results. */
	/* */
	/* The number of bytes that one second of samples occupies. */
	u32 bytesPerSecond;

	/* */
	siAudioState state;

	/* The start of the audio unit playback (in bytes). */
	usize start;
	/* The end of the audio unit playback (in bytes). */
	usize end;

	/* == Internal settings == */
	/* NOTE: Musn't be set outside of sigar's header. */
	/* */
	/* Current offset of the unit (in bytes). */
	usize __offset;
} siAudio;





/* Finds and returns the default audio device of the specified type.
 * ERRORS:
 * - SIGAR_ERROR_DEVICE - a default device wasn't found. */
SIDEF siAudioDevice sigar_deviceMake(siAudioDeviceType type);
/* Finds and returns the specified audio device.
 * ERROR:
 * - SIGAR_ERROR_DEVICE - a device based on the identifier wasn't found. */
SIDEF siAudioDevice sigar_deviceMakeID(siAudioDeviceID identifier);
/* Enables support for custom configurations for the device. Returns the pointer
 * to the configuration structure.
 * NOTE: Devices which were found via a config do not need to call this function. */
SIDEF siAudioDeviceConfig* sigar_deviceConfigEnable(siAudioDevice* device);

/* Starts the playback of the device. */
SIDEF void sigar_deviceStart(siAudioDevice* device);
/* Pauses the running device. */
SIDEF void sigar_devicePause(siAudioDevice* device);
/* Waits for the callback thread to finish with an option to force a wake-up, if 
 * paused at all. */
SIDEF void sigar_deviceWait(siAudioDevice* device, b32 forceUnpause);

/* Closes the specified audio device and frees any allocated resources. */
SIDEF void sigar_deviceClose(siAudioDevice* device);


/* Returns the very first available element in the list of audio streams. If
 * there are no available streams, 'nil' will be written instead. */
SIDEF siAudio* sigar_devicePollAudioHead(const siAudioDevice* device);
/* Writes the next available element in the list after the specified stream into
 * the same out parameter, while also checking if current the stream can be looped,
 * closed, etc. If there are no available streams, 'nil' will be written instead. */
SIDEF void sigar_devicePollAudio(const siAudioDevice* device, siAudio** outAudio);


/* Returns the available information about the specified device. */
SIDEF siAudioDeviceInfo sigar_deviceGetInfo(siAudioDevice device);

/* Returns the total number of available audio devices. */
SIDEF usize sigar_deviceGetAmount(void);
/* Retrieves information about all available output devices and fills the given
 * array with it up to the specified length. Returns the amount found.
 * NOTE: The first element is guaranteed to be the default audio device. */
SIDEF usize sigar_deviceGetInfoAll(siAudioDeviceInfo* outDevices, usize length);


/* */
SIDEF siString sigar_audioErrorToStr(siAudioError error);
/* Returns a string version of the device format enumerator. */
SIDEF siString sigar_deviceFormatToStr(siAudioDeviceFormat format);
/* Returns a string version of the device access enumerator. */
SIDEF siString sigar_deviceAccessToStr(siAudioDeviceAccess access);



/* Creates an audio object from a specified buffer. */
SIDEF siAudio sigar_audioMakeEx(siAudioDevice* device, rawptr buffer, usize length,
    siSampleFormat format, siSampleRate rate, u32 channels);

/* Starts playing the given audio object.
 * ERROR:
 * - SIGAR_ERROR_THREAD - the initialization of the playback thread failed somehow. */
SIDEF siAudioError sigar_audioPlay(siAudio* audio);
/* Pauses the playback of the given audio object. Returns 'true' is the object
 * was already paused or has been paused successfully, otherwise 'false' is returned. */
SIDEF b32 sigar_audioPause(siAudio* audio);
/* Stops and completely unitializes the given audio object. */
SIDEF b32 sigar_audioClose(siAudio* audio);

/* Returns the audio buffer that's offset by current timestamp. */
SIDEF siBuffer sigar_audioCurrentBufferGet(const siAudio* audio);

/* Returns the current playback position of the unit. */
SIDEF f32 sigar_audioTell(siAudio audio);
/* Seeks the playback position of the given audio object to a specific time. */
SIDEF void sigar_audioSeek(siAudio* audio, u32 milliseconds);


/* Returns the playback start of the unit. */
SIDEF f32 sigar_audioGetStart(siAudio audio);
/* Sets the playback start of the unit. */
SIDEF void sigar_audioSetStart(siAudio* audio, u32 miliseconds);

/* Returns the playback end of the unit. */
SIDEF f32 sigar_audioGetEnd(siAudio audio);
/* Sets the playback end of the unit. */
SIDEF void sigar_audioSetEnd(siAudio* audio, u32 miliseconds);

/* Returns the duration of the audio unit. */
SIDEF f32 sigar_audioGetDuration(siAudio audio);


#if !defined(SIGAR_NO_DEFAULT_MIXING)

/*
 * NOTE: All mixing functions assume that the channel count and sample rate between
 * the input and output buffers are the same.
*/

/* Mixes the input audio source with the output buffer. Converts the input format
 * into the outpu's if needed. */
void sigar_mixBuffers(siByte* restrict output, siSampleFormat formatOut,
		const siByte* restrict input, siSampleFormat formatIn, usize len);
/* Converts a mono stream to stereo. */
void sigar_upmixBufferToStereo(siByte* restrict output, const siByte* restrict input,
		usize frameCount, u32 sizePerChannel);

/* Mixes an 8-bit unsigned integer source with an 8-bit unsigned integer destination. */
SIDEF void sigar_mixUInt8ToUInt8(u8* restrict output, const u8* restrict input,
		usize len);
/* Mixes a 16-bit integer source with an 8-bit unsigned integer destination. */
SIDEF void sigar_mixInt16ToUInt8(u8* output, const i16* input, usize len);
/* Mixes a 24-bit integer source with an 8-bit unsigned integer destination. */
SIDEF void sigar_mixInt24ToUInt8(u8* restrict output, const siByte* restrict input,
		usize len);
/* Mixes a 32-bit integer source with an 8-bit unsigned integer destination. */
SIDEF void sigar_mixInt32ToUInt8(u8* output, const i32* input, usize len);
/* Mixes a 32-bit float source with an 8-bit unsigned integer destination. */
SIDEF void sigar_mixFloat32ToUInt8(u8* output, const f32* input, usize len);

/* Mixes an 8-bit unsigned integer source with a 16-bit integer destination. */
SIDEF void sigar_mixUInt8ToInt16(i16* output, const u8* input, usize len);
/* Mixes a 16-bit integer source with a 16-bit integer destination. */
SIDEF void sigar_mixInt16ToInt16(i16* restrict output, const i16* restrict input,
		usize len);
/* Mixes a 24-bit integer source with a 16-bit integer destination. */
SIDEF void sigar_mixInt24ToInt16(i16* output, const siByte* input, usize len);
/* Mixes a 32-bit integer source with a 16-bit integer destination. */
SIDEF void sigar_mixInt32ToInt16(i16* output, const i32* input, usize len);
/* Mixes a 32-bit float source with a 16-bit integer destination. */
SIDEF void sigar_mixFloat32ToInt16(i16* output, const f32* input, usize len);


/* Mixes an 8-bit unsigned integer source with a 32-bit float destination. */
SIDEF void sigar_mixUInt8ToFloat32(f32* output, const u8* input, usize len);
/* Mixes a 16-bit integer source with a 32-bit float destination. */
SIDEF void sigar_mixInt16ToFloat32(f32* output, const i16* input, usize len);
/* Mixes a 24-bit integer source with a 32-bit float destination. */
SIDEF void sigar_mixInt24ToFloat32(f32* output, const siByte* input, usize len);
/* Mixes a 32-bit integer source with a 32-bit float destination. */
SIDEF void sigar_mixInt32ToFloat32(f32* output, const i32* input, usize len);
/* Mixes a 32-bit float source with a 32-bit float destination. */
SIDEF void sigar_mixFloat32ToFloat32(f32* restrict output, const f32* restrict input,
		usize len);

#endif /* !SIGAR_NO_DEFAULT_MIXING */


#if defined(SIGAR_IMPLEMENTATION)

siIntern
void sigar__callbackOutputDefault(siAudioDevice* device, siByte* restrict output,
		const siByte* input, usize frameCount);

siIntern
siSampleFormat sigar__formatPriority[] = {
	SIGAR_FORMAT_I16,
	SIGAR_FORMAT_F32,
	SIGAR_FORMAT_I32,
	SIGAR_FORMAT_I24,

	SIGAR_FORMAT_I16 + 4,
	SIGAR_FORMAT_F32 + 4,
	SIGAR_FORMAT_I32 + 4,
	SIGAR_FORMAT_I24 + 4,

	SIGAR_FORMAT_U8
};

siIntern
siSampleRate sigar__ratePriority[] = {
	44100,
	48000,
	22050,
	32000,
	16000,
	11025,
	8000
};


SIDEF
b32 sigar_formatIsPrefered(siSampleFormat format1, siSampleFormat format2) {
	static const i32 priorityMap[] = {
		[SIGAR_FORMAT_I16]     = 0,
		[SIGAR_FORMAT_F32]     = 1,
		[SIGAR_FORMAT_I32]     = 2,
		[SIGAR_FORMAT_I24]     = 3,

		[SIGAR_FORMAT_I16 + 4] = 4,
		[SIGAR_FORMAT_F32 + 4] = 5,
		[SIGAR_FORMAT_I32 + 4] = 6,
		[SIGAR_FORMAT_I24 + 4] = 7,

		[SIGAR_FORMAT_U8]      = 8,
	};

	/* NOTE(EimaMei): The lower the number is, the more preferred it is. */
	return priorityMap[format1] < priorityMap[format2];
}

const usize SIGAR_FORMAT_SIZE_MAP[] = {
	[SIGAR_FORMAT_U8] = 1,

	[SIGAR_FORMAT_I16] = 2,
	[SIGAR_FORMAT_I24] = 3,
	[SIGAR_FORMAT_I32] = 4,
	[SIGAR_FORMAT_F32] = 4,

	[SIGAR_FORMAT_I16 + 4] = 2,
	[SIGAR_FORMAT_I24 + 4] = 3,
	[SIGAR_FORMAT_I32 + 4] = 4,
	[SIGAR_FORMAT_F32 + 4] = 4,

};
#define sigar_formatSize(format) SIGAR_FORMAT_SIZE_MAP[format]
#define sigar_formatIsNative(format) si_between(format, SIGAR_FORMAT_U8, SIGAR_FORMAT_F32)

#if !defined(SI_NO_ERROR_STRUCT)
	#define SIGAR_ASSERT_DEVICE(device) \
		SI_ASSERT_FMT( \
			(device)->status.error == SIGAR_SUCCESS, "'%S' error from the '%s' function.", \
			sigar_audioErrorToStr((device)->status.error), (device)->status.function \
		)
#else
	#define SIGAR_ASSERT_DEVICE(device) \
		SI_ASSERT_FMT( \
			(device)->status.error == SIGAR_SUCCESS, "'%S' error from a prior function.", \
			sigar_audioErrorToStr((device)->status.error)
		)
#endif


#define SIGAR_ASSERT_FORMAT(format) \
	SI_ASSERT_MSG( \
		si_between(format, SIGAR_FORMAT_U8, SIGAR_FORMAT_F32 + 4), \
		"An invalid sample format was provided." \
	)



#if SI_SYSTEM_IS_APPLE

siIntern
void sigar__callbackOutputDefault(siAudio* audio, siByte* restrict output,
		const siByte* restrict input, usize length) {
	si_memcopy(output, input, length);
}

#if (defined(MAC_OS_VERSION_12_0) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_VERSION_12_0) || \
    (defined(__IPHONE_15_0) && __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_15_0)
	#define SIGAR__COREAUDIO_AUDIO_OBJECT_PROPERTY_ELEMENT kAudioObjectPropertyElementMain
#else
	/* kAudioObjectPropertyElementMaster is deprecated. */
	#define SIGAR__COREAUDIO_AUDIO_OBJECT_PROPERTY_ELEMENT kAudioObjectPropertyElementMaster
#endif

siIntern
OSStatus sigar__callbackOutput(void* inRefCon, AudioUnitRenderActionFlags* ioActionFlags,
		const AudioTimeStamp* inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames,
		AudioBufferList* ioData) {
    siAudio* audio = (siAudio*)inRefCon;
    usize remaining = audio->length - audio->__offset;

	for_range (i, 0, ioData->mNumberBuffers) {
        AudioBuffer* buffer = &ioData->mBuffers[i];

        u32 len = (u32)si_min(buffer->mDataByteSize, remaining);
		audio->callback(buffer->mData, &audio->buffer[audio->__offset], len);

		audio->__offset += len;
		remaining -= len;

		if (SI_UNLIKELY(len < buffer->mDataByteSize)) {
			audio->__offset = 0;

			if (audio->loops != 0) {
				if (audio->loops > 0) {
					audio->loops -= 1;
				}
				return 0;
			}

			si_memset((siByte*)buffer->mData + len, 0, len);
			sigar_audioClose(audio);

			return 0;
		}
	}
	SI_UNUSED(inTimeStamp); SI_UNUSED(inBusNumber); SI_UNUSED(inNumberFrames);
	SI_UNUSED(ioActionFlags);

    return 0;
}

siIntern
OSStatus sigar__callbackDeviceUnplugged(AudioObjectID inObjectID, UInt32 inNumberAddresses,
		const AudioObjectPropertyAddress* inAddresses, void* inClientData
	) {
    siAudio* audio = (siAudio*)inClientData;
	if (audio->isClosed) {
		return 0;
	}

	sigar_audioClose(audio);
	SI_UNUSED(inObjectID); SI_UNUSED(inNumberAddresses); SI_UNUSED(inAddresses);
	return 0;
}

#elif SI_SYSTEM_IS_UNIX

siIntern snd_pcm_format_t sigar__alsaToFormat(siSampleFormat format);

siIntern
void sigar__alsaDeviceInit(siAudioDevice* device) {
	SI_STOPIF(device->state == SIGAR_STOPPED, return);

	siAudioDeviceConfig* config = &device->config;
	snd_pcm_hw_params_t* params;
	snd_pcm_hw_params_alloca(&params);

	SI_STOPIF(config->format == 0, config->format = SIGAR_DEFAULT_FORMAT);
	SI_STOPIF(config->rate == 0, config->rate = SIGAR_DEFAULT_SAMPLE_RATE);
	SI_STOPIF(config->channels == 0, config->channels = SIGAR_DEFAULT_CHANNELS);
	SI_STOPIF(config->periodCount == 0, config->periodCount = SIGAR_DEFAULT_PERIODS);
	SI_STOPIF(config->frameCount == 0, config->frameCount = SIGAR_DEFAULT_FRAME_COUNT);

	i32 res = snd_pcm_hw_params_any(device->id, params);
	SI_ASSERT(res >= 0);

	res = snd_pcm_hw_params_set_access(device->id, params, SND_PCM_ACCESS_RW_INTERLEAVED);
	SI_ASSERT(res == 0);

	res = snd_pcm_hw_params_set_format(device->id, params, sigar__alsaToFormat(config->format));
	if (res != 0) {
		for_range (i, 0, countof(sigar__formatPriority)) {
			siSampleFormat format = sigar__formatPriority[i];
			SI_STOPIF(sigar_formatSize(format) > sigar_formatSize(config->format), continue);

			res = snd_pcm_hw_params_set_format(device->id, params, sigar__alsaToFormat(format));
			SI_STOPIF(res == 0, config->format = format; break);
		}
		SI_ASSERT(res == 0);
	}

	res = snd_pcm_hw_params_set_channels(device->id, params, config->channels);
	if (res != 0) {
		res = snd_pcm_hw_params_set_channels(device->id, params, 2);
		SI_ASSERT(res == 0);
	}


	res = snd_pcm_hw_params_set_rate_near(device->id, params, &config->rate, 0);
	if (res != 0) {
		for_range (i, 0, countof(sigar__formatPriority)) {
			siSampleRate rate = sigar__ratePriority[i];
			SI_STOPIF(rate > config->rate, continue);

			res = snd_pcm_hw_params_set_rate(device->id, params, rate, 0);
			SI_STOPIF(res == 0, config->rate = rate; break);
		}
		SI_ASSERT(res == 0);
	}

	res = snd_pcm_hw_params_set_period_size(device->id, params, config->frameCount, 0);
	if (res != 0) {
		res = snd_pcm_hw_params_set_period_size(device->id, params, 1024, 0);
		SI_STOPIF(res == 0, config->frameCount = 0);
	}

	res = snd_pcm_hw_params_set_periods(device->id, params, config->periodCount, 0);
	SI_ASSERT(res == 0);

	res = snd_pcm_hw_params(device->id, params);
	SI_ASSERT(res == 0);

	if (config->frameCount == 0) {
		unsigned long frames;
		res = snd_pcm_hw_params_get_period_size(params, &frames, 0);
		SI_ASSERT(res == 0);

		config->frameCount = (u32)frames;
	}

	struct pollfd* pfds = device->pfd;
	res = snd_pcm_poll_descriptors(device->id, &pfds[1], countof(device->pfd) - 1);
	if (pfds[0].fd == 0) {
		pfds[0].fd = eventfd(0, 0);
		pfds[0].events = POLLIN;
		pfds[0].revents = 0;
	}
	SI_ASSERT(pfds[0].fd != -1);
	SI_ASSERT(res != 0);

	if (device->alloc.ptr == nil) {
		si_allocatorFree(&device->alloc);
	}

	usize frameTotalSize = config->frameCount * config->channels * sigar_formatSize(config->format);
	device->alloc = si_allocatorMake(frameTotalSize);

	if (!device->thread.initialized) {
		pthread_mutex_init(&device->mutex, nil);
		pthread_cond_init(&device->cond, nil);
	}

	res = snd_pcm_nonblock(device->id, 1);
	SI_ASSERT(res == 0);

	res = snd_pcm_prepare(device->id);
	SI_ASSERT(res == 0);
}

siIntern
b32 sigar__alsaDeviceWait(siAudioDevice* device) {
	/* NOTE(EimaMei): Taken and reworked from miniaudio's 'ma_device_wait__alsa'. */
	u16 revents;
    while (true) {
        i32 res = poll(device->pfd, countof(device->pfd), -1);
		if (res < 0) {
			SI_ERROR_DECLARE(device->status, SIGAR_ERROR_POLL);
			return false;
		}

		/* NOTE(EimaMei): It's time to forcefully wake up the device. */
		if (device->pfd[0].revents & POLLIN) {
			u64 tmp;
			read(device->pfd[0].fd, &tmp, sizeof(tmp));

			return false;
		}

        res = snd_pcm_poll_descriptors_revents(
			device->id, &device->pfd[1], countof(device->pfd) - 1, &revents
		);
		if (res < 0) {
			SI_ERROR_DECLARE(device->status, SIGAR_ERROR);
            return false;
        }

		if (revents & POLLERR) {
            if (snd_pcm_state(device->id) != SND_PCM_STATE_XRUN) {
				SI_ERROR_DECLARE(device->status, SIGAR_ERROR_POLL);
				return false;
			}
		}

		if (revents & POLLOUT)
			return true;
	}
}

siIntern
void sigar__alsaDeviceWrite(siAudioDevice* device) {
	i64 status = snd_pcm_writei(device->id, device->alloc.ptr, device->config.frameCount);

	if (status < 0) {
		if (status == -EAGAIN) {
			si_sleep(1);
			return ;
		}
		else if (status == -EPIPE) {
			snd_pcm_prepare(device->id);
		}
		status = snd_pcm_recover(device->id, (int)status, false);
		if (status < 0) {
			SI_ERROR_DECLARE(device->status, SIGAR_ERROR);
			device->state = SIGAR_CLOSED;
			return ;
		}
	}
}

SIDEF
void sigar__alsaDevicePause(siAudioDevice* device) {
	snd_pcm_drain(device->id);
	snd_pcm_prepare(device->id);

	pthread_mutex_lock(&device->mutex);
	while (device->state == SIGAR_PAUSED) {
		pthread_cond_wait(&device->cond, &device->mutex);
	}
	pthread_mutex_unlock(&device->mutex);

	snd_pcm_start(device->id);
}

siIntern
void sigar__threadOutputCallback(siAudioDevice* device) {
	siAudioDeviceConfig* config = &device->config;
	u8 silence = 0; //(config->format == SIGAR_FORMAT_U8) ? 0x80 : 0;
	usize frameSize = config->frameCount * config->channels;

start:
	while (device->state == SIGAR_RUNNING) {
		b32 res = sigar__alsaDeviceWait(device);
		SI_STOPIF(!res, return);

		siByte* frame = device->alloc.ptr;
		si_memset(frame, silence, device->alloc.capacity);

		config->callback(device, frame, nil, frameSize);
		sigar__alsaDeviceWrite(device);
	}

	if (device->state == SIGAR_PAUSED) {
		sigar__alsaDevicePause(device);
		goto start;
	}
}



siIntern
snd_pcm_format_t sigar__alsaToFormat(siSampleFormat format) {
	switch (format) {
		case SIGAR_FORMAT_U8: return SND_PCM_FORMAT_U8;

		case SIGAR_FORMAT_I16LE: return SND_PCM_FORMAT_S16_LE;
		case SIGAR_FORMAT_I16BE: return SND_PCM_FORMAT_S16_BE;

		case SIGAR_FORMAT_I24LE: return SND_PCM_FORMAT_FLOAT_LE;
		case SIGAR_FORMAT_I24BE: return SND_PCM_FORMAT_FLOAT_BE;

		case SIGAR_FORMAT_I32LE: return SND_PCM_FORMAT_FLOAT_LE;
		case SIGAR_FORMAT_I32BE: return SND_PCM_FORMAT_FLOAT_BE;

		case SIGAR_FORMAT_F32LE: return SND_PCM_FORMAT_FLOAT_LE;
		case SIGAR_FORMAT_F32BE: return SND_PCM_FORMAT_FLOAT_BE;

		default: SI_DEBUG_TRAP();
	}
}

siIntern
siSampleFormat sigar__alsaCheckFormat(siSampleFormat format) {
	switch (format) {
		case SIGAR_FORMAT_I24LE: return SIGAR_FORMAT_F32LE;
		case SIGAR_FORMAT_I24BE: return SIGAR_FORMAT_F32BE;

		case SIGAR_FORMAT_I32LE: return SIGAR_FORMAT_F32LE;
		case SIGAR_FORMAT_I32BE: return SIGAR_FORMAT_F32BE;
	}

	return format;
}

siIntern
cstring sigar__alsaDefaultDevices[] = {
	"default",
	"hw:0",
	"hw:0,0"
};

siIntern
b32 sigar__alsaFillDeviceInfoID(snd_pcm_t* id, siAudioDeviceInfo* device) {
	static const snd_pcm_access_t accesses[] = {
		SND_PCM_ACCESS_RW_INTERLEAVED,
		SND_PCM_ACCESS_RW_NONINTERLEAVED
	};

	static const snd_pcm_format_t formats[] = {
	    SND_PCM_FORMAT_S8,
	    SND_PCM_FORMAT_U8,
	    SND_PCM_FORMAT_S16_LE,
	    SND_PCM_FORMAT_S16_BE,
		SND_PCM_FORMAT_U16_LE,
	    SND_PCM_FORMAT_U16_BE,
		SND_PCM_FORMAT_S24_3LE,
		SND_PCM_FORMAT_S24_3BE,
		SND_PCM_FORMAT_U24_3LE,
		SND_PCM_FORMAT_U24_3BE,
	    SND_PCM_FORMAT_S32_LE,
	    SND_PCM_FORMAT_S32_BE,
	    SND_PCM_FORMAT_U32_LE,
	    SND_PCM_FORMAT_U32_BE,
	    SND_PCM_FORMAT_FLOAT_LE,
	    SND_PCM_FORMAT_FLOAT_BE,
	    SND_PCM_FORMAT_FLOAT64_LE,
	    SND_PCM_FORMAT_FLOAT64_BE,
		SND_PCM_FORMAT_S24_LE,
		SND_PCM_FORMAT_S24_BE,
		SND_PCM_FORMAT_U24_LE,
		SND_PCM_FORMAT_U24_BE,
	};

	i32 res1, res2;
	unsigned int min, max;

	snd_pcm_hw_params_t* params;
	snd_pcm_hw_params_alloca(&params);

	res1 = snd_pcm_hw_params_any(id, params);
	SI_STOPIF(res1 < 0, return false);

	device->accesses = SIGAR_DEVICE_ACCESS_UNKNOWN;
	for_range (i, 0, countof(accesses)) {
		res1 = snd_pcm_hw_params_test_access(id, params, accesses[i]);
		if (res1 == 0) {
			device->accesses |= SI_BIT(i);
		}
	}

	device->formats = SIGAR_DEVICE_FORMAT_UNKNOWN;
	for_range (i, 0, countof(formats)) {
		res1 = snd_pcm_hw_params_test_format(id, params, formats[i]);
		if (res1 == 0) {
			device->formats |= SI_BIT(i);
		}
	}

	res1 = snd_pcm_hw_params_get_rate_min(params, &min, nil);
	res2 = snd_pcm_hw_params_get_rate_max(params, &max, nil);

	device->sampleRates[0] = (res1 == 0) ? min : 0;
	device->sampleRates[1] = (res2 == 0) ? max : 0;

	res1 = snd_pcm_hw_params_get_channels_min(params, &min);
	res2 = snd_pcm_hw_params_get_channels_max(params, &max);

	device->channels[0] = (res1 == 0) ? min : 0;
	device->channels[1] = (res2 == 0) ? max : 0;

	return true;
}

siIntern
b32 sigar__alsaFillDeviceInfoName(cstring name, siAudioDeviceInfo* device) {
	snd_pcm_t* id;
	b32 res = snd_pcm_open(&id, name, SND_PCM_STREAM_PLAYBACK, 0);
	SI_STOPIF(res != 0, return false);

	res = sigar__alsaFillDeviceInfoID(id, device);
	snd_pcm_close(id);

	usize len = si_cstrLen(name);
	si_memcopy(device->identifier.alsa, name, len + 1);

	return res;
}



#endif


SIDEF
siAudioDevice sigar_deviceMake(siAudioDeviceType type) {
	siAudioDevice device = {0};
	device.config.type = type;
	device.config.callback = sigar__callbackOutputDefault;

#if SI_SYSTEM_IS_APPLE
	AudioObjectPropertyAddress properties = {
		kAudioHardwarePropertyDefaultOutputDevice,
		kAudioObjectPropertyScopeGlobal,
		SIGAR__COREAUDIO_AUDIO_OBJECT_PROPERTY_ELEMENT
	};

	u32 size = sizeof(device.device);
	i32 res = AudioObjectGetPropertyData(
		kAudioObjectSystemObject, &properties, 0, nil, &size, &device.id
	);
	SI_ASSERT(res == 0);

	properties.mSelector = kAudioDevicePropertyDeviceIsAlive;
	res = AudioObjectAddPropertyListener(device.id, &properties, sigar__callbackDeviceUnplugged, audio);
	SI_ASSERT(res == 0);

#elif SI_SYSTEM_IS_UNIX
	i32 defaultFound = SIGAR_ERROR_DEVICE;
	for_range (i, 0, countof(sigar__alsaDefaultDevices)) {
		i32 res = snd_pcm_open(
			&device.id, sigar__alsaDefaultDevices[i],
			SND_PCM_STREAM_PLAYBACK, 0
		);

		if (res == 0) {
			defaultFound = SIGAR_SUCCESS;
			break;
		}
	}
	SI_ERROR_DECLARE(device.status, defaultFound);
#endif

	return device;
}
SIDEF
siAudioDevice sigar_deviceMakeID(siAudioDeviceID identifier) {
	siAudioDevice device = {0};
	device.config.callback = sigar__callbackOutputDefault;

#if SI_SYSTEM_IS_APPLE

#else
	SI_ASSERT_NOT_NULL(identifier.alsa);

	i32 res = snd_pcm_open(&device.id, identifier.alsa, SND_PCM_STREAM_PLAYBACK, 0);
	SI_ERROR_DECLARE(device.status, (res == 0) ? SIGAR_SUCCESS : SIGAR_ERROR_DEVICE);

#endif

	return device;
}
inline
siAudioDeviceConfig* sigar_deviceConfigEnable(siAudioDevice* device) {
	SI_ASSERT_NOT_NULL(device);

	siAudioDeviceConfig* config = &device->config;
	config->isSet = true;
	return config;
}


SIDEF
usize sigar_deviceGetAmount(void) {
#if SI_SYSTEM_IS_APPLE
	AudioObjectPropertyAddress properties = {
		kAudioHardwarePropertyDevices,
		kAudioObjectPropertyScopeGlobal,
		SIGAR__COREAUDIO_AUDIO_OBJECT_PROPERTY_ELEMENT
	};
	u32 size;
	AudioObjectGetPropertyDataSize(kAudioObjectSystemObject, &properties, 0, nil, &size);
	return size / sizeof(AudioObjectID);
#else
	usize len = 0;
	snd_pcm_t* id;
	i32 res;

	/* NOTE(EimaMei): Check if a "default" exists, because it might not! */
	for_range (i, 0, countof(sigar__alsaDefaultDevices)) {
		res = snd_pcm_open(&id, sigar__alsaDefaultDevices[i], SND_PCM_STREAM_PLAYBACK, 0);

		if (res == 0) {
			len += 1;
			snd_pcm_close(id);
			break;
		}
	}

	char** ppDeviceHints;
	res = snd_device_name_hint(-1, "pcm", (void***)&ppDeviceHints);
	SI_ASSERT(res == 0);

	char** ppNextDeviceHint = ppDeviceHints;
    while (*ppNextDeviceHint != nil) {
		char* IOID = snd_device_name_get_hint(*ppNextDeviceHint, "IOID");
		SI_STOPIF(IOID == nil, ppNextDeviceHint += 1; continue);

		if (si_memcompare(IOID, "Output", countof_str("Output")) == 0) {
			char* NAME = snd_device_name_get_hint(*ppNextDeviceHint, "NAME");

			res = snd_pcm_open(&id, NAME, SND_PCM_STREAM_PLAYBACK, 0);
			if (res == 0) {
				snd_pcm_hw_params_t* params;
				snd_pcm_hw_params_alloca(&params);
				res = snd_pcm_hw_params_any(id, params);

				len += (res >= 0);
				snd_pcm_close(id);
			}
			free(NAME);
		}

		free(IOID);
		ppNextDeviceHint += 1;
	}
	snd_device_name_free_hint((void**)ppDeviceHints);

	return len;
#endif
}

SIDEF
siAudioDeviceInfo sigar_deviceGetInfo(siAudioDevice device) {
	SIGAR_ASSERT_DEVICE(&device);

	siAudioDeviceInfo info = {0};
	sigar__alsaFillDeviceInfoID(device.id, &info);

	return info;
}
SIDEF
usize sigar_deviceGetInfoAll(siAudioDeviceInfo* outDevices, usize length) {
	SI_ASSERT_NOT_NULL(outDevices);
	SI_STOPIF(length == 0, return 0);

#if SI_SYSTEM_IS_APPLE

#elif SI_SYSTEM_IS_UNIX
	/* NOTE(EimaMei): Since Linux has no good way of checking the default output
	 * device (because Linux) other than to check if "default" exists, we'll just
	 * do that. If it exists, 'i' will start as 1, otherwise the index will start
	 * at 0 and hopefully index 0 of 'snd_device_name_hint' will give the default
	 * option, not some random sound card. */
	usize i;
	for_range (j, 0, countof(sigar__alsaDefaultDevices)) {
		i = sigar__alsaFillDeviceInfoName(sigar__alsaDefaultDevices[j], &outDevices[0]);
		SI_STOPIF(i == 1, break);
	}

	char** ppDeviceHints;
	b32 res = snd_device_name_hint(-1, "pcm", (void***)&ppDeviceHints);
	SI_ASSERT(res == 0);

	char** ppNextDeviceHint = ppDeviceHints;
    while (i < length && *ppNextDeviceHint != nil) {
		char* IOID = snd_device_name_get_hint(*ppNextDeviceHint, "IOID");
		SI_STOPIF(IOID == nil, ppNextDeviceHint += 1; continue);

		if (si_memcompare(IOID, "Output", countof_str("Output")) == 0) {
			char* NAME = snd_device_name_get_hint(*ppNextDeviceHint, "NAME");
			res = sigar__alsaFillDeviceInfoName(NAME, &outDevices[i]);

			i += res;
			free(NAME);
		}

		free(IOID);
		ppNextDeviceHint += 1;
	}
	snd_device_name_free_hint((void**)ppDeviceHints);

	return i;
#endif
}

SIDEF
siString sigar_audioErrorToStr(siAudioError error) {
	switch (error) {
		case SIGAR_UNKNOWN: return SI_STR("SIGAR_UNKNOWN");
		case SIGAR_SUCCESS: return SI_STR("SIGAR_SUCCESS");
		case SIGAR_ERROR: return SI_STR("SIGAR_ERROR");
		case SIGAR_ERROR_FORMAT: return SI_STR("SIGAR_ERROR_FORMAT");
		case SIGAR_ERROR_DEVICE: return SI_STR("SIGAR_ERROR_DEVICE");
		case SIGAR_ERROR_THREAD: return SI_STR("SIGAR_ERROR_THREAD");
		case SIGAR_ERROR_POLL: return SI_STR("SIGAR_ERROR_POLL");
	}

	return SI_STR("SIGAR_ERROR_INVALID");
}
SIDEF
siString sigar_deviceFormatToStr(siAudioDeviceFormat format) {
	switch (format) {
		case SIGAR_DEVICE_FORMAT_S8: return SI_STR("SIGAR_DEVICE_FORMAT_S8");
		case SIGAR_DEVICE_FORMAT_U8: return SI_STR("SIGAR_DEVICE_FORMAT_U8");

		case SIGAR_DEVICE_FORMAT_S16LE: return SI_STR("SIGAR_DEVICE_FORMAT_S16LE");
		case SIGAR_DEVICE_FORMAT_S16BE: return SI_STR("SIGAR_DEVICE_FORMAT_S16BE");

		case SIGAR_DEVICE_FORMAT_U16LE: return SI_STR("SIGAR_DEVICE_FORMAT_U16LE");
		case SIGAR_DEVICE_FORMAT_U16BE: return SI_STR("SIGAR_DEVICE_FORMAT_U16BE");

		case SIGAR_DEVICE_FORMAT_S24LE: return SI_STR("SIGAR_DEVICE_FORMAT_S24LE");
		case SIGAR_DEVICE_FORMAT_S24BE: return SI_STR("SIGAR_DEVICE_FORMAT_S24BE");

		case SIGAR_DEVICE_FORMAT_U24LE: return SI_STR("SIGAR_DEVICE_FORMAT_U24LE");
		case SIGAR_DEVICE_FORMAT_U24BE: return SI_STR("SIGAR_DEVICE_FORMAT_U24BE");

		case SIGAR_DEVICE_FORMAT_S32LE: return SI_STR("SIGAR_DEVICE_FORMAT_S32LE");
		case SIGAR_DEVICE_FORMAT_S32BE: return SI_STR("SIGAR_DEVICE_FORMAT_S32BE");

		case SIGAR_DEVICE_FORMAT_U32LE: return SI_STR("SIGAR_DEVICE_FORMAT_U32LE");
		case SIGAR_DEVICE_FORMAT_U32BE: return SI_STR("SIGAR_DEVICE_FORMAT_U32BE");

		case SIGAR_DEVICE_FORMAT_F32LE: return SI_STR("SIGAR_DEVICE_FORMAT_F32LE");
		case SIGAR_DEVICE_FORMAT_F32BE: return SI_STR("SIGAR_DEVICE_FORMAT_F32BE");

		case SIGAR_DEVICE_FORMAT_F64LE: return SI_STR("SIGAR_DEVICE_FORMAT_F64LE");
		case SIGAR_DEVICE_FORMAT_F64BE: return SI_STR("SIGAR_DEVICE_FORMAT_F64BE");

		case SIGAR_DEVICE_FORMAT_S24LE_32: return SI_STR("SIGAR_DEVICE_FORMAT_S24LE_32");
		case SIGAR_DEVICE_FORMAT_S24BE_32: return SI_STR("SIGAR_DEVICE_FORMAT_S24BE_32");

		case SIGAR_DEVICE_FORMAT_U24LE_32: return SI_STR("SIGAR_DEVICE_FORMAT_U24LE_32");
		case SIGAR_DEVICE_FORMAT_U24BE_32: return SI_STR("SIGAR_DEVICE_FORMAT_U24BE_32");
	}

	return SI_STR("SIGAR_DEVICE_FORMAT_UNKNOWN");
}
SIDEF
siString sigar_deviceAccessToStr(siAudioDeviceAccess access) {
	switch (access) {
		case SIGAR_DEVICE_ACCESS_INTERLEAVED: return SI_STR("SIGAR_DEVICE_ACCESS_INTERLEAVED");
		case SIGAR_DEVICE_ACCESS_NONINTERLEAVED: return SI_STR("SIGAR_DEVICE_ACCESS_NONINTERLEAVED");
	}

	return SI_STR("SIGAR_DEVICE_ACCESS_UNKNOWN");
}


SIDEF
void sigar_deviceStart(siAudioDevice* device) {
	SI_ASSERT_NOT_NULL(device);
	SIGAR_ASSERT_DEVICE(device);


	if (device->state == SIGAR_PAUSED) {
		pthread_mutex_lock(&device->mutex);
		device->state = SIGAR_RUNNING;
		pthread_cond_signal(&device->cond);
		pthread_mutex_unlock(&device->mutex);
	}
	else if (device->state == SIGAR_CLOSED || device->state == SIGAR_STOPPED) {
		sigar__alsaDeviceInit(device);
		device->state = SIGAR_RUNNING;

		b32 res = si_threadMake(
			siFunc(sigar__threadOutputCallback), device, true, &device->thread
		);

		if (res == false) {
			SI_ERROR_DECLARE(device->status, SIGAR_ERROR_THREAD);
			return;
		}
	}
}

inline
void sigar_devicePause(siAudioDevice* device) {
	SI_ASSERT_NOT_NULL(device);
	SIGAR_ASSERT_DEVICE(device);

	if (device->state == SIGAR_RUNNING) {
		device->state = SIGAR_PAUSED;
	}
}
void sigar_deviceWait(siAudioDevice* device, b32 forceUnpause) {
	SI_ASSERT_NOT_NULL(device);
	SIGAR_ASSERT_DEVICE(device);
	SI_STOPIF(device->state != SIGAR_RUNNING, return);


	if (forceUnpause) {
		/* NOTE(EimaMei): Force a wake-up from a poll. */
		u64 t = 1;
		write(device->pfd[0].fd, &t, sizeof(t));

		/* NOTE(EimaMei): Force a wake-up from a possible pause. */
		if (!device->numberOfSources) {
			pthread_mutex_lock(&device->mutex);
			if (device->state == SIGAR_PAUSED) {
				device->state = SIGAR_RUNNING;
			}
			pthread_cond_signal(&device->cond);
			pthread_mutex_unlock(&device->mutex);
		}
	}
	si_threadJoin(&device->thread);
}


SIDEF
void sigar_deviceClose(siAudioDevice* device) {
	SI_ASSERT_NOT_NULL(device);
	SIGAR_ASSERT_DEVICE(device);

	device->audioHead = nil;
	device->state = SIGAR_CLOSED;

#if SI_SYSTEM_IS_APPLE
	AudioObjectPropertyAddress properties = {
		kAudioDevicePropertyDeviceIsAlive,
		kAudioObjectPropertyScopeGlobal,
		SIGAR__COREAUDIO_AUDIO_OBJECT_PROPERTY_ELEMENT
	};
	AudioObjectRemovePropertyListener(
		device->id, &properties,
		sigar__callbackDeviceUnplugged, audio
	);
#else
	if (device->thread.isRunning) {
		sigar_deviceWait(device, true);
	}

	if (device->pfd[0].fd) {
		close(device->pfd[0].fd);
	}

	if (device->alloc.ptr) {
		si_allocatorFree(&device->alloc);
	}

	if (device->id) {
		snd_pcm_drop(device->id);
		snd_pcm_drain(device->id);
		snd_pcm_close(device->id);
	}

	if (device->thread.id) {
		si_threadDestroy(&device->thread);
		pthread_mutex_destroy(&device->mutex);
		pthread_cond_destroy(&device->cond);
	}
#endif

	si_memset(device, 0, sizeof(*device));
}

inline
siAudio* sigar_devicePollAudioHead(const siAudioDevice* device) {
	siAudio* audio = device->audioHead;

	while (audio && (audio)->state == SIGAR_PAUSED) {
		audio = audio->nextUnit;
	}

	return audio;
}

#define sigar__audioSetNextValid(audio, out) \
	do { \
		audio = (audio)->nextUnit; \
	} while (audio && (audio)->state == SIGAR_PAUSED); \
	*(out) = audio

void sigar_devicePollAudio(const siAudioDevice* device, siAudio** outAudio) {
	SI_ASSERT_NOT_NULL(device);
	SI_ASSERT_NOT_NULL(outAudio);

	siAudio* audio = *outAudio;
	const siAudioDeviceConfig* config = &device->config;

	usize remaining = audio->end - audio->__offset;
	usize length = config->frameCount * audio->channels * sigar_formatSize(audio->format);

	if (SI_UNLIKELY(remaining < length)) {
		audio->__offset = audio->start;

		if (audio->loops != 0) {
			SI_STOPIF(audio->loops > 0, audio->loops -= 1);
			sigar__audioSetNextValid(audio, outAudio);
			return;
		}

		sigar_audioClose(audio);
		sigar__audioSetNextValid(audio, outAudio);
		return;
	}

	audio->__offset += length;
	sigar__audioSetNextValid(audio, outAudio);
}
#undef sigar__audioSetNextValid


SIDEF
siAudio sigar_audioMakeEx(siAudioDevice* device, rawptr buffer, usize length,
		siSampleFormat format, siSampleRate rate, u32 channels) {
	SI_ASSERT_NOT_NULL(device);
	SIGAR_ASSERT_DEVICE(device);
	SIGAR_ASSERT_FORMAT(format);

	/* NOTE(EimaMei): Iškart nustatomas į nulį, kad nesusigadinčiau savo ausų
	 * įvykus "techniniam nesklandumui". */
	siAudio audio = {0};

#if SI_SYSTEM_IS_APPLE
	// sigar_deviceClose
	// audio.frameCount = ...;
	SI_PANIC();

 	OSStatus res;

	AudioUnit unit;
	{
		AudioComponentDescription desc;
		desc.componentType = kAudioUnitType_Output;
		desc.componentManufacturer = kAudioUnitManufacturer_Apple;
		desc.componentSubType = kAudioUnitSubType_HALOutput;
		desc.componentFlags = 0;
		desc.componentFlagsMask = 0;

		AudioComponent comp = AudioComponentFindNext(nil, &desc);
		res = AudioComponentInstanceNew(comp, &unit);
		SI_ASSERT(res == 0);
	}

	{
		res = AudioUnitSetProperty(
			unit, kAudioOutputUnitProperty_CurrentDevice, kAudioUnitScope_Global,
			0, &device->device, sizeof(device->device)
		);
		SI_ASSERT(res == 0);

		u32 enableIO = true;
		res = AudioUnitSetProperty(
			unit, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Output,
            0, &enableIO, sizeof(enableIO)
		);
		SI_ASSERT(res == 0);
	}
	{

		AudioStreamBasicDescription asbd;
		asbd.mSampleRate = rate;
		asbd.mFormatID = kAudioFormatLinearPCM;
		asbd.mFormatFlags = kAudioFormatFlagIsPacked;
		asbd.mFramesPerPacket = 1;
		asbd.mChannelsPerFrame = channels;
		asbd.mBitsPerChannel = format & SIGAR_SAMPLE_SIZE;
		asbd.mBytesPerPacket = asbd.mBytesPerFrame = (asbd.mBitsPerChannel / 8) * asbd.mChannelsPerFrame;
		asbd.mReserved = 0;

		if (format & SIGAR_SAMPLE_FLOAT) {
			asbd.mFormatFlags |= kAudioFormatFlagIsFloat;
		}
		else if (format & SIGAR_SAMPLE_SIGNED) {
			asbd.mFormatFlags |= kAudioFormatFlagIsSignedInteger;
		}
		if (format & SIGAR_SAMPLE_ENDIAN) {
			asbd.mFormatFlags |= kAudioFormatFlagIsBigEndian;
		}

		res = AudioUnitSetProperty(
			unit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 0,
			&asbd, sizeof(asbd)
		);
		SI_ASSERT(res == 0);
	}
	audio.id = unit;

	res = AudioUnitInitialize(unit);
	SI_ASSERT(res == 0);
#else

	siAudioDeviceConfig* config = &device->config;
	if (!config->isSet) {
		if (sigar_formatIsPrefered(format, config->format)) {
			config->format = sigar__alsaCheckFormat(format);
		}
		if (channels > config->channels) {
			config->channels = channels;
		}
		if (rate > config->rate) {
			config->rate = rate;
		}
	}

	audio.nextUnit = nil;
#endif

	audio.buffer = buffer;
	audio.length = length;
	audio.device = device;

	audio.rate = rate;
	audio.channels = channels;
	audio.format = format;

	audio.start = 0;
	audio.end = length;
	audio.loops = 0;

	audio.bytesPerSecond = rate * channels * (u32)sigar_formatSize(format);
	audio.state = SIGAR_CLOSED;
	audio.__offset = 0;

	return audio;
}

SIDEF
siAudioError sigar_audioPlay(siAudio* audio) {
	SI_ASSERT_NOT_NULL(audio);
	SIGAR_ASSERT_DEVICE(audio->device);
	SI_STOPIF(audio->state == SIGAR_RUNNING, return true);

#if SI_SYSTEM_IS_APPLE
	if (!audio->isClosed) {
		i32 res = AudioOutputUnitStart(audio->id);
		audio->isRunning = (res == 0);
	}
	else {
	    AURenderCallbackStruct callback;
	    callback.inputProc = sigar__callbackOutput;
	    callback.inputProcRefCon = audio;

	    i32 res = AudioUnitSetProperty(
			audio->id, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Output, 0,
			&callback, sizeof(callback)
		);
		SI_ASSERT(res == 0);

		res = AudioOutputUnitStart(audio->id);
		audio->isRunning = (res == 0);
		audio->isClosed = !audio->isRunning;
	}
#elif SI_SYSTEM_IS_UNIX
	siAudioDevice* device = audio->device;

	if (device->audioHead != nil) {
		device->numberOfSources += 1;

		if (audio->state == SIGAR_PAUSED) {
			audio->state = SIGAR_RUNNING;
			sigar_deviceStart(device);
			return true;
		}

		siAudio* next = device->audioHead;
		while (next->nextUnit != nil) {
			next = next->nextUnit;
		}

		audio->state = SIGAR_RUNNING;
		audio->__offset = audio->start;
		next->nextUnit = audio;

		return true;
	}
	/* NOTE(EimaMei): If the device was started through 'sigar_deviceStart'. */
	else if (device->state == SIGAR_RUNNING) {
		audio->__offset = audio->start;
		audio->state = SIGAR_RUNNING;

		device->audioHead = audio;
		device->numberOfSources += 1;
	}

	audio->__offset = audio->start;
	audio->state = SIGAR_RUNNING;
	device->audioHead = audio;
	device->numberOfSources += 1;

	sigar_deviceStart(device);
#endif

	return audio->state;
}

SIDEF
b32 sigar_audioPause(siAudio* audio) {
	SI_ASSERT_NOT_NULL(audio);

	if (audio->state == SIGAR_RUNNING) {
#if SI_SYSTEM_IS_APPLE
		i32 status = AudioOutputUnitStop(audio->id);
		audio->isRunning = !(status == 0);
#elif SI_SYSTEM_IS_UNIX
		siAudioDevice* device = audio->device;
		audio->state = SIGAR_PAUSED;
		device->numberOfSources -= 1;

		if (device->numberOfSources == 0) {
			device->state = SIGAR_PAUSED;
		}
#endif
	}

	return (audio->state == SIGAR_PAUSED);
}

SIDEF
b32 sigar_audioClose(siAudio* audio) {
	SI_ASSERT_NOT_NULL(audio);

	if (audio->state != SIGAR_CLOSED) {
#if SI_SYSTEM_IS_APPLE
		i32 status = AudioUnitUninitialize(audio->id);
		status = AudioComponentInstanceDispose(audio->id);
		audio->isRunning = !(status == 0);
		audio->isClosed = !audio->isRunning;

#elif SI_SYSTEM_IS_UNIX
		siAudioDevice* device = audio->device;
		siAudio* audioHead = device->audioHead;
		if (audioHead == audio) {
			device->audioHead = audio->nextUnit;
		}
		else {
			while (audioHead->nextUnit != audio) {
				audioHead = audioHead->nextUnit;
			}
			audioHead->nextUnit = audioHead->nextUnit->nextUnit;
		}

		/* NOTE(EimaMei): Doesn't decrement if the audio was paused before closure. */
		device->numberOfSources -= (audio->state == SIGAR_RUNNING);
		audio->state = SIGAR_CLOSED;
#endif
	}

	return true;
}

inline
siBuffer sigar_audioCurrentBufferGet(const siAudio* audio) {
	return (siBuffer){(siByte*)&audio->buffer[audio->__offset], audio->__offset - audio->end};
}

inline
void sigar_audioSeek(siAudio* audio, u32 miliseconds) {
	SI_ASSERT_NOT_NULL(audio);

	usize samples = (f32)audio->bytesPerSecond * ((f32)miliseconds / 1000.f);
	SI_ASSERT(samples <= audio->end);
	audio->__offset = samples;
}
inline
f32 sigar_audioTell(siAudio audio) {
	return (f32)audio.__offset / (f32)audio.bytesPerSecond;
}


inline
f32 sigar_audioGetStart(siAudio audio) {
	return (f32)audio.start / (f32)audio.bytesPerSecond;
}
inline
void sigar_audioSetStart(siAudio* audio, u32 miliseconds) {
	SI_ASSERT_NOT_NULL(audio);

	usize samples = (f32)audio->bytesPerSecond * ((f32)miliseconds / 1000.f);
	SI_ASSERT(samples <= audio->end);

	audio->start = samples;
}

inline
f32 sigar_audioGetEnd(siAudio audio) {
	return (f32)audio.end / (f32)audio.bytesPerSecond;
}
inline
void sigar_audioSetEnd(siAudio* audio, u32 miliseconds) {
	SI_ASSERT_NOT_NULL(audio);

	usize samples = (f32)audio->bytesPerSecond * ((f32)miliseconds / 1000.f);
	SI_ASSERT(samples <= audio->length);

	audio->end = samples;
}

inline
f32 sigar_audioGetDuration(siAudio audio) {
	usize time = audio.end - audio.start;
	return (f32)time / (f32)audio.bytesPerSecond;
}


#if !defined(SIGAR_NO_DEFAULT_MIXING)

typedef struct { u8 m[3]; } si__u8x3;

force_inline
void sigar__mixTypes_f32f32(f32* outPtr, f32 inValue) {
	f32 outValue = *outPtr + inValue;

	if (outValue > 1.0f) { outValue = 1.0f; }
	else if (outValue < -1.0f) { outValue= -1.0f; }

	*outPtr = outValue;
}
force_inline
void sigar__mixTypes_i16i16(i16* outPtr, i16 inValue) {
	i32 outValue = *outPtr + inValue;

	if (outValue > INT16_MAX) { outValue = INT16_MAX; }
	else if (outValue < INT16_MIN) { outValue = INT16_MIN; }

	*outPtr = (i16)outValue;
}
force_inline
void sigar__mixTypes_u8u8(u8* outPtr, u8 inValue) {
	u32 outValue = *outPtr;
	outValue += inValue - (outValue * inValue) / 256;

	*outPtr = (u8)outValue;
}


SIDEF
void sigar_upmixBufferToStereo(siByte* restrict output, const siByte* restrict input,
		usize frameCount, u32 sizePerChannel) {

	switch (sizePerChannel) {
		case 1: {
			for_range (i, 0, frameCount) {
				output[i * 2 + 0] = input[i];
				output[i * 2 + 1] = input[i];
			}
		} break;

		case 2: {
			u16* out = (u16*)output, *in = (u16*)input;
			for_range (i, 0, frameCount) {
				out[i * 2 + 0] = in[i];
				out[i * 2 + 1] = in[i];
			}
		} break;

		case 3: {
			si__u8x3* out = (si__u8x3*)output, *in = (si__u8x3*)input;
			for_range (i, 0, frameCount) {
				out[i * 2 + 0] = in[i];
				out[i * 2 + 1] = in[i];
			}
		} break;

		case 4: {
			u32* out = (u32*)output, *in = (u32*)input;
			for_range (i, 0, frameCount) {
				out[i * 2 + 0] = in[i];
				out[i * 2 + 1] = in[i];
			}
		} break;
		default: SI_PANIC();
	}
}

SIDEF
void sigar_mixBuffers(siByte* restrict output, siSampleFormat formatOut,
		const siByte* restrict input, siSampleFormat formatIn, usize len) {
	SI_ASSERT_NOT_NULL(output);
	SI_ASSERT_NOT_NULL(input);
	SI_ASSERT(sigar_formatIsNative(formatOut));
	SI_ASSERT(sigar_formatIsNative(formatIn));

	if (formatOut == formatIn) {
		switch (formatOut) {
			case SIGAR_FORMAT_U8:      sigar_mixUInt8ToUInt8((u8*)output,   (u8*)input, len); return;
			case SIGAR_FORMAT_I16:     sigar_mixInt16ToInt16((i16*)output, (i16*)input, len); return;
			case SIGAR_FORMAT_F32: sigar_mixFloat32ToFloat32((f32*)output, (f32*)input, len); return;
			default: return;
		}
	}

	switch (formatOut) {
		case SIGAR_FORMAT_F32:
			switch (formatIn) {
				case SIGAR_FORMAT_U8:  sigar_mixUInt8ToFloat32((f32*)output,  (u8*)input, len); return;
				case SIGAR_FORMAT_I16: sigar_mixInt16ToFloat32((f32*)output, (i16*)input, len); return;
				case SIGAR_FORMAT_I24: sigar_mixInt24ToFloat32((f32*)output,  (u8*)input, len); return;
				case SIGAR_FORMAT_I32: sigar_mixInt32ToFloat32((f32*)output, (i32*)input, len); return;
			} break;

		case SIGAR_FORMAT_I16:
			switch (formatIn) {
				case SIGAR_FORMAT_U8:  sigar_mixUInt8ToInt16((i16*)output,   (u8*)input, len); return;
				case SIGAR_FORMAT_I24: sigar_mixInt24ToInt16((i16*)output,   (u8*)input, len); return;
				case SIGAR_FORMAT_I32: sigar_mixInt32ToInt16((i16*)output,   (i32*)input, len); return;
				case SIGAR_FORMAT_F32: sigar_mixFloat32ToInt16((i16*)output, (f32*)input, len); return;
			} break;

		case SIGAR_FORMAT_U8:
			switch (formatIn) {
				case SIGAR_FORMAT_I16: sigar_mixInt16ToUInt8((u8*)output, (i16*)input, len); return;
				case SIGAR_FORMAT_I24: sigar_mixInt24ToUInt8((u8*)output,  (u8*)input, len); return;
				case SIGAR_FORMAT_I32: sigar_mixInt32ToUInt8((u8*)output, (i32*)input, len); return;
				case SIGAR_FORMAT_F32: sigar_mixFloat32ToUInt8((u8*)output, (f32*)input, len); return;
			} break;
	}
}



SIDEF
void sigar_mixUInt8ToUInt8(u8* restrict output, const u8* restrict input, usize len) {
	for_range (i, 0, len) {
		sigar__mixTypes_u8u8(&output[i], input[i]);
	}
}

SIDEF
void sigar_mixInt16ToUInt8(u8* output, const i16* input, usize len) {
	for_range (i, 0, len) {
		i32 inValue = input[i];
		inValue += INT16_MAX;
		inValue /= UINT8_MAX + 1;
		inValue -= 1;

		sigar__mixTypes_u8u8(&output[i], (u8)inValue);
	}
}

SIDEF
void sigar_mixInt24ToUInt8(u8* restrict output, const siByte* restrict input,
		usize len) {
	const si__u8x3* inPtr = (si__u8x3*)input;

	usize i;
	for (i = 0; i < len; i += 1) {
		i32 inValue = *(i32*)&inPtr[i] & 0xFFFFFFu;
		SI_STOPIF(inValue & 0x800000, inValue |= ~0xFFFFFFu);
		inValue += (1 << 23) - 1;
		inValue /= UINT16_MAX + 1;

		sigar__mixTypes_u8u8(&output[i], (u8)inValue);
	}

	i32 inValue = 0;
	si_memcopy(&inValue, &inPtr[i], 3);
	inValue = (inValue ^ 0x800000) - 0x800000;
	inValue += (1 << 23) - 1;
	inValue /= UINT16_MAX + 1;

	sigar__mixTypes_u8u8(&output[i], (u8)inValue);

}

SIDEF
void sigar_mixInt32ToUInt8(u8* output, const i32* input, usize len) {
	for_range (i, 0, len) {
		i32 inValue = input[i] + (INT32_MAX - 1);
		inValue /= UINT16_MAX + 1;
		inValue /= UINT8_MAX + 1;

		sigar__mixTypes_u8u8(&output[i], (u8)inValue);
	}
}

SIDEF
void sigar_mixFloat32ToUInt8(u8* output, const f32* input, usize len) {
	for_range (i, 0, len) {
		f32 inValue = input[i] * 127 + 128;
		sigar__mixTypes_u8u8(&output[i], (u8)inValue);
	}
}


SIDEF
void sigar_mixUInt8ToInt16(i16* output, const u8* input, usize len) {
	for_range (i, 0, len) {
		i32 inValueI32 = (i32)input[i] - 128;
		inValueI32 *= 256;

		sigar__mixTypes_i16i16(&output[i], (i16)inValueI32);
	}
}

SIDEF
void sigar_mixInt16ToInt16(i16* restrict output, const i16* restrict input, usize len) {
	for_range (i, 0, len) {
		sigar__mixTypes_i16i16(&output[i], input[i]);
	}
}

SIDEF
void sigar_mixInt24ToInt16(i16* output, const siByte* input, usize len) {
	si__u8x3* inPtr = (si__u8x3*)input;

	usize i;
	for (i = 0; i < len - 1; i += 1) {
		i32 inValue = *(i32*)&inPtr[i] & 0xFFFFFFu;
		inValue = (inValue ^ 0x800000) - 0x800000;
		inValue >>= 8;

		sigar__mixTypes_i16i16(&output[i], (i16)inValue);
	}

	i32 inValue = 0;
	si_memcopy(&inValue, &inPtr[i], 3);
	inValue = (inValue ^ 0x800000) - 0x800000;
	inValue >>= 8;

	sigar__mixTypes_i16i16(&output[i], (u8)inValue);
}

SIDEF
void sigar_mixInt32ToInt16(i16* output, const i32* input, usize len) {
	for_range (i, 0, len) {
		i32 inValue = input[i] >> 16;
		sigar__mixTypes_i16i16(&output[i], (i16)inValue);
	}
}

SIDEF
void sigar_mixFloat32ToInt16(i16* output, const f32* input, usize len) {
	for_range (i, 0, len) {
		f32 inValue = input[i];
		inValue *= INT16_MAX;
		sigar__mixTypes_i16i16(&output[i], (i16)inValue);
	}
}


SIDEF
void sigar_mixUInt8ToFloat32(f32* output, const u8* input, usize len) {
	for_range (i, 0, len) {
		f32 inValue = input[i] - 128;
		inValue /= INT8_MAX + 1;

		sigar__mixTypes_f32f32(&output[i], inValue);
	}
}

SIDEF
void sigar_mixInt16ToFloat32(f32* output, const i16* input, usize len) {
	for_range (i, 0, len) {
		f32 inValue = input[i];
		inValue /= INT16_MAX + 1;

		sigar__mixTypes_f32f32(&output[i], inValue);
	}
}

SIDEF
void sigar_mixInt24ToFloat32(f32* output, const siByte* input, usize len) {
	si__u8x3* inPtr = (si__u8x3*)input;

	usize i;
	for (i = 0; i < len - 1; i += 1) {
		i32 inValue = *(i32*)&inPtr[i] & 0xFFFFFFu;
		inValue = (inValue ^ 0x800000) - 0x800000;

		f32 inValueFloat = (f32)inValue;
		inValueFloat /= 0x800000;

		sigar__mixTypes_f32f32(&output[i], inValueFloat);
	}

	i32 inValue = 0;
	si_memcopy(&inValue, &inPtr[i], 3);
	inValue = (inValue ^ 0x800000) - 0x800000;
	inValue >>= 8;

	f32 inValueFloat = (f32)inValue;
	inValueFloat /= 0x800000;

	sigar__mixTypes_f32f32(&output[i], inValueFloat);
}

SIDEF
void sigar_mixInt32ToFloat32(f32* output, const i32* input, usize len) {
	for_range (i, 0, len) {
		f32 inValue = (f32)input[i];
		inValue /= (INT32_MAX + 1);

		sigar__mixTypes_f32f32(&output[i], inValue);
	}
}

SIDEF
void sigar_mixFloat32ToFloat32(f32* restrict output, const f32* restrict input,
		usize len) {
	for_range (i, 0, len) {
		sigar__mixTypes_f32f32(&output[i], input[i]);
	}
}


siIntern
void sigar__callbackOutputDefault(siAudioDevice* device, siByte* restrict output,
		const siByte* input, usize frameCount) {
	siAudioDeviceConfig* config = &device->config;

	siAudio* audio = sigar_devicePollAudioHead(device);
	if (audio == nil) {
		/* NOTE(EimaMei): There are no more audio sources left, leave. For more
		 * information about 'SIGAR_STOPPED', check its documentation. */
		device->state = SIGAR_STOPPED;
		return ;
	}

	while (audio != nil) {
		siBuffer buffer = sigar_audioCurrentBufferGet(audio);
		u8 mixed[SI_KILO(8)];

		if (audio->channels < config->channels) {
			SI_ASSERT(audio->channels == 1);
			SI_ASSERT(frameCount <= SI_KILO(8));

			sigar_upmixBufferToStereo(
				mixed, buffer.data, config->frameCount,
				(u32)sigar_formatSize(audio->format)
			);
			buffer.data = mixed;
		}

		SI_ASSERT(audio->rate == config->rate); /* NOTE(EimaMei): No sample rate resampling. */
		sigar_mixBuffers(output, config->format, buffer.data, audio->format, frameCount);

		sigar_devicePollAudio(device, &audio);
	}

	SI_UNUSED(input);
}

#endif /* !SIGAR_NO_DEFAULT_MIXING */


#if SI_SYSTEM_IS_APPLE
	#undef SIGAR__COREAUDIO_AUDIO_OBJECT_PROPERTY_ELEMENT
#endif
#undef SIGAR_ASSERT_DEVICE

#endif /* SIGAR_IMPLEMENTATION */

#if defined(__cplusplus)
}
#endif

#endif /* SIGAR_INCLUDE_SI_H */

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
