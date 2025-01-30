///////////////////////////////////////////////////////////
// CWAVE.CPP: Implementation file for the WAVE class.
///////////////////////////////////////////////////////////

#include "cwave.h"

///////////////////////////////////////////////////////////
// CWave::CWave()
///////////////////////////////////////////////////////////
CWave::CWave(char* fileName)
{
    // Initialize the class's members.
    m_waveSize = 0;
    m_waveOK = false;
    m_pWave= NULL;

    // Load the wave file.
    m_waveOK = LoadWaveFile(fileName);
}

///////////////////////////////////////////////////////////
// CWave::~CWave()
///////////////////////////////////////////////////////////
CWave::~CWave()
{
    // Free the memory assigned to the wave data.
    SDL_free(m_pWave);
}

///////////////////////////////////////////////////////////
// CWave::LoadWaveFile()
//
// This function loads a wave from disk into memory. It
// also initializes various class data members.
///////////////////////////////////////////////////////////
bool CWave::LoadWaveFile(char* fileName)
{
	Uint8* audio_buf;
    Uint32 audio_len;
    bool success = SDL_LoadWAV(fileName, &m_waveSpec, &audio_buf, &audio_len);
    if (!success)
    {
        return false;
    }

    // Have to convert each buffer to a single format to not deal with different formats

    SDL_AudioSpec destSpec = { SDL_AUDIO_S16, 2, 22050 };
    SDL_AudioStream* conversionStream = SDL_CreateAudioStream(&m_waveSpec, &destSpec);
    if (!conversionStream)
    {
        SDL_free(audio_buf);
        return false;
    }
    success = SDL_PutAudioStreamData(conversionStream, audio_buf, audio_len);
    if (!success)
    {
        SDL_free(audio_buf);
        SDL_DestroyAudioStream(conversionStream);
        return false;
    }
    success = SDL_FlushAudioStream(conversionStream);
    if (!success)
    {
        SDL_free(audio_buf);
        SDL_DestroyAudioStream(conversionStream);
        return false;
    }

    // We assume that this will return the whole buffer
    int availableBufLen = SDL_GetAudioStreamAvailable(conversionStream);

    void* convertedData = malloc(availableBufLen);
    int numRead = SDL_GetAudioStreamData(conversionStream, convertedData, availableBufLen);
    if (numRead != availableBufLen)
    {
        SDL_free(audio_buf);
        SDL_DestroyAudioStream(conversionStream);
        free(convertedData);
        return false;
    }

    SDL_free(audio_buf);
    SDL_DestroyAudioStream(conversionStream);

    m_waveSpec = { SDL_AUDIO_S16, 2, 22050 };
    m_pWave = (char*)convertedData;
    m_waveSize = availableBufLen;

 //   SDL_AudioStream* playback = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &m_waveSpec, nullptr, nullptr);
 //   SDL_ResumeAudioStreamDevice(playback);
	//SDL_PutAudioStreamData(playback, m_pWave, m_waveSize);
 //   SDL_Delay(3000);

    return true;
}

///////////////////////////////////////////////////////////
// CWave::GetWaveSize()
//
// This returns the size in bytes of the wave data.
///////////////////////////////////////////////////////////
unsigned long CWave::GetWaveSize()
{
    return m_waveSize;
}

/////////////////////////////////////////////////////////////
//// CWave::GetWaveFormatPtr()
////
//// This function returns a pointer to the wave file's
//// WAVEFORMATEX structure.
/////////////////////////////////////////////////////////////
//LPWAVEFORMATEX CWave::GetWaveFormatPtr()
//{
//    return &m_waveFormatEx;
//}

SDL_AudioSpec* CWave::GetWaveSpecPtr()
{
    return &m_waveSpec;
}

///////////////////////////////////////////////////////////
// CWave::GetWaveDataPtr()
//
// This function returns a pointer to the wave's
// actual sound data.
///////////////////////////////////////////////////////////
char* CWave::GetWaveDataPtr()
{
    return m_pWave;
}

///////////////////////////////////////////////////////////
// CWave::WaveOK()
//
// This function returns a Boolean value indicating whether
// the wave file object was set up successfully.
///////////////////////////////////////////////////////////
bool CWave::WaveOK()
{
    return m_waveOK;
}
