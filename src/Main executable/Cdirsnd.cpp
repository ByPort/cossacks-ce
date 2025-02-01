///////////////////////////////////////////////////////////
// CDIRSND.CPP -- Implementation file for the CDirSound
//                class.
///////////////////////////////////////////////////////////

#include "cdirsnd.h"

static void StreamAudioBuffer(SoundBuffer* pSoundBuffer, Sint16* destSamples, int additionalAmount)
{
	if (!pSoundBuffer->playing)
	{
		return;
	}

	int bufferSize = min(additionalAmount, pSoundBuffer->size - pSoundBuffer->pos);

	// FIXME: the size in bytes should be just additionalAmount since it's the size in bytes, not words
	Sint16* samples = (Sint16*)malloc(sizeof(Sint16) * bufferSize);
	memcpy(samples, (byte*)pSoundBuffer->data + pSoundBuffer->pos, (size_t)bufferSize);
	// Some fade-out effect?
	//if (bufferSize < additionalAmount)
	//{
	//	float endQueue;
	//	for (int i = 0; i < bufferSize; i += 2)
	//	{
	//		endQueue = clip->currentVolume - (clip->currentVolume / bufferSize) * i;
	//		endQueue = endQueue > 0.05f ? endQueue : 0.0f;
	//		samples[i] *= endQueue * clip->leftPan;
	//		samples[i + 1] *= endQueue * clip->rightPan;
	//		destSamples[i] += samples[i];
	//		destSamples[i + 1] += samples[i + 1];
	//	}
	//}
	//else
	if (true)
	{
		// FIXME: bufferSize is size in bytes, and since we operate words in samples, bufferSize/2 iterations are enough
		for (int i = 0; i < bufferSize; i += 2)
		{
			// TODO: calculate float volume and pans in Set functions so we don't load the CPU here
			float volume = pow(10.0f, pSoundBuffer->volume / 2000.0f);
			float leftPan;
			float rightPan;
			if (pSoundBuffer->pan < 0)
			{
				leftPan = 1.0f;
				rightPan = pow(10.0f, pSoundBuffer->pan / 2000.0f);
			}
			else if (pSoundBuffer->pan > 0) {
				leftPan = pow(10.0f, -pSoundBuffer->pan / 2000.0f);
				rightPan = 1.0f;
			}
			else
			{
				leftPan = 1.0f;
				rightPan = 1.0f;
			}
			if (volume < 0.0001f) volume = 0.0f;
			else if (volume > 1.0f) volume = 1.0f;
			if (leftPan < 0.0001) leftPan = 0;
			else if (leftPan > 1.0f) leftPan = 1.0f;
			if (rightPan < 0.0001) rightPan = 0;
			else if (rightPan > 1.0f) rightPan = 1.0f;

			samples[i] *= volume * leftPan;
			samples[i + 1] *= volume * rightPan;
			destSamples[i] += samples[i];
			destSamples[i + 1] += samples[i + 1];
		}
	}
	pSoundBuffer->pos += bufferSize;

	if (pSoundBuffer->pos >= pSoundBuffer->size)
	{
		//if (clip->loop == SDL_TRUE)
		//{
		//	clip->audioLength = 0;
		//	clip->audioPos = clip->wavBuffer;
		//}
		//else
		if (true)
		{
			pSoundBuffer->playing = false;
		}
	}

	free(samples);
}

static void SDLCALL AudioStreamCallback(void* userdata, SDL_AudioStream* stream, int additionalAmount, int totalAmount)
{
	SoundBuffer* pSoundBuffer = (SoundBuffer*)userdata;

	// FIXME: the size in bytes should be just additionalAmount since it's the size in bytes, not words
	Sint16* samples = (Sint16*)malloc(sizeof(Sint16) * additionalAmount);
	memset(samples, '\0', additionalAmount);

	StreamAudioBuffer(pSoundBuffer, samples, additionalAmount);

	Sint8* copiedSamples = (Sint8*)malloc(additionalAmount);
	memset(copiedSamples, '\0', additionalAmount);
	memcpy(copiedSamples, ((const Sint16*)samples), (size_t)additionalAmount);

	Uint8* mixBuffer = (Uint8*)malloc(additionalAmount);
	memset(mixBuffer, '\0', additionalAmount);
	float volume = pow(10.0f, pSoundBuffer->volume / 2000.0f);
	if (!SDL_MixAudio(mixBuffer, (const Uint8*)copiedSamples, SDL_AUDIO_S16, additionalAmount, volume))
	{
		SDL_Log("Error mixing the bus samples %s", SDL_GetError());
	}
	if (!SDL_PutAudioStreamData(stream, mixBuffer, additionalAmount))
	{
		SDL_Log("Could not put data on Audio stream, %s", SDL_GetError());
	}

	free(samples);
	free(copiedSamples);
	free(mixBuffer);
}

///////////////////////////////////////////////////////////
// CDirSound::CDirSound()
//
// This is the class's constructor.
///////////////////////////////////////////////////////////
void CDirSound::CreateDirSound()
{
	// Initialize class data members.
	m_currentBufferNum = 0;

	for (UINT x = 0; x < MAXSND1; ++x)
	{
		m_bufferPointers[x] = NULL;
	}

	// Create the main SDL audio device.
	bool success;
	success = SDL_InitSubSystem(SDL_INIT_AUDIO);
	if (success)
	{
		m_SDLAudioDeviceID = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &m_audioSpec);
		if (m_SDLAudioDeviceID)
		{
			SDL_ResumeAudioDevice(m_SDLAudioDeviceID);
		}
	}
}

///////////////////////////////////////////////////////////
// CDirSound::~CDirSound()
//
// This is the class's destructor.
///////////////////////////////////////////////////////////
CDirSound::~CDirSound()
{
	if (m_SDLAudioDeviceID)
		ReleaseAll();
}

///////////////////////////////////////////////////////////
// CDirSound::ReleaseAll()
//
// This member function releases all the DirectSound
// objects created in the class.
///////////////////////////////////////////////////////////
void CDirSound::ReleaseAll()
{
	// Release all sound buffers.
	for (UINT x = 1; x <= m_currentBufferNum; ++x)
	{
		if (!m_bufferPointers[x]->isDuplicated)
		{
			SDL_free(m_bufferPointers[x]->data);
		}
		free(m_bufferPointers[x]);
		m_bufferPointers[x] = NULL;
	}

	// Release the DirectSound object.
	if (m_SDLAudioDeviceID)
		SDL_CloseAudioDevice(m_SDLAudioDeviceID);
}

///////////////////////////////////////////////////////////
// CDirSound::CreateSoundBuffer()
//
// This member function creates secondary sound buffers.
// The class can accommodate up to MAXSND such buffers.
///////////////////////////////////////////////////////////
UINT CDirSound::CreateSoundBuffer(CWave* pWave)
{
	// Make sure there's room for another buffer.
	if (m_currentBufferNum == MAXSND)
		return 0;

	// Calculate the next available buffer number.
	++m_currentBufferNum;

	m_bufferPointers[m_currentBufferNum] = (SoundBuffer*)malloc(sizeof(SoundBuffer));
	if (!m_bufferPointers[m_currentBufferNum])
	{
		return 0;
	}
	memset(m_bufferPointers[m_currentBufferNum], 0, sizeof(SoundBuffer));

	return m_currentBufferNum;
}
///////////////////////////////////////////////////////////
// CDirSound::DuplicateSoundBuffer()
//
// This member function creates secondary sound buffers.
// The class can accommodate up to MAXSND such buffers.
///////////////////////////////////////////////////////////
UINT CDirSound::DuplicateSoundBuffer(UINT bufferNum)
{
	// TODO: maybe remove this and create different buffer for the same sound

	// Make sure there's room for another buffer.
	if (m_currentBufferNum == MAXSND)
		return 0;
	// Calculate the next available buffer number.
	++m_currentBufferNum;

	m_bufferPointers[m_currentBufferNum] = (SoundBuffer*)malloc(sizeof(SoundBuffer));
	if (!m_bufferPointers[m_currentBufferNum])
	{
		return 0;
	}
	memcpy(m_bufferPointers[m_currentBufferNum], m_bufferPointers[bufferNum], sizeof(SoundBuffer));
	m_bufferPointers[m_currentBufferNum]->isDuplicated = true;

	m_bufferPointers[m_currentBufferNum]->stream = SDL_OpenAudioDeviceStream(m_SDLAudioDeviceID, &m_audioSpec, AudioStreamCallback, m_bufferPointers[m_currentBufferNum]);
	if (m_bufferPointers[m_currentBufferNum]->stream == nullptr)
	{
		return 0;
	}
	if (!SDL_ResumeAudioStreamDevice(m_bufferPointers[m_currentBufferNum]->stream))
	{
		SDL_DestroyAudioStream(m_bufferPointers[m_currentBufferNum]->stream);
		return 0;
	}

	return m_currentBufferNum;
}
///////////////////////////////////////////////////////////
// CDirSound::CopyWaveToBuffer()
//
// This function copies wave data to a DirectSound buffer.
///////////////////////////////////////////////////////////
BOOL CDirSound::CopyWaveToBuffer(CWave* pWave, UINT bufferNum)
{
	bool success;

	// Check for a valid buffer number.
	if ((bufferNum > m_currentBufferNum) || (bufferNum == 0))
		return FALSE;

	// Get a pointer to the requested buffer.
	SoundBuffer* pSoundBuffer =
		m_bufferPointers[bufferNum];

	m_bufferPointers[m_currentBufferNum]->stream = SDL_OpenAudioDeviceStream(m_SDLAudioDeviceID, &m_audioSpec, AudioStreamCallback, m_bufferPointers[m_currentBufferNum]);
	if (m_bufferPointers[m_currentBufferNum]->stream == nullptr)
	{
		return false;
	}
	if (!SDL_ResumeAudioStreamDevice(m_bufferPointers[m_currentBufferNum]->stream))
	{
		SDL_DestroyAudioStream(m_bufferPointers[m_currentBufferNum]->stream);
		return false;
	}


	// Lock the buffer.
	success = SDL_LockAudioStream(pSoundBuffer->stream);
	if (!success)
		return FALSE;

	// Copy the data into the buffer.
	char* pWaveData = pWave->GetWaveDataPtr();
	pSoundBuffer->size = pWave->GetWaveSize();
	pSoundBuffer->data = malloc(pSoundBuffer->size);
	pSoundBuffer->spec = *pWave->GetWaveSpecPtr();
	memcpy(pSoundBuffer->data, pWaveData, pSoundBuffer->size);

	SDL_UnlockAudioStream(pSoundBuffer->stream);

	return TRUE;
}

///////////////////////////////////////////////////////////
// CDirSound::DirectSoundOK()
//
// This member function returns TRUE if the DirectSound
// object was created and initialized okay. Otherwise, it
// returns FALSE.
///////////////////////////////////////////////////////////
// TODO: rename to SDLAudioOK
BOOL CDirSound::DirectSoundOK()
{
	if (!m_SDLAudioDeviceID)
		return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////
// CDirSound::PlaySound()
//
// This member function plays the sound stored in the given
// buffer.
///////////////////////////////////////////////////////////
BOOL CDirSound::PlaySoundNum(UINT bufferNum)
{
	// Check for a valid buffer number.
	if ((bufferNum > m_currentBufferNum) || (bufferNum == 0))
		return FALSE;

	// Get a pointer to the requested buffer.
	SoundBuffer* pSoundBuffer = m_bufferPointers[bufferNum];

	// Make sure the buffer is set to play from the beginning.
	pSoundBuffer->pos = 0;
	// Play the sound.
	pSoundBuffer->playing = true;

	return true;
}
BOOL CDirSound::PlayCoorSound(UINT bufferNum, int x, int vx)
{
	// Check for a valid buffer number.
	if ((bufferNum > m_currentBufferNum) || (bufferNum == 0))
		return FALSE;

	// Get a pointer to the requested buffer.
	SoundBuffer* pSoundBuffer = m_bufferPointers[bufferNum];

	// Make sure the buffer is set to play from the beginning.
	pSoundBuffer->pos = 0;
	// Play the sound.
	pSoundBuffer->playing = true;

	pSoundBuffer->x = x;
	pSoundBuffer->y = vx;

	return TRUE;
}

extern int CenterX;

void CDirSound::ControlPan(UINT bufferNum) 
{
	if (m_bufferPointers[bufferNum]->playing)
	{
		m_bufferPointers[bufferNum]->x += m_bufferPointers[bufferNum]->y;
		int pan = (m_bufferPointers[bufferNum]->x - CenterX) << 1;

		if (pan < -4000)
		{
			pan = -4000;
		}

		if (pan > 4000)
		{
			pan = 4000;
		}

		SetPan(bufferNum, pan);

		if (rand() < 350)
		{
			IsPlaying(bufferNum);
		}
	}
}

void CDirSound::SetVolume(UINT bufferNum, int vol) {
	// Check for a valid buffer number.
	if ((bufferNum > m_currentBufferNum) || (bufferNum == 0))
		return;

	// Get a pointer to the requested buffer.
	SoundBuffer* pSoundBuffer = m_bufferPointers[bufferNum];

	// Make sure the buffer is set to play from the beginning.
	pSoundBuffer->volume = vol;
};
void CDirSound::SetPan(UINT bufferNum, int pan) {
	// Check for a valid buffer number.
	if ((bufferNum > m_currentBufferNum) || (bufferNum == 0))
		return;

	// Get a pointer to the requested buffer.
	SoundBuffer* pSoundBuffer = m_bufferPointers[bufferNum];

	// Make sure the buffer is set to play from the beginning.
	pSoundBuffer->pan;
}

void CDirSound::ProcessSoundSystem()
{
	for (int i = 1; i < min(MAXSND1, m_currentBufferNum+1); i++)
	{
		if (m_bufferPointers[i]->playing)
		{
			ControlPan(i);
		}
	}
}

///////////////////////////////////////////////////////////
// CDirSound::StopSound()
//
// This member function stops the sound stored in the given
// buffer.
///////////////////////////////////////////////////////////
BOOL CDirSound::StopSound(UINT bufferNum)
{
	// Check for a valid buffer number.
	if ((bufferNum > m_currentBufferNum) || (bufferNum == 0))
		return FALSE;

	// Get a pointer to the requested buffer.
	SoundBuffer* pSoundBuffer = m_bufferPointers[bufferNum];

	// Make sure the buffer is set to play from the beginning.
	pSoundBuffer->playing = false;

	return TRUE;
}
int CDirSound::GetPos(UINT bufferNum)
{
	// Check for a valid buffer number.
	if ((bufferNum > m_currentBufferNum) || (bufferNum == 0))
		return 0;

	// Get a pointer to the requested buffer.
	SoundBuffer* pSoundBuffer = m_bufferPointers[bufferNum];

	// Make sure the buffer is set to play from the beginning.
	return pSoundBuffer->pos;

}

bool CDirSound::IsPlaying(UINT bufferNum)
{
	HRESULT result;

	// Check for a valid buffer number.
	if ((bufferNum > m_currentBufferNum) || (bufferNum == 0))
	{
		return 0;
	}

	return false;

	// Get a pointer to the requested buffer.
	SoundBuffer* pSoundBuffer = m_bufferPointers[bufferNum];

	// Make sure the buffer is set to play from the beginning.
	return pSoundBuffer->playing;
}