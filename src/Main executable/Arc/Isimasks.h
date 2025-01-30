#if !defined(_MASKS_H_INCLUDED_)
#define _MASKS_H_INCLUDED_

BOOL	isiMatchesMask(LPSTR lpszFile, LPSTR lpszMask);

unsigned long	isiCalcHash(LPSTR lpszFileName);

void	isiDecryptMem(unsigned char* lpbBuffer, unsigned long dwSize, BYTE dbKey);

void	isiEncryptMem(unsigned char* lpbBuffer, unsigned long dwSize, BYTE dbKey);

BOOL	isiFileExists(LPSTR lpszFileName);

#endif //_MASKS_H_INCLUDED_