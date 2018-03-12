#include <xio.h>
#include <iostream>

size_t Get_Next_Separator_Position(const std::string i_szLine, size_t i_tRoot, bool i_bWhitespace_Separated, char i_chColumn_Separator)
{
	size_t tPos;
	if (i_bWhitespace_Separated)
	{

		size_t tSpace = i_szLine.find(' ',i_tRoot);
		size_t tTab = i_szLine.find('\t',i_tRoot);
		if (tSpace < tTab)
			tPos = tSpace;
		else
			tPos = tTab;
	}
	else
		tPos = i_szLine.find(i_chColumn_Separator,i_tRoot);
	return tPos;
}

void xdataparse::Parse_String(const std::string & i_szString, bool i_bWhitespace_Separated, char i_chColumn_Seperator)
{
	// get rid of CR/LF
	std::string szLine = i_szString;
	size_t tPos = szLine.find(10);
	if (tPos != std::string::npos)
	{
		std::string szRem = szLine.substr(0,tPos);
		szLine = szRem;
	}
	tPos = szLine.find(13);
	if (tPos != std::string::npos)
	{
		std::string szRem = szLine.substr(0,tPos);
		szLine = szRem;
	}
	if (!szLine.empty())
	{
		std::vector<std::string> vsLine;
		std::vector<xstdlib::datatype> vsLineTypes;
		// parse the line, using the user specified column separator
		// for each entry, create a string and add it to the vector vsLine
		size_t tRoot = 0;
		while (tRoot != std::string::npos)
		{
			while ((szLine[tRoot] == ' ' || szLine[tRoot] == '\t') && tRoot < szLine.size()) // ignore whitespace
				tRoot++;
			if (tRoot < szLine.size())
			{
				// make sure not to pick up column separators that are enclosed within strings, denoted by " or '
				bool bString_Flag = false;
				//@@TODO handle cases when the string delimitor is inside of a string, i.e. "here is a """string""" dude" should render as 'here is a "string" dude'
				if (szLine[tRoot] == '"' || szLine[tRoot] == '\'')
				{
					switch (szLine[tRoot])
					{
					case '"':
						tPos = szLine.find('"',tRoot + 1);
						break;
					case '\'':
						tPos = szLine.find('\'',tRoot + 1);
						break;
					}
					tPos = Get_Next_Separator_Position(szLine, tPos, i_bWhitespace_Separated, i_chColumn_Seperator);
					bString_Flag = true;
				}
				else
					tPos = Get_Next_Separator_Position(szLine, tRoot, i_bWhitespace_Separated, i_chColumn_Seperator);
				if (tPos == std::string::npos)
					tPos = szLine.size();
				//std::cout << tRoot << " " << tPos << " " << szLine.substr(tRoot,tPos - tRoot) << std::endl;
				size_t tEnd = tEnd = tPos - 1;
				while (tEnd > tRoot && (szLine[tEnd] == ' ' || szLine[tEnd] == '\t'))
					tEnd--;
				if (bString_Flag && szLine[tEnd] == szLine[tRoot])
				{
					tEnd--;
					tRoot++;
				}
				std::string szEntry = szLine.substr(tRoot,tEnd - tRoot + 1);
				vsLine.push_back(szEntry);
				vsLineTypes.push_back(xstdlib::identify_datatype(szEntry));

				if (tPos < szLine.size())
					tRoot = tPos + 1;
				else
					tRoot = std::string::npos;
			}
			else
				tRoot = std::string::npos;
		}
		// add the data for this line to the data for this file
		m_vvsData.push_back(vsLine);
		m_vvsDatatypes.push_back(vsLineTypes);
		if (m_tMax_Cols != 0)
			m_bAll_Rows_Same_Size &= (vsLine.size() == m_tMax_Cols);
		if (vsLine.size() > m_tMax_Cols)
			m_tMax_Cols = vsLine.size();
	}
}

void xdataparse::Read_File(const std::string & i_szFilename, bool i_bWhitespace_Separated, char i_chColumn_Separator)
{
	m_tMax_Cols = 0;
	m_vvsData.clear();
	// open and process this file
	m_bAll_Rows_Same_Size = true;
	FILE * fileData = fopen(i_szFilename.c_str(),"rt");
	if (fileData != nullptr)
	{
		while (feof(fileData) == 0)
		{
			char lpszBuffer[2048];
			if (fgets(lpszBuffer,sizeof(lpszBuffer),fileData) != nullptr)
			{
				Parse_String(lpszBuffer, i_bWhitespace_Separated, i_chColumn_Separator);
			}
		}
		fclose(fileData);
		fileData = nullptr;
	}
	else
		std::cerr << "Error: xdataparse::Read_File unable to open file " << i_szFilename.c_str() << std::endl;
}
