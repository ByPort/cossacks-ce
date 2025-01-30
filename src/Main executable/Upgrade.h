class OneObject;
class SimpleUTP{
public:
	char* Message;
	unsigned char NLinks;
	unsigned char Kind;
	int* Links;
	unsigned char NAuto;
	int* AutoPerf;
	bool Enabled:1;
	bool OneTime:1;
	bool Done:1;
	bool Finished:1;
	unsigned short IFileID;
	unsigned short IFIndex;
	unsigned short Cost;
	unsigned short Wood;
	unsigned short Oil;
	unsigned short MonsterID;
	unsigned short Time;
	unsigned short Stage;
};

class UTP1:public SimpleUTP{
public:
	unsigned char ResType;
	unsigned char AddVal;
};
class UTP2:public SimpleUTP{
public:
	unsigned short MID;
	unsigned char CharID;
	unsigned char AddVal;
};
class UTP3:public SimpleUTP{
public:
	unsigned short FinalMID;
};
class Upgrade{
public:
	union{
		UTP1* utp1[1024];
		UTP2* utp2[1024];
		UTP3* utp3[1024];
		SimpleUTP* utp[1024];
	};
	int NUpgrades;
	Upgrade();
};
