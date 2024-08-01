/*
sigar.h - a cross-platform audio library for playback and recording.
===========================================================================
	- YOU MUST DEFINE 'SIGAR_IMPLEMENTATION' in EXACTLY _one_ C file that includes
	this header, BEFORE the include like this:

		#define SIGAR_IMPLEMENTATION
		#include "sigar.h"

	- All other files should just include the library without the #define macro.

===========================================================================
MACROS
	- For any of the macros to work, you must _always_ define it before including
	the library. Example:
	```c
		#define SIGAR_IMPLEMENTATION
		#define SI_RELEASE_MODE
		#include "sili.h"
	```

===========================================================================
LICENSE:
	- This software is licensed under the zlib license (see the LICENSE at the
	bottom of the file).

WARNING
	- This library is _slightly_ experimental and features may not work as
	expected.
	- This also means that functions may not be documented.

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
#endif


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


	SIGAR_FORMAT_I16 = SIGAR_FORMAT_I16LE,
	SIGAR_FORMAT_I24 = SIGAR_FORMAT_I24LE,
	SIGAR_FORMAT_I32 = SIGAR_FORMAT_I32LE,

	SIGAR_FORMAT_F32 = SIGAR_FORMAT_F32LE,
	SIGAR_FORMAT_F64 = SIGAR_FORMAT_F64LE,
};

/* Type definition for sample rate. */
typedef u32 siSampleRate;

/* Struct representing an audio device. */
typedef struct {
	/* Status of the audio device. If the value is negative, an error occured
	 * when initializing the device, otherwise it's set to zero. */
	i32 status;

#if SI_SYSTEM_IS_APPLE
	/* Apple-specific audio object identifier for the physical device. */
	AudioObjectID id;
#endif
} siAudioDevice;

/* Function pointer type for an audio callback function that gets called every frame.
   - output: Pointer to the buffer where the processed audio data will be stored.
   - input: Pointer to the buffer containing the input audio data.
   - frameSize: The size of one frame of audio data in bytes. */
typedef void SI_FUNC_PTR(siAudioCallbackFunction, (siByte* output, const siByte* input, u32 frameSize));

typedef struct {
	/* The device that the audio unit is connected to. */
	siAudioDevice* device;

	/* User-specified buffer. */
	const siByte* buffer;
	/* User-specified buffer's length (in bytes). */
	usize length;

#if SI_SYSTEM_IS_APPLE
	/* Apple-specific audio object identifier for the audio unit. */
	AudioUnit id;
#endif

	/* The number of bytes that one second of samples occupies. */
	u32 bytesPerSecond;
	/* The numbers of bytes that one frame of audio contains. If this is set to
	 * zero, this gets automatically set by the device. */
	u32 frameSize;
	/* Duration of the audio data in the buffer, measured in seconds. */
	f32 duration;
	/* Number of times the audio should loop after the initial run. A negative
	 * value loops the unit indefinitely, zero closes the audio unit, and if the
	 * member is still a natural number, it gets decremented and the unit gets
	 * looped. */
	i32 loops;

	/* Function pointer for a callback function, which is called each audio frame
	 * with the parameters being the audio buffer, its size and the input being
	 * this structure's buffer offsetted by '__offset'.  */
	siAudioCallbackFunction callback;

	/* Indicates whether the audio unit is closed.
	 * NOTE: This musn't be changed or written over for ordinary use. */
	b32 isClosed;
	/* Indicates whether the audio unit is currently playing.
	 * NOTE: This musn't be changed or written over for ordinary use. */
	b32 isRunning;

	/* Internal offset that tracks the current position of the audio buffer.
	 * NOTE: This musn't be changed or written over for ordinary use. */
	usize __offset;
} siAudio;




/* Finds the default output device and returns it. */
SIDEF siAudioDevice sigar_deviceMake(void);
/* Finds the specified-device and returns it. */
SIDEF siAudioDevice sigar_deviceMakeIndex(usize i);

/* Gets the total number of available audio devices. */
SIDEF usize sigar_deviceGetAmount(void);



/* Creates an audio object from a specified buffer. */
SIDEF siAudio sigar_audioMakeEx(siAudioDevice* device, rawptr buffer, usize length,
    siSampleFormat format, siSampleRate rate, u32 channels);
/* Creates an audio object that uses an indefinitely-looped callback function
 * with no specified input buffer. */
SIDEF siAudio sigar_audioMakeCallback(siAudioDevice* device, siSampleFormat format,
    siSampleRate rate, u32 channels, siAudioCallbackFunction callback);


/* Starts playing the given audio object. Returns 'true' is the object is already
 * playing or has been run successfully, otherwise 'false' is returned. */
SIDEF b32 sigar_audioPlay(siAudio* audio);
/* Pauses the playback of the given audio object. Returns 'true' is the object
 * was already paused or has been paused successfully, otherwise 'false' is returned. */
SIDEF b32 sigar_audioPause(siAudio* audio);

/* Stops and completely unitializes the given audio object. */
SIDEF b32 sigar_audioClose(siAudio* audio);


/* Seeks the playback position of the given audio object to a specific time. */
void sigar_audioSeekTo(siAudio* audio, u32 milliseconds);



#if defined(SIGAR_IMPLEMENTATION)


siIntern
void sigar__callbackOutputDefault(siByte* output, const siByte* input, u32 frameSize) {
    memcpy(output, input, frameSize);
}

#if SI_SYSTEM_IS_APPLE

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

			memset((siByte*)buffer->mData + len, 0, len);
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
		kAudioObjectSystemObject, &properties, 0, nil, &size, &device.device
	);
	device.status = (res == 0) - 1;
#else
	SI_PANIC_MSG("Currently unsupported.");
#endif

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


inline
siAudio sigar_audioMakeCallback(siAudioDevice* device, siSampleFormat format,
		siSampleRate rate, u32 channels, siAudioCallbackFunction callback) {
	siAudio audio = sigar_audioMakeEx(device, nil, USIZE_MAX, format, rate, channels);
	audio.callback = callback;
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
#endif

	audio.bytesPerSecond = rate * channels * (format & SIGAR_SAMPLE_SIZE) / 8;
	audio.duration = (f32)length / (f32)audio.bytesPerSecond;

	audio.device = device;
	audio.callback = sigar__callbackOutputDefault;

	audio.buffer = buffer;
	audio.length = length;
	audio.__offset = 0;
	audio.frameSize = 0;
	audio.loops = 0;

	audio.isRunning = false;
	audio.isClosed = true;

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
	else if (!audio->isRunning) {
	    AURenderCallbackStruct callback;
	    callback.inputProc = sigar__callbackOutput;
	    callback.inputProcRefCon = audio;

	    i32 res = AudioUnitSetProperty(
			audio->id, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Output, 0,
			&callback, sizeof(callback)
		);
		SI_ASSERT(res == 0);

		AudioObjectPropertyAddress properties = {
			kAudioDevicePropertyDeviceIsAlive,
			kAudioObjectPropertyScopeGlobal,
			SIGAR__COREAUDIO_AUDIO_OBJECT_PROPERTY_ELEMENT
		};
		res = AudioObjectAddPropertyListener(audio->device->device, &properties, sigar__callbackDeviceUnplugged, audio);
		SI_ASSERT(res == 0);

		res = AudioOutputUnitStart(audio->id);
		audio->isRunning = (res == 0);
		audio->isClosed = !audio->isRunning;
	}
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
#endif
	}

	return !audio->isRunning;
}

SIDEF
b32 sigar_audioClose(siAudio* audio) {
	SI_ASSERT_NOT_NULL(audio);

	if (!audio->isClosed) {
#if SI_SYSTEM_IS_APPLE
		AudioObjectPropertyAddress properties = {
			kAudioDevicePropertyDeviceIsAlive,
			kAudioObjectPropertyScopeGlobal,
			SIGAR__COREAUDIO_AUDIO_OBJECT_PROPERTY_ELEMENT
		};
		AudioObjectRemovePropertyListener(
			audio->device->device, &properties,
			sigar__callbackDeviceUnplugged, audio
		);

		i32 status = AudioUnitUninitialize(audio->id);
		status = AudioComponentInstanceDispose(audio->id);
		audio->isRunning = !(status == 0);
		audio->isClosed = !audio->isRunning;
#endif
	}

	return audio->isClosed;
}

SIDEF
void sigar_audioSeekTo(siAudio* audio, u32 miliseconds) {
	SI_ASSERT_NOT_NULL(audio);
	audio->__offset = audio->bytesPerSecond * (miliseconds / 1000);
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
