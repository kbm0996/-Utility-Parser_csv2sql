#include "CParser.h"

mylib::CParser::CParser()
{
	_iFileSize = 0;
	_pDataBuffer = nullptr;
	_pLineBuffer = nullptr;
}

mylib::CParser::~CParser()
{
	if (_pDataBuffer != NULL)
		free(_pDataBuffer);
}

bool mylib::CParser::SkipNoneCommand()	//스페이스, 탭, 엔터코드, 주석 처리.
{
	while (1)
	{
		if (*_pLineBuffer == ',' || *_pLineBuffer == '"' || *_pLineBuffer == 0x20 || *_pLineBuffer == 0x08 || *_pLineBuffer == 0x09 || *_pLineBuffer == 0x0a || *_pLineBuffer == 0x0d)
		{
			_pLineBuffer++;
			continue;
		}

		if (*_pLineBuffer == '/')
		{
			_pLineBuffer++;
			while (1)
			{
				////////////////////////////////////////
				if (*_pLineBuffer == '/')	// // 진입
				{
					while (1)
					{
						_pLineBuffer++;
						if (*_pLineBuffer == 0x0a || *_pLineBuffer == 0x0d)
						{
							_pLineBuffer++;
							if (*_pLineBuffer == '/')
								continue;
							break;
						}
						continue;
					}
					_pLineBuffer++;
				}
				////////////////////////////////////////
				if (*_pLineBuffer == '*')	// /* 진입
				{
					while (1)
					{
						_pLineBuffer++;
						if (*_pLineBuffer == '*')
						{
							_pLineBuffer++;
							if (*_pLineBuffer == '/')
							{
								_pLineBuffer++;
								break;
							}
							continue;
						}
						continue;
					}
					_pLineBuffer++;
				}
				////////////////////////////////////////
				else
					break;
			}
		}
		else
		{
			return true;
		}
	}
}

bool mylib::CParser::GetNextWord(char** chppBuffer, int* ipLength)
{
	int iLength = 0;
	*chppBuffer = _pLineBuffer;

	while (1)
	{
		if (*_pLineBuffer == ',' || *_pLineBuffer == '"' || *_pLineBuffer == 0x20 || *_pLineBuffer == 0x08 || *_pLineBuffer == 0x09 || *_pLineBuffer == 0x0a || *_pLineBuffer == 0x0d)
		{
			break;
		}
		_pLineBuffer++;
		iLength++;
	}

	if (iLength >= _iFileSize)
		return false;

	*ipLength = iLength;

	if (!SkipNoneCommand())
	{
		return true;
	}
	return true;
}

bool mylib::CParser::GetStringWord(char **chppBuffer, int *ipLength)
{
	int iLength = 0;
	*chppBuffer = _pLineBuffer;

	while (1)
	{
		if (*_pLineBuffer == ',' || *_pLineBuffer == '"' || *_pLineBuffer == 0x20 || *_pLineBuffer == 0x08 || *_pLineBuffer == 0x09 || *_pLineBuffer == 0x0a || *_pLineBuffer == 0x0d)
		{
			break;
		}
		_pLineBuffer++;
		iLength++;
	}

	*ipLength = iLength;

	if (!SkipNoneCommand())
		return true;
}

bool mylib::CParser::LoadFile(char * szpFileName)
{
	FILE *pFile;
	fopen_s(&pFile, szpFileName, "r+t");
	if (pFile == NULL)
		return false;

	fseek(pFile, 0, SEEK_END);
	_iFileSize = ftell(pFile);
	_pDataBuffer = (char*)malloc(_iFileSize);
	rewind(pFile);

	fread(_pDataBuffer, _iFileSize, 1, pFile);
	fclose(pFile);
	return true;
}

bool mylib::CParser::GetValue(char* szKey, int* pOutBuf)
{
	_pLineBuffer = _pDataBuffer;
	char *chpBuff = _pLineBuffer;
	char chWord[256];
	int	iLength = 0;

	while (GetNextWord(&chpBuff, &iLength))
	{
		memset(chWord, 0, 256);
		memcpy(chWord, chpBuff, iLength);
		if (0 == strcmp(szKey, chWord))
		{
			if (GetNextWord(&chpBuff, &iLength))
			{
				memset(chWord, 0, 256);
				memcpy(chWord, chpBuff, iLength);
				if (0 == strcmp(chWord, "="))
				{
					if (GetNextWord(&chpBuff, &iLength))
					{
						memset(chWord, 0, 256);
						memcpy(chWord, chpBuff, iLength);
						*pOutBuf = atoi(chWord);
						return true;
					}
					return false;
				}
			}
			return false;
		}
	}
	return false;
}

bool mylib::CParser::GetValue(char* szKey, char* pOutBuf, int iOutBufSize)
{
	_pLineBuffer = _pDataBuffer;
	char *chpBuff = _pLineBuffer;
	char chWord[256];
	int	iLength = 0;

	while (GetNextWord(&chpBuff, &iLength))
	{
		memset(chWord, 0, 256);
		memcpy(chWord, chpBuff, iLength);
		if (0 == strcmp(szKey, chWord))
		{
			if (GetNextWord(&chpBuff, &iLength))
			{
				memset(chWord, 0, 256);
				memcpy(chWord, chpBuff, iLength);
				if (0 == strcmp(chWord, "="))
				{
					if (GetStringWord(&chpBuff, &iLength))
					{
						iLength = iOutBufSize < iLength ? iOutBufSize : iLength;
						memset(pOutBuf, 0, iOutBufSize);
						memcpy(pOutBuf, chpBuff, iLength);
						return true;
					}
					break;
				}
			}
			break;
		}
	}
	return false;
}
bool mylib::CParser::GetValue(char * szKey, WCHAR * pOutBuf, int iOutBufSize)
{
	_pLineBuffer = _pDataBuffer;
	char *chpBuff = _pLineBuffer;
	char chWord[256];
	int	iLength = 0;

	while (GetNextWord(&chpBuff, &iLength))
	{
		memset(chWord, 0, 256);
		memcpy(chWord, chpBuff, iLength);
		if (0 == strcmp(szKey, chWord))
		{
			if (GetNextWord(&chpBuff, &iLength))
			{
				memset(chWord, 0, 256);
				memcpy(chWord, chpBuff, iLength);
				if (0 == strcmp(chWord, "="))
				{
					if (GetStringWord(&chpBuff, &iLength))
					{
						iLength = iOutBufSize < iLength ? iOutBufSize : iLength;
						memset(pOutBuf, 0, iOutBufSize);
						if (MultiByteToWideChar(CP_UTF8, 0, chpBuff, iLength, pOutBuf, iLength) == 0)
							break;
						return true;
					}
					break;
				}
			}
			break;
		}
	}
	return false;
}
	