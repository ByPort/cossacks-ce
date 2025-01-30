///////////////////////////////////////////////////////////
// CDIRSND.H -- Header file for the CDirSound class.
///////////////////////////////////////////////////////////

#ifndef __CDIRSND_H
#define __CDIRSND_H

#include "cwave.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

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
    //unsigned long m_bufferSizes[MAXSND1];
	SoundBuffer* m_bufferPointers[MAXSND1];
    SDL_AudioSpec m_audioSpec = { SDL_AUDIO_S16, 2, 22050 };

public:
	//short Volume[MAXSND1];
	//short SrcX[MAXSND1];
	//short SrcY[MAXSND1];
	//unsigned char  BufIsRun[MAXSND1];
	unsigned int m_currentBufferNum;
	void CDirSound::CreateDirSound();
    ~CDirSound();
    unsigned int CreateSoundBuffer(CWave* pWave);
    unsigned int DuplicateSoundBuffer(unsigned int bufferNum);
    bool DirectSoundOK();
	void SetLastVolume(short Vol){
        m_bufferPointers[m_currentBufferNum]->volume=Vol;
	};
    bool CopyWaveToBuffer(CWave* pWave, unsigned int bufferNum);
	void SetVolume(unsigned int bufferNum,int vol);
	void SetPan(unsigned int bufferNum,int pan);
    // GameSound includes windows.h which defines PlaySound as PlaySoundA,
	// so GameSound.obj expects symbol PlaySoundA, and Cdirsnd.obj generates PlaySound
	// TODO: maybe handle this somehow other than renaming the function
    bool PlaySoundN(unsigned int bufferNum);
    bool StopSound(unsigned int bufferNum);
	bool PlayCoorSound(unsigned int bufferNum,int x,int vx);
	void ControlPan(unsigned int bufferNum);
	bool IsPlaying(unsigned int bufferNum);
	int GetPos(unsigned int bufferNum);
	void ProcessSoundSystem();
protected:
    void ReleaseAll();
};

#endif

#define MaxSnd 1024