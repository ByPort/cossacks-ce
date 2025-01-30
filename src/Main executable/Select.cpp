#include "ddini.h"
#include "ResFile.h"
#include "FastDraw.h"
#include "mgraph.h"
#include "mouse.h"
#include "menu.h"
#include "MapDiscr.h"


extern unsigned short SelCenter[8];
SelGroup SelSet[80];

SelGroup::SelGroup()
{
	NMemb = 0;
	Member = nullptr;
	SerialN = nullptr;
	CanMove = true;
	CanSearchVictim = true;
	CanHelpToFriend = true;
	Egoizm = false;
}

void SelGroup::CreateFromSelection(unsigned char NI)
{
	if (int(Member))
	{
		DeleteMembers();
	}

	Member = new unsigned short[NSL[NI]];
	SerialN = new unsigned short[NSL[NI]];
	unsigned short Nsel = NSL[NI];
	memcpy(Member, Selm[NI], Nsel << 1);
	memcpy(SerialN, SerN[NI], Nsel << 1);
	int k;
	unsigned short MID;
	OneObject* OB;

	for (k = 0; k < Nsel; k++)
	{
		MID = Member[k];
		if (k != 0xFFFF)
		{
			OB = Group[k];
			if (OB)
			{
				OB->GroupIndex = nullptr;
			}
		}
	}
	NMemb = Nsel;
}

void SelGroup::DeleteMembers()
{
	unsigned short MID;
	OneObject* OB;

	for (int k = 0; k < NMemb; k++)
	{
		MID = Member[k];
		if (k != 0xFFFF)
		{
			OB = Group[k];
			if (OB)
			{
				OB->GroupIndex = nullptr;
			}
		}
	}

	if (int(Member))
	{
		free(Member);
	}

	if (int(SerialN))
	{
		free(SerialN);
	}

	SelGroup();
}

void SelGroup::SelectMembers(unsigned char NI, bool Shift)
{
	SelCenter[NI] = 0;
	unsigned short MID;
	OneObject* OB;
	unsigned short Nsel = NSL[NI];
	unsigned short* SMon = Selm[NI];
	unsigned short* ser = SerN[NI];
	unsigned short NR = 0;
	unsigned char NeedNI = NatRefTBL[NI];
	if (!Shift) {
		for (int i = 0; i < Nsel; i++) {
			MID = SMon[i];
			if (MID != 0xFFFF) {
				OB = Group[MID];
				if (OB&&OB->Serial == ser[i] && !OB->Sdoxlo) {
					//OB->Egoist=false;
					OB->Selected &= ~GM(NI);
				};
			};
		};
		if (Nsel) {
			free(SMon);
			free(ser);
		};
		SMon = new unsigned short[NMemb];
		ser = new unsigned short[NMemb];
	}
	else {
		NR = NSL[NI];
		int N1 = NMemb + NR;
		SMon = (unsigned short*)realloc(SMon, N1 << 1);
		ser = (unsigned short*)realloc(ser, N1 << 1);
	};
	Selm[NI] = SMon;
	SerN[NI] = ser;
	for (int i = 0; i < NMemb; i++) {
		MID = Member[i];
		if (MID != 0xFFFF) {
			OB = Group[MID];
			if (int(OB) && OB->Serial == SerialN[i] && OB->NNUM == NeedNI && !OB->Sdoxlo) {
				if (!(OB->Selected&GM(NI))) {
					SMon[NR] = MID;
					ser[NR] = SerialN[i];
					OB->Selected |= GM(NI);
					NR++;
				};
			};
		};
	};
	NSL[NI] = NR;
};
void SelGroup::ImSelectMembers(unsigned char NI, bool Shift) {
	SelCenter[NI] = 0;
	unsigned short MID;
	OneObject* OB;
	unsigned short Nsel = ImNSL[NI];
	unsigned short* SMon = ImSelm[NI];
	unsigned short* ser = ImSerN[NI];
	unsigned short NR = 0;
	unsigned char NeedNI = NatRefTBL[NI];
	if (!Shift) {
		for (int i = 0; i < Nsel; i++) {
			MID = SMon[i];
			if (MID != 0xFFFF) {
				OB = Group[MID];
				if (OB&&OB->Serial == ser[i] && !OB->Sdoxlo) {
					//OB->Egoist=false;
					OB->ImSelected &= ~GM(NI);
				};
			};
		};
		if (Nsel) {
			free(SMon);
			free(ser);
		};
		SMon = new unsigned short[NMemb];
		ser = new unsigned short[NMemb];
	}
	else {
		NR = ImNSL[NI];
		int N1 = NMemb + NR;
		SMon = (unsigned short*)realloc(SMon, N1 << 1);
		ser = (unsigned short*)realloc(ser, N1 << 1);
	}
	ImSelm[NI] = SMon;
	ImSerN[NI] = ser;
	for (int i = 0; i < NMemb; i++) {
		MID = Member[i];
		if (MID != 0xFFFF) {
			OB = Group[MID];
			if (OB&&OB->Serial == SerialN[i] && OB->NNUM == NeedNI && !OB->Sdoxlo) {
				if (!(OB->ImSelected&GM(NI))) {
					SMon[NR] = MID;
					ser[NR] = SerialN[i];
					OB->ImSelected |= GM(NI);
					NR++;
				};
			};
		};
	};
	ImNSL[NI] = NR;
};
inline bool PxInside(int x, int y, int x1, int y1, int xp, int yp) {
	if (xp >= x&&xp <= x1&&yp >= y&&yp <= y1)return true;
	else return false;
};
