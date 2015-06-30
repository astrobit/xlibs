#include <xio.h>
#include <mpi.h>
#include <float.h>


void XDATASET_ADVANCED::DescribeRecord(DATATYPE * i_lpeColumn_Descriptors,unsigned int i_uiNum_Columns)
{
	if (i_uiNum_Columns > 0)
	{
		if (m_lpeRecord_Descriptor)
		{
			delete [] m_lpeRecord_Descriptor;
			if (m_lpData)
				memset(m_lpData,0,m_uiSize_Alloc);
			if (m_lpuiData_Element_Offsets)
				delete [] m_lpuiData_Element_Offsets;
			if (m_lpuiData_Element_Sizes)
				delete [] m_lpuiData_Element_Sizes;

			m_uiString_Lookup_Table_Entry_Cursor = 0;
			if (m_lpcString_Lookup_Table)
				memset(m_lpcString_Lookup_Table,0,m_uiString_Lookup_Table_Size_Alloc * sizeof(XDATASET_ADVANCED_STRING_INDEX_DATA));


			m_uiString_Data_Cursor = 0;
			m_uiString_Data_Dead_Space = 0;
			if (m_lpchString_Data_Table)
				memset(m_lpchString_Data_Table,0,m_uiString_Data_Size_Alloc * sizeof(XDATASET_ADVANCED_STRING_INDEX_DATA));
		}
		m_lpeRecord_Descriptor = new DATATYPE[i_uiNum_Columns];
		m_lpuiData_Element_Offsets = new unsigned int[i_uiNum_Columns];
		m_lpuiData_Element_Sizes = new unsigned int[i_uiNum_Columns];
		m_uiNum_Columns = i_uiNum_Columns;
		memcpy(m_lpeRecord_Descriptor,i_lpeColumn_Descriptors,sizeof(DATATYPE) * i_uiNum_Columns);

		m_uiRecord_Length = 0;
		for (unsigned int uiI = 0; uiI < m_uiNum_Columns; uiI++)
		{
			switch (m_lpeRecord_Descriptor[uiI])
			{
				case XDT_ERR:
					uiI = m_uiNum_Columns;
					m_uiRecord_Length = -1;
					break;
				case XDT_LOGICAL:
					m_lpuiData_Element_Offsets[uiI] = m_uiRecord_Length;
					m_lpuiData_Element_Sizes[uiI] = sizeof(bool);
					m_uiRecord_Length += sizeof(bool);
					break;
				case XDT_INT:
				case XDT_UINT:
					m_lpuiData_Element_Offsets[uiI] = m_uiRecord_Length;
					m_lpuiData_Element_Sizes[uiI] = sizeof(int);
					m_uiRecord_Length += sizeof(int);
					break;
				case XDT_SHORT:
				case XDT_USHORT:
					m_lpuiData_Element_Offsets[uiI] = m_uiRecord_Length;
					m_lpuiData_Element_Sizes[uiI] = sizeof(short);
					m_uiRecord_Length += sizeof(short);
					break;
				case XDT_CHAR:
				case XDT_UCHAR:
					m_lpuiData_Element_Offsets[uiI] = m_uiRecord_Length;
					m_lpuiData_Element_Sizes[uiI] = sizeof(char);
					m_uiRecord_Length += sizeof(char);
					break;
/*				case XDT_WCHAR:
				case XDT_UWCHAR:
					m_lpuiData_Element_Offsets[uiI] = m_uiRecord_Length;
					m_lpuiData_Element_Sizes = sizeof(wchar_t);
					m_uiRecord_Length += sizeof(wchar_t);
					break;*/
				case XDT_STRING:
					m_lpuiData_Element_Offsets[uiI] = m_uiRecord_Length;
					m_lpuiData_Element_Sizes[uiI] = sizeof(int);
					m_uiRecord_Length += sizeof(int);
					break;
/*				case XDT_WSTRING:
					m_lpuiData_Element_Offsets[uiI] = m_uiRecord_Length;
					m_lpuiData_Element_Sizes = sizeof(int);
					m_uiRecord_Length += sizeof(int);
					break;*/
				case XDT_FLOAT:
					m_lpuiData_Element_Offsets[uiI] = m_uiRecord_Length;
					m_lpuiData_Element_Sizes[uiI] = sizeof(float);
					m_uiRecord_Length += sizeof(float);
					break;
				case XDT_DOUBLE:
					m_lpuiData_Element_Offsets[uiI] = m_uiRecord_Length;
					m_lpuiData_Element_Sizes[uiI] = sizeof(double);
					m_uiRecord_Length += sizeof(double);
					break;
			}
		}
	}
}

XDATASET_ADVANCED::DATATYPE XDATASET_ADVANCED::GetColumnType(unsigned int i_uiColumn)
{
	DATATYPE eType = XDT_ERR;
	if (i_uiColumn < m_uiNum_Columns)
	{
		eType = m_lpeRecord_Descriptor[i_uiColumn];
	}
	return eType;
}

unsigned int XDATASET_ADVANCED::Add_String_Lookup_Entry(unsigned int i_uiString_Table_Offset, unsigned int i_uiString_Length)
{
	unsigned int uiReturn_Index = m_uiString_Lookup_Table_Entry_Cursor;
	if (m_uiString_Lookup_Table_Size_Alloc == 0 || (uiReturn_Index+ 1) >= m_uiString_Lookup_Table_Size_Alloc)
	{
		Realloc_String_Lookup(m_uiString_Lookup_Table_Size_Alloc << 1);
	}
	m_uiString_Lookup_Table_Entry_Cursor++;
	m_lpcString_Lookup_Table[uiReturn_Index].m_uiTable_Offset = i_uiString_Table_Offset;
	m_lpcString_Lookup_Table[uiReturn_Index].m_uiString_Allocated_Length = i_uiString_Length + 1;
	m_lpcString_Lookup_Table[uiReturn_Index].m_uiString_Used_Length = i_uiString_Length + 1;
	return uiReturn_Index;
}

unsigned int XDATASET_ADVANCED::Add_String_Table_Entry(const char * i_lpszString, unsigned int i_uiString_Length)
{
	unsigned int uiReturn_Index = m_uiString_Data_Cursor;
	if (m_uiString_Data_Size_Alloc == 0 || (uiReturn_Index + i_uiString_Length + 1) > m_uiString_Data_Size_Alloc)
	{
		Realloc_String_Storage(m_uiString_Data_Size_Alloc << 1);
	}
	m_uiString_Data_Cursor += (i_uiString_Length + 1);
	memcpy(&m_lpchString_Data_Table[uiReturn_Index],i_lpszString,sizeof(char) * i_uiString_Length);
	m_lpchString_Data_Table[uiReturn_Index + i_uiString_Length] = 0;
	return uiReturn_Index;
}

unsigned int XDATASET_ADVANCED::Add_String(const char * i_lpszString, unsigned int i_uiString_Length)
{
	unsigned int uiString_Table_Offset = Add_String_Table_Entry(i_lpszString,i_uiString_Length);
	return Add_String_Lookup_Entry(uiString_Table_Offset,i_uiString_Length);
}
void XDATASET_ADVANCED::Modify_String(unsigned int i_uiString_Lookup_Index, const char * i_lpszString, unsigned int i_uiString_Length)
{
	if (i_uiString_Length + 1 < m_lpcString_Lookup_Table[i_uiString_Lookup_Index].m_uiString_Allocated_Length)
	{ // still fits in same space
		char * lpDest = m_lpchString_Data_Table + m_lpcString_Lookup_Table[i_uiString_Lookup_Index].m_uiTable_Offset;
		memset(lpDest,0,m_lpcString_Lookup_Table[i_uiString_Lookup_Index].m_uiString_Allocated_Length * sizeof(char));
		memcpy(lpDest,i_lpszString,i_uiString_Length * sizeof(char));
		m_lpcString_Lookup_Table[i_uiString_Lookup_Index].m_uiString_Used_Length = i_uiString_Length + 1;
		m_lpcString_Lookup_Table[i_uiString_Lookup_Index].m_uiString_Data_Length = i_uiString_Length;
	
	}
	else
	{ // need to shift table
//		unsigned int uiSize_Increase = i_uiString_Length - m_lpcString_Lookup_Table[i_uiString_Lookup_Index].m_uiString_Allocated_Length - 1;
		m_uiString_Data_Dead_Space += m_lpcString_Lookup_Table[i_uiString_Lookup_Index].m_uiString_Allocated_Length;
		// add the string to the end of the table 		
		m_lpcString_Lookup_Table[i_uiString_Lookup_Index].m_uiTable_Offset = Add_String_Table_Entry(i_lpszString,i_uiString_Length);
		m_lpcString_Lookup_Table[i_uiString_Lookup_Index].m_uiString_Used_Length = i_uiString_Length + 1;
		m_lpcString_Lookup_Table[i_uiString_Lookup_Index].m_uiString_Data_Length = i_uiString_Length;
	}
	unsigned int uiSemi_Dead_Space = 0;
	for (unsigned int uiI = 0; uiI < m_uiString_Lookup_Table_Entry_Cursor; uiI++)
	{
		uiSemi_Dead_Space += m_lpcString_Lookup_Table[uiI].m_uiString_Allocated_Length - m_lpcString_Lookup_Table[uiI].m_uiString_Used_Length;
	}
	if ((uiSemi_Dead_Space + m_uiString_Data_Dead_Space) * sizeof(char) > (m_uiString_Data_Size_Alloc >> 3)) // if more than 1/8 of the table is dead space
	{
		Regenerate_String_Table();
	}
}
char * XDATASET_ADVANCED::Get_String(unsigned int i_uiLookup_Table_Index, unsigned int &o_uiString_Length)
{
	char * lpszRet = NULL;
	o_uiString_Length = 0;
	if (i_uiLookup_Table_Index < m_uiString_Lookup_Table_Entry_Cursor)
	{
		lpszRet = m_lpchString_Data_Table + m_lpcString_Lookup_Table[i_uiLookup_Table_Index].m_uiTable_Offset;
		o_uiString_Length = m_lpcString_Lookup_Table[i_uiLookup_Table_Index].m_uiString_Used_Length - 1;
	}
	return lpszRet;
}
void XDATASET_ADVANCED::Regenerate_String_Table(void)
{
	unsigned int uiRequired_Space = 0;
	for (unsigned int uiI = 0; uiI < m_uiString_Lookup_Table_Entry_Cursor; uiI++)
	{
		uiRequired_Space += m_lpcString_Lookup_Table[uiI].m_uiString_Used_Length;
	}
	//round to nearest power of 2
	XRound2(uiRequired_Space);
	if (uiRequired_Space > 0)
	{
		char * lpszNew_Table = new char [uiRequired_Space];
		unsigned int uiNew_Cursor = 0;
		memset(lpszNew_Table,0,sizeof(char) * uiRequired_Space);
		for (unsigned int uiI = 0; uiI < m_uiString_Lookup_Table_Entry_Cursor; uiI++)
		{
			memcpy(lpszNew_Table + uiNew_Cursor,m_lpchString_Data_Table + m_lpcString_Lookup_Table[uiI].m_uiTable_Offset,m_lpcString_Lookup_Table[uiI].m_uiString_Used_Length);
			m_lpcString_Lookup_Table[uiI].m_uiTable_Offset = uiNew_Cursor;
			m_lpcString_Lookup_Table[uiI].m_uiString_Allocated_Length = m_lpcString_Lookup_Table[uiI].m_uiString_Used_Length;
			uiNew_Cursor += m_lpcString_Lookup_Table[uiI].m_uiString_Used_Length;
		}
		delete [] m_lpchString_Data_Table;
		m_lpchString_Data_Table = lpszNew_Table;
		m_uiString_Data_Cursor = uiNew_Cursor;
		m_uiString_Data_Size_Alloc = uiRequired_Space;
		m_uiString_Data_Dead_Space = 0;
	}
	else
	{
		printf("XDATASET_ADVANCED::Regenerate_String_Table Failure: Need giant database.\n");
	}
	
}

void XDATASET_ADVANCED::Allocate(unsigned int i_uiNum_Rows)
{
	if (m_lpeRecord_Descriptor && m_uiNum_Columns > 0 && i_uiNum_Rows > 0)
	{
		unsigned int uiDesired_Size = m_uiRecord_Length * i_uiNum_Rows;
		XRound4k(uiDesired_Size); // round to nearest 4k - typical page size
		unsigned char * lpNew_Table = new unsigned char[uiDesired_Size];
		memset(lpNew_Table,0,sizeof(char) * uiDesired_Size);
		if (m_uiSize_Alloc > 0 && m_uiNum_Rows > 0)
		{// copy old data
			memcpy(lpNew_Table,m_lpData,sizeof(char) * m_uiRecord_Length * m_uiNum_Rows);
			delete [] m_lpData;
		}
		m_lpData = lpNew_Table;
		m_uiNum_Rows = i_uiNum_Rows;
		m_uiSize_Alloc = uiDesired_Size;
	}
	else
	{
		printf("XDATASET_ADVANCED::Allocate Failure: No record size.\n");
	}
}

void XDATASET_ADVANCED::GetElement(unsigned int i_uiColumn, unsigned int i_uiRow_Num,void * o_lpvReturn_Value)
{
	if (o_lpvReturn_Value && i_uiColumn < m_uiNum_Columns && i_uiRow_Num < m_uiNum_Rows)
	{
		unsigned int uiData_Offset = m_lpuiData_Element_Offsets[i_uiColumn] + i_uiRow_Num * m_uiRecord_Length;
		unsigned int uiString_Offset;
		unsigned int uiString_Length;
		char * lpString_Data;
		switch (m_lpeRecord_Descriptor[i_uiColumn])
		{
		case XDT_ERR:
			break;
		case XDT_LOGICAL:
		case XDT_INT:
		case XDT_UINT:
		case XDT_SHORT:
		case XDT_USHORT:
		case XDT_CHAR:
		case XDT_UCHAR:
		case XDT_FLOAT:
		case XDT_DOUBLE:
			memcpy(o_lpvReturn_Value,&m_lpData[uiData_Offset],m_lpuiData_Element_Sizes[i_uiColumn]);
			break;
		case XDT_STRING:
			// Find string table entry offset
			memcpy(&uiString_Offset,&m_lpData[uiData_Offset],m_lpuiData_Element_Sizes[i_uiColumn]);
			if (uiString_Offset & 0x080000000)
			{
				uiString_Offset &= 0x07fffffff; // clear high order bit - used to mark an entry that has been filled
				// get data from string table
				lpString_Data = Get_String(uiString_Offset,uiString_Length);
				// copy into output
				memcpy(o_lpvReturn_Value,lpString_Data,(uiString_Length + 1) * sizeof(char));
			}
			break;
		}
	}
}

void XDATASET_ADVANCED::SetElement(unsigned int i_uiColumn, unsigned int i_uiRow_Num,const void * i_lpvValue)
{
	if (i_lpvValue && i_uiColumn < m_uiNum_Columns && i_uiRow_Num < m_uiNum_Rows)
	{
		unsigned int uiData_Offset = m_lpuiData_Element_Offsets[i_uiColumn] + i_uiRow_Num * m_uiRecord_Length;
		unsigned int uiString_Offset;
//		int iVal;
		switch (m_lpeRecord_Descriptor[i_uiColumn])
		{
		case XDT_ERR:
			break;
		case XDT_LOGICAL:
		case XDT_INT:
		case XDT_UINT:
		case XDT_SHORT:
		case XDT_USHORT:
		case XDT_CHAR:
		case XDT_UCHAR:
		case XDT_FLOAT:
		case XDT_DOUBLE:
			memcpy(&m_lpData[uiData_Offset],i_lpvValue,m_lpuiData_Element_Sizes[i_uiColumn]);
			break;
		case XDT_STRING:
			// Check string table entry offset
			memcpy(&uiString_Offset,&m_lpData[uiData_Offset],m_lpuiData_Element_Sizes[i_uiColumn]);
			if (uiString_Offset & 0x080000000) // check to see if the high order bit is set.  If it is, then there is already data in the string table
			{	
				uiString_Offset = uiString_Offset & 0x07fffffff; // get actual index
				Modify_String(uiString_Offset,(const char *)i_lpvValue,strlen((const char *)i_lpvValue));
			}
			else
			{
				uiString_Offset = Add_String((const char *)i_lpvValue,strlen((char *)i_lpvValue));
				uiString_Offset |= 0x080000000; // set high order bit to indicate there is an entry
				// WARNING: there is a chance that this can cause bad things to happen; specifically if there is more than
				// 2 billion entries in the string lookup table.  However, this code is only expected to run using 32-bit integers and with a only a few GB of storage
				// if it is working with a larger data set, this code will need to be rewritten...

				// Put string table offset into data table
				memcpy(&m_lpData[uiData_Offset],&uiString_Offset,sizeof(int));
			}
			break;
		}
	}
}

class XDATASET_ADVANCED_COLUMN_POTENTIAL_DATA
{
public://@@TODO do some tests to see if we can store in a char or short.
	bool	bNumber;
	bool	bInteger;

	XDATASET_ADVANCED_COLUMN_POTENTIAL_DATA(void)
	{
		bNumber = true;
		bInteger = true;
	}
};

void XDATASET_ADVANCED::ReadDataFileAutomated(const char * i_lpszFilename, bool i_bWhitespace_Separated_Columns, char i_chColumn_Separator, char i_chString_Identifier, unsigned int i_uiHeader_Lines)
{
	if (i_lpszFilename && i_lpszFilename[0] && (i_bWhitespace_Separated_Columns || i_chColumn_Separator != 0))
	{
		FILE * fileIn = fopen(i_lpszFilename,"rt");
		if (fileIn)
		{
			XDATASET_ADVANCED_COLUMN_POTENTIAL_DATA * lpColumn_Type_Data = NULL;
			char *lpszBuffer;
			char *lpszCursor;
			unsigned int uiLine_Count = 0;
			unsigned int uiCurr_Line = 0;
//			unsigned int uiFault_Lines = 0;
			unsigned int uiData_Lines = 0;
			unsigned int uiCurr_Column = 0;
			unsigned int uiNum_Columns = 0;
			unsigned int uiMaxLineSize = 0;
			char	chLastChar = 0;
			char chChar;
			unsigned int uiLineSize = 0;

			// Count # of lines in the file, and determine the maximum line size
			while ((chChar = fgetc(fileIn)) != EOF)
			{
				if ((chLastChar == 10 || chLastChar == 13) && (chChar != 10 && chChar != 13))
				{
					if (uiLineSize > uiMaxLineSize)
						uiMaxLineSize = uiLineSize;
					uiLineSize = 0;
					uiLine_Count++;
				}
				else
					uiLineSize++;
				chLastChar = chChar;
			}
			if (uiLineSize > uiMaxLineSize)
				uiMaxLineSize = uiLineSize;
			uiLine_Count++; // count last line
			uiMaxLineSize += 3; // add space for zero termination
			XRound2(uiMaxLineSize);

			// Allocate buffer for reading
			lpszBuffer = new char[uiMaxLineSize];
			// rewind file
			fseek(fileIn,0,SEEK_SET);
			// First parse the file to determine column data types and number of columns.
			while (fgets(lpszBuffer,uiMaxLineSize,fileIn) != 0)
			{
				if (uiCurr_Line	>= i_uiHeader_Lines)
				{
					if (uiCurr_Line	== i_uiHeader_Lines)
					{
						uiNum_Columns = CountColumns(lpszBuffer,i_bWhitespace_Separated_Columns, i_chColumn_Separator,i_chString_Identifier);
						lpColumn_Type_Data = new XDATASET_ADVANCED_COLUMN_POTENTIAL_DATA[uiNum_Columns];
					}

					lpszCursor = lpszBuffer;
					uiCurr_Column = 0;
					while (uiCurr_Column < uiNum_Columns && lpszCursor[0] != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13)
					{
						if (xIsWhitespace(lpszCursor))
							lpszCursor = xPassWhitespace(lpszCursor);
						while (uiCurr_Column < uiNum_Columns && lpszCursor[0] != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13 && ((i_bWhitespace_Separated_Columns && xIsWhitespace(lpszCursor)) || (!i_bWhitespace_Separated_Columns && lpszCursor[0] == i_chColumn_Separator)))
						{ // handle case of ,,,,,,
							if (i_bWhitespace_Separated_Columns)
								lpszCursor = xPassWhitespace(lpszCursor);
							else
								lpszCursor++;
							if (lpszCursor != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13)
								uiCurr_Column++;
						}

						while (lpszCursor[0] != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13 && !(i_bWhitespace_Separated_Columns && xIsWhitespace(lpszCursor)) && !(!i_bWhitespace_Separated_Columns && lpszCursor[0] == i_chColumn_Separator))
						{ // parse data in the column
							if (lpszCursor[0] == i_chString_Identifier && lpszCursor[1] == i_chString_Identifier && lpszCursor[2] == i_chString_Identifier)
							{ // " or ' character
								lpColumn_Type_Data[uiCurr_Column].bNumber = false;
								lpColumn_Type_Data[uiCurr_Column].bInteger = false;
								lpszCursor+=3;
							}
							else if (lpszCursor[0] == i_chString_Identifier) // whitespace/separator preserving string
							{
								lpColumn_Type_Data[uiCurr_Column].bNumber = false;
								lpColumn_Type_Data[uiCurr_Column].bInteger = false;
								lpszCursor++;// bypass leading quote
								while (lpszCursor[0] != i_chString_Identifier)
								{
									lpszCursor++;
								}
								lpszCursor++; // bypass trailing quote
							}
							else
							{
								if (!xIsWhitespace(lpszCursor))
								{
									lpColumn_Type_Data[uiCurr_Column].bNumber &= xIsANumber(lpszCursor);
									lpColumn_Type_Data[uiCurr_Column].bInteger &= lpColumn_Type_Data[uiCurr_Column].bNumber && lpszCursor[0] != '.' && lpszCursor[0]!='e' && lpszCursor[0]!='E'; // look for cues to floating point data
									lpszCursor++;
								}
								else
									lpszCursor++;
							}
						}
					}
					uiData_Lines++;
				}
				uiCurr_Line++;
			}
			DATATYPE * lpeColumn_Descriptors = new DATATYPE[uiNum_Columns];
			for (unsigned int uiI = 0; uiI < uiNum_Columns; uiI++)
			{
				if (!lpColumn_Type_Data[uiI].bNumber)
					lpeColumn_Descriptors[uiI] = XDT_STRING;
				else if (!lpColumn_Type_Data[uiI].bInteger)
					lpeColumn_Descriptors[uiI] = XDT_DOUBLE;
				else
					lpeColumn_Descriptors[uiI] = XDT_INT;
			}
			DescribeRecord(lpeColumn_Descriptors,uiNum_Columns);
			Allocate(uiLine_Count - i_uiHeader_Lines);

			delete [] lpeColumn_Descriptors;
			delete [] lpszBuffer;
			lpszBuffer = NULL;
					
			fseek(fileIn,0,SEEK_SET);
			ReadDataFileInternal(fileIn,i_bWhitespace_Separated_Columns, i_chColumn_Separator, i_chString_Identifier, i_uiHeader_Lines, uiMaxLineSize);

			if (fileIn)
				fclose(fileIn);
		}
		else
			printf("XDATASET_ADVANCED::ReadDatafileAutomated Failure: Unable to find file %s\n",i_lpszFilename);
	}
	else if (!i_bWhitespace_Separated_Columns && i_chColumn_Separator == 0)
		printf("XDATASET_ADVANCED::ReadDatafileAutomated Failure: No column separator specified.\n");
	else
		printf("XDATASET_ADVANCED::ReadDatafileAutomated Failure: No filename specified.\n");
}

void XDATASET_ADVANCED::ReadDataFile(const char * i_lpszFilename, bool i_bWhitespace_Separated_Columns, char i_chColumn_Separator, char i_chString_Identifier, unsigned int i_uiHeader_Lines)
{
	if (i_lpszFilename && i_lpszFilename[0] && (i_bWhitespace_Separated_Columns || i_chColumn_Separator != 0))
	{
		FILE * fileIn = fopen(i_lpszFilename,"rt");
		if (fileIn)
		{
			char *lpszBuffer;
//			char *lpszCursor;
			bool bColumn_Check_Complete = false;
			unsigned int uiLine_Count = 0;
			unsigned int uiCurr_Line = 0;
//			unsigned int uiFault_Lines = 0;
//			unsigned int uiData_Lines = 0;
//			unsigned int uiCurr_Column = 0;
			unsigned int uiNum_Columns = 0;
			unsigned int uiMaxLineSize = 0;
			char	chChar;
			char	chLastChar = 0;
			unsigned int uiLineSize = 0;

			// Count # of lines in the file, and determine the maximum line size
			while ((chChar = fgetc(fileIn)) != EOF)
			{ // make sure not to count CRLF as two lines
				if ((chLastChar == 10 || chLastChar == 13) && (chChar != 10 && chChar != 13))
				{
					if (uiLineSize > uiMaxLineSize)
						uiMaxLineSize = uiLineSize;
					uiLineSize = 0;
					uiLine_Count++;
				}
				else
					uiLineSize++;
				chLastChar = chChar;
			}
			if (uiLineSize > uiMaxLineSize)
				uiMaxLineSize = uiLineSize;
			uiLine_Count++; // count last line
			uiMaxLineSize+=3; // add space for zero termination
			XRound2(uiMaxLineSize);

			// Allocate buffer for reading
			lpszBuffer = new char[uiMaxLineSize];
			// rewind file
			fseek(fileIn,0,SEEK_SET);
			// First parse the file to determine column data types and number of columns.
			while (fgets(lpszBuffer,uiMaxLineSize,fileIn) != 0 && !bColumn_Check_Complete)
			{
				if (uiCurr_Line	>= i_uiHeader_Lines)
				{
					if (uiCurr_Line	== i_uiHeader_Lines)
					{
						uiNum_Columns = CountColumns(lpszBuffer,i_bWhitespace_Separated_Columns, i_chColumn_Separator,i_chString_Identifier);
						bColumn_Check_Complete = true;
					}
				}
				uiCurr_Line++;
			}
			delete [] lpszBuffer;
			lpszBuffer = NULL;
			if (uiNum_Columns == m_uiNum_Columns)
			{
				Allocate(uiLine_Count - i_uiHeader_Lines);
				fseek(fileIn,0,SEEK_SET);
				ReadDataFileInternal(fileIn,i_bWhitespace_Separated_Columns, i_chColumn_Separator, i_chString_Identifier, i_uiHeader_Lines, uiMaxLineSize);
			}
			else
			{
				printf("XDATASET_ADVANCED::ReadDatafile Failure: Error reading file.  Number of columns mismatch. File has %i, while the specified record format has %i. \n",uiNum_Columns,m_uiNum_Columns);
			}
			if (fileIn)
				fclose(fileIn);
		}
		else
			printf("DATASET_ADVANCED::ReadDatafile: Unable to find file %s\n",i_lpszFilename);
	}
	else if (!i_bWhitespace_Separated_Columns && i_chColumn_Separator == 0)
		printf("XDATASET_ADVANCED::ReadDatafile Failure: No column separator specified.\n");
	else
		printf("XDATASET_ADVANCED::ReadDatafile Failure: No filename specified.\n");
}

unsigned int XDATASET_ADVANCED::CountColumns(const char * lpszBuffer,bool i_bWhitespace_Separated_Columns, char i_chColumn_Separator, char i_chString_Identifier) const
{
	unsigned int uiNum_Columns = 0;
	char * lpszCursor = (char *)lpszBuffer;
	if (xIsWhitespace(lpszCursor))
		lpszCursor = xPassWhitespace(lpszCursor);
	if (lpszCursor[0] != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13)
		uiNum_Columns++;
	while (lpszCursor[0] != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13)
	{
		if (lpszCursor[0] == i_chString_Identifier && lpszCursor[1] == i_chString_Identifier && lpszCursor[2] == i_chString_Identifier)
		{ // " or ' character
			lpszCursor+=3;
		}
		else if (lpszCursor[0] == i_chString_Identifier && lpszCursor[1] == i_chString_Identifier)
		{ // empty string
			lpszCursor+=2;
		}
		else if (lpszCursor[0] == i_chString_Identifier) // whitespace/separator preserving string
		{
			lpszCursor++;// bypass leading quote
			while (lpszCursor[0] != i_chString_Identifier)
			{
				lpszCursor++;
			}
			lpszCursor++; // bypass trailing quote
		}
		else if (i_chColumn_Separator != 0 && lpszCursor[0] == i_chColumn_Separator)
		{
			lpszCursor++;
			if (lpszCursor != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13)
				uiNum_Columns++;

		}
		else if (i_bWhitespace_Separated_Columns && xIsWhitespace(lpszCursor))
		{
			lpszCursor = xPassWhitespace(lpszCursor);
			if (lpszCursor != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13)
				uiNum_Columns++;
		}
		else
		{
			lpszCursor++;
		}
	}
	return uiNum_Columns;
}
// warning: ReadDataFile does not have strings fully implemented.
void XDATASET_ADVANCED::ReadDataFileInternal(FILE * fileIn, bool i_bWhitespace_Separated_Columns, char i_chColumn_Separator, char i_chString_Identifier, unsigned int i_uiHeader_Lines, unsigned int uiMax_Line_Length)
{
	if (fileIn)
	{
		char *lpszBuffer = new char[uiMax_Line_Length];
		unsigned int uiString_Data_Allocated = 256;
		char *lpszStringData = new char[uiString_Data_Allocated];
		unsigned int uiString_Size;
		char *lpszCursor;
		unsigned int uiCurr_Line = 0;
		unsigned int uiData_Lines = 0;
		unsigned int uiCurr_Column = 0;
		unsigned int uiFault_Lines = 0;
		fseek(fileIn,0,SEEK_SET);
		if (i_uiHeader_Lines != 0)
		{
			if (m_uiHeader_Lines > 0)
			{
				for (unsigned int uiI = 0; uiI < m_uiHeader_Lines; uiI++)
				{
					delete [] m_lpszHeader_Lines[uiI];
				}
				delete [] m_lpszHeader_Lines;
				m_lpszHeader_Lines = NULL;
			}
			m_uiHeader_Lines = i_uiHeader_Lines;
			if (m_uiHeader_Lines > 0)
				m_lpszHeader_Lines = new char *[m_uiHeader_Lines];
		}
		while (uiFault_Lines == 0 && fgets(lpszBuffer,uiMax_Line_Length,fileIn) != 0)
		{
			if (uiCurr_Line	>= i_uiHeader_Lines)
			{
				lpszCursor = lpszBuffer;
				uiCurr_Column = 0;
//				printf("Read line %i: %s\n",uiCurr_Line,lpszBuffer);
				while (uiCurr_Column < m_uiNum_Columns && lpszCursor[0] != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13 && uiFault_Lines == 0)
				{
					if (xIsWhitespace(lpszCursor))
						lpszCursor = xPassWhitespace(lpszCursor);
					while (uiCurr_Column < m_uiNum_Columns && lpszCursor[0] != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13 && ((i_bWhitespace_Separated_Columns && xIsWhitespace(lpszCursor)) || (!i_bWhitespace_Separated_Columns && lpszCursor[0] == i_chColumn_Separator)))
					{ // handle case of ,,,,,,
						if (i_bWhitespace_Separated_Columns)
							lpszCursor = xPassWhitespace(lpszCursor);
						else
							lpszCursor++;
						if (lpszCursor != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13)
							uiCurr_Column++;
					}
					if (xIsWhitespace(lpszCursor))
						lpszCursor = xPassWhitespace(lpszCursor);
//					printf("Currently at %s\n",	lpszCursor);
					switch(m_lpeRecord_Descriptor[uiCurr_Column])
					{
					case XDT_STRING:
						uiString_Size = 0;
						while (lpszCursor[0] != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13 && uiFault_Lines == 0 && !(i_bWhitespace_Separated_Columns && xIsWhitespace(lpszCursor)) && !(!i_bWhitespace_Separated_Columns && lpszCursor[0] == i_chColumn_Separator))
						{ // parse data in the column
							if (uiString_Size == (uiString_Data_Allocated - 1))
							{
								char * lpszNewString = new char[uiString_Data_Allocated * 2];
								memset(lpszNewString,0,uiString_Data_Allocated * 2 * sizeof(char));
								memcpy(lpszNewString,lpszStringData,uiString_Data_Allocated * sizeof(char));
								delete [] lpszStringData;
								lpszStringData = lpszNewString;
								uiString_Data_Allocated *= 2;
							}
							if (lpszCursor[0] == i_chString_Identifier && lpszCursor[1] == i_chString_Identifier && lpszCursor[2] == i_chString_Identifier)
							{ // " character
								lpszStringData[uiString_Size] = i_chString_Identifier;
								uiString_Size++;
								lpszCursor+=3;
							}
							else if (lpszCursor[0] == i_chString_Identifier && lpszCursor[1] == i_chString_Identifier)
							{ // empty string
								lpszCursor+=2;
							}
							else if (lpszCursor[0] == i_chString_Identifier) // whitespace/separator preserving string
							{
								lpszCursor++;// bypass leading quote
								while (lpszCursor[0] != i_chString_Identifier)
								{
									if (uiString_Size == (uiString_Data_Allocated - 1))
									{
										char * lpszNewString = new char[uiString_Data_Allocated * 2];
										memset(lpszNewString,0,uiString_Data_Allocated * 2 * sizeof(char));
										memcpy(lpszNewString,lpszStringData,uiString_Data_Allocated * sizeof(char));
										delete [] lpszStringData;
										lpszStringData = lpszNewString;
										uiString_Data_Allocated *= 2;
									}
									lpszStringData[uiString_Size] = lpszCursor[0];
									uiString_Size++;
									lpszCursor++;
								}
								lpszCursor++; // bypass trailing quote
							}
							else
							{
								if (uiString_Size == (uiString_Data_Allocated - 1))
								{
									char * lpszNewString = new char[uiString_Data_Allocated * 2];
									memset(lpszNewString,0,uiString_Data_Allocated * 2 * sizeof(char));
									memcpy(lpszNewString,lpszStringData,uiString_Data_Allocated * sizeof(char));
									delete [] lpszStringData;
									lpszStringData = lpszNewString;
									uiString_Data_Allocated *= 2;
								}
								lpszStringData[uiString_Size] = lpszCursor[0];
								uiString_Size++;
								lpszCursor++;
							}
						}
						lpszStringData[uiString_Size] = 0;
						SetElement(uiCurr_Column,uiData_Lines,lpszStringData);
						break;
					case XDT_FLOAT:
					case XDT_DOUBLE:
						SetElementDbl(uiCurr_Column,uiData_Lines,atof(lpszCursor));
						lpszCursor = xPassNumber(lpszCursor);
//						printf("Moved to %s\n",lpszCursor);
						if ((!i_bWhitespace_Separated_Columns && lpszCursor[0] != i_chColumn_Separator) || (i_bWhitespace_Separated_Columns && !xIsWhitespace(lpszCursor)))
						{
							// may be some garbage in here...
							if (!i_bWhitespace_Separated_Columns)
								lpszCursor = xPassWhitespace(lpszCursor);
							if (!xIsWhitespace(lpszCursor) && lpszCursor[0] != i_chColumn_Separator && lpszCursor[0] != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13)
							{
								uiFault_Lines++; // data fault
								printf("XDATASET_ADVANCED::ReadDatafile Failure (float): Data fault on line %i column %i (%s). \n",uiCurr_Line,uiCurr_Column,lpszCursor);
							}
						}
						break;
					default:
						SetElementInt(uiCurr_Column,uiData_Lines,atoi(lpszCursor));
						lpszCursor = xPassNumber(lpszCursor);
						if ((!i_bWhitespace_Separated_Columns && lpszCursor[0] != i_chColumn_Separator) || (i_bWhitespace_Separated_Columns && !xIsWhitespace(lpszCursor)))
						{
							// may be some garbage in here...
							if (!i_bWhitespace_Separated_Columns)
								lpszCursor = xPassWhitespace(lpszCursor);
							if (!xIsWhitespace(lpszCursor) && lpszCursor[0] != i_chColumn_Separator && lpszCursor[0] != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13)
							{
								uiFault_Lines++; // data fault
								printf("XDATASET_ADVANCED::ReadDatafile Failure (int): Data fault on line %i column %i (%s). \n",uiCurr_Line,uiCurr_Column,lpszCursor);
							}
						}
						break;
					}
				}
				uiData_Lines++;
			}
			else
			{
				m_lpszHeader_Lines[uiCurr_Line] = new char[strlen(lpszBuffer) + 1];
				strcpy(m_lpszHeader_Lines[uiCurr_Line],lpszBuffer);
			}
			uiCurr_Line++;
		}
		delete [] lpszBuffer;
		delete [] lpszStringData;
		lpszBuffer = NULL;
	}
}

void XDATASET_ADVANCED::SetElementDbl(unsigned int i_uiColumn, unsigned int i_uiRow, const double &i_dValue)
{
	if (m_lpeRecord_Descriptor[i_uiColumn] == XDT_FLOAT)
	{
		float fValue = (float)i_dValue;
		SetElement(i_uiColumn,i_uiRow,&fValue);
	}
	else if (m_lpeRecord_Descriptor[i_uiColumn] == XDT_DOUBLE)
		SetElement(i_uiColumn,i_uiRow,&i_dValue);
}
void XDATASET_ADVANCED::SetElementInt(unsigned int i_uiColumn, unsigned int i_uiRow, int i_iValue)
{
	switch(m_lpeRecord_Descriptor[i_uiColumn])
	{
	case XDT_CHAR:
	case XDT_UCHAR:
		{
			char chValue = (char)i_iValue;
			SetElement(i_uiColumn,i_uiRow,&chValue);
		}
		break;
	case XDT_SHORT:
	case XDT_USHORT:
		{
			short sValue = (short)i_iValue;
			SetElement(i_uiColumn,i_uiRow,&sValue);
		}
		break;
	case XDT_INT:
	case XDT_UINT:
		SetElement(i_uiColumn,i_uiRow,&i_iValue);
		break;
        default:
                break;
	}
}

double	XDATASET_ADVANCED::GetElementDbl(unsigned int i_uiColumn, unsigned int i_uiRow)
{
	double dRet = DBL_MAX;
	switch(m_lpeRecord_Descriptor[i_uiColumn])
	{
	case XDT_FLOAT:
		{
			float fValue;
			GetElement(i_uiColumn,i_uiRow,&fValue);
			dRet = fValue;
		}
		break;
	case XDT_DOUBLE:
		GetElement(i_uiColumn,i_uiRow,&dRet);
		break;
        default:
                break;
	}
	return dRet;
}
int		XDATASET_ADVANCED::GetElementInt(unsigned int i_uiColumn, unsigned int i_uiRow)
{
	int iRet = -1;
	switch(m_lpeRecord_Descriptor[i_uiColumn])
	{
	case XDT_LOGICAL:
		{
			bool bValue;
			GetElement(i_uiColumn,i_uiRow,&bValue);
			iRet = bValue ? 1 : 0;
		}
		break;
	case XDT_CHAR:
	case XDT_UCHAR:
		{
			char chValue;
			GetElement(i_uiColumn,i_uiRow,&chValue);
			iRet = chValue;
		}
		break;
	case XDT_SHORT:
	case XDT_USHORT:
		{
			short sValue;
			GetElement(i_uiColumn,i_uiRow,&sValue);
			iRet = sValue;
		}
		break;
	case XDT_INT:
	case XDT_UINT:
		GetElement(i_uiColumn,i_uiRow,&iRet);
		break;
        default:
                break;
	}
	return iRet;
}
const char *	XDATASET_ADVANCED::GetElementString(unsigned int i_uiColumn, unsigned int i_uiRow)
{
	char * lpszRet = NULL;
	if (m_lpeRecord_Descriptor && i_uiColumn < m_uiNum_Columns && m_lpeRecord_Descriptor[i_uiColumn] == XDT_STRING)
	{
		unsigned int uiData_Offset = m_lpuiData_Element_Offsets[i_uiColumn] + i_uiRow * m_uiRecord_Length;
		unsigned int uiString_Offset;
		// Find string table entry offset
		memcpy(&uiString_Offset,&m_lpData[uiData_Offset],m_lpuiData_Element_Sizes[i_uiColumn]);
		if (uiString_Offset & 0x080000000)
		{
			unsigned int uiLength;
			uiString_Offset &= 0x07fffffff; // get actual index
			lpszRet = Get_String(uiString_Offset,uiLength);
		}
	}
	return lpszRet;
}

void XDATASET_ADVANCED::SaveFile(const char * i_lpszFilename, char i_chColumn_Separator, char i_chString_Delimiter, const char * lpszFloatingPointFormat)
{
	FILE * fileOut = fopen(i_lpszFilename,"wt");
	if (fileOut && m_uiNum_Rows > 0 && m_uiNum_Columns > 0)
	{
		unsigned int uiRow, uiCol;
		const char * lpszString;
		const char * lpColSep;
		const char * lpStrDel;
		int iValue;
		double dValue;
		for (unsigned int uiI = 0; uiI < m_uiHeader_Lines; uiI++)
		{
			fprintf(fileOut,m_lpszHeader_Lines[uiI]);
		}

		for (uiRow = 0; uiRow < m_uiNum_Rows; uiRow++)
		{
			for (uiCol = 0; uiCol < m_uiNum_Columns; uiCol++)
			{
				switch (m_lpeRecord_Descriptor[uiCol])
				{
				case XDT_ERR:
					printf("XDATASET_ADVANCED::SaveFile Failure: Bad column format (Column %i).\n",uiCol);
					uiCol = m_uiNum_Columns;
					uiRow = m_uiNum_Rows;
					break;
				case XDT_LOGICAL:
				case XDT_INT:
				case XDT_SHORT:
				case XDT_CHAR:
					iValue = GetElementInt(uiCol,uiRow);
					fprintf(fileOut,"%i",iValue);
					break;
				case XDT_UINT:
				case XDT_USHORT:
				case XDT_UCHAR:
					iValue = GetElementInt(uiCol,uiRow);
					fprintf(fileOut,"%u",iValue);
					break;
				case XDT_STRING:
					lpszString = GetElementString(uiCol,uiRow);
					lpColSep = strchr(lpszString,i_chColumn_Separator);
					lpStrDel = strchr(lpszString,i_chString_Delimiter);
					if (lpColSep || lpStrDel)
					{ // must parse the string
						const char * lpszCursor = lpszString;
						fprintf(fileOut,"%c",i_chString_Delimiter); // give it an opening " (or equivalent)

						while (lpszCursor != 0)
						{
							if (lpszCursor[0] == i_chString_Delimiter)
							{
								fprintf(fileOut,"%c%c%c",i_chString_Delimiter,i_chString_Delimiter,i_chString_Delimiter); // insert """ (or equivalent)
							}
							else
								fprintf(fileOut,"%c",lpszCursor[0]);
							lpszCursor++;
						}
					}
					else
						fprintf(fileOut,"%s",lpszString);
					break;
				case XDT_FLOAT:
				case XDT_DOUBLE:
					dValue = GetElementDbl(uiCol,uiRow);
					fprintf(fileOut,lpszFloatingPointFormat,dValue);
					break;
                                default:
                                        break;
				}
				if (uiCol != m_uiNum_Columns - 1)
					fprintf(fileOut,"%c",i_chColumn_Separator);
			}
			fprintf(fileOut,"\n");
		}
		fclose(fileOut);
		fileOut = NULL;
	}
	else
	{
		printf("XDATASET_ADVANCED::SaveFile Failure:  Unable to open file %s.\n",i_lpszFilename);
	}
}





#ifndef OPEN_MPI
void XDATASET_ADVANCED::TransferMPIBCast(int i_lpCommunicator, int i_iSource_Rank)
#else
void XDATASET_ADVANCED::TransferMPIBCast(ompi_communicator_t * i_lpCommunicator, int i_iSource_Rank)
#endif
{
	int iMyRank;
	MPI_Comm_rank(i_lpCommunicator,&iMyRank);
	bool bI_Am_Source = iMyRank == i_iSource_Rank;

	// Basic Sizing data
	unsigned int uiNum_Columns = m_uiNum_Columns, uiNum_Rows = m_uiNum_Rows, uiString_Block_Size, uiString_Lookup_Size, uiRecord_Length = m_uiRecord_Length;
	MPI_Bcast(&uiNum_Columns,1,MPI_UNSIGNED,i_iSource_Rank,i_lpCommunicator);
	MPI_Bcast(&uiNum_Rows,1,MPI_UNSIGNED,i_iSource_Rank,i_lpCommunicator);
	MPI_Bcast(&uiRecord_Length,1,MPI_UNSIGNED,i_iSource_Rank,i_lpCommunicator);
	if (bI_Am_Source)
	{
		// string table size information		
		MPI_Bcast(&m_uiString_Lookup_Table_Size_Alloc,1,MPI_UNSIGNED,i_iSource_Rank,i_lpCommunicator); // number of lookup table entries used
		MPI_Bcast(&m_uiString_Data_Size_Alloc,1,MPI_UNSIGNED,i_iSource_Rank,i_lpCommunicator); // total string data block size

		// Record format information
		MPI_Bcast(m_lpeRecord_Descriptor,m_uiNum_Columns,MPI_UNSIGNED,i_iSource_Rank,i_lpCommunicator); // actually an enum, but unsigned should work
	}
	else
	{

		// string table size information		
		MPI_Bcast(&uiString_Lookup_Size,1,MPI_UNSIGNED,i_iSource_Rank,i_lpCommunicator); // number of lookup table entries used
		MPI_Bcast(&uiString_Block_Size,1,MPI_UNSIGNED,i_iSource_Rank,i_lpCommunicator); // total string data block size

		// Record Format
		DATATYPE * lpdDatatpes = new DATATYPE[uiNum_Columns];
		MPI_Bcast(lpdDatatpes,uiNum_Columns,MPI_UNSIGNED,i_iSource_Rank,i_lpCommunicator); // actually an enum, but unsigned should work
		// Generate data blocks
		DescribeRecord(lpdDatatpes,uiNum_Columns);
		delete [] lpdDatatpes;
		Allocate(uiNum_Rows);

		if (uiRecord_Length == m_uiRecord_Length && uiNum_Columns == m_uiNum_Columns && uiNum_Rows == m_uiNum_Rows)
		{
			// Get string data
			Realloc_String_Storage(uiString_Block_Size);
			Realloc_String_Lookup(uiString_Lookup_Size);
		}
		else
		{
			printf("XDATASET_ADVANCED::TransferMPIBCast Failure:  Size mismatch.\n");
			MPI_Abort(MPI_COMM_WORLD,0);
		}
	}
	// String data
	MPI_Bcast(&m_uiString_Data_Cursor,1,MPI_UNSIGNED,i_iSource_Rank,i_lpCommunicator);
	MPI_Bcast(&m_uiString_Data_Dead_Space,1,MPI_UNSIGNED,i_iSource_Rank,i_lpCommunicator); // total string data block size
	MPI_Bcast(m_lpchString_Data_Table,m_uiString_Data_Size_Alloc,MPI_CHAR,i_iSource_Rank,i_lpCommunicator);
	// String lookup data
	MPI_Bcast(&m_uiString_Lookup_Table_Entry_Cursor,1,MPI_UNSIGNED,i_iSource_Rank,i_lpCommunicator);
	for (unsigned int uiI = 0; uiI < m_uiString_Lookup_Table_Entry_Cursor; uiI++)
	{
		MPI_Bcast(&m_lpcString_Lookup_Table[uiI].m_uiTable_Offset,1,MPI_UNSIGNED,i_iSource_Rank,i_lpCommunicator);
		MPI_Bcast(&m_lpcString_Lookup_Table[uiI].m_uiString_Allocated_Length,1,MPI_UNSIGNED,i_iSource_Rank,i_lpCommunicator);
		MPI_Bcast(&m_lpcString_Lookup_Table[uiI].m_uiString_Used_Length,1,MPI_UNSIGNED,i_iSource_Rank,i_lpCommunicator);
		MPI_Bcast(&m_lpcString_Lookup_Table[uiI].m_uiString_Data_Length,1,MPI_UNSIGNED,i_iSource_Rank,i_lpCommunicator);
	}
	// Data block
	MPI_Bcast(m_lpData,m_uiNum_Columns * m_uiNum_Rows * m_uiRecord_Length,MPI_CHAR,i_iSource_Rank,i_lpCommunicator);
}
#ifndef OPEN_MPI
void XDATASET_ADVANCED::TransferMPI(int i_lpCommunicator, int i_iSource_Rank, int i_iDest_Rank)
#else
void XDATASET_ADVANCED::TransferMPI(ompi_communicator_t * i_lpCommunicator, int i_iSource_Rank, int i_iDest_Rank)
#endif
{
	int iMyRank;
	MPI_Comm_rank(i_lpCommunicator,&iMyRank);
	bool bI_Am_Source = iMyRank == i_iSource_Rank;

	// Basic Sizing data
	if (bI_Am_Source)
	{
		// Basic Sizing data
		MPI_Send(&m_uiNum_Columns,1,MPI_UNSIGNED,i_iDest_Rank,MPI_ANY_TAG,i_lpCommunicator);
		MPI_Send(&m_uiNum_Rows,1,MPI_UNSIGNED,i_iDest_Rank,MPI_ANY_TAG,i_lpCommunicator);
		MPI_Send(&m_uiRecord_Length,1,MPI_UNSIGNED,i_iDest_Rank,MPI_ANY_TAG,i_lpCommunicator);

		// string table size information		
		MPI_Send(&m_uiString_Lookup_Table_Size_Alloc,1,MPI_UNSIGNED,i_iDest_Rank,MPI_ANY_TAG,i_lpCommunicator); // number of lookup table entries used
		MPI_Send(&m_uiString_Data_Size_Alloc,1,MPI_UNSIGNED,i_iDest_Rank,MPI_ANY_TAG,i_lpCommunicator); // total string data block size

		// Record format information
		MPI_Send(m_lpeRecord_Descriptor,m_uiNum_Columns,MPI_UNSIGNED,i_iDest_Rank,MPI_ANY_TAG,i_lpCommunicator); // actually an enum, but unsigned should work

		// String data
		MPI_Send(&m_uiString_Data_Cursor,1,MPI_UNSIGNED,i_iDest_Rank,MPI_ANY_TAG,i_lpCommunicator);
		MPI_Send(&m_uiString_Data_Dead_Space,1,MPI_UNSIGNED,i_iDest_Rank,MPI_ANY_TAG,i_lpCommunicator); // total string data block size
		MPI_Send(m_lpchString_Data_Table,m_uiString_Data_Size_Alloc,MPI_CHAR,i_iDest_Rank,MPI_ANY_TAG,i_lpCommunicator);
		// String lookup data
		MPI_Send(&m_uiString_Lookup_Table_Entry_Cursor,1,MPI_UNSIGNED,i_iDest_Rank,MPI_ANY_TAG,i_lpCommunicator);
		for (unsigned int uiI = 0; uiI < m_uiString_Lookup_Table_Entry_Cursor; uiI++)
		{
			MPI_Send(&m_lpcString_Lookup_Table[uiI].m_uiTable_Offset,1,MPI_UNSIGNED,i_iDest_Rank,MPI_ANY_TAG,i_lpCommunicator);
			MPI_Send(&m_lpcString_Lookup_Table[uiI].m_uiString_Allocated_Length,1,MPI_UNSIGNED,i_iDest_Rank,MPI_ANY_TAG,i_lpCommunicator);
			MPI_Send(&m_lpcString_Lookup_Table[uiI].m_uiString_Used_Length,1,MPI_UNSIGNED,i_iDest_Rank,MPI_ANY_TAG,i_lpCommunicator);
			MPI_Send(&m_lpcString_Lookup_Table[uiI].m_uiString_Data_Length,1,MPI_UNSIGNED,i_iDest_Rank,MPI_ANY_TAG,i_lpCommunicator);
		}
		// Data block
		MPI_Send(m_lpData,m_uiNum_Columns * m_uiNum_Rows * m_uiRecord_Length,MPI_CHAR,i_iDest_Rank,MPI_ANY_TAG,i_lpCommunicator);
	}
	else
	{
		MPI_Status sStatus;
		unsigned int uiNum_Columns, uiNum_Rows, uiString_Block_Size, uiString_Lookup_Size, uiRecord_Length;

		// Basic Sizing data
		MPI_Recv(&uiNum_Columns,1,MPI_UNSIGNED,i_iSource_Rank,MPI_ANY_TAG,i_lpCommunicator,&sStatus);
		MPI_Recv(&uiNum_Rows,1,MPI_UNSIGNED,i_iSource_Rank,MPI_ANY_TAG,i_lpCommunicator,&sStatus);
		MPI_Recv(&uiRecord_Length,1,MPI_UNSIGNED,i_iSource_Rank,MPI_ANY_TAG,i_lpCommunicator,&sStatus);

		// string table size information		
		MPI_Recv(&uiString_Lookup_Size,1,MPI_UNSIGNED,i_iSource_Rank,MPI_ANY_TAG,i_lpCommunicator,&sStatus); // number of lookup table entries used
		MPI_Recv(&uiString_Block_Size,1,MPI_UNSIGNED,i_iSource_Rank,MPI_ANY_TAG,i_lpCommunicator,&sStatus); // total string data block size

		// Record Format
		DATATYPE * lpdDatatpes = new DATATYPE[uiNum_Columns];
		MPI_Recv(lpdDatatpes,uiNum_Columns,MPI_UNSIGNED,i_iSource_Rank,MPI_ANY_TAG,i_lpCommunicator,&sStatus); // actually an enum, but unsigned should work
		// Generate data blocks
		DescribeRecord(lpdDatatpes,uiNum_Columns);
		delete [] lpdDatatpes;
		Allocate(uiNum_Rows);

		if (uiRecord_Length == m_uiRecord_Length && uiNum_Columns == m_uiNum_Columns && uiNum_Rows == m_uiNum_Rows)
		{
			// Get string data
			Realloc_String_Storage(uiString_Block_Size);
			Realloc_String_Lookup(uiString_Lookup_Size);
		}
		else
		{
			printf("XDATASET_ADVANCED::TransferMPIBCast Failure:  Size mismatch.\n");
			MPI_Abort(MPI_COMM_WORLD,0);
		}

		// String data
		MPI_Recv(&m_uiString_Data_Cursor,1,MPI_UNSIGNED,i_iSource_Rank,MPI_ANY_TAG,i_lpCommunicator,&sStatus);
		MPI_Recv(&m_uiString_Data_Dead_Space,1,MPI_UNSIGNED,i_iSource_Rank,MPI_ANY_TAG,i_lpCommunicator,&sStatus); // total string data block size
		MPI_Recv(m_lpchString_Data_Table,m_uiString_Data_Size_Alloc,MPI_CHAR,i_iSource_Rank,MPI_ANY_TAG,i_lpCommunicator,&sStatus);
		// String lookup data
		MPI_Recv(&m_uiString_Lookup_Table_Entry_Cursor,1,MPI_UNSIGNED,i_iSource_Rank,MPI_ANY_TAG,i_lpCommunicator,&sStatus);
		for (unsigned int uiI = 0; uiI < m_uiString_Lookup_Table_Entry_Cursor; uiI++)
		{
			MPI_Recv(&m_lpcString_Lookup_Table[uiI].m_uiTable_Offset,1,MPI_UNSIGNED,i_iSource_Rank,MPI_ANY_TAG,i_lpCommunicator,&sStatus);
			MPI_Recv(&m_lpcString_Lookup_Table[uiI].m_uiString_Allocated_Length,1,MPI_UNSIGNED,i_iSource_Rank,MPI_ANY_TAG,i_lpCommunicator,&sStatus);
			MPI_Recv(&m_lpcString_Lookup_Table[uiI].m_uiString_Used_Length,1,MPI_UNSIGNED,i_iSource_Rank,MPI_ANY_TAG,i_lpCommunicator,&sStatus);
			MPI_Recv(&m_lpcString_Lookup_Table[uiI].m_uiString_Data_Length,1,MPI_UNSIGNED,i_iSource_Rank,MPI_ANY_TAG,i_lpCommunicator,&sStatus);
		}
		// Data block
		MPI_Recv(m_lpData,m_uiNum_Columns * m_uiNum_Rows * m_uiRecord_Length,MPI_CHAR,i_iDest_Rank,MPI_ANY_TAG,i_lpCommunicator,&sStatus);
	}


}

void XDATASET_ADVANCED::Realloc_String_Storage(unsigned int i_uiDesired_Size)
{
	XRound2(i_uiDesired_Size); // make sure it is a power of 2
	if (i_uiDesired_Size == 0 || i_uiDesired_Size > m_uiString_Data_Size_Alloc)
	{
		if (m_uiString_Data_Size_Alloc > 0)
		{
			char * lpNew_Table = new char[i_uiDesired_Size];
			memset(lpNew_Table,0,sizeof(char) * (i_uiDesired_Size));
			memcpy(lpNew_Table,m_lpchString_Data_Table,sizeof(char) * m_uiString_Data_Size_Alloc);
			delete [] m_lpchString_Data_Table;
			m_lpchString_Data_Table = lpNew_Table;
			m_uiString_Data_Size_Alloc = i_uiDesired_Size; // same as *2
		}
		else
		{
			if (i_uiDesired_Size < 1024)
				i_uiDesired_Size = 1024;
			m_uiString_Data_Size_Alloc = i_uiDesired_Size;
			m_lpchString_Data_Table = new char[m_uiString_Data_Size_Alloc];
			memset(m_lpchString_Data_Table,0,sizeof(char) * (m_uiString_Data_Size_Alloc));
			m_uiString_Data_Cursor = 0;
			m_uiString_Data_Dead_Space = 0;
		}
	}
}
void XDATASET_ADVANCED::Realloc_String_Lookup(unsigned int i_uiDesired_Size)
{
	XRound2(i_uiDesired_Size); // make sure it is a power of 2
	if (i_uiDesired_Size == 0 || i_uiDesired_Size > m_uiString_Lookup_Table_Size_Alloc)
	{
		// need more storage space
		if (m_uiString_Lookup_Table_Size_Alloc > 0)
		{
			XDATASET_ADVANCED_STRING_INDEX_DATA * lpNew_Lookup = new XDATASET_ADVANCED_STRING_INDEX_DATA[m_uiString_Lookup_Table_Size_Alloc << 1];
			memset(lpNew_Lookup,0,sizeof(XDATASET_ADVANCED_STRING_INDEX_DATA) * (m_uiString_Lookup_Table_Size_Alloc << 1));
			memcpy(lpNew_Lookup,m_lpcString_Lookup_Table,sizeof(XDATASET_ADVANCED_STRING_INDEX_DATA) * m_uiString_Lookup_Table_Size_Alloc);
			delete [] m_lpcString_Lookup_Table;
			m_lpcString_Lookup_Table = lpNew_Lookup;
			m_uiString_Lookup_Table_Size_Alloc <<= 1; // same as *2
		}
		else
		{ // no lookup table yet allocated
			// start by determining how many entries we expect
			unsigned int uiNum_Records = 0;
			if (m_uiNum_Rows > 0 && m_uiNum_Columns > 0)
			{
				unsigned int uiNum_Records = 0;
				for (unsigned int uiI = 0; uiI < m_uiNum_Columns; uiI++)
				{
					if (m_lpeRecord_Descriptor[uiI] == XDT_STRING)
						uiNum_Records ++;
				}
				if (uiNum_Records == 0)
					uiNum_Records = 1;
				uiNum_Records *= m_uiNum_Rows;
			}
			if (uiNum_Records == 0)
				uiNum_Records = 256;
			else //round to nearest power of 2
				XRound2(uiNum_Records);
			if (uiNum_Records != 0 && uiNum_Records < i_uiDesired_Size)
				uiNum_Records = i_uiDesired_Size;
			// if it is 0 we need more than 4 billion records....
			if (uiNum_Records > 0)
			{
				XDATASET_ADVANCED_STRING_INDEX_DATA * lpNew_Lookup = new XDATASET_ADVANCED_STRING_INDEX_DATA[uiNum_Records];
				memset(lpNew_Lookup,0,sizeof(XDATASET_ADVANCED_STRING_INDEX_DATA) * uiNum_Records);
				m_lpcString_Lookup_Table = lpNew_Lookup;
				m_uiString_Lookup_Table_Size_Alloc = uiNum_Records;
			}
			else
			{
				printf("XDATASET_ADVANCED::Realloc_String_Lookup Failure: Need giant database.\n");
			}
		}
	}
}


XDATASET_ADVANCED::XDATASET_ADVANCED(void)
{
	// String lookup table
	m_lpcString_Lookup_Table = NULL;
	m_uiString_Lookup_Table_Size_Alloc = 0;
	m_uiString_Lookup_Table_Entry_Cursor = 0;

	// String data table
	m_lpchString_Data_Table = NULL;
	m_uiString_Data_Cursor = 0;
	m_uiString_Data_Size_Alloc = 0;
	m_uiString_Data_Dead_Space = 0;

	// Record information
	m_lpeRecord_Descriptor = NULL;
	m_lpuiData_Element_Offsets = NULL;
	m_lpuiData_Element_Sizes = NULL;
	m_uiRecord_Length = 0;
	// Data table
	m_lpData = NULL;
	m_uiNum_Columns = 0;
	m_uiNum_Rows = 0;
	m_uiSize_Alloc = 0;

	m_lpszHeader_Lines = NULL;
	m_uiHeader_Lines = 0;
}
XDATASET_ADVANCED::XDATASET_ADVANCED(const XDATASET_ADVANCED & i_cRHO)
{
	// String lookup table
	m_lpcString_Lookup_Table = NULL;
	m_uiString_Lookup_Table_Size_Alloc = 0;
	m_uiString_Lookup_Table_Entry_Cursor = 0;

	// String data table
	m_lpchString_Data_Table = NULL;
	m_uiString_Data_Cursor = 0;
	m_uiString_Data_Size_Alloc = 0;
	m_uiString_Data_Dead_Space = 0;

	// Record information
	m_lpeRecord_Descriptor = NULL;
	m_lpuiData_Element_Offsets = NULL;
	m_lpuiData_Element_Sizes = NULL;
	m_uiRecord_Length = 0;
	// Data table
	m_lpData = NULL;
	m_uiNum_Columns = 0;
	m_uiNum_Rows = 0;
	m_uiSize_Alloc = 0;

	Copy(i_cRHO);
}
void XDATASET_ADVANCED::Copy(const XDATASET_ADVANCED & i_cRHO)
{
	DescribeRecord(i_cRHO.m_lpeRecord_Descriptor,i_cRHO.m_uiNum_Columns);
	Allocate(i_cRHO.m_uiNum_Rows);
	Realloc_String_Storage(i_cRHO.m_uiString_Data_Size_Alloc);
	Realloc_String_Lookup(i_cRHO.m_uiString_Lookup_Table_Size_Alloc);

	// String lookup table
	memcpy(m_lpcString_Lookup_Table,i_cRHO.m_lpcString_Lookup_Table,i_cRHO.m_uiString_Lookup_Table_Size_Alloc);
	m_uiString_Lookup_Table_Entry_Cursor = i_cRHO.m_uiString_Lookup_Table_Entry_Cursor;

	// String data table
	memcpy(m_lpchString_Data_Table,i_cRHO.m_lpchString_Data_Table,i_cRHO.m_uiString_Data_Size_Alloc);
	m_uiString_Data_Cursor = i_cRHO.m_uiString_Data_Cursor;
	m_uiString_Data_Dead_Space = i_cRHO.m_uiString_Data_Dead_Space;

	if (m_uiHeader_Lines > 0)
	{
		for (unsigned int uiI = 0; uiI < m_uiHeader_Lines; uiI++)
		{
			delete [] m_lpszHeader_Lines[uiI];
		}
		delete [] m_lpszHeader_Lines;
		m_lpszHeader_Lines = NULL;
	}
	m_uiHeader_Lines = i_cRHO.m_uiHeader_Lines;
	m_lpszHeader_Lines = new char *[m_uiHeader_Lines];
	for (unsigned int uiI = 0; uiI < m_uiHeader_Lines; uiI++)
	{
		m_lpszHeader_Lines[uiI] = new char[strlen(i_cRHO.m_lpszHeader_Lines[uiI]) + 1];
		strcpy(m_lpszHeader_Lines[uiI],i_cRHO.m_lpszHeader_Lines[uiI]);
	}


}

XDATASET_ADVANCED & XDATASET_ADVANCED::operator =(const XDATASET_ADVANCED &i_cRHO)
{
	Copy(i_cRHO);
	return *this;
}

XDATASET_ADVANCED::~XDATASET_ADVANCED(void)
{
	// String lookup table
	if (m_lpcString_Lookup_Table)
		delete [] m_lpcString_Lookup_Table;
	m_lpcString_Lookup_Table = NULL;
	m_uiString_Lookup_Table_Size_Alloc = 0;
	m_uiString_Lookup_Table_Entry_Cursor = 0;

	// String data table
	if (m_lpchString_Data_Table)
		delete [] m_lpchString_Data_Table;
	m_lpchString_Data_Table = NULL;
	m_uiString_Data_Cursor = 0;
	m_uiString_Data_Size_Alloc = 0;
	m_uiString_Data_Dead_Space = 0;

	// Record information
	if (m_lpeRecord_Descriptor)
		delete [] m_lpeRecord_Descriptor;
	m_lpeRecord_Descriptor = NULL;
	if (m_lpuiData_Element_Offsets)
		delete [] m_lpuiData_Element_Offsets;
	m_lpuiData_Element_Offsets = NULL;
	if (m_lpuiData_Element_Sizes)
		delete [] m_lpuiData_Element_Sizes;
	m_lpuiData_Element_Sizes = NULL;
	m_uiRecord_Length = 0;
	// Data table
	if (m_lpData)
		delete [] m_lpData;
	m_lpData = NULL;
	m_uiNum_Columns = 0;
	m_uiNum_Rows = 0;
	m_uiSize_Alloc = 0;
}

void XDATASET_ADVANCED::SetHeaderLines(unsigned int i_uiNum_Header_Lines, const char ** i_lpszHeader_Lines)
{
	if (m_uiHeader_Lines > 0)
	{
		for (unsigned int uiI = 0; uiI < m_uiHeader_Lines; uiI++)
		{
			delete [] m_lpszHeader_Lines[uiI];
		}
		delete [] m_lpszHeader_Lines;
		m_lpszHeader_Lines = NULL;
	}
	m_uiHeader_Lines = i_uiNum_Header_Lines;
	if (m_uiHeader_Lines > 0)
	{
		m_lpszHeader_Lines = new char *[m_uiHeader_Lines];
		for (unsigned int uiI = 0; uiI < m_uiHeader_Lines; uiI++)
		{
			m_lpszHeader_Lines[uiI] = new char [strlen(i_lpszHeader_Lines[uiI]) + 1];
			strcpy(m_lpszHeader_Lines[uiI],i_lpszHeader_Lines[uiI]);
		}
	}
}

