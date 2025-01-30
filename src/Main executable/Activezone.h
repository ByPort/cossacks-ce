class ActiveZone{
public:
	int x,y,R;
	char* Name;
	unsigned char Dir;
//--------------------//
	void Draw();
};
class ActiveGroup{
public:
	unsigned short* Units;
	unsigned short* Serials;
	int N;
	char* Name;
	int MinX,MinY,MaxX,MaxY;
	int AminX,AminY,AmaxX,AmaxY;
	bool Visible;
	void Draw(unsigned char c);
};
extern ActiveGroup* AGroups;
extern int NAGroups;
extern int MaxAGroups;

extern ActiveZone* AZones;
extern int NAZones;
extern int MaxAZones;
void ClearAllZones();
void RefreshAGroups();