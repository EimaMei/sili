/*
sigar.h - a cross-platform audio library for playback and recording.
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

Support for Android (AAudio/OpenSL), Web Audio, sndio, OSS and custom backends 
might be considered.

===========================================================================
MACROS
	- For any of the macros to work, you must _always_ define it before including
	the library. Example:
	```c
		#define SIGAR_IMPLEMENTATION
		#define SIGAR_NO_ALSA
		#include "siligar.h"
	```

===========================================================================
DOCUMENTATION
	- All sigar functions, structures, macros, etc generally follow the same
	documentation style of 'sili.h'. Please refer to the original file's
	"Documentation" section if you haven't already as this library assumes that
	you are knowledgable how things are documented.

===========================================================================
LICENSE:
	- This software is licensed under the zlib license (see the LICENSE at the
	bottom of the file).

WARNING
	- Siligar, like sili, is designed to be a fast, modern, but also an experimental
	library and because of it some unwarranted results may occur or be present when
	using the library, those being:
		1) Features may not work as expected
		2) Functions may not be documented or only contain incomplete documentation
		3) API breaking changes between releases.
		4) Little to no security checks for malicious code attempting to explode
		sections of the code.

	- Siligar is also NOT responsible if garbage data gets played, be it through
	incorrect audio format inputs or wrong mixing done by the user or sili.

*/

#ifndef SIGAR_INCLUDE_SI_H
#define SIGAR_INCLUDE_SI_H

#if defined(__cplusplus)
extern "C" {
#endif

#ifndef SI_INCLUDE_SI_H
#include <sili.h>
#endif

#if (defined(SI_COMPILER_MSVC) && SI_COMPILER_VERSION <= SI_VERSION(4, 2, 0)) \
	|| defined(SI_COMPILER_CLANG) || (defined(SI_COMPILER_GCC) && SI_COMPILER_VERSION <= SI_VERSION(3, 4, 0))
	#pragma once
#endif

#ifndef SIGAR_VERSION_MAJOR
	#define SIGAR_VERSION_MAJOR 0
	#define SIGAR_VERSION_MINOR 1
	#define SIGAR_VERSION_PATCH 0
#endif
#define SIGAR_VERSION_CURRENT SI_VERSION(SIGAR_VERSION_MAJOR, SIGAR_VERSION_MINOR, SIGAR_VERSION_PATCH)


#if SI_SYSTEM_IS_APPLE
	#include <CoreAudio/CoreAudio.h>
	#include <AudioToolbox/AudioToolbox.h>
#elif SI_SYSTEM_IS_UNIX
	#include <alsa/asoundlib.h>
#endif



/* x - UINT
 * Converts seconds into miliseconds. */
#define SIGAR_SEC(x) ((x) * 1000)
/* x - UINT
 * Converts minutes into miliseconds. */
#define SIGAR_MIN(x) (SIGAR_SEC(x) * 60)
/* x - UINT
 * Converts hours into miliseconds */
#define SIGAR_HRS(x) (SIGAR_MIN(x * 60)
/* hours - UINT | minutes - UINT | seconds - UINT
 * Converts a 'hours:minutes:seconds' timestamp into miliseconds. */
#define SIGAR_TIMESTAMP(hours, minutes, seconds) \
	SIGAR_SEC(hours * 3600 + minutes * 60 + seconds)


/* Sample size mask to get the size of the sample format (in bits). */
#define SIGAR_SAMPLE_SIZE    0xFF
/* Sample signedness bit, the sample is signed if the bit is set. */
#define SIGAR_SAMPLE_SIGNED  SI_BIT(15)
/* Sample floating-point bit, the sample is a floating-point if the bit is set. */
#define SIGAR_SAMPLE_FLOAT   SI_BIT(14)
/* Sample endianness bit, the sample is in big-endian if the bit is set. */
#define SIGAR_SAMPLE_ENDIAN  SI_BIT(13)

/* Enumeration for sigar supported sample formats. */
SI_ENUM(u32, siSampleFormat) {
	SIGAR_FORMAT_U8    = 0000000000000000000 | 8,
	SIGAR_FORMAT_I8    = SIGAR_SAMPLE_SIGNED | 8,

	SIGAR_FORMAT_I16LE = SIGAR_SAMPLE_SIGNED | 16,
	SIGAR_FORMAT_I16BE = SIGAR_SAMPLE_ENDIAN | SIGAR_FORMAT_I16LE,

	SIGAR_FORMAT_I24LE = SIGAR_SAMPLE_SIGNED | 24,
	SIGAR_FORMAT_I24BE = SIGAR_SAMPLE_ENDIAN | SIGAR_FORMAT_I24LE,

	SIGAR_FORMAT_I32LE = SIGAR_SAMPLE_SIGNED | 32,
	SIGAR_FORMAT_I32BE = SIGAR_SAMPLE_ENDIAN | SIGAR_FORMAT_I32LE,

	SIGAR_FORMAT_F32LE = SIGAR_SAMPLE_FLOAT  | 32,
	SIGAR_FORMAT_F32BE = SIGAR_SAMPLE_ENDIAN | SIGAR_FORMAT_F32LE,

	SIGAR_FORMAT_F64LE = SIGAR_SAMPLE_FLOAT  | 64,
	SIGAR_FORMAT_F64BE = SIGAR_SAMPLE_ENDIAN | SIGAR_FORMAT_F64LE,

#if SI_ENDIAN_IS_LITTLE
	SIGAR_FORMAT_I16 = SIGAR_FORMAT_I16LE,
	SIGAR_FORMAT_I24 = SIGAR_FORMAT_I24LE,
	SIGAR_FORMAT_I32 = SIGAR_FORMAT_I32LE,

	SIGAR_FORMAT_F32 = SIGAR_FORMAT_F32LE,
	SIGAR_FORMAT_F64 = SIGAR_FORMAT_F64LE,
#else
	SIGAR_FORMAT_I16 = SIGAR_FORMAT_I16BE,
	SIGAR_FORMAT_I24 = SIGAR_FORMAT_I24BE,
	SIGAR_FORMAT_I32 = SIGAR_FORMAT_I32BE,

	SIGAR_FORMAT_F32 = SIGAR_FORMAT_F32BE,
	SIGAR_FORMAT_F64 = SIGAR_FORMAT_F64BE,

#endif
};


/* Type definition for sample rate. */
typedef u32 siSampleRate;

/* Struct representing an audio device. */
typedef struct siAudioDevice {
	/* Status of the audio device. If the value is negative, an error occured
	 * when initializing the device, otherwise it's set to zero. */
	i32 status;

	/* */
	siSampleRate rate;
	/* */
	siSampleFormat format;
	/* */
	u32 channels;
	/* The numbers of samples that one frame will contain */
	u32 frameSize;

#if SI_SYSTEM_IS_APPLE
	/* Apple-specific audio object identifier for the physical device. */
	AudioObjectID id;
#elif SI_SYSTEM_IS_UNIX
	u32 arePlaying;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	siThread thread;
	siAllocator alloc;
	snd_pcm_t* id;
	struct siAudio* unitHead;
#endif
/*  Function callback that gets called every frame during playback
 * - audio: Audio unit that's being processed.
   - output: The frame that will be played by the sound card.
   - input: Pointer to the current frame of the input audio data.
   - length: The total size of the audio frame in bytes. */
	void SI_FUNC_PTR(callback, (struct siAudio* audio, siByte* restrict output,
				const siByte* restrict input, usize length));
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

	/* Number of times the audio should loop after the initial run. outValue negative
	 * value loops the unit indefinitely, zero closes the audio unit, and if the
	 * member is still a natural number, it gets decremented and the unit gets
	 * looped. */
	i32 loops;


	/* === User-readable settings === */
	/* NOTE: Editing any of the following might result in unwarranted results. */

	/* The number of bytes that one second of samples occupies. */
	u32 bytesPerSecond;

	/* Indicates whether the audio unit is closed. */
	b32 isClosed;
	/* Indicates whether the audio unit is currently playing. */
	b32 isRunning;

	/* The start of the audio unit playback (in bytes). */
	usize start;
	/* The end of the audio unit playback (in bytes). */
	usize end;

	/* == Internal settings == */
	/* NOTE: Musn't be set outside of sigar's header. */

	/* Current offset of the unit (in bytes). */
	usize __offset;
} siAudio;




/* Finds the default output device and returns it. */
SIDEF siAudioDevice sigar_deviceMake(void);
/* Finds the specified-device and returns it. */
SIDEF siAudioDevice sigar_deviceMakeIndex(usize i);

/* Gets the total number of available audio devices. */
SIDEF usize sigar_deviceGetAmount(void);

/* Exits and frees any allocated objects. */
SIDEF void sigar_deviceClose(siAudioDevice* device);


/* Creates an audio object from a specified buffer. */
SIDEF siAudio sigar_audioMakeEx(siAudioDevice* device, rawptr buffer, usize length,
    siSampleFormat format, siSampleRate rate, u32 channels);
/* TODO(EimaMei): Needs a rework. */

SIDEF siAudio sigar_audioMakeCallback(siAudioDevice* device, siSampleFormat format,
    siSampleRate rate, u32 channels);


/* Starts playing the given audio object. Returns 'true' is the object is already
 * playing or has been run successfully, otherwise 'false' is returned. */
SIDEF b32 sigar_audioPlay(siAudio* audio);
/* Pauses the playback of the given audio object. Returns 'true' is the object
 * was already paused or has been paused successfully, otherwise 'false' is returned. */
SIDEF b32 sigar_audioPause(siAudio* audio);

/* Stops and completely unitializes the given audio object. */
SIDEF b32 sigar_audioClose(siAudio* audio);


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



#if defined(SIGAR_IMPLEMENTATION)



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

force_inline
u32 sigar__fastMod(u32 input, u32 ceil) {
    return (input >= ceil)
		? input % ceil
		: input;
}

force_inline
void sigar__mixFloats(f32* outPtr, f32 inValue) {
	f32 outValue = *outPtr + inValue;
	if (outValue > 1.0f) {
		outValue = 1.0f;
	} else if (outValue < -1.0f) {
		outValue= -1.0f;
    }
	*outPtr = outValue;
}

siIntern
void sigar__callbackOutputDefault(siAudio* audio, siByte* restrict output,
		const siByte* restrict input, usize length) {
	/* NOTE(EimaMei): List of format support for mixing. 
	SIGAR_FORMAT_U8 +
	SIGAR_FORMAT_I8 -

	SIGAR_FORMAT_I16LE -
	SIGAR_FORMAT_I16BE - 

	SIGAR_FORMAT_I24LE - 
	SIGAR_FORMAT_I24BE -

	SIGAR_FORMAT_I32LE -
	SIGAR_FORMAT_I32BE -

	SIGAR_FORMAT_F32LE -
	SIGAR_FORMAT_F32BE -

	SIGAR_FORMAT_F64LE -
	SIGAR_FORMAT_F64BE -

	SIGAR_FORMAT_I16 -
	SIGAR_FORMAT_I24 -
	SIGAR_FORMAT_I32 -

	SIGAR_FORMAT_F32 +
	SIGAR_FORMAT_F64 -
	
	NOTE 1: sigar currently doesn't have support for playing non-native endian data.
	NOTE 2: sigar will most likely not support mixing multiple sources with different 
	sample rates, at least for the time being.
	*/

	siAudioDevice* device = audio->device;
	SI_ASSERT(audio->rate == device->rate);
	SI_ASSERT((audio->format & SIGAR_SAMPLE_ENDIAN) == (device->format & SIGAR_SAMPLE_ENDIAN));

	usize i = 0;
	usize end = device->frameSize;

	if (audio->format == device->format) {
		if (audio->format & SIGAR_SAMPLE_FLOAT) {
			f32* out = (f32*)output;
			f32* outPtr;
			const f32* in = (const f32*)input;
			const f32* inPtr;

			//siTimeStamp ts = si_timeStampStart();
			for_range (i, 0, end) {
				inPtr = &in[i * audio->channels];

				for_range (j, 0, device->channels) {
					f32 inValue = inPtr[sigar__fastMod(j, audio->channels)];
					outPtr = &out[i * device->channels + j];

					sigar__mixFloats(outPtr, inValue);
				}
			}
		}

		else if (audio->format & SIGAR_SAMPLE_SIGNED) {
			SI_PANIC();
			usize stride = (audio->format & SIGAR_SAMPLE_SIZE) / 8;
			u32 max = 2 << stride;
			while (i < end) {

			}
		}
		else {
			u8* out = (u8*)output;
			u8* outPtr;
			const u8* inPtr;

		//	siTimeStamp ts = si_timeStampStart();
			for_range (i, 0, end) {
				inPtr = &input[i * audio->channels];
				for_range (j, 0, device->channels) {
					outPtr = &out[i * device->channels + j];
					u32 outValue = *outPtr,
						inValue = inPtr[sigar__fastMod(j, audio->channels)];

					outValue += inValue - (outValue * inValue) / 256;
					*outPtr = (u8)outValue;
				}
			}
		}
	}
	else {
		if (device->format & SIGAR_SAMPLE_FLOAT) {

			//usize stride = (audio->format & SIGAR_SAMPLE_SIZE) / 8;

			/* NOTE(EimaMei): In the context of alsa in sigar, this condition
			 * is only met when the audio format is float 32-bit and the device
			 * format is float 64-bit. */
			if (audio->format & SIGAR_SAMPLE_FLOAT) {
				f64* out = (f64*)output;
				f64* outPtr;
				SI_PANIC();
			}
			else if (audio->format & SIGAR_SAMPLE_SIGNED) {

			}
			else {
				/* NOTE(EimaMei): SIGAR_FORMAT_U8 -> SIGAR_FORMAT_F32 */
				if ((device->format & SIGAR_SAMPLE_SIZE) == 32) {
					f32* out = (f32*)output;
					f32* outPtr;
					const u8* inPtr;

					for_range (i, 0, end) {
						inPtr = &input[i * audio->channels];
						for_range (j, 0, device->channels) {
							outPtr = &out[i * device->channels + j];

							f32 inValue = (f32)(inPtr[sigar__fastMod(j, audio->channels)] - 128);
							inValue /= 128.f;

							sigar__mixFloats(outPtr, inValue);
						}
					}
				}
				else {
					SI_PANIC();
				}
			}
		}
		else { SI_PANIC(); }
	}
}

siIntern
void sigar__callbackOutput(siAudioDevice* device) {
	u32 optimalSleep = (device->frameSize * 1000) / device->rate;

start:
	while (device->unitHead != nil) {
		siByte* frame = device->alloc.ptr;
		si_memset(frame, 0, device->alloc.capacity);

		siAudio* audio = device->unitHead;
		while (audio != nil) {
			if (!audio->isRunning) {
				/* NOTE(EimaMei): Every track is paused, meaning we can pause
				 * the thread too. */
				if (!device->arePlaying) {
					pthread_mutex_lock(&device->mutex);
					while (!device->arePlaying) {
						pthread_cond_wait(&device->cond, &device->mutex);
					}
					pthread_mutex_unlock(&device->mutex);
					goto start;
				}

				audio = audio->nextUnit;
				continue;
			}

			usize remaining = audio->end - audio->__offset;
			siByte* audioBuffer = (siByte*)&audio->buffer[audio->__offset];

			usize frameSize = device->frameSize * audio->channels
				* (audio->format & SIGAR_SAMPLE_SIZE) / 8;
			device->callback(audio, frame, audioBuffer, frameSize);

			if (SI_UNLIKELY(remaining < frameSize)) {
				audio->__offset = audio->start;

				if (audio->loops != 0) {
					if (audio->loops > 0) {
						audio->loops -= 1;
					}
					audio = audio->nextUnit;
					continue;
				}

				sigar_audioClose(audio);
				if (device->unitHead != nil) {
					audio = audio->nextUnit;
					continue;
				}
				else {
					return ;
				}
			}

			audio->__offset += frameSize;
			audio = audio->nextUnit;
		}


		i64 status = snd_pcm_writei(device->id, frame, device->frameSize);//device->frameSize >> formatSize);
		if (status < 0) {
			if (status == -EAGAIN) {
				si_sleep(1);
				continue;
			}
			else if (status == -EPIPE) {
				snd_pcm_prepare(device->id);
			}
			status = snd_pcm_recover(device->id, (int)status, 0);
			if (status < 0) {
				break;
			}
		}

		si_sleep(optimalSleep);
	}
}

siIntern
snd_pcm_format_t sigar__alsaToFormat(siSampleFormat format) {
	if (format & SIGAR_SAMPLE_FLOAT) {
		return SND_PCM_FORMAT_FLOAT_LE + (format & SIGAR_SAMPLE_ENDIAN) + ((format & SIGAR_SAMPLE_SIZE) / 64) * 2;
	}

	u32 n = SND_PCM_FORMAT_UNKNOWN + (format & SIGAR_SAMPLE_SIZE) / 8 + !(format & SIGAR_SAMPLE_SIGNED);
	return n;
}

#endif

SIDEF
siAudioDevice sigar_deviceMake(void) {
	siAudioDevice device;

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
	/* NOTE(EimaMei): Palyginus su Core Audio, ALSA tikrai nelepina. */
	i32 res = snd_pcm_open(&device.id, "default", SND_PCM_STREAM_PLAYBACK, 0);
	SI_ASSERT(res == 0);

	device.arePlaying = 0;
	device.unitHead = nil;
	device.alloc.ptr = nil;
#endif
	device.status = 0;
	device.rate = 0;
	device.format = 0;
	device.channels = 0;
	device.frameSize = 0;
	device.callback = sigar__callbackOutputDefault;

	return device;
}

SIDEF
siAudioDevice sigar_deviceMakeIndex(usize i) {
	siAudioDevice device;

#if SI_SYSTEM_IS_APPLE
	u32 index;
	i32 res;
	AudioObjectPropertyAddress properties = {
		kAudioHardwarePropertyDevices,
		kAudioObjectPropertyScopeGlobal,
		SIGAR__COREAUDIO_AUDIO_OBJECT_PROPERTY_ELEMENT
	};
	AudioObjectID devices[1024];

	index = ((u32)i + 1) * sizeof(AudioObjectID);
	res = AudioObjectGetPropertyData(kAudioObjectSystemObject, &properties, 0, nil, &index, devices);

	if (res != 0) {
		device.status = -1;
		return device;
	}

	device.device = devices[i];
	device.status = 0;
#else
	SI_PANIC_MSG("Currently unsupported.");
#endif

	return device;
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
	SI_PANIC_MSG("Currently unsupported.");
	return 0;
#endif
}

SIDEF
void sigar_deviceClose(siAudioDevice* device) {
	SI_ASSERT_NOT_NULL(device);

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
	device->unitHead = nil;
	if (!device->arePlaying) {
		device->arePlaying = 1;
		pthread_mutex_lock(&device->mutex);
		pthread_cond_signal(&device->cond);
		pthread_mutex_unlock(&device->mutex);
	}

	si_threadJoin(&device->thread);

	if (device->alloc.ptr) {
		si_allocatorFree(&device->alloc);
		device->alloc.ptr = nil;
	}
	i32 res = snd_pcm_drain(device->id);
	SI_ASSERT(res == 0);

	res = snd_pcm_close(device->id);
	SI_ASSERT(res == 0);

	si_threadDestroy(&device->thread);
    pthread_mutex_destroy(&device->mutex);
    pthread_cond_destroy(&device->cond);
#endif
	si_memset(device, 0, sizeof(*device));
}



inline
siAudio sigar_audioMakeCallback(siAudioDevice* device, siSampleFormat format,
		siSampleRate rate, u32 channels) {
	siAudio audio = sigar_audioMakeEx(device, nil, USIZE_MAX, format, rate, channels);
	//audio.callback = callback;
	audio.loops = -1;

	return audio;
}

SIDEF
siAudio sigar_audioMakeEx(siAudioDevice* device, rawptr buffer, usize length,
		siSampleFormat format, siSampleRate rate, u32 channels) {
	SI_ASSERT_NOT_NULL(device);

	/* NOTE(EimaMei): Iškart nustatomas į nulį, kad nesusigadinčiau savo ausų
	 * įvykus "techniniam nesklandumui". */
	siAudio audio = {0};

#if SI_SYSTEM_IS_APPLE
	// sigar_deviceClose
	// audio.frameSize = ...;
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
#if SI_ENDIAN_IS_LITTLE
	#define SIGAR_ALLOW_BE 0
#else
	#define SIGAR_ALLOW_BE 1
#endif

	snd_pcm_hw_params_t* params;
	snd_pcm_hw_params_alloca(&params);
	snd_pcm_hw_params_any(device->id, params);

	b32 changeFrameSize = false;
	i32 res = snd_pcm_hw_params_set_access(device->id, params, SND_PCM_ACCESS_RW_INTERLEAVED);
	SI_ASSERT(res == 0);

	if ((format & SIGAR_SAMPLE_SIZE) >= (device->format & SIGAR_SAMPLE_SIZE)
			&& (format & SIGAR_SAMPLE_ENDIAN) == SIGAR_ALLOW_BE) {
		res = snd_pcm_hw_params_set_format(device->id, params, sigar__alsaToFormat(format));
		if (res == 0){
			device->format = format;
			changeFrameSize = true;
		}
		else {
			si_printf("Failed to change format.\n");
		}
	}
	else {
		snd_pcm_hw_params_set_format(device->id, params, sigar__alsaToFormat(device->format));
	}

	if (channels > device->channels) {
		res = snd_pcm_hw_params_set_channels(device->id, params, channels);
		if (res == 0){
			device->channels = channels;
			changeFrameSize = true;
		}
		else {
			si_printf("Failed to change channels.\n");
		}
	} else {
		snd_pcm_hw_params_set_channels(device->id, params, device->channels);
	}

	if (rate > device->rate) {
		res = snd_pcm_hw_params_set_rate_near(device->id, params, &rate, 0);
		if (res == 0){
			device->rate = rate;
			changeFrameSize = true;
		}
		else {
			si_printf("Failed to change channels.\n");
		}
	}
	else {
		snd_pcm_hw_params_set_rate_near(device->id, params, &device->rate, 0);
	}

	res = snd_pcm_hw_params(device->id, params);
	SI_ASSERT(res == 0);

	if (changeFrameSize) {
		unsigned long frames;
		res = snd_pcm_hw_params_get_period_size(params, &frames, 0);
		SI_ASSERT(res == 0);

		device->frameSize = (u32)frames;
	}

	audio.nextUnit = nil;

	#undef SIGAR_ALLOW_BE
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

	audio.bytesPerSecond = rate * channels * (format & SIGAR_SAMPLE_SIZE) / 8;
	audio.isRunning = false;
	audio.isClosed = true;
	audio.__offset = 0;

	return audio;
}

SIDEF
b32 sigar_audioPlay(siAudio* audio) {
	SI_STOPIF(audio->isRunning, return true);

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

	if (device->unitHead != nil) {
		if (audio->isClosed) {
			siAudio* next = device->unitHead;
			while (next->nextUnit != nil) {
				next = audio->nextUnit;
			}
			next->nextUnit = audio;
			audio->isRunning = true;
			audio->isClosed = false;
			audio->__offset = audio->start;
		}
		else { /* Paused */
			pthread_mutex_lock(&device->mutex);
			audio->isRunning = true;
			pthread_cond_signal(&device->cond);
			pthread_mutex_unlock(&device->mutex);
		}
	}
	else {
		device->unitHead = audio;
		audio->__offset = audio->start;

		if (device->alloc.ptr == nil) {
			u32 frameTotalSize = device->frameSize * device->channels *
				(device->format & SIGAR_SAMPLE_SIZE) / 8;
			device->alloc = si_allocatorMake(frameTotalSize);
		}
		pthread_mutex_init(&device->mutex, nil);
		pthread_cond_init(&device->cond, nil);

		audio->isRunning = si_threadMake(sigar__callbackOutput, device, true, &device->thread);
		audio->isClosed = !audio->isRunning;
		si_sleep(1);
	}
	device->arePlaying += audio->isRunning;

#endif

	return audio->isRunning;
}

SIDEF
b32 sigar_audioPause(siAudio* audio) {
	SI_ASSERT_NOT_NULL(audio);
	SI_STOPIF(audio->isClosed, return false);

	if (audio->isRunning) {
#if SI_SYSTEM_IS_APPLE
		i32 status = AudioOutputUnitStop(audio->id);
		audio->isRunning = !(status == 0);
#elif SI_SYSTEM_IS_UNIX
		audio->isRunning = false;
		audio->device->arePlaying -= 1;
#endif
	}

	return !audio->isRunning;
}

SIDEF
b32 sigar_audioClose(siAudio* audio) {
	SI_ASSERT_NOT_NULL(audio);

	if (!audio->isClosed) {
#if SI_SYSTEM_IS_APPLE
		i32 status = AudioUnitUninitialize(audio->id);
		status = AudioComponentInstanceDispose(audio->id);
		audio->isRunning = !(status == 0);
		audio->isClosed = !audio->isRunning;

#elif SI_SYSTEM_IS_UNIX
		siAudio* next = audio->device->unitHead;
		while (next != audio) {
			next = audio->nextUnit;
		}
		if (next == audio->device->unitHead) {
			audio->device->unitHead = nil;
		}
		else {
			next->nextUnit = audio->nextUnit;
		}
		/* NOTE(EimaMei): Doesn't decrement if the audio was already paused. */
		audio->device->arePlaying -= audio->isRunning;

		audio->isClosed = true;
		audio->isRunning = false;
#endif
	}

	return audio->isClosed;
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

#if SI_SYSTEM_IS_APPLE
	#undef SIGAR__COREAUDIO_AUDIO_OBJECT_PROPERTY_ELEMENT
#endif

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
