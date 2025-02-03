/***********************************************************************
 *Direct Draw initialisation module
 *
 * This module creates the Direct Draw object with the primary surface
 * and a backbuffer and sets 800x600x8 display mode.
 *
 ***********************************************************************/
#define __ddini_cpp_
#include "ddini.h"
#include "ResFile.h"
#include "FastDraw.h"
#include "mode.h"
#include "MapDiscr.h"
#include "fog.h"
#include "GSound.h"
#include "fonts.h"
#include "VirtScreen.h"

//Dimensions of possible screen resolutions
__declspec( dllexport ) int ModeLX[32];
__declspec( dllexport ) int ModeLY[32];
SDL_DisplayMode SDLDisplayModes[32];

//Number of possible screen resolutions
__declspec( dllexport ) int NModes = 0;

void SERROR();
void SERROR1();
void SERROR2();

void InitRLCWindows();

const int InitLx = 1024;
const int InitLy = 768;

__declspec( dllexport ) int RealLx;
__declspec( dllexport ) int RealLy;
__declspec( dllexport ) int SCRSizeX;
__declspec( dllexport ) int SCRSizeY;
__declspec( dllexport ) int RSCRSizeX;
__declspec( dllexport ) int RSCRSizeY;
__declspec( dllexport ) int COPYSizeX;
__declspec( dllexport ) int Pitch;

//LPDIRECTDRAW            lpDD = NULL;      // DirectDraw object
//LPDIRECTDRAWSURFACE     lpDDSPrimary;   // DirectDraw primary surface
//LPDIRECTDRAWSURFACE     lpDDSBack;      // DirectDraw back surface
SDL_Renderer* renderer;                 // SDL Renderer object
SDL_Surface* primarySurface;            // SDL primary surface
SDL_Texture* primaryTexture;            // SDL primary texture
SDL_Surface* backSurface;               // SDL back surface
BOOL                    bActive;        // is application active (not minimized / has focus)?
//BOOL                    DDError;        //=FALSE if Direct Draw works normally 
bool                    SDLError;       // false if SDL works normally
//DDSURFACEDESC           ddsd;
//PALETTEENTRY            GPal[256];
//LPDIRECTDRAWPALETTE     lpDDPal;
SDL_Palette*            sdlPal;

extern bool PalDone;
extern word PlayerMenuMode;

// Get closest palette color from RGB
__declspec( dllexport ) byte GetPaletteColor( int r, int g, int b )
{
	int dmax = 10000;
	int bestc = 0;
	for (int i = 0; i < 256; i++)
	{
		int d = abs( r - sdlPal->colors[i].r ) + abs( g - sdlPal->colors[i].g ) + abs( b - sdlPal->colors[i].b );
		if (d < dmax)
		{
			dmax = d;
			bestc = i;
		}
	}
	return bestc;
}

//typedef byte barr[ScreenSizeX*ScreenSizeY];
void* offScreenPtr;
/*
 * Flipping Pages
 */

extern int SCRSZY;

void ClearRGB()
{
	if (!bActive)
	{
		return;
	}

	memset( RealScreenPtr, 0, RSCRSizeX*SCRSZY );
}

extern bool InGame;

extern void yield();

//Copies secundary screen buffer into primary buffer
//Call rate: menu ~545 Hz, ingame ~38 Hz
__declspec( dllexport ) void FlipPages( void )
{
	if (!bActive)
	{
		yield();
		return;
	}

	if (window_mode)
	{
		SDL_Surface* srcSurface;
		SDL_Surface* targetSurface;

		srcSurface = SDL_CreateSurfaceFrom(COPYSizeX, RSCRSizeY, SDL_PIXELFORMAT_INDEX8, RealScreenPtr, SCRSizeX);
		SDL_SetSurfacePalette(srcSurface, sdlPal);

		SDL_LockTextureToSurface(primaryTexture, nullptr, &targetSurface);
		SDL_BlitSurfaceScaled(srcSurface, nullptr, targetSurface, nullptr, SDL_SCALEMODE_LINEAR);
		// IMG_SavePNG(targetSurface, "test.png");
		SDL_UnlockTexture(primaryTexture);

		// SDL_RenderClear(renderer);
		SDL_RenderTexture(renderer, primaryTexture, nullptr, nullptr);
		SDL_RenderPresent(renderer);

		SDL_DestroySurface(srcSurface);

		yield();

		return;
	}


	//TODO: replace asm
	int ofs = 0;
	int	lx = COPYSizeX >> 2;
	int	ly = RealLy;
	int	addOf = SCRSizeX - ( lx << 2 );
	int RaddOf = RSCRSizeX - ( lx << 2 );
	__asm 
	{
		push	esi
		push	edi
		mov		esi, ScreenPtr
		mov		edi, RealScreenPtr
		add		esi, ofs
		add		edi, ofs
		cld
		mov		eax, ly
		xxx :
		mov		ecx, lx
			rep		movsd
			add		esi, addOf
			add		edi, RaddOf
			dec		eax
			jnz		xxx
	}

	/*
	//Refactored asm... doesn't work for 'Connecting to master server' message when in fullscreen?

	int	addOf = SCRSizeX - COPYSizeX;
	int RaddOf = RSCRSizeX - COPYSizeX;

	int src = (int) ScreenPtr;
	int dest = (int) RealScreenPtr;

	for (int i = 0; i < RealLy; i++)
	{
		memcpy( (void*) dest, (void*) src, COPYSizeX );
		src += addOf;
		dest += RaddOf;
	}
	*/

	yield();
}

/*
 * Getting Screen Pointer
 *
 * You will get the pointer to the invisible area of the screen
 * i.e, if primary surface is visible, then you will obtain the
 * pointer to the backbuffer.
 * You must call UnlockSurface() to allow Windows draw on the screen
 */
int SCRSZY = 0;

void LockSurface( void )
{
	if (window_mode)
	{
		ScreenPtr = (void*) ( int( offScreenPtr ) + MaxSizeX * 32 );
		// ddsd has no alternative in SDL, so we just create lpSurface variable
		RealScreenPtr = ScreenPtr;
		return;
	}

	if (SDLError)
	{
		return;
	}
	// TODO: actually primarySurface is not needed and can be removed
	if (!SDL_LockSurface(primarySurface))
	{
		SDLError = true;
	}
	//else
	//{
	//	lpSurface = primarySurface->pixels;
	//}

	RSCRSizeX = primarySurface->pitch;

	ScreenPtr = (void*) ( int( offScreenPtr ) + MaxSizeX * 32 );
	RealScreenPtr = malloc(primarySurface->w * primarySurface->h);
	//RealScreenPtr = lpSurface;
	SCRSZY = primarySurface->h;
	ClearScreen();
}

/*
 *  Unlocking the surface
 *
 *  You must unlock the Video memory for Windows to work properly
 */
void UnlockSurface( void )
{
	if (window_mode)
	{
		return;
	}

	if (SDLError)
	{
		return;
	}
	SDL_UnlockSurface(primarySurface);
}

int BestVX = 640;
int BestVY = 480;
int BestBPP = 32;

//Save results of DirectX display mode enumeration
//HRESULT CALLBACK ModeCallback( LPDDSURFACEDESC pdds, LPVOID lParam )
//{
//	if (1024 > pdds->dwWidth || 768 > pdds->dwHeight)
//	{//Don't allow for resolutions less than 1024 x 768 ot bigger than 1920x[...]
//		return S_FALSE;
//	}
//
//	if (1920 < pdds->dwWidth)
//	{//Also disable all resolutions above ~1920 px wide for fairness reasons
//		return S_FALSE;
//	}
//
//	if (32 == pdds->ddpfPixelFormat.dwRGBBitCount)
//	{
//		ModeLX[NModes] = pdds->dwWidth;
//		ModeLY[NModes] = pdds->dwHeight;
//		NModes++;
//	}
//
//	//return S_TRUE to stop enuming modes, S_FALSE to continue
//	return S_FALSE;
//}

void SDLModeCallback(SDL_DisplayMode* mode)
{
	
	if (1024 > mode->w || 768 > mode->h)
	{//Don't allow for resolutions less than 1024 x 768 ot bigger than 1920x[...]
		return;
	}

	if (1920 < mode->w)
	{//Also disable all resolutions above ~1920 px wide for fairness reasons
		return;
	}

	const SDL_PixelFormatDetails* formatDetails = SDL_GetPixelFormatDetails(mode->format);

	if (32 == formatDetails->bits_per_pixel)
	{
		ModeLX[NModes] = mode->w;
		ModeLY[NModes] = mode->h;
		SDLDisplayModes[NModes] = *mode;
		NModes++;
	}
}

//Init DirectDraw and find possible resolutions
bool EnumModesOnly()
{
	//HRESULT ddrval = DirectDrawCreate_wrapper( NULL, &lpDD, NULL );
	//if (ddrval == DD_OK)
	//{
	//	lpDD->EnumDisplayModes( 0, NULL, NULL, ModeCallback );
	//	lpDD->Release();
	//	lpDD = NULL;

	//	return true;
	//}
	//else
	//{
	//	MessageBox( hwnd, "Unable to initialise Direct Draw. Cossacks should not run.", "Loading error", MB_ICONSTOP );
	//	exit( 0 );
	//}

	SDL_DisplayID primaryDisplay = SDL_GetPrimaryDisplay();
	if (primaryDisplay)
	{
		int numModes;
		SDL_DisplayMode** modes = SDL_GetFullscreenDisplayModes(primaryDisplay, &numModes);
		if (modes)
		{
			for (int i = 0; i < numModes; i++)
			{
				SDLModeCallback(modes[i]);
			}

			SDL_free(modes);
		}
	}

	return true;
}

bool CreateDDObjects( SDL_Window* sdlWindow )
{
	//HRESULT ddrval;
	bool success;
	char buf[256];
	SDLError = false;

	if (window_mode)
	{
		SVSC.SetSize( RealLx, RealLy );
		SDLError = false;
		SCRSizeX = MaxSizeX;
		SCRSizeY = MaxSizeY;
		COPYSizeX = RealLx;
		RSCRSizeX = RealLx;
		RSCRSizeY = RealLy;
		ScrHeight = SCRSizeY;
		ScrWidth = SCRSizeX;

		InitRLCWindows();

		WindX = 0;
		WindY = 0;
		WindLx = RealLx;
		WindLy = RealLy;
		WindX1 = WindLx - 1;
		WindY1 = WindLy - 1;
		BytesPerPixel = 1;
		offScreenPtr = ( malloc( SCRSizeX*( SCRSizeY + 32 * 4 ) ) );

		if (renderer)
		{
			SDL_DestroySurface(primarySurface);
			SDL_DestroyTexture(primaryTexture);
			success = true;
		}
		else
		{
			renderer = nullptr;
			success = CreateSDLRenderer();
		}
		if (success)
		{
			primarySurface = SDL_CreateSurface(COPYSizeX, RSCRSizeY, SDL_PIXELFORMAT_INDEX8);
			primaryTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, COPYSizeX, RSCRSizeY);
			if (primarySurface)
			{
				return true;
			}
		}

		//SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
		//SDL_Rect displayBounds;
		//SDL_GetDisplayBounds(displayID, &displayBounds);

		//const int screen_width = displayBounds.w;
		//const int screen_height = displayBounds.h;

		//const int ModeLX_candidates[] = { 1024, 1152, 1280, 1280, 1366, 1600, 1920 };
		//const int ModeLY_candidates[] = { 768,  864,  720, 1024,  768,  900, 1080 };

		//// Overwrites EnumModesOnly results
		//NModes = 0;
		//for (int i = 0; i < 8; i++)
		//{
		//	//Only show resolutions up to current screen resolution
		//	if (ModeLX_candidates[i] <= screen_width
		//		&& ModeLY_candidates[i] <= screen_height)
		//	{
		//		ModeLX[i] = ModeLX_candidates[i];
		//		ModeLY[i] = ModeLY_candidates[i];
		//		NModes++;
		//	}
		//}

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", SDL_GetError(), NULL);
		return false;
	}

	SVSC.SetSize( RealLx, RealLy );
	offScreenPtr = offScreenPtr = ( malloc( MaxSizeX*( MaxSizeY + 32 * 4 ) ) );

	if (renderer)
	{
		SDL_DestroySurface(primarySurface);
		SDL_DestroyTexture(primaryTexture);
		goto SDMOD;
	}

	renderer = nullptr;

	success = CreateSDLRenderer();

	if (success)
	{
SDMOD:;
		// TODO: check SDL_WINDOW_FULLSCREEN, SDL_WINDOW_FULLSCREEN_DESKTOP modes if any problem here
		success = SDL_SetWindowFullscreen(sdlWindow, true);
		SDL_SyncWindow(sdlWindow);
		//ddrval = lpDD->SetCooperativeLevel( hwnd,
		//	DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
		if (success)
		{
			// TODO: originally was set to RealLx, RealLy, 8. Need to make sure we choose the same display mode or sync back chosen with RealLx and RealLy (and a lot of other variables)
			for (int i = 0; i < NModes; i++)
			{
				if (ModeLX[i] == RealLx && ModeLY[i] == RealLy)
				{
					success = SDL_SetWindowFullscreenMode(sdlWindow, &SDLDisplayModes[i]);
					SDL_SyncWindow(sdlWindow);
					break;
				}
			}
			//ddrval = lpDD->SetDisplayMode( RealLx, RealLy, 8 );
			if (success)
			{
				//ddsd.dwSize = sizeof( ddsd );
				//ddsd.dwFlags = DDSD_CAPS;
				//ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
				//primarySurface = SDL_GetWindowSurface(sdlWindow);

				// TODO: remove primaryTexture as not needed
				primarySurface = SDL_CreateSurface(RealLx, RealLy, SDL_PIXELFORMAT_INDEX8);
				primaryTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, RealLx, RealLy);
				//ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, nullptr );
				if (primarySurface)
				{
					SDLError = false;
					SCRSizeX = MaxSizeX;
					SCRSizeY = MaxSizeY;
					RSCRSizeX = RealLx;
					Pitch = primarySurface->pitch;
					COPYSizeX = RealLx;
					RSCRSizeY = RealLy;
					ScrHeight = SCRSizeY;
					ScrWidth = SCRSizeX;
					WindX = 0;
					WindY = 0;
					WindLx = RealLx;
					WindLy = RealLy;
					WindX1 = WindLx - 1;
					WindY1 = WindLy - 1;
					//BytesPerPixel = 1;
					BytesPerPixel = SDL_GetPixelFormatDetails(primarySurface->format)->bytes_per_pixel;

					return true;
				}
			}
		}
	}
	//wsprintf( buf, "SDL Init Failed (%08lx)\n", ddrval );
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", SDL_GetError(), NULL);
	return false;
}

//BOOL CreateRGBDDObjects( HWND hwnd )
//{
//	HRESULT ddrval;
//	char    buf[256];
//	DDError = false;
//	CurrentSurface = true;
//	if (window_mode)
//	{
//
//		DDError = false;
//		SCRSizeX = MaxSizeX;
//		SCRSizeY = MaxSizeY;
//		COPYSizeX = RealLx;
//		RSCRSizeX = RealLx;
//		RSCRSizeY = RealLy;
//		ScrHeight = SCRSizeY;
//		ScrWidth = SCRSizeX;
//		InitRLCWindows();
//		WindX = 0;
//		WindY = 0;
//		WindLx = RealLx;
//		WindLy = RealLy;
//		WindX1 = WindLx - 1;
//		WindY1 = WindLy - 1;
//		BytesPerPixel = 1;
//		offScreenPtr = ( malloc( SCRSizeX*( SCRSizeY + 32 * 4 ) ) );
//		return true;
//	}
//#ifdef COPYSCR
//	offScreenPtr = offScreenPtr = ( malloc( MaxSizeX*( MaxSizeY + 32 * 4 ) ) );
//#endif
//	if (lpDD)
//	{
//		lpDDSPrimary->Release();
//		goto SDMOD;
//	}
//	ddrval = DirectDrawCreate_wrapper( NULL, &lpDD, NULL );
//	if (ddrval == DD_OK)
//	{
//		// Get exclusive mode
//		ddrval = lpDD->SetCooperativeLevel( hwnd,
//			DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
//		if (ddrval == DD_OK)
//		{
//SDMOD:
//			ddrval = lpDD->SetDisplayMode( 800, 600, 32 ); //COPYSizeX,RSCRSizeY, 8 );
//			if (ddrval == DD_OK)
//			{
//				// Create the primary surface with 1 back buffer
//				ddsd.dwSize = sizeof( ddsd );
//				ddsd.dwFlags = DDSD_CAPS;
//				ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
//				ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
//				if (ddrval == DD_OK)
//				{
//					DDError = false;
//					SCRSizeX = MaxSizeX;
//					SCRSizeY = MaxSizeY;
//					RSCRSizeX = RealLx;//ddsd.lPitch;
//					Pitch = ddsd.lPitch;
//					COPYSizeX = RealLx;
//					RSCRSizeY = RealLy;
//					ScrHeight = SCRSizeY;
//					ScrWidth = SCRSizeX;
//					WindX = 0;
//					WindY = 0;
//					WindLx = SCRSizeX;
//					WindLy = SCRSizeY;
//					WindX1 = WindLx - 1;
//					WindY1 = WindLy - 1;
//					BytesPerPixel = 1;
//					return true;
//				}
//			}
//		}
//	}
//	wsprintf( buf, "Direct Draw Init Failed (%08lx)\n", ddrval );
//	MessageBox( hwnd, buf, "ERROR", MB_OK );
//	return false;
//}

//BOOL CreateRGB640DDObjects( HWND hwnd )
//{
//	HRESULT ddrval;
//	//DDSCAPS ddscaps;
//	char    buf[256];
//	DDError = false;
//	CurrentSurface = true;
//	if (window_mode)
//	{
//
//		DDError = false;
//		SCRSizeX = MaxSizeX;
//		SCRSizeY = MaxSizeY;
//		COPYSizeX = RealLx;
//		RSCRSizeX = RealLx;
//		RSCRSizeY = RealLy;
//		ScrHeight = SCRSizeY;
//		ScrWidth = SCRSizeX;
//		InitRLCWindows();
//		WindX = 0;
//		WindY = 0;
//		WindLx = SCRSizeX;
//		WindLy = SCRSizeY;
//		WindX1 = WindLx - 1;
//		WindY1 = WindLy - 1;
//		BytesPerPixel = 1;
//		offScreenPtr = ( malloc( SCRSizeX*( SCRSizeY + 32 * 4 ) ) );
//		return true;
//	}
//#ifdef COPYSCR
//	offScreenPtr = offScreenPtr = ( malloc( MaxSizeX*( MaxSizeY + 32 * 4 ) ) );
//#endif
//	if (lpDD)
//	{
//		lpDDSPrimary->Release();
//		goto SDMOD;
//	};
//	ddrval = DirectDrawCreate_wrapper( NULL, &lpDD, NULL );
//	if (ddrval == DD_OK)
//	{
//SDMOD:;
//		// Get exclusive mode
//		ddrval = lpDD->SetCooperativeLevel( hwnd,
//			DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
//		if (ddrval == DD_OK)
//		{
//			ddrval = lpDD->SetDisplayMode( 640, 480, BestBPP ); //COPYSizeX,RSCRSizeY, 8 );
//			if (ddrval == DD_OK)
//			{
//				// Create the primary surface with 1 back buffer
//				ddsd.dwSize = sizeof( ddsd );
//				ddsd.dwFlags = DDSD_CAPS;
//				ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
//				ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
//				if (ddrval == DD_OK)
//				{
//					DDError = false;
//					SCRSizeX = MaxSizeX;
//					SCRSizeY = MaxSizeY;
//					RSCRSizeX = RealLx;//ddsd.lPitch;
//					Pitch = ddsd.lPitch;
//					COPYSizeX = RealLx;
//					RSCRSizeY = RealLy;
//					ScrHeight = SCRSizeY;
//					ScrWidth = SCRSizeX;
//					WindX = 0;
//					WindY = 0;
//					WindLx = SCRSizeX;
//					WindLy = SCRSizeY;
//					WindX1 = WindLx - 1;
//					WindY1 = WindLy - 1;
//					BytesPerPixel = 1;
//					return true;
//				}
//			}
//		}
//	}
//	wsprintf( buf, "Direct Draw Init Failed (%08lx)\n", ddrval );
//	MessageBox( hwnd, buf, "ERROR", MB_OK );
//	return false;
//}

/*   Direct Draw palette loading*/
void LoadPalette( LPCSTR lpFileName )
{
	// Palette originally was set somewhere in mdraw.dll
	// These LoadPalette functions weren't working originally
	if (sdlPal)
	{
		return;
	}

	// Need to init palette in window mode as well
	// Probably mdraw.dll somehow sets the palette itself so that code worked before
	//if (window_mode)
	//{
	//	return;
	//}

	if (SDLError)
	{
		return;
	}

	ResFile pf = RReset( lpFileName );
	sdlPal = SDL_CreatePalette(256);
	//memset( &GPal, 0, 1024 );

	if (pf != INVALID_HANDLE_VALUE)
	{
		for (int i = 0; i < 256; i++)
		{
			SDL_Color color = { 0, 0, 0, 255 };
			RBlockRead( pf, &color, 3 );
			SDL_SetPaletteColors(sdlPal, &color, i, 1);
		}
		RClose( pf );

		if (!strcmp( lpFileName, "agew_1.pal" ))
		{
			int DCL = 6;
			int C0 = 65;//128-DCL*4;
			for (int i = 0; i < 12; i++)
			{
				int gray = 0;
				if (i > 2)gray = ( i - 2 ) * 2;
				if (i > 7)gray += ( i - 7 ) * 8;
				if (i > 9)gray += ( i - 10 ) * 10;
				if (i > 10)gray += 50;
				gray = gray * 6 / 3;
				int rr = 0 * C0 / 150 + gray * 8 / 2;
				int gg = 80 * C0 / 150 + gray * 6 / 2;//80
				int bb = 132 * C0 / 150 + gray * 4 / 2;
				if (rr > 255)rr = 255;
				if (gg > 255)gg = 255;
				if (bb > 255)bb = 255;
				if (i < 5)
				{
					rr = rr - ( ( rr*( 5 - i ) ) / 6 );
					gg = gg - ( ( rr*( 5 - i ) ) / 6 );
					bb = bb - ( ( rr*( 5 - i ) ) / 6 );
				}
				if (i < 3)
				{
					rr = rr - ( ( rr*( 3 - i ) ) / 4 );
					gg = gg - ( ( rr*( 3 - i ) ) / 4 );
					bb = bb - ( ( rr*( 3 - i ) ) / 4 );
				}
				if (i < 2)
				{
					rr = rr - ( ( rr*( 2 - i ) ) / 3 );
					gg = gg - ( ( rr*( 2 - i ) ) / 3 );
					bb = bb - ( ( rr*( 2 - i ) ) / 3 );
				}

				SDL_Color color = { (Uint8)rr, (Uint8)gg, (Uint8)bb, 255 };
				SDL_SetPaletteColors(sdlPal, &color, 0xB0 + i, 1);
				C0 += 5;
			}
			ResFile pf = RRewrite( lpFileName );
			for (int i = 0; i < 256; i++)
			{
				RBlockWrite(pf, &(sdlPal->colors[i]), 3);
			}
			RClose( pf );
		}

		if (!PalDone)
		{
			// Already created above
			//lpDD->CreatePalette( DDPCAPS_8BIT, &GPal[0], &lpDDPal, NULL );
			PalDone = true;
			SDL_SetSurfacePalette(primarySurface, sdlPal);
		}
	}
}
void CBar( int x, int y, int Lx, int Ly, unsigned char c );

void SetDarkPalette()
{
	if (SDLError)
	{
		return;
	}

	//memset( &GPal, 0, 1024 );
	for (int i = 0; i < 256; i++)
	{
		SDL_Color color = { 0, 0, 0, 255 };
		SDL_SetPaletteColors(sdlPal, &color, i, 1);
	}

	if (!PalDone)
	{
		//lpDD->CreatePalette( DDPCAPS_8BIT, &GPal[0], &lpDDPal, nullptr );
		PalDone = true;
		SDL_SetSurfacePalette(primarySurface, sdlPal);
	}
}

// SlowLoadPalette creates fadein effect, originally by setting the palette from black to colors
// And it worked because changes reflected on the screen immediately
// TODO: Rewrite SlowLoadPalette to render each palette update or to make fade-in effect different way
// For now just rendering once after SlowLoadPalette
__declspec( dllexport ) void SlowLoadPalette( const char* lpFileName )
{
	if (SDLError)
	{
		return;
	}

	SetDarkPalette();
	ResFile pf = RReset( lpFileName );
	//memset( &GPal, 0, 1024 );
	for (int i = 0; i < 256; i++)
	{
		SDL_Color color = { 0, 0, 0, 255 };
		SDL_SetPaletteColors(sdlPal, &color, i, 1);
	}

	if (pf != INVALID_HANDLE_VALUE)
	{
		for (int i = 0; i < 256; i++)
		{
			SDL_Color color = { 0, 0, 0, 255 };
			RBlockRead(pf, &color, 3);
			SDL_SetPaletteColors(sdlPal, &color, i, 1);
		}

		RClose( pf );

		if (!strcmp( lpFileName, "agew_1.pal" ))
		{
			int DCL = 6;
			int C0 = 65;//128-DCL*4;
			for (int i = 0; i < 12; i++)
			{
				int gray = 0;
				if (i > 2)gray = ( i - 2 ) * 2;
				if (i > 7)gray += ( i - 7 ) * 8;
				if (i > 9)gray += ( i - 10 ) * 10;
				if (i > 10)gray += 50;
				gray = gray * 6 / 3;
				//gray=(i+5)*6;
				int rr = 0 * C0 / 150 + gray * 8 / 2;
				int gg = 80 * C0 / 150 + gray * 6 / 2;//80
				int bb = 132 * C0 / 150 + gray * 4 / 2;
				if (rr > 255)rr = 255;
				if (gg > 255)gg = 255;
				if (bb > 255)bb = 255;
				if (i < 5)
				{
					rr = rr - ( ( rr*( 5 - i ) ) / 6 );
					gg = gg - ( ( rr*( 5 - i ) ) / 6 );
					bb = bb - ( ( rr*( 5 - i ) ) / 6 );
				};
				if (i < 3)
				{
					rr = rr - ( ( rr*( 3 - i ) ) / 4 );
					gg = gg - ( ( rr*( 3 - i ) ) / 4 );
					bb = bb - ( ( rr*( 3 - i ) ) / 4 );
				};
				if (i < 2)
				{
					rr = rr - ( ( rr*( 2 - i ) ) / 3 );
					gg = gg - ( ( rr*( 2 - i ) ) / 3 );
					bb = bb - ( ( rr*( 2 - i ) ) / 3 );
				};
				//if(!i){
				//	rr=rr*10/11;
				//	gg=gg*10/11;
				//	bb=bb*10/11;
				//};
				SDL_Color color = { (Uint8)rr, (Uint8)gg, (Uint8)bb, 255 };
				SDL_SetPaletteColors(sdlPal, &color, 0xB0 + i, 1);
				C0 += 5;
			}
			ResFile pf = RRewrite( lpFileName );

			for (int i = 0; i < 256; i++)
			{
				RBlockWrite(pf, &(sdlPal->colors[0]), 3);
			}

			RClose( pf );
		}

		if (!window_mode)
		{
			SDL_Color originalColors[256];
			memcpy(originalColors, sdlPal->colors, sizeof(SDL_Color) * 256);
			int mul = 0;
			int t0 = GetTickCount();
			int mul0 = 0;
			do
			{
				mul = ( GetTickCount() - t0 ) * 2;
				if (mul > 255)
				{
					mul = 255;
				}

				if (mul != mul0)
				{
					//for (int j = 0; j < 1024; j++)
					//{
						//pal[j] = byte( ( int( pal0[j] )*mul ) >> 8 );
					//}
					for (int j = 0; j < 255; j++)
					{
						SDL_Color color = {
							byte((int(originalColors[j].r) * mul) >> 8),
							byte((int(originalColors[j].g) * mul) >> 8),
							byte((int(originalColors[j].b) * mul) >> 8),
							//byte((int(originalColors[j].a) * mul) >> 8),
							255,
						};
						SDL_SetPaletteColors(sdlPal, &color, j, 1);
					}
					SDL_Color color = { 0, 0, 0, 255 };
					SDL_SetPaletteColors(sdlPal, &color, 255, 1);
				}
				mul0 = mul;
			} while (mul != 255);
		}
	}
}

__declspec( dllexport ) void SlowUnLoadPalette( const char* lpFileName )
{
	if (SDLError)
	{
		return;
	}

	if (!window_mode)
	{
		SDL_Color originalColors[256];
		memcpy(originalColors, sdlPal->colors, sizeof(SDL_Color) * 256);
		int mul = 0;
		int t0 = GetTickCount();
		int mul0 = 0;
		do
		{
			mul = ( GetTickCount() - t0 ) * 2;
			if (mul > 255)
			{
				mul = 255;
			}

			if (mul != mul0)
			{
				//for (int j = 0; j < 1024; j++)
				//{
				//	pal[j] = byte( ( int( pal0[j] )*( 255 - mul ) ) >> 8 );
				//}
				for (int j = 0; j < 255; j++)
				{
					SDL_Color color = {
						byte((int(originalColors[j].r) * ( 255 - mul )) >> 8),
						byte((int(originalColors[j].g) * ( 255 - mul )) >> 8),
						byte((int(originalColors[j].b) * ( 255 - mul )) >> 8),
						//byte((int(originalColors[j].a) * ( 255 - mul )) >> 8),
						255,
					};
					SDL_SetPaletteColors(sdlPal, &color, j, 1);
				}
				SDL_Color color = { 0, 0, 0, 255 };
				SDL_SetPaletteColors(sdlPal, &color, 255, 1);
			};
			mul0 = mul;
		} while (mul != 255);
	}
}

/*     Closing all Direct Draw objects
 *
 * This procedure must be called before the program terminates,
 * otherwise Windows can occur some problems.
 */
void FreeDDObjects( void )
{
	free( offScreenPtr );

	offScreenPtr = nullptr;

	if (window_mode)
	{
		return;
	}

	free(RealScreenPtr);

	if (renderer != nullptr)
	{
		//ClearScreen();
		if (primarySurface != nullptr)
		{
			SDL_DestroySurface(primarySurface);
			SDL_DestroyTexture(primaryTexture);
			primarySurface = nullptr;
			primaryTexture = nullptr;
		};
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}
}

__declspec( dllexport )
void GetPalColor( byte idx, byte* r, byte* g, byte* b )
{
	//*r = GPal[idx].peRed;
	//*g = GPal[idx].peGreen;
	//*b = GPal[idx].peBlue;
	*r = sdlPal->colors[idx].r;
	*g = sdlPal->colors[idx].g;
	*b = sdlPal->colors[idx].b;
}

/*
	DirectDraw substitute.
	Uses mdraw.dll instead of the original, ddraw.lib exported DirectDrawCreate().
	Prevents the color palette corruption bug in modern Windows systems.
	No idea what the mdraw.dll funtion does, but you end up with a working
	IDirectDraw interface and no legacy bugs.
*/
//HRESULT DirectDrawCreate_wrapper( GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter )
//{
//	HMODULE mdrawHandle = LoadLibrary( "mdraw.dll" );
//	if (nullptr != mdrawHandle)
//	{
//		typedef HRESULT( __stdcall *mdrawProcType )( GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter );
//		mdrawProcType mdrawProc = (mdrawProcType) GetProcAddress( mdrawHandle, "DirectDrawCreate" );
//		if (nullptr != mdrawProc)
//		{
//			HRESULT mdrawResult = mdrawProc( lpGUID, lplpDD, pUnkOuter );
//			return mdrawResult;
//		}
//		FreeLibrary( mdrawHandle );
//	}
//	return DDERR_GENERIC;
//}

bool CreateSDLRenderer()
{
	if (renderer)
	{
		return true;
	}
	renderer = SDL_CreateRenderer(sdlWindow, NULL);
	if (!renderer)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Loading error", "Unable to create SDL renderer", NULL);
		//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		//SDL_RenderClear(renderer);
		//SDL_RenderPresent(renderer);
		return false;
	}
	return true;
}

bool InitSDL()
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Loading error", "Unable to initialise SDL3", NULL);
		exit(0);
	}
	return true;
}

//bool CreateSDLWindow()
//{
//	SDL_PropertiesID props = SDL_CreateProperties();
//	SDL_SetPointerProperty(props, SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER, hwnd);
//	sdlWindow = SDL_CreateWindowWithProperties(props);
//	if (!sdlWindow)
//	{
//		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Loading error", "Unable to create SDL window", NULL);
//		exit(0);
//	}
//	return true;
//}