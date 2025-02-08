#include "ddini.h"
#include "ResFile.h"
#include "FastDraw.h"
#include "mgraph.h"
#include "mouse.h"
#include "menu.h"
#include "MapDiscr.h"

void AssignHintLo(char* s, uint64_t time);

class MouseZone 
{
public:
	int x, y, x1, y1, Index, MoveIndex;
	SDL_Scancode ScanCode;
	byte KeyState;
	byte Pressed;
	HandlePro* Pro;
	HandlePro* RPro;
	HandlePro* MoveOver;
	char* Hint;
	char* HintLo;
	MouseZone();
};

extern bool LockMouse;
MouseZone::MouseZone() 
{
	Index = -1;
	Hint = NULL;
}

#define NZones 64
MouseZone Zones[NZones];
void InitZones() 
{
	for (int i = 0; i < NZones; i++) 
	{
		Zones[i].Index = -1;
	}
}

int CreateRZone(int x, int y, int lx, int ly, HandlePro* HPro, HandlePro* RHPro, int Index, char* Hint) 
{
	int i;
	for (i = 0; i < NZones; i++) 
	{
		if (Zones[i].Index == -1)
		{
			break;
		}
	}
	if (i < NZones) 
	{
		MouseZone* Z = &(Zones[i]);
		Z->x = x;
		Z->y = y;
		Z->x1 = x + lx - 1;
		Z->y1 = y + ly - 1;
		Z->Pro = HPro;
		Z->RPro = RHPro;
		Z->MoveOver = NULL;
		Z->Index = Index;
		Z->Pressed = false;
		if (int(Z->Hint))
			free(Z->Hint);
		Z->Hint = new char[strlen(Hint) + 1];
		strcpy(Z->Hint, Hint);
		Z->KeyState = 0;
		Z->ScanCode = SDL_SCANCODE_UNKNOWN;
		return i;
	}
	return -1;
}

int CreateRZone(
	int x, int y, int lx, int ly,
	HandlePro* HPro, HandlePro* RHPro,
	int Index, char* Hint, char* HintLo
) 
{
	int i;
	for (i = 0; i < NZones; i++) 
	{
		if (Zones[i].Index == -1)
			break;
	}
	if (i < NZones) 
	{
		MouseZone* Z = &(Zones[i]);

		Z->x = x;
		Z->y = y;
		Z->x1 = x + lx - 1;
		Z->y1 = y + ly - 1;
		Z->Pro = HPro;
		Z->RPro = RHPro;
		Z->MoveOver = NULL;
		Z->Index = Index;
		Z->Pressed = false;
		Z->KeyState = 0;
		Z->ScanCode = SDL_SCANCODE_UNKNOWN;

		if (Z->Hint) 
		{
			free(Z->Hint);
			Z->Hint = NULL;
		}
		if (Z->HintLo) 
		{
			free(Z->HintLo);
			Z->HintLo = NULL;
		}
		if (Hint) 
		{
			Z->Hint = new char[strlen(Hint) + 1];
			strcpy(Z->Hint, Hint);
		}
		if (HintLo) 
		{
			Z->HintLo = new char[strlen(HintLo) + 1];
			strcpy(Z->Hint, Hint);
		}
		return i;
	}
	return -1;
}

int CreateZone(int x, int y, int lx, int ly, HandlePro* HPro, int Index, char* Hint) 
{
	int i;
	for (i = 0; i < NZones; i++) 
	{
		if (Zones[i].Index == -1)break;
	}
	if (i < NZones) 
	{
		MouseZone* Z = &(Zones[i]);
		Z->x = x;
		Z->y = y;
		Z->x1 = x + lx - 1;
		Z->y1 = y + ly - 1;
		Z->Pro = HPro;
		Z->RPro = NULL;
		Z->Index = Index;
		Z->Pressed = false;
		Z->KeyState = 0;
		Z->ScanCode = SDL_SCANCODE_UNKNOWN;
		if (int(Z->Hint))
			free(Z->Hint);
		Z->Hint = new char[strlen(Hint) + 1];
		strcpy(Z->Hint, Hint);
		return i;
	}
	return -1;
}

void AssignMovePro(int i, HandlePro* HPro, int id) 
{
	if (i != -1) 
	{
		Zones[i].MoveOver = HPro;
		Zones[i].MoveIndex = id;
	}
}

void AssignKeys(int i, SDL_Scancode Scan, byte State) 
{
	if (i != -1) 
	{
		Zones[i].ScanCode = Scan;
		Zones[i].KeyState = State;
	}
}

int CreateZone(int x, int y, int lx, int ly, HandlePro* HPro, int Index, char* Hint, char* HintLo) {
	int i;
	for (i = 0; i < NZones; i++) {
		if (Zones[i].Index == -1)break;
	};
	if (i < NZones) {
		MouseZone* Z = &(Zones[i]);
		Z->x = x;
		Z->y = y;
		Z->x1 = x + lx - 1;
		Z->y1 = y + ly - 1;
		Z->Pro = HPro;
		Z->RPro = NULL;
		Z->MoveOver = NULL;
		Z->Index = Index;
		Z->Pressed = false;
		Z->KeyState = 0;
		Z->ScanCode = SDL_SCANCODE_UNKNOWN;
		if (int(Z->Hint)) {
			free(Z->Hint);
			Z->Hint = NULL;
		};
		if (int(Z->HintLo)) {
			free(Z->HintLo);
			Z->HintLo = NULL;
		};
		if (Hint) {
			Z->Hint = new char[strlen(Hint) + 1];
			strcpy(Z->Hint, Hint);
		};
		if (HintLo) {
			Z->HintLo = new char[strlen(HintLo) + 1];
			strcpy(Z->HintLo, HintLo);
		};
		return i;
	};
	return -1;
};
bool MouseOverZone = 0;
extern byte SpecCmd;

extern bool GetSDLKeyState(SDL_Scancode scancode, bool leftright = true);

SDL_Scancode LastPressedCodes[8] = {
	SDL_SCANCODE_UNKNOWN,
	SDL_SCANCODE_UNKNOWN,
	SDL_SCANCODE_UNKNOWN,
	SDL_SCANCODE_UNKNOWN,
	SDL_SCANCODE_UNKNOWN,
	SDL_SCANCODE_UNKNOWN,
	SDL_SCANCODE_UNKNOWN,
	SDL_SCANCODE_UNKNOWN
};
extern bool EnterChatMode;
extern bool EditMapMode;
// TODO: this better be a dictionary of <SDL_Scancode:bool>
// For now we'll just use an array with size of the maximum value from ScanKeys: SDL_SCANCODE_KP_0 + 1
bool ScanPressed[SDL_SCANCODE_KP_0 + 1];
int CheckZonePressed(int i) {
	if (EnterChatMode || EditMapMode)return false;
	for (int j = 0; j < 8; j++)if (LastPressedCodes[j] != SDL_SCANCODE_UNKNOWN) {
		if (!(GetSDLKeyState(LastPressedCodes[j])))LastPressedCodes[j] = SDL_SCANCODE_UNKNOWN;
	};
	if (i < NZones&&Zones[i].Index != -1) {
		if (Zones[i].ScanCode != SDL_SCANCODE_UNKNOWN) {
			if ((GetSDLKeyState(Zones[i].ScanCode)) || ScanPressed[Zones[i].ScanCode]) {
				byte State = Zones[i].KeyState;
				SDL_Scancode Scan = Zones[i].ScanCode;

				if (State & 1) {
					if (!(GetSDLKeyState(SDL_SCANCODE_LCTRL)))return false;
				}
				else if (GetSDLKeyState(SDL_SCANCODE_LCTRL))return false;
				if (State & 2) {
					if (!(GetSDLKeyState(SDL_SCANCODE_LALT)))return false;
				}
				else if (GetSDLKeyState(SDL_SCANCODE_LALT))return false;
				if (State & 4) {
					if (!(GetSDLKeyState(SDL_SCANCODE_LSHIFT)))return false;
				}
				else if (GetSDLKeyState(SDL_SCANCODE_LSHIFT))return false;

				for (int j = 0; j < 8; j++)if (LastPressedCodes[j] == Scan)return 1;
				for (int j = 0; j < 8; j++)if (LastPressedCodes[j] == SDL_SCANCODE_UNKNOWN) {
					LastPressedCodes[j] = Scan;
					return 2;
				};
				return 2;
			}
			else return false;
		}
		else return false;
	}
	else return false;
};
extern byte KeyCodes[512][2];
#define NKEYS 61
extern SDL_Scancode ScanKeys[NKEYS];
bool CheckSpritePressed(int sp) {
	if (sp < 0 || sp >= 512 || EnterChatMode || EditMapMode)return false;
	if (KeyCodes[sp][0]) {
		if ((GetSDLKeyState(ScanKeys[KeyCodes[sp][0]])) || ScanPressed[ScanKeys[KeyCodes[sp][0]]]) {
			byte State = KeyCodes[sp][1];
			if (State & 1) {
				if (!(GetSDLKeyState(SDL_SCANCODE_LCTRL)))return false;
			}
			else if (GetSDLKeyState(SDL_SCANCODE_LCTRL))return false;
			if (State & 2) {
				if (!(GetSDLKeyState(SDL_SCANCODE_LALT)))return false;
			}
			else if (GetSDLKeyState(SDL_SCANCODE_LALT))return false;
			if (State & 4) {
				if (!(GetSDLKeyState(SDL_SCANCODE_LSHIFT)))return false;
			}
			else if (GetSDLKeyState(SDL_SCANCODE_LSHIFT))return false;
			return 1;
		};
	};
	return false;
};

void ControlZones()
{
	MouseOverZone = 0;
	if (LockMouse)
	{
		return;
	}
	int i;
	MouseZone* Z = nullptr;
	if (!Lpressed)
	{
		for (i = 0; i < NZones; i++)
		{
			Zones[i].Pressed = CheckZonePressed(i);
		}
	}
	for (i = 0; i < NZones; i++)
	{
		Z = &(Zones[i]);
		if ((Z->Index != -1 && mouseX >= Z->x&&mouseX <= Z->x1
			&& mouseY >= Z->y&&mouseY <= Z->y1)
			|| Z->Pressed)
		{
			break;
		}
	}
	if (i < NZones)
	{
		MouseOverZone = 1;
		if (SpecCmd == 241)
		{
			SpecCmd = 0;
		}
		if (CheckZonePressed(i) == 2 || Z->Pressed != 1)
		{
			if (Lpressed)
			{
				Z->Pressed = true;
			}
			if (int(Z->Hint))
			{
				// FIXME: time should be in milliseconds?
				// For AssignHint and AssignHintLo everywhere
				AssignHint(Z->Hint, 3);
			}
			if (Z->HintLo)
			{
				AssignHintLo(Z->HintLo, 3);
			}
			if ((Lpressed || Z->Pressed == 2) && int(Z->Pro))
			{//Handle mouse clicks?
				(*Z->Pro)(Z->Index);
			}
			Lpressed = false;
			if (Rpressed&&Z->RPro)
			{
				(*Z->RPro)(Z->Index);
			}
			if (Z->MoveOver)
			{
				Z->MoveOver(Z->MoveIndex);
			}
			Rpressed = false;
			if (Z->Pressed == 2)
			{
				Z->Pressed = 0;
			}
		}
	}
	for (int i = 0; i < NZones; i++)
	{
		Zones[i].Pressed = 0;
	}
	memset(ScanPressed, false, sizeof(ScanPressed));
}

void DeleteZone(int i)
{
	if (i < NZones&&i >= 0)
	{
		Zones[i].Index = -1;
	}
}
