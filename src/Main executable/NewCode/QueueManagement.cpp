#include <SDL3/SDL.h>

extern bool GetSDLKeyState(SDL_Scancode scancode, bool leftright = true);

/*
	Determine increment / decrement amount for production queues.
*/
int GetQueueMultiplier()
{
	if (GetSDLKeyState(SDL_SCANCODE_F5)) return 250;
	if (GetSDLKeyState(SDL_SCANCODE_TAB)) return 50;
	if (GetSDLKeyState(SDL_SCANCODE_F2)) return 36;
	if (GetSDLKeyState(SDL_SCANCODE_LALT)) return 20;
	if (GetSDLKeyState(SDL_SCANCODE_F1)) return 15;
	if (GetSDLKeyState(SDL_SCANCODE_LSHIFT)) return 5;
	return 1;
}