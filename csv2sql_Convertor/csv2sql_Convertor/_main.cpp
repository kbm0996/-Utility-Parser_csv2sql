#include <cstdio>
#include <Windows.h>
#include <conio.h>
#define df_FILENAME_SIZE	128
#define df_COLUMN_MAX		100
#define df_COLUMN_LENGTH	30
#define df_ROW_LENGTH		1000

void main()
{
	FILE *pfCSV, *pfSQL;
	WCHAR szCSVName[df_FILENAME_SIZE], szSQLName[df_FILENAME_SIZE];

	// WRITE FILE(*.sql) Open
	wprintf(L"Save File(*.sql) : ");
	wscanf_s(L"%s", szSQLName, df_FILENAME_SIZE);
	if (wcsstr(szSQLName, L".sql") == NULL)
		swprintf_s(szSQLName, df_FILENAME_SIZE, L"%s.sql", szSQLName);
	if (_wfopen_s(&pfSQL, szSQLName, L"w,ccs=UTF-8") != 0)
	{
		wprintf(L"%s Open Fail\n", szSQLName);
		return;
	}
	rewind(pfSQL);

	while (1)
	{
		// READ FILE(*.csv) Open
		wprintf(L"Open File(*.csv) : ");
		wscanf_s(L"%s", szCSVName, df_FILENAME_SIZE);
		if (wcsstr(szCSVName, L".csv") == NULL)
			swprintf_s(szCSVName, df_FILENAME_SIZE, L"%s.csv", szCSVName);
		if (_wfopen_s(&pfCSV, szCSVName, L"r,ccs=UTF-8") != 0)
		{
			wprintf(L"%s Open Fail\n", szCSVName);
			break;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////
		// Make SQL Header 
		/////////////////////////////////////////////////////////////////////////////////////////////
		WCHAR ColumnName[df_COLUMN_MAX][df_COLUMN_LENGTH];	// Header에 쓰일 Column들을 담을 Buffer. Header 생성 후에는 Value를 임시 저장
		WCHAR wszRow[df_ROW_LENGTH];	// 파일로부터 긁어온 Row를 저장할 Buffer.
		WCHAR *ptStart, *ptEnd;
		int iColumnCnt = 0;
		ZeroMemory(ColumnName, sizeof(ColumnName));
		ZeroMemory(wszRow, sizeof(wszRow));

		// Column Analysis
		fgetws(wszRow, df_ROW_LENGTH, pfCSV);
		ptStart = wszRow;
		while (1)
		{
			ptEnd = wcschr(ptStart, L',');
			if (ptEnd == NULL)
			{
				wcsncpy_s(ColumnName[iColumnCnt], sizeof(ColumnName[iColumnCnt]), ptStart, wcschr(ptStart, L'\n') - ptStart);
				break;
			}
			wcsncpy_s(ColumnName[iColumnCnt], sizeof(ColumnName[iColumnCnt]), ptStart, ptEnd - ptStart);
			ptStart = ptEnd + 1;

			++iColumnCnt;
		}

		// TODO : Debug :: Column Check
		//for (int i = 0; i <= iColumnCnt; ++i)
		//	wprintf(L"%d %s\n", i, ColumnName[i]);

		// Make SQL Header
		WCHAR wszSQLHeader[df_COLUMN_MAX * df_COLUMN_LENGTH + 500];
		WCHAR wszDBName[17], wszTableName[30];		// 오라클 기준 DB 이름 최대 길이 17, 테이블 이름 최대 길이 30
		ZeroMemory(wszSQLHeader, sizeof(wszSQLHeader));
		ZeroMemory(wszDBName, sizeof(wszDBName));
		ZeroMemory(wszTableName, sizeof(wszTableName));

		// DB Name Input
		wprintf(L"Database(Schema) Name (max 17) : ");
		wscanf_s(L"%s", wszDBName, 17);

		// Table Name Input
		wprintf(L"Table Name (max 30) : ");
		wscanf_s(L"%s", wszTableName, 30);

		// Make SQL Header
		swprintf_s(wszSQLHeader, sizeof(wszSQLHeader) / 2, L"INSERT INTO `%s`.`%s` (", wszTableName, wszDBName);
		for (int iCnt = 0; iCnt < iColumnCnt; ++iCnt)
		{
			swprintf_s(wszSQLHeader, sizeof(wszSQLHeader) / 2, L"%s`%s`, ", wszSQLHeader, ColumnName[iCnt]);
		}
		swprintf_s(wszSQLHeader, sizeof(wszSQLHeader) / 2, L"%s`%s`) VALUES (", wszSQLHeader, ColumnName[iColumnCnt]);

		///////////////////////////////////////////////////////////////////////////////////////////////
		// File Print SQL
		///////////////////////////////////////////////////////////////////////////////////////////////
		// START
		//  fprintf	- Param에 따른 서식에 맞는 출력. Parsing과정이 들어가 \0을 만난다던가 하면 출력 종료
		//  fputs	- 한 행의 문자열을 출력. 개행은 하지 않음
		fputws(L"SET AUTOCOMMIT = 0;\nSTART TRANSACTION; \n", pfSQL);

		int iCnt = 1;
		while (!feof(pfCSV))
		{
			fgetws(wszRow, df_ROW_LENGTH, pfCSV);
			ptStart = wszRow;

			fwprintf_s(pfSQL, L"%s", wszSQLHeader);
			while (1)
			{
				ptEnd = wcschr(ptStart, L',');
				if (ptEnd == NULL)
				{
					wcsncpy_s(ColumnName[0], sizeof(ColumnName[0]), ptStart, wcschr(ptStart, L'\n') - ptStart);
					// TODO : Debug :: Column Check
					//wprintf(L"%s\n", ColumnName[0]);
					fwprintf_s(pfSQL, L"`%s`);\n", ColumnName[0]);
					break;
				}
				wcsncpy_s(ColumnName[0], sizeof(ColumnName[0]), ptStart, ptEnd - ptStart);
				// TODO : Debug :: Column Check
				//wprintf(L"%s,", ColumnName[0]);
				fwprintf_s(pfSQL, L"`%s`, ", ColumnName[0]);
				ptStart = ptEnd + 1;
			}
		}

		// END
		fputws(L"COMMIT;\nSET AUTOCOMMIT = 1;\n\n\n\n", pfSQL);

		wprintf(L"Convrt SQL Success. Continue? (Y/N)\n");
		WCHAR chKey = _getwch();
		if (chKey == L'n' || chKey == L'N')
			break;

	}
	fclose(pfSQL);
}