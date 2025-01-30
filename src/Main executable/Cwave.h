///////////////////////////////////////////////////////////
// CWAVE.H: Header file for the WAVE class.
///////////////////////////////////////////////////////////

#ifndef __CWAVE_H
#define __CWAVE_H

#include "SDL3/SDL.h"
#include "SDL3/SDL_audio.h"

class CWave //: public CObject
{
protected:
    unsigned long m_waveSize;
    bool m_waveOK;
    char* m_pWave;
    //WAVEFORMATEX m_waveFormatEx;
    SDL_AudioSpec m_waveSpec;

public:
    CWave(char* fileName);
    ~CWave();

    unsigned long GetWaveSize();
    //LPWAVEFORMATEX GetWaveFormatPtr();
	SDL_AudioSpec* GetWaveSpecPtr();
    char* GetWaveDataPtr();
    bool WaveOK();

protected:
    bool LoadWaveFile(char* fileName);
};

#endif
