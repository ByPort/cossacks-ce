#ifndef _INC_GSC_TYPES
#define _INC_GSC_TYPES

#ifndef GFILE_USER
#define GFILE_API __declspec(dllexport)
#else
#define GFILE_API __declspec(dllimport)
#endif

#pragma pack(1)

struct TGSCFindData
{
 char	m_Mask[64];
 char	m_FileName[64];
 int	m_Found;
};

struct TGSCFileList
{
 char			m_FileName[64];
 TGSCFileList*	m_NextEntry;
};

struct TGSCFindInfo
{
 char			m_FileName[64];
 TGSCFileList*	m_Pos;
 TGSCFileList*	m_FileList;
};

struct TGSCarchHDR
{
 unsigned char	m_Descriptor[6]; // unsigned char
 unsigned short	m_Version;
 unsigned short	m_Key;
 unsigned long	m_Entries;
};

struct TGSCarchFAT
{
 unsigned long	m_Hash;
 unsigned char	m_FileName[64];
 unsigned long	m_Offset;
 unsigned long	m_Size;
 unsigned long	m_Reserved;
 unsigned char	m_Flags;
};

typedef TGSCFindData* LPGSCFindData;

typedef TGSCarchFAT* LPGSCarchFAT;

typedef TGSCFileList* LPGSCFileList;

typedef TGSCFindInfo* LPGSCFindInfo;

#endif // _INC_GSC_TYPES