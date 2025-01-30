#ifndef __TNTFILEIO_H
#define __TNTFILEIO_H

#ifndef TNTFILEIO_API
#define TNTFILEIO_API __declspec(dllimport)
#endif	

	// folder fuctions //
TNTFILEIO_API HRESULT tntOpenFolder				( LPSTR lpFolderName				// folder name
												);
TNTFILEIO_API HRESULT tntCloseFolder			( );								// w/o params

	// file access functions //
TNTFILEIO_API HANDLE tntCreateFile				( LPCTSTR lpFileName,               // file name
												  unsigned long dwDesiredAccess,            // access mode
												  unsigned long dwShareMode,                // share mode
												  LPSECURITY_ATTRIBUTES lpSecurityAttributes, // SD
												  unsigned long dwCreationDisposition,      // how to create
												  unsigned long dwFlagsAndAttributes,       // file attributes
												  HANDLE hTemplateFile              // handle to template file
												);
TNTFILEIO_API BOOL	tntCloseHandle				( HANDLE hObject					// handle to object
												);
TNTFILEIO_API BOOL	tntReadFile					( HANDLE hFile,						// handle to file
												  void* lpBuffer,					// data buffer
												  unsigned long nNumberOfBytesToRead,		// number of bytes to read
												  LPDWORD lpNumberOfBytesRead,		// number of bytes read
												  LPOVERLAPPED lpOverlapped			// overlapped buffer
												);
TNTFILEIO_API BOOL	tntWriteFile				( HANDLE hFile,						// handle to file
												  LPCVOID lpBuffer,					// data buffer
												  unsigned long nNumberOfBytesToWrite,		// number of bytes to write
												  LPDWORD lpNumberOfBytesWritten,	// number of bytes written
												  LPOVERLAPPED lpOverlapped			// overlapped buffer
												);

	// file location functions //
TNTFILEIO_API unsigned long	tntGetFileAttributes		( LPCTSTR lpFileName				// name of file or directory
												);
TNTFILEIO_API unsigned long	tntSearchPath				( LPCTSTR lpPath,					// search path
												  LPCTSTR lpFileName,				// file name
												  LPCTSTR lpExtension,				// file extension
												  unsigned long nBufferLength,				// size of buffer
												  LPTSTR lpBuffer,					// found file name buffer
												  LPTSTR *lpFilePart				// file component
												);
// WATCOM SPECIFIC
TNTFILEIO_API unsigned long	tntGetFileSize				( HANDLE hFile,						// handle of file to get size of
												  LPDWORD lpFileSizeHigh			// pointer to high-order unsigned short for file size
												);

TNTFILEIO_API unsigned long	tntSetFilePointer			( HANDLE hFile,						// handle of file
												  long lDistanceToMove,				// number of bytes to move file pointer
												  PLONG lpDistanceToMoveHigh,		// pointer to high-order unsigned long of 
																					// distance to move
												  unsigned long dwMoveMethod				// how to move
												  );

TNTFILEIO_API BOOL	tntFileIO_IsZip				( HANDLE hFile );

#endif //__TNTFILEIO_H