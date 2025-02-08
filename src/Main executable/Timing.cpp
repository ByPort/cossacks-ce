#include <SDL3/SDL_timer.h>

int AddTime = 0;
int NeedAddTime = 0;

// This is mostly assigned to int or DWORD variables, which is fine as far as the game session takes less than 24 days
__declspec(dllexport) uint64_t GetSDLTickCount()
{
	return SDL_GetTicks();
}
