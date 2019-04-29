#include <cstdio>
#include "CParser.h"

mylib::CParser g_Parser;
void main()
{
	int iVersion, iServerID, iServerBindPort, iWorkerThread, iMaxUser;
	char szServerBindIP[256];

	if (g_Parser.LoadFile("test.ini"))
	{
		g_Parser.GetValue("Version", &iVersion);
		g_Parser.GetValue("ServerID", &iServerID);
		g_Parser.GetValue("ServerBindIP", szServerBindIP, sizeof(szServerBindIP));
		g_Parser.GetValue("ServerBindPort", &iServerBindPort);
		g_Parser.GetValue("WorkerThread", &iWorkerThread);
		g_Parser.GetValue("MaxUser", &iMaxUser);

		printf("*Version	= %d\n", iVersion);
		printf("*ServerID	= %d\n", iServerID);
		printf("*ServerBindIP	= \"%s\" \n", szServerBindIP);
		printf("*ServerBindPort	= %d\n", iServerBindPort);
		printf("*WorkerThread	= %d\n", iWorkerThread);
		printf("*MaxUser	= %d\n", iMaxUser);

		return;
	}
}