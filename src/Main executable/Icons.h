class WIcon{
public:
	char* Message;
	unsigned short FileID;
	unsigned short Spr;
	unsigned char Kind;
	unsigned short MagReq;
	//==0 - directory entry
	//==1 - uniq command, no parameters
	//==2 - uniq command, (x,y) requrired
	//==3 - upgrade
	//==4 - producind
	unsigned short Param1;
	unsigned short Param2;
	unsigned short* SubList;
};
