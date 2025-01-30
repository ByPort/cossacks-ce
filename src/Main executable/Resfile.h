/*             Work with the resource files
 *   
 *  You must use this module for  accesss to files.This 
 * routine allows you to read  files from disk  or from 
 * the resource file, you even will not recognise where
 * the given file is.
 */
//#include "Arc\GSCSet.h"
//typedef LPGSCfile ResFile;
typedef void* ResFile;
//Opening the resource file
ResFile RReset(const char* lpFileName);
//Rewriting file
ResFile RRewrite(const char* lpFileName);
//Getting size of the resource file
unsigned long RFileSize(ResFile hFile);
// Setting file position 
unsigned long RSeek(ResFile hFile,int pos);
//Reading the file
unsigned long RBlockRead(ResFile hFile,void* lpBuffer,unsigned long BytesToRead);
//Writing the file
unsigned long RBlockWrite(ResFile hFile,void* lpBuffer,unsigned long BytesToWrite);
//Returns last error
unsigned long IOresult(void);
//Close the file
void RClose(ResFile hFile);