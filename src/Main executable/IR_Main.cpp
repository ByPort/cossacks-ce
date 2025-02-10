#include "ddini.h"
#include "FastDraw.h"
#include "fonts.h"
#include "DrawForm.h"

#pragma pack(1)

extern bool KeyPressed;

void ShowCentralMessage3( char* Message, int GPIDX );
void ShowCentralMessage4( char* Message, int GPIDX, int DX );

char* GetTextByID( char* ID );

bool ProcessMessages();

void WaitWithError( char* ID, int GPID )
{
	char ccc[1256];
	strcpy( ccc, GetTextByID( ID ) );
	ShowCentralMessage3( GetTextByID( ID ), GPID );
	FlipPages();
	KeyPressed = 0;
	do
	{
		ProcessMessages();
	} while (!KeyPressed);
}

void WaitWithError4( char* ID, int GPID )
{
	char ccc[4096];
	strcpy( ccc, GetTextByID( ID ) );
	ShowCentralMessage4( GetTextByID( ID ), GPID, 90 );
	FlipPages();
	KeyPressed = 0;
	do
	{
		ProcessMessages();
	} while (!KeyPressed);
}


char* LASTNICK;
extern int RealLx;
extern int RealLy;

void GetAllPar3( int LXMAX, int* N, int* LMaxX, char* Message )
{
	int NStr = 1;
	int pos = 0;
	int LM = 0;
	int L = strlen( Message );
	int L0 = 0;
	do
	{
		if (L0 > LM)LM = L0;
		if (Message[pos] == ' '&&L0 > LXMAX)
		{
			NStr++;
			L0 = 0;
			pos++;
		}
		else
		{
			int DX;
			L0 += GetRLCWidthUNICODE( YellowFont.RLC, (byte*) ( Message + pos ), &DX );
			pos += DX;
		};
		if (L0 > LM)LM = L0;
	} while (Message[pos]);
	*N = NStr;
	*LMaxX = LM;
};
extern int CurPalette;
void ShowCentralMessage4( char* Message, int GPIDX, int DX )
{
	int LXMAX = 400;
	char ccx[256];
	int L0 = 0;
	int NStr = 1;
	int pos = 0;
	int LM = 0;
	int L = strlen( Message );
	GetAllPar3( LXMAX, &NStr, &LM, Message );
	int N1, LM1;
	int DXXX = 32;
	do
	{
		GetAllPar3( LXMAX - DXXX, &N1, &LM1, Message );
		DXXX += 32;
	} while (N1 == NStr&&N1 > 1);
	LXMAX -= DXXX - 64;
	GetAllPar3( LXMAX, &NStr, &LM, Message );
	ccx[0] = 0;
	pos = 0;
	int pos1 = 0;

	int DY = 25;
	if (CurPalette == 2)
	{
		DrawStdBar2(
			( RealLx - 64 - LM ) / 2 - DX,
			( RealLy - 70 ) / 2 + 18 - DY - ( NStr - 1 ) * 13,
			( RealLx + 64 + LM ) / 2 - DX,
			( RealLy - 70 ) / 2 + 55 - DY + ( NStr - 1 ) * 13,
			GPIDX
		);
	}
	else
	{
		DX = 0;
		DrawStdBar(
			( RealLx - 64 - LM ) / 2 - DX,
			( RealLy - 70 ) / 2 + 18 - DY - ( NStr - 1 ) * 13,
			( RealLx + 64 + LM ) / 2 - DX,
			( RealLy - 70 ) / 2 + 55 - DY + ( NStr - 1 ) * 13
		);
	}
	int y0 = ( RealLy - GetRLCHeight( YellowFont.RLC, 'W' ) ) / 2 + 1 - DY - ( NStr - 1 ) * 13;
	L0 = 0;
	do
	{
		if (( Message[pos] == ' '&&L0 > LXMAX ))
		{
			int LL = GetRLCStrWidth( ccx, &YellowFont );
			ShowString( ( RealLx - LL ) / 2 + 1 - DX, y0 + 1, ccx, &BlackFont );
			ShowString( ( RealLx - LL ) / 2 - DX, y0, ccx, &YellowFont );
			pos1 = 0;
			ccx[0] = 0;
			L0 = 0;
			pos++;
			y0 += 26;
		}
		else
		{
			int length;
			L0 += GetRLCWidthUNICODE( YellowFont.RLC, (byte*) ( Message + pos ), &length );
			memcpy( ccx + pos1, Message + pos, length );
			pos += length;
			pos1 += length;
			ccx[pos1] = 0;
		}
		if (!Message[pos])
		{
			int LL = GetRLCStrWidth( ccx, &YellowFont );
			ShowString( ( RealLx - LL ) / 2 + 1 - DX, y0 + 1, ccx, &BlackFont );
			ShowString( ( RealLx - LL ) / 2 - DX, y0, ccx, &YellowFont );
		}
	} while (Message[pos]);
}

void ShowCentralMessage3( char* Message, int GPIDX )
{
	ShowCentralMessage4( Message, GPIDX, 206 );
}
