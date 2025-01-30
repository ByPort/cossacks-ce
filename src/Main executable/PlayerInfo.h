#pragma once

typedef unsigned long DPID1;

#pragma pack(1)

//Beware: Sensitive to pragma pack(1) and exact structure size
//Can cause bugs. Fixed one in StartIGame()
//Original size: 132 bytes
//Used in MPlayer.cpp & CommCore with offset/size calculations
//e.g. memcpy(x, &PINFO[i].NationID, sizeof(PlayerInfo)-36)
struct PlayerInfo
{
	char name[32];
	DPID1 PlayerID;
	unsigned char NationID;
	unsigned char ColorID;//7 player colors coded as 0x00 - 0x06
	unsigned char GroupID;
	char MapName[36 + 8];//60-16-1-7-4 (?)
	int ProfileID;
	unsigned long Game_GUID;
	unsigned char UserParam[7];//Values of additional game options
	unsigned char Rank;
	unsigned short COMPINFO[8];
	int  CHKSUMM;
	unsigned char MapStyle;
	unsigned char HillType;
	unsigned char StartRes;
	unsigned char ResOnMap;
	unsigned char Ready;
	unsigned char Host;
	unsigned char Page;
	unsigned char CD;
	unsigned short Version;
	unsigned char VictCond;
	unsigned short GameTime;

	//New elements added after this point

	//3 elements for additional options
	//Didn't want to resize UserParam because 'unsigned short Version' would be moved.
	//This way you can recognize and be recognized by old 1.35 as new version.
	unsigned char UserParam2[3];
	
	int speed_mode;
};
