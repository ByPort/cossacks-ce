void AddPoint(short XL,short YL,short x,short y,OneObject* OB,unsigned short FileID,unsigned short SpriteID,int Param1,int Param2);
void AddLine(short X1,short Y1,short X2,short Y2,short x,short y,OneObject* OB,unsigned short FileID,unsigned short SpriteID,int Param1,int Param2);
void AddHiPoint(short x,short y,OneObject* OB,unsigned short FileID,unsigned short SpriteID,int Param1,int Param2);
void AddLoPoint(short x,short y,OneObject* OB,unsigned short FileID,unsigned short SpriteID,int Param1,int Param2);
void AddSuperLoPoint(short x,short y,OneObject* OB,unsigned short FileID,unsigned short SpriteID,int Param1,int Param2);
void AddOptPoint(unsigned char Method,short XL,short YL,short x,short y,OneObject* OB,unsigned short FileID,unsigned short Sprite,int Options);
void AddOptLine(short X1,short Y1,short X2,short Y2,short x,short y,OneObject* OB,unsigned short FileID,unsigned short SpriteID,int Options);
void ShowZBuffer();
void ClearZBuffer();
#define ZBF_NORMAL	0
#define ZBF_LO		1
#define ZBF_HI		2
extern unsigned char CurDrawNation;