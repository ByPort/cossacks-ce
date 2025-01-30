// GSCset.h: interface for the CGSCset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GSCSET_H__E81AB1CB_A7B5_4DFE_B67D_9C1AC503EAD2__INCLUDED_)
#define AFX_GSCSET_H__E81AB1CB_A7B5_4DFE_B67D_9C1AC503EAD2__INCLUDED_

#include "GSCtypes.h"	// Added by ClassView
#include "GSCarch.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class GFILE_API CGSCset  
{
public:
	LPGSCfile gWriteOpen(const char* lpcsFileName);
	int gWriteFile(LPGSCfile gFile, unsigned char* lpbBuffer, unsigned long dwSize);
	unsigned char* gMapFile(LPGSCfile gFile);
	unsigned long gFileSize(LPGSCfile gFile);
	void gClose();
	int gOpen();
	int gFindNext(LPGSCFindInfo gFindInfo);
	LPGSCFindInfo gFindFirst(const char* lpcsMask);
	unsigned long gFilePos(LPGSCfile gFile);
	void gSeekFile(LPGSCfile gFile, unsigned long dwPosition);
	void gReadFile(LPGSCfile gFile, unsigned char* lpbBuffer, unsigned long dwSize);
	void gCloseFile(LPGSCfile gFile);
	LPGSCfile gOpenFile(const char* lpcsFileName,bool Only);
	CGSCset();
	virtual ~CGSCset();

//private:
	LPGSCArchList m_ArchList;
};

#endif // !defined(AFX_GSCSET_H__E81AB1CB_A7B5_4DFE_B67D_9C1AC503EAD2__INCLUDED_)
