// DeviceCD.cpp : implementation file
//

#include "windows.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_audio.h"
#include "NewCode/stb_vorbis.h"
#pragma pack(1)
#include "DeviceCD.h"
#include <stdio.h>
#include "ResFile.h"
#include "gFile.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void PlayRandomTrack();

static void SDLCALL MusicStreamCallback(void* userdata, SDL_AudioStream* stream, int additionalAmount, int totalAmount)
{
	if (!SDL_GetAudioStreamAvailable(stream))
	{
		PlayRandomTrack();
	}
}


/////////////////////////////////////////////////////////////////////////////
// CDeviceCD
int StartTrack = 2;
int NTracks = 19;
byte TracksMask[32] = { 2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20 };
CDeviceCD::CDeviceCD()
{
	GFILE* f = Gopen("Tracks.cd", "r");
	if (f) {
		Gscanf(f, "%d%d", &StartTrack, &NTracks);
		for (int i = 0; i < NTracks; i++)Gscanf(f, "%d", TracksMask + i);
		Gclose(f);
	};
	Open();
}

CDeviceCD::~CDeviceCD()
{
	Close();
}


bool CDeviceCD::Open()
{
	bool success = SDL_InitSubSystem(SDL_INIT_AUDIO);
	if (!success)
	{
		audioStream = nullptr;
		FOpened = false;
		audioError = true;
		return false;
	}

	SDL_AudioSpec audioSpec = { SDL_AUDIO_S16, 2, 44100 };
	audioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audioSpec, nullptr, nullptr);
	if (!audioStream)
	{
		audioStream = nullptr;
		FOpened = false;
		audioError = true;
		return false;
	}

	success = SDL_ResumeAudioStreamDevice(audioStream);
	if (!success)
	{
		SDL_DestroyAudioStream(audioStream);
		audioStream = nullptr;
		FOpened = false;
		audioError = true;
		return false;
	}

	FOpened = true;
	audioError = false;
	return true;
}

bool CDeviceCD::Close()
{
	if (FOpened)
	{
		SDL_SetAudioStreamGetCallback(audioStream, nullptr, nullptr);
		SDL_DestroyAudioStream(audioStream);

		FOpened = false;
		audioError = false;
		audioStream = nullptr;
		return true;
	}
	else
		return FALSE;
}

bool CDeviceCD::Pause()
{
	

	if (FOpened)
	{
		bool success = SDL_PauseAudioStreamDevice(audioStream);
		audioError = !success;

		if (!audioError)
			return true;
		else
			return false;
	}
	else
		return FALSE;
}

bool CDeviceCD::Resume()
{
	if (FOpened)
	{
		bool success = SDL_ResumeAudioStreamDevice(audioStream);
		audioError = !success;

		if (!audioError)
			return true;
		else
			return false;
	}
	else
		return FALSE;
}

bool CDeviceCD::Stop()
{
	return Pause();
}

DWORD CDeviceCD::GetVolume()
{
	float gain = SDL_GetAudioStreamGain(audioStream);
	if (gain == -1.0f)
	{
		gain = 0.0f;
	}
	return static_cast<DWORD>(round(gain * 100.0f));
}

bool CDeviceCD::SetVolume(DWORD Volume)
{
	// TODO: set volume only if different from current volume to avoid calling it every frame
	bool success = SDL_SetAudioStreamGain(audioStream, static_cast<float>(Volume / 100.0f));

	return 1;
}
bool CDeviceCD::Play(DWORD Track)
{
	if (FOpened)
	{
		bool success;
		
		success = SDL_ClearAudioStream(audioStream);
		if (!success)
		{
			return false;
		}

		int channels;
		int sampleRate;
		short* buffer;
		int dataRead;

		//Uint8* audio_buf;
		//Uint32 audio_len;

		char filename[30];

		sprintf(filename, "Tracks\\Track_%02d.ogg", Track);

		// TODO: probably need to make this in a separate thread to avoid blocking the main thread with decoding
		// (if SDL doesn't call this from a separate thread already)
		dataRead = stb_vorbis_decode_filename(filename, &channels, &sampleRate, &buffer);
		if (dataRead == -1)
		{
			return false;
		}

		SDL_AudioSpec srcSpec = { SDL_AUDIO_S16, channels, sampleRate };
		SDL_AudioSpec destSpec = { SDL_AUDIO_S16, 2, 44100 };
		SDL_AudioStream* conversionStream = SDL_CreateAudioStream(&srcSpec, &destSpec);
		if (!conversionStream)
		{
			SDL_free(buffer);
			return false;
		}
		success = SDL_PutAudioStreamData(conversionStream, buffer, dataRead * channels * 2);
		const char* error = SDL_GetError();
		if (!success)
		{
			SDL_free(buffer);
			SDL_DestroyAudioStream(conversionStream);
			return false;
		}
		success = SDL_FlushAudioStream(conversionStream);
		if (!success)
		{
			SDL_free(buffer);
			SDL_DestroyAudioStream(conversionStream);
			return false;
		}

		int availableBufLen = SDL_GetAudioStreamAvailable(conversionStream);

		void* convertedData = malloc(availableBufLen);
		int numRead = SDL_GetAudioStreamData(conversionStream, convertedData, availableBufLen);
		if (!(numRead == availableBufLen && numRead == dataRead * channels * 2))
		{
			SDL_free(buffer);
			SDL_DestroyAudioStream(conversionStream);
			free(convertedData);
			return false;
		}

		SDL_free(buffer);
		SDL_DestroyAudioStream(conversionStream);

		success = SDL_PutAudioStreamData(audioStream, convertedData, availableBufLen);
		SDL_free(convertedData);
		if (!success)
		{
			return false;
		}
		success = SDL_FlushAudioStream(audioStream);
		if (!success)
		{
			return false;
		}

		success = SDL_SetAudioStreamGetCallback(audioStream, MusicStreamCallback, nullptr);

		return true;
	}
	else
		return false;
}
int PrevTrack3 = -1;
int PrevTrack2 = -1;
int PrevTrack1 = -1;
int NextCommand = -1;
void PlayCDTrack(int Id);
extern int srando();

CDeviceCD CDPLAY;
void PlayCDTrack(int Id) {
	CDPLAY.Play(Id);
};
extern int CurrentNation;
extern int PlayMode;

extern uint64_t GetSDLTickCount();

void PlayRandomTrack()
{
	if (PlayMode == 1 && CurrentNation != -1) {
		PlayCDTrack(TracksMask[CurrentNation]);
		return;
	};
	int Track = -1;
	do {
		// TODO: replace with random
		Track = (((GetSDLTickCount() & 0b111111111111)*NTracks) >> 12) + StartTrack;
		if (Track == PrevTrack1 || Track == PrevTrack2 || Track == PrevTrack3)Track = -1;
	} while (Track == -1);
	PrevTrack3 = PrevTrack2;
	PrevTrack2 = PrevTrack1;
	PrevTrack1 = Track;
	PlayCDTrack(Track);
}

void StopPlayCD()
{
	CDPLAY.Stop();
}

int GetCDVolume()
{
	return static_cast<int>(CDPLAY.GetVolume());
}

void SetCDVolume(int Vol)
{
	CDPLAY.SetVolume(static_cast<DWORD>(Vol));
}