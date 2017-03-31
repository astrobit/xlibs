#include <xio.h>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <fstream>
#include <ostream>
#include <iostream>

xstdlib::datatype xdataset_improved::Get_Column_Type(size_t i_tColumn)
{
	xstdlib::datatype eType = xstdlib::empty;
	if (i_tColumn < Get_Num_Columns())
	{
		eType = Get_Element_Type(0,i_tColumn);
		bool bFail = false;

		for (auto iterI = m_vvsDatatypes.begin(); iterI != m_vvsDatatypes.end() && !bFail; iterI++)
		{
			bFail = ((*iterI)[i_tColumn] != eType);
		}
		if (bFail)
			eType = xstdlib::string;
	}
	return eType;
}

void xdataset_improved::Allocate(size_t i_tNum_Rows, size_t i_tNum_Columns)
{
	m_vvsData.clear();
	m_vvsDatatypes.clear();
	m_tMax_Cols = i_tNum_Columns;
	m_bAll_Rows_Same_Size = true;
	for (size_t tI = 0; tI < i_tNum_Rows; tI++)
	{
		std::vector<std::string> vsRow;
		std::vector<xstdlib::datatype> vtRow;

		for (size_t tJ = 0; tJ < i_tNum_Columns; tJ++)
		{
			vsRow.push_back("");
			vtRow.push_back(xstdlib::empty);
		}
		m_vvsData.push_back(vsRow);
		m_vvsDatatypes.push_back(vtRow);
	}
}
void xdataset_improved::Read_Data_File(std::string i_lpszFilename, bool i_bWhitespace_Separated_Columns, char i_chColumn_Separator, size_t i_tHeader_Lines)
{
	Read_File(i_lpszFilename, i_bWhitespace_Separated_Columns, i_chColumn_Separator);
	std::vector< std::vector< std::string> > vvsData;
	std::vector< std::vector< xstdlib::datatype> > vvsDatatypes;
	m_tMax_Cols = 0;
	m_bAll_Rows_Same_Size = true;
	for (size_t tI = 0; tI < m_vvsData.size(); tI++)
	{
		if (tI < i_tHeader_Lines)
		{
			m_szHeader_Lines.push_back(m_vvsData[tI]);
		}
		else
		{
			if (tI == i_tHeader_Lines)
				m_tMax_Cols = m_vvsData[tI].size();
			m_bAll_Rows_Same_Size &= (m_vvsData[tI].size() == m_tMax_Cols);
			if (m_vvsData[tI].size() > m_tMax_Cols)
				m_tMax_Cols = m_vvsData[tI].size();
			vvsData.push_back(m_vvsData[tI]);
			vvsDatatypes.push_back(m_vvsDatatypes[tI]);
		}
	}
	m_vvsData = vvsData;
	m_vvsDatatypes = vvsDatatypes;
}

double xdataset_improved::Get_Element_Double(size_t i_tRow, size_t i_tColumn) const
{
	double dRet = nan("");
	if (Get_Element_Type(i_tRow,i_tColumn) == xstdlib::floating_point)
	{
		std::string szElem = Get_Element(i_tRow,i_tColumn);
		if (szElem == "inf" || szElem == "Inf" || szElem == "INF")
			dRet = INFINITY;
		else if (szElem == "-inf" || szElem == "-Inf" || szElem == "-INF")
		{
			dRet = INFINITY;
			dRet *= -1;
		}
		else if (szElem != "nan" && szElem != "NaN" && szElem != "NAN")
			dRet = std::stod(szElem);
	}
	return dRet;
}
int xdataset_improved::Get_Element_Int(size_t i_tRow, size_t i_tColumn) const
{
	int iRet = 0;
	switch (Get_Element_Type(i_tRow,i_tColumn))
	{
	case xstdlib::integer:
		iRet = std::stol(Get_Element(i_tRow,i_tColumn));
		break;
	case xstdlib::binary:
		{
			std::string szElem = Get_Element(i_tRow,i_tColumn);
			for (auto iterI = szElem.begin(); iterI != szElem.end(); iterI++)
			{
				if ((*iterI) != 'b')
				{
					iRet <<= 1;
					if ((*iterI) == '1')
						iRet  |= 1;
				}
			}
		}
		break;
	case xstdlib::octal:
		{
			std::string szElem = Get_Element(i_tRow,i_tColumn);
			for (auto iterI = szElem.begin(); iterI != szElem.end(); iterI++)
			{
				iRet <<= 3;
				iRet  += ((*iterI) - '0');
			}
		}
		break;
	case xstdlib::hex:
		{
			std::string szElem = Get_Element(i_tRow,i_tColumn);
			auto iterI = szElem.begin();
			iterI++; // bypass 0
			iterI++; // bypass x
			for (; iterI != szElem.end(); iterI++)
			{
				iRet <<= 4;
				if ((*iterI) >= 'a' && (*iterI) <= 'f')
					iRet  += ((*iterI) - 'a' + 10);
				else if ((*iterI) >= 'A' && (*iterI) <= 'F')
					iRet  += ((*iterI) - 'A');
				else// if ((*iterI) >= '0' && (*iterI) <= '9')
					iRet  += ((*iterI) - '0');
			}
		}
		break;
	case xstdlib::logical:
		{
			std::string szElem = Get_Element(i_tRow,i_tColumn);
			if (szElem == "true" || szElem == "TRUE" || szElem == "True" || szElem == "t" || szElem == "T")
				iRet =1;
		}
		break;
	}
	return iRet;
}

void xdataset_improved::Set_Element(size_t i_tRow, size_t i_tColumn, const std::string &i_szString)
{
	if (i_tRow < Get_Num_Rows() && i_tColumn < Get_Num_Columns())
	{
		m_vvsData[i_tRow][i_tColumn] = i_szString;
		m_vvsDatatypes[i_tRow][i_tColumn] = xstdlib::identify_datatype(i_szString);
	}
}
void xdataset_improved::Set_Element(size_t i_tRow, size_t i_tColumn, const double & i_dValue)
{
	if (i_tRow < Get_Num_Rows() && i_tColumn < Get_Num_Columns())
	{
		char lpszString[24];
		sprintf(lpszString,"%.17e",i_dValue);
		m_vvsData[i_tRow][i_tColumn] = lpszString;
		m_vvsDatatypes[i_tRow][i_tColumn] = xstdlib::floating_point;
	}
}
void xdataset_improved::Set_Element(size_t i_tRow, size_t i_tColumn, int i_iValue)
{
	if (i_tRow < Get_Num_Rows() && i_tColumn < Get_Num_Columns())
	{
		char lpszString[24];
		sprintf(lpszString,"%i",i_iValue);
		m_vvsData[i_tRow][i_tColumn] = lpszString;
		m_vvsDatatypes[i_tRow][i_tColumn] = xstdlib::integer;
	}
}
void xdataset_improved::Set_Element(size_t i_tRow, size_t i_tColumn, unsigned int i_uiValue)
{
	if (i_tRow < Get_Num_Rows() && i_tColumn < Get_Num_Columns())
	{
		char lpszString[24];
		sprintf(lpszString,"%i",i_uiValue);
		m_vvsData[i_tRow][i_tColumn] = lpszString;
		m_vvsDatatypes[i_tRow][i_tColumn] = xstdlib::integer;
	}
}
void xdataset_improved::Set_Element(size_t i_tRow, size_t i_tColumn, bool i_bValue)
{
	if (i_tRow < Get_Num_Rows() && i_tColumn < Get_Num_Columns())
	{
		if (i_bValue)
			m_vvsData[i_tRow][i_tColumn] = "true";
		else
			m_vvsData[i_tRow][i_tColumn] = "false";

		m_vvsDatatypes[i_tRow][i_tColumn] = xstdlib::logical;
	}
}

void xdataset_improved::Save_Data_File_CSV(const std::string & i_szFilename, char i_chColumn_Separator, char i_chString_Delimiter, const std::string i_lpszNumberFormatString) const
{
	std::filebuf fbFile;
	fbFile.open (i_szFilename,std::ios::out);
	if (fbFile.is_open())
	{ //@@TODO handle cases when the string delimitor is inside of a string
		std::ostream osFile(&fbFile);
		for (auto iterI = m_szHeader_Lines.begin(); iterI != m_szHeader_Lines.end(); iterI++)
		{
			for (auto iterJ = iterI->begin(); iterJ != iterI->end(); iterJ++)
			{
				if (iterJ != iterI->begin())
					osFile << i_chColumn_Separator << " ";
				if (iterJ->find(i_chColumn_Separator) != std::string::npos)
					osFile << i_chString_Delimiter << *iterJ << i_chString_Delimiter;
				else
					osFile << *iterJ;
			}
			osFile << std::endl;
		}
		for (auto iterI = m_vvsData.begin(); iterI != m_vvsData.end(); iterI++)
		{
			for (auto iterJ = iterI->begin(); iterJ != iterI->end(); iterJ++)
			{
				if (iterJ != iterI->begin())
					osFile << i_chColumn_Separator << " ";
				if (iterJ->find(i_chColumn_Separator) != std::string::npos)
					osFile << i_chString_Delimiter << *iterJ << i_chString_Delimiter;
				else
					osFile << *iterJ;
			}
			osFile << std::endl;
		}
		fbFile.close();	
	}
	else
		std::cerr << "xdataset_improved::Save_Data_File_CSV: Unable to open file " << i_szFilename << std::endl;
}

void Write_Integer_Compact(FILE * fileOut, int64_t i_t64, bool bSign)
{
	const int64_t tLim8 = 1 << 8;
	const int64_t tLim16 = 1 << 16;
	const int64_t tLim32 = (int64_t)(1) << 32;

	size_t tLen = 0;
	int8_t t8 = 0;
	int16_t t16;
	int32_t t32;
	if (bSign)
		t8 = 1;
	fwrite(&t8,1,1,fileOut); // write sign
	if (i_t64 < tLim8)
	{
		tLen = 1;
		t8 = (uint8_t)i_t64;
		fwrite(&tLen,sizeof(size_t),1,fileOut); // write data length
		fwrite(&t8,1,tLen,fileOut); // write data
	}
	else if (i_t64 < tLim16)
	{
		tLen = 2;
		t16 = (uint8_t)i_t64;
		fwrite(&tLen,sizeof(size_t),1,fileOut); // write data length
		fwrite(&t16,2,tLen,fileOut); // write data
	}
	else if (i_t64 < tLim32)
	{
		tLen = 4;
		t32 = (uint8_t)i_t64;
		fwrite(&tLen,sizeof(size_t),1,fileOut); // write data length
		fwrite(&t32,4,tLen,fileOut); // write data
	}
	else// if (t64 <= (1 << 63))
	{
		tLen = 8;
		fwrite(&tLen,sizeof(size_t),1,fileOut); // write data length
		fwrite(&i_t64,8,tLen,fileOut); // write data
	}
}
void xdataset_improved::Save_Data_File_Binary(const std::string & i_szFilename) const
{
		// need to use C file handler - the c++ stream operators don't deal with binary data
	FILE * fileOut = fopen(i_szFilename.c_str(),"wb");

	if (fileOut != nullptr)
	{
		// first put identifying data in
		int8_t chData[] = {2,0,1,7,3,31,'x','d','a','t','a','s','e','t','_','i','m','p','r','o','v','e','d'};
		size_t uiHeaderSize = sizeof(chData) / sizeof(int8_t);
		fwrite(chData,sizeof(int8_t),uiHeaderSize,fileOut);
		int8_t chVersion[4] = {1,0,0,0}; // write out version # of this file
		fwrite(chVersion,sizeof(int8_t),4,fileOut);
		int64_t bigNumber = 0x1234567890abcdef; // write a large integer to store endianness of source machine
		fwrite(&bigNumber,sizeof(bigNumber),1,fileOut);
		bigNumber = 0xfedcba9876543210; // write another large integer to confirm endianness of source machine
		fwrite(&bigNumber,sizeof(bigNumber),1,fileOut);
		int8_t chSize_Ref = sizeof(size_t);
		fwrite(&chSize_Ref,sizeof(int8_t),1,fileOut); // write the size of the size type in bytes
		chSize_Ref = sizeof(double);
		fwrite(&chSize_Ref,sizeof(int8_t),1,fileOut); // write the size of the double type in bytes
		double dDoubleNumber = 1.1234567890123456e-42; // write a double precision number to idenfity double precision data format
		fwrite(&dDoubleNumber,sizeof(dDoubleNumber),1,fileOut);
		chSize_Ref = sizeof(xstdlib::datatype);
		fwrite(&chSize_Ref,sizeof(int8_t),1,fileOut); // write the size of the enum datatype type in bytes

		// end of header indicator
		int8_t chHeaderEnd[] = {'e','n','d','h','e','a','d','e','r'};
		size_t uiHeaderEndSize = sizeof(chHeaderEnd) / sizeof(int8_t);
		fwrite(chHeaderEnd,sizeof(int8_t),uiHeaderEndSize,fileOut);

		// header lines		
		size_t tNum_Header_Lines = m_szHeader_Lines.size();
		fwrite(&tNum_Header_Lines,sizeof(size_t),1,fileOut);

		for (auto iterI = m_szHeader_Lines.begin(); iterI != m_szHeader_Lines.end(); iterI++)
		{
			for (auto iterJ = iterI->begin(); iterJ != iterI->end(); iterJ++)
			{
				size_t tLen = iterJ->size();
				xstdlib::datatype eType = (xstdlib::datatype)(xstdlib::string - xstdlib::empty);
				fwrite(&eType,sizeof(xstdlib::datatype),1,fileOut); // write type
				fwrite(&tLen,sizeof(size_t),1,fileOut); // write string length
				fwrite(iterJ->c_str(),1,tLen,fileOut); // write string
			}
			xstdlib::datatype eData = (xstdlib::datatype)-1; // end of line
			fwrite(&eData,sizeof(xstdlib::datatype),1,fileOut);
		}
		// write 'databegin' to delineate start of actual data
		int8_t chDataBegin[] = {'d','a','t','a','b','e','g','i','n'};
		size_t uiDataBeginSize = sizeof(chDataBegin) / sizeof(int8_t);
		fwrite(chDataBegin,sizeof(int8_t),uiDataBeginSize,fileOut);

		
		size_t tNum_Data_Lines = m_vvsData.size();
		fwrite(&tNum_Data_Lines,sizeof(size_t),1,fileOut);
		for (size_t tI = 0; tI < m_vvsData.size(); tI++)
		{
			for (size_t tJ = 0; tJ < m_vvsData[tI].size(); tJ++)
			{
				size_t tLen;
				xstdlib::datatype eType = (xstdlib::datatype)(m_vvsDatatypes[tI][tJ] - xstdlib::empty);
				fwrite(&eType,sizeof(xstdlib::datatype),1,fileOut); // write type
				uint8_t t8;
				uint16_t t16;
				uint32_t t32;
				uint64_t t64;
				double dData;
				std::string szData = m_vvsData[tI][tJ];

				switch (m_vvsDatatypes[tI][tJ])
				{
				case xstdlib::empty:
					break;
				case xstdlib::logical:
					tLen = 1;
					t8 = 0;
					if (szData == "true" || szData == "True" || szData == "TRUE" || szData == "t" || szData == "T")
						t8 = 1;
					fwrite(&tLen,sizeof(size_t),1,fileOut); // write string length
					fwrite(&t8,1,tLen,fileOut); // write data
					break;
				case xstdlib::hex:
					{
						auto iterK = szData.cbegin();
						t8 = 0;
						t64 = 0;
						if (*iterK == '-')
							t8 = 1;
						if (*iterK == '-' || *iterK == '+')
							iterK++;
						iterK++; //'0'
						iterK++; //'x'
						for (;iterK != szData.cend(); iterK++)
						{
							t64 <<= 4;
							if (*iterK >= '0' && *iterK <= '9')
								t64 += (*iterK) - '0';
							else if (*iterK >= 'a' && *iterK <= 'f')
								t64 += (*iterK) - 'a' + 10;
							else if (*iterK >= 'A' && *iterK <= 'F')
								t64 += (*iterK) - 'A' + 10;
						}
						Write_Integer_Compact(fileOut,t64,t8 == 1);
					}
					break;
				case xstdlib::octal:
					{
						auto iterK = szData.cbegin();
						t8 = 0;
						t64 = 0;
						if (*iterK == '-')
							t8 = 1;
						if (*iterK == '-' || *iterK == '+')
							iterK++;
						for (;iterK != szData.cend(); iterK++)
						{
							t64 <<= 3;
							if (*iterK >= '0' && *iterK <= '9')
								t64 += (*iterK) - '0';
						}
						Write_Integer_Compact(fileOut,t64,t8 == 1);
					}
					break;
				case xstdlib::binary:
					{
						auto iterK = szData.cbegin();
						t8 = 0;
						t64 = 0;
						if (*iterK == '-')
							t8 = 1;
						if (*iterK == '-' || *iterK == '+')
							iterK++;
						iterK++; // 'b'
						for (;iterK != szData.cend(); iterK++)
						{
							t64 <<= 1;
							if (*iterK >= '0' && *iterK <= '9')
								t64 += (*iterK) - '0';
						}
						Write_Integer_Compact(fileOut,t64,t8 == 1);
					}
					break;
				case xstdlib::integer:
					t64 = std::stol(szData);
					t8 = 0;
					if (t64 < 0)
					{
						t64 *= -1;
						t8 = 1;
					}
					Write_Integer_Compact(fileOut,t64,t8 == 1);
					break;
				case xstdlib::floating_point:
					if (szData == "nan" || szData == "NaN" || szData == "NAN")
						dData = nan("");
					else if (szData == "inf" || szData == "Inf" || szData == "INF")
						dData = INFINITY;
					else if (szData == "-inf" || szData == "-Inf" || szData == "-INF")
						dData = -INFINITY;
					else
						dData = std::stod(szData);
					
					fwrite(&dData,sizeof(double),tLen,fileOut); // write data
					break;
				case xstdlib::string:
					tLen = szData.size();
					fwrite(&tLen,sizeof(size_t),1,fileOut); // write string length
					fwrite(szData.c_str(),1,tLen,fileOut); // write string
					break;
				}
			}
			xstdlib::datatype eData = (xstdlib::datatype)-1; // end of line
			fwrite(&eData,sizeof(xstdlib::datatype),1,fileOut);
		}
		xstdlib::datatype eData = (xstdlib::datatype)-2; // end of data
		fwrite(&eData,sizeof(xstdlib::datatype),1,fileOut);
	}
	else
		std::cerr << "xdataset_improved::Save_Data_File_CSV: Unable to open file " << i_szFilename << std::endl;
}

