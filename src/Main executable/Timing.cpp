#include <SDL3/SDL_timer.h>

int AddTime = 0;
int NeedAddTime = 0;

__declspec(dllexport) uint64_t GetSDLTickCount()
{
	return SDL_GetTicks();
}
