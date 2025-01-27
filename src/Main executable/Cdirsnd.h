///////////////////////////////////////////////////////////
// CDIRSND.H -- Header file for the CDirSound class.
///////////////////////////////////////////////////////////

#ifndef __CDIRSND_H
#define __CDIRSND_H

//#include <afxwin.h>
#include <windows.h>
#include "cwave.h"
//#include "dsound.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
//#include <mmsystem.h>

#define MAXSND 600
#define MAXSND1 601

struct SoundBuffer {
    //bool active;
    unsigned int size;
	SDL_AudioSpec spec;
    //int bitsPerSample;
    //int channels;
    //int rate;
    void* data;
    // DirectSound value from -10000 to 0
    int volume;
    // TODO: add pan
    bool playing;
    //bool looping;
    unsigned int pos;
    SDL_AudioStream* stream;
    int x;
    int y;
    // DirectSound value from -10000 to 10000
    long pan;
    bool isDuplicated;
    //std::recursive_mutex mutex;
};

class CDirSound
{
protected:
    //LPDIRECTSOUND m_pDirectSoundObj;
	SDL_AudioDeviceID m_SDLAudioDeviceID;
    //HWND m_hWindow;
    //LPDIRECTSOUNDBUFFER m_bufferPointers[MAXSND1];
    //DWORD m_bufferSizes[MAXSND1];
	SoundBuffer* m_bufferPointers[MAXSND1];
    SDL_AudioSpec m_audioSpec = { SDL_AUDIO_S16, 2, 22050 };

public:
	//short Volume[MAXSND1];
	//short SrcX[MAXSND1];
	//short SrcY[MAXSND1];
	//byte  BufIsRun[MAXSND1];
	UINT m_currentBufferNum;
	void CDirSound::CreateDirSound();
    ~CDirSound();
    UINT CreateSoundBuffer(CWave* pWave);
	UINT DuplicateSoundBuffer(UINT bufferNum);
    BOOL DirectSoundOK();
	void SetLastVolume(short Vol){
        m_bufferPointers[m_currentBufferNum]->volume=Vol;
	};
    BOOL CopyWaveToBuffer(CWave* pWave, UINT bufferNum);
	void SetVolume(UINT bufferNum,int vol);
	void SetPan(UINT bufferNum,int pan);
    BOOL PlaySound(UINT bufferNum);
    BOOL StopSound(UINT bufferNum);
	BOOL PlayCoorSound(UINT bufferNum,int x,int vx);
	void ControlPan(UINT bufferNum);
	bool IsPlaying(UINT bufferNum);
	int GetPos(UINT bufferNum);
	void ProcessSoundSystem();
protected:
    void ReleaseAll();
};

#endif

#define MaxSnd 1024