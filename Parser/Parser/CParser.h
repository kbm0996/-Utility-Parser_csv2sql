/*-----------------------------------------------------------------
 Parser Class

 example)

int iVersion, iServerID, iServerBindPort, iWorkerThread, iMaxUser;
char szServerBindIP[256];
if (LoadFile("Config.ini"))
{
GetValue("Version", &iVersion);
GetValue("ServerID", &iServerID);
GetValue("ServerBindIP", szServerBindIP);
GetValue("ServerBindPort", &iServerBindPort);
GetValue("WorkerThread", &iWorkerThread);
GetValue("MaxUser", &iMaxUser);
printf("*Version	= %d\n", iVersion);
printf("*ServerID	= %d\n", iServerID);
printf("*ServerBindIP	= \"%s\" \n", szServerBindIP);
printf("*ServerBindPort	= %d\n", iServerBindPort);
printf("*WorkerThread	= %d\n", iWorkerThread);
printf("*MaxUser	= %d\n", iMaxUser);
}
-----------------------------------------------------------------*/

#ifndef __PARSER_H__
#define __PARSER_H__
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>

namespace mylib
{
	class CParser
	{
	public:
		CParser();
		virtual ~CParser();

		bool LoadFile(char* szpFileName);
		bool GetValue(char* szKey, int* pOutBuf);
		bool GetValue(char* szKey, char* pOutBuf, int iOutBufSize);
		bool GetValue(char* szKey, WCHAR* pOutBuf, int iOutBufSize);

	private:
		bool SkipNoneCommand();
		bool GetNextWord(char** chppBuffer, int* ipLength);
		bool GetStringWord(char **chppBuffer, int *ipLength);

		int _iFileSize;
		char* _pDataBuffer;
		char* _pLineBuffer;
	};
}
#endif