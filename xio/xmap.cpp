#include <xio.h>
#include <cmath>

void	XMAP::Read_File(const char * i_lpszFilename)
{
	FILE * fileRead = fopen(i_lpszFilename,"rt");
	char lpszKey[256];
	char lpszValue[256];
	char lpszBuffer[512];
	if (fileRead)
	{
		fgets(lpszBuffer,sizeof(lpszBuffer),fileRead);
		while (!feof(fileRead))
		{
			const char * lpszCursor = lpszBuffer;
			lpszCursor = xPassWhitespace(lpszCursor);
			if (lpszCursor && lpszCursor[0] != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13 && lpszCursor[0] != '#' &&
				((lpszCursor[0] >= 'a' && lpszCursor[0] <= 'z') || (lpszCursor[0] >= 'A' && lpszCursor[0] <= 'Z')))
			{
				unsigned int uiI = 0;
				while (lpszCursor[0] != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13 && lpszCursor[0] != ' ' && lpszCursor[0] != '\t' && lpszCursor[0] != '=')
				{
					lpszKey[uiI] = lpszCursor[0];
					uiI++;
					lpszCursor++;
				}
				lpszKey[uiI] = 0;
				lpszCursor = xPassWhitespace(lpszCursor);
//				printf("Cursor(a) %s",lpszCursor);
				if (lpszCursor[0] == '=')
					lpszCursor++;
				lpszCursor = xPassWhitespace(lpszCursor);
				if (lpszCursor[0] == 0 || lpszCursor[0] == 10 || lpszCursor[0] == 13) // assume value is on the next line
				{
					fgets(lpszBuffer,sizeof(lpszBuffer),fileRead);
					lpszCursor = lpszBuffer;
					lpszCursor = xPassWhitespace(lpszCursor);
					if (lpszCursor[0] == '=')
						lpszCursor++;
					lpszCursor = xPassWhitespace(lpszCursor);
				}
//				printf("Cursor(b) %s",lpszCursor);
				bool bCheck_String = false;
				char chString_Char;
				if (lpszCursor[0] == '\"' || lpszCursor[0] == '\'')
				{
					bCheck_String = true;
					chString_Char = lpszCursor[0];
					lpszCursor++;
				}

				uiI = 0;
//				printf("Cursor(c) %s",lpszCursor);
				while (lpszCursor[0] != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13 && 
					!(bCheck_String && lpszCursor[0] == chString_Char) && !(!bCheck_String && (lpszCursor[0] == ' ' || lpszCursor[0] == '\t')))
				{
					lpszValue[uiI] = lpszCursor[0];
					uiI++;
					lpszCursor++;
				}
				lpszValue[uiI] = 0;
//				printf("Add %s : %s\n",lpszKey,lpszValue);
				m_cMap[std::string(lpszKey)] = std::string(lpszValue);
			}
			fgets(lpszBuffer,sizeof(lpszBuffer),fileRead);
		}
		fclose(fileRead);
	}
	else
		fprintf(stderr,"XMAP: unable to open %s.\n",i_lpszFilename);
}
/*
bool XMAP::Key_Exists(const char * i_lpszKey) const
{
	bool bRet = Key_Exists(std::string(i_lpszKey));
	return bRet;
}
double	XMAP::Get_Value_Double(const char * i_lpszKey) const
{
	double dRet = Get_Value_Double(std::string(i_lpszKey));
	return dRet;
}
bool	XMAP::Get_Value_Bool(const char * i_lpszKey) const
{
	bool bRet = Get_Value_Bool(std::string(i_lpszKey));
	return bRet;
}
int	XMAP::Get_Value_Int(const char * i_lpszKey) const
{
	int iRet = Get_Value_Int(std::string(i_lpszKey));
	return iRet;
}
unsigned int	XMAP::Get_Value_Uint(const char * i_lpszKey) const
{
	unsigned int uiRet = Get_Value_Uint(std::string(i_lpszKey));
	return uiRet;
}
int64_t	XMAP::Get_Value_Int64(const char * i_lpszKey) const
{
	int64_t iRet = Get_Value_Int64(std::string(i_lpszKey));
	return iRet;
}
uint64_t	XMAP::Get_Value_Uint64(const char * i_lpszKey) const
{
	uint64_t uiRet = Get_Value_Uint64(std::string(i_lpszKey));
	return uiRet;
}
std::string	XMAP::Get_Value_String(const char * i_lpszKey) const
{
	std::string strRet = Get_Value_String(std::string(i_lpszKey));
	return strRet;
}
*/

bool XMAP::Key_Exists(const std::string &i_strKey) const
{
	return (!m_cMap.empty() && m_cMap.count(i_strKey) > 0);
}
double	XMAP::Get_Value_Double(const std::string &i_szKey) const
{
	double dRet = nan("");
	if (Key_Exists(i_szKey))
	{
		std::string szValue = Get_Value_String(i_szKey);
		dRet = atof(szValue.c_str());
		const char * lpszExp = strchr(szValue.c_str(),'d');
		if (!lpszExp)
			lpszExp = strchr(szValue.c_str(),'D');
		if (lpszExp)
			dRet *= pow(10.0,atoi(lpszExp + 1));
	}
	return dRet;
}
bool	XMAP::Get_Value_Bool(const std::string &i_szKey) const
{
	bool bRet = false;
	if (Key_Exists(i_szKey))
	{
		std::string strValue = Get_Value_String(i_szKey);
		bRet = (strValue == "true" || strValue == ".true." || strValue == "t" || strValue == "True" || strValue == "TRUE" || strValue == ".TRUE." || strValue == "y" || strValue == "yes" || strValue == "Yes" || strValue == "YES" || strValue == ".yes." || strValue == ".YES.");
	}
	return bRet;
}
int	XMAP::Get_Value_Int(const std::string &i_szKey) const
{
	int iRet = 0;
	if (Key_Exists(i_szKey))
	{
		iRet = atoi(Get_Value_String(i_szKey).c_str());
	}
	return iRet;
}
unsigned int	XMAP::Get_Value_Uint(const std::string &i_szKey) const
{
	unsigned int uiRet = -1;
	if (Key_Exists(i_szKey))
	{
		uiRet = atoi(Get_Value_String(i_szKey).c_str());
	}
	return uiRet;
}
int64_t	XMAP::Get_Value_Int64(const std::string &i_szKey) const
{
	int64_t iRet = 0;
	if (Key_Exists(i_szKey))
	{
		iRet = atol(Get_Value_String(i_szKey).c_str());
	}
	return iRet;
}
uint64_t	XMAP::Get_Value_Uint64(const std::string &i_szKey) const
{
	uint64_t uiRet = -1;
	if (Key_Exists(i_szKey))
	{
		uiRet = atol(Get_Value_String(i_szKey).c_str());
	}
	return uiRet;
}
std::string	XMAP::Get_Value_String(const std::string &i_szKey) const
{
	std::string strRet;
	if (Key_Exists(i_szKey))
	{
		std::map<const std::string, std::string>::const_iterator itRef = m_cMap.find(i_szKey);
		strRet = itRef->second;
	}
	return strRet;
}

