#include <cstdio>
#include <Windows.h>
#include <conio.h>
#define df_FILENAME_LEN_MAX	128		// ���� �̸� �ִ� ����
#define df_COL_CNT_MAX		100		// �� �ִ� ����
#define df_COL_LEN_MAX		30		// �� �̸� �ִ� ����
#define df_ROW_LEN_MAX		1000	// �� �̸� �ִ� ����

void main()
{
	while (1)
	{
		////////////////////////////////////////////////////////////
		// ���� ����
		//
		// SrcFile(*.csv), DestFile(*.sql)
		////////////////////////////////////////////////////////////
		FILE *pFile_csv, *pFile_sql;
		WCHAR szName_csv[df_FILENAME_LEN_MAX], szName_sql[df_FILENAME_LEN_MAX];

		// TODO 1:  READ FILE(*.csv) open
		wprintf(L" - Open File(*.csv) : ");
		wscanf_s(L"%s", szName_csv, df_FILENAME_LEN_MAX);
		if (wcsstr(szName_csv, L".csv") == NULL)	// add file extension
			swprintf_s(szName_csv, df_FILENAME_LEN_MAX, L"%s.csv", szName_csv);
		if (_wfopen_s(&pFile_csv, szName_csv, L"r,ccs=UTF-8") != 0)
		{
			wprintf(L"%s Open Error(%d)\n", szName_csv, GetLastError());
			continue;
		}

		// TODO 2: WRITE File(*.sql) open
		wprintf(L" - Save File(*.sql) : ");
		wscanf_s(L"%s", szName_sql, df_FILENAME_LEN_MAX);
		if (wcsstr(szName_sql, L".sql") == NULL)	// add file extension
			swprintf_s(szName_sql, df_FILENAME_LEN_MAX, L"%s.sql", szName_sql);
		if (_wfopen_s(&pFile_sql, szName_sql, L"w,ccs=UTF-8") != 0)
		{
			wprintf(L"%s Open Error(%d)\n", szName_sql, GetLastError());
			if (pFile_csv != nullptr)
				fclose(pFile_csv);
			continue;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////
		// SQL ��� �����
		//
		// *.csv ������ ù ���� column �̸��� �����ϱ�� ���Ѵ�.
		/////////////////////////////////////////////////////////////////////////////////////////////
		WCHAR ColumnName[df_COL_CNT_MAX][df_COL_LEN_MAX];
		WCHAR wszRow[df_ROW_LEN_MAX];
		WCHAR *pBeginPos, *pEndPos;
		int iColumnCnt = 0;

		// TODO 3:  �÷� ����, �÷� �̸� ����
		fgetws(wszRow, df_ROW_LEN_MAX, pFile_csv);
		pBeginPos = wszRow;
		while (1)
		{
			pEndPos = wcschr(pBeginPos, L',');
			if (pEndPos == NULL)
			{
				wcsncpy_s(ColumnName[iColumnCnt], sizeof(ColumnName[iColumnCnt]), pBeginPos, wcschr(pBeginPos, L'\n') - pBeginPos);
				break;
			}
			wcsncpy_s(ColumnName[iColumnCnt], sizeof(ColumnName[iColumnCnt]), pBeginPos, pEndPos - pBeginPos);
			pBeginPos = pEndPos + 1;

			++iColumnCnt;
		}

		// TODO 4: SQL ��� �����
		WCHAR wszSQLHeader[df_COL_CNT_MAX * df_COL_LEN_MAX + 500];
		WCHAR wszDBName[17], wszTableName[30];		// ����Ŭ ���� DB �̸� �ִ� ���� 17, ���̺� �̸� �ִ� ���� 30

		wprintf(L" - DB(Schema) Name (max 17) : ");
		wscanf_s(L"%s", wszDBName, 17);

		wprintf(L" - Table Name (max 30) : ");
		wscanf_s(L"%s", wszTableName, 30);

		swprintf_s(wszSQLHeader, sizeof(wszSQLHeader) / 2, L"INSERT INTO `%s`.`%s` (", wszTableName, wszDBName);
		for (int iCnt = 0; iCnt < iColumnCnt; ++iCnt)
		{
			swprintf_s(wszSQLHeader, sizeof(wszSQLHeader) / 2, L"%s`%s`, ", wszSQLHeader, ColumnName[iCnt]);
		}
		swprintf_s(wszSQLHeader, sizeof(wszSQLHeader) / 2, L"%s`%s`) VALUES (", wszSQLHeader, ColumnName[iColumnCnt]);

		///////////////////////////////////////////////////////////////////////////////////////////////
		// SQL ���� ����
		//
		///////////////////////////////////////////////////////////////////////////////////////////////
		//  fprintf	- ���Ŀ� �ִ� ���. �Ľ��Ͽ� \0�� �����ٴ��� �ϸ� �۾� ����
		//  fputs	- �� ���� ���ڿ��� ���. ���� ����
		
		// SET AUTOCOMMIT = 0;	// �ڵ� Ŀ��(����� �����ϸ� �״�� �ݿ�) OFF
		// START TRANSACTION;	// Ʈ�����(���� ������ ��ɾ� �ϳ��� �����ϸ� �ѹ�) ����
		fputws(L"SET AUTOCOMMIT = 0;\nSTART TRANSACTION; \n", pFile_sql);

		int iCnt = 1;
		while (1)
		{
			fgetws(wszRow, df_ROW_LEN_MAX, pFile_csv);
			if (feof(pFile_csv))
				break;

			pBeginPos = wszRow;

			// SQL ��� ���̱�
			fwprintf_s(pFile_sql, L"%s", wszSQLHeader);
			while (1)
			{
				pEndPos = wcschr(pBeginPos, L',');
				if (pEndPos == NULL)
				{
					wcsncpy_s(ColumnName[0], sizeof(ColumnName[0]), pBeginPos, wcschr(pBeginPos, L'\n') - pBeginPos);
					// TODO : Debug :: Column Check
					//wprintf(L"%s\n", ColumnName[0]);
					fwprintf_s(pFile_sql, L"`%s`);\n", ColumnName[0]);
					break;
				}
				wcsncpy_s(ColumnName[0], sizeof(ColumnName[0]), pBeginPos, pEndPos - pBeginPos);
				// TODO : Debug :: Column Check
				//wprintf(L"%s,", ColumnName[0]);
				fwprintf_s(pFile_sql, L"`%s`, ", ColumnName[0]);
				pBeginPos = pEndPos + 1;
			}

			
		}

		fputws(L"COMMIT;\nSET AUTOCOMMIT = 1;\n\n\n\n", pFile_sql);


		///////////////////////////////////////////////////////////////////////////////////////////////
		// �۾� ����
		//
		///////////////////////////////////////////////////////////////////////////////////////////////
		wprintf(L" - %s creation successful. continue? (Y/N)\n", szName_sql);
		WCHAR chKey = _getwch();
		if (chKey == L'n' || chKey == L'N')
			break;
		
		fclose(pFile_csv);
		fclose(pFile_sql);
	}
	
}