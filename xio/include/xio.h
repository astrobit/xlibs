#pragma once
#include <cstdlib>
#include <cstdio>
#include <xstdlib.h>
#ifdef PARALLEL
#include <mpi.h>
#endif // ifdef PARALLEL

#include <cmath>

#include <string>
#include <map>
//#include <stdint.h>
#include <cstdint>

#include <vector>

// xdataparse
// used for reading matrix data of any type from a file

class xdataparse
{
protected:
	std::vector< std::vector< std::string> > m_vvsData;
	std::vector< std::vector< xstdlib::datatype> > m_vvsDatatypes;
	size_t m_tMax_Cols;
	bool m_bAll_Rows_Same_Size;
public:
	xdataparse(void)
	{
		m_tMax_Cols = 0;
		m_bAll_Rows_Same_Size = true;
	}
	xdataparse(const std::string & i_szFilename, bool i_bWhitespace_Separated = false, char i_chColumn_Seperator = ',')
	{
		Read_File(i_szFilename, i_bWhitespace_Separated, i_chColumn_Seperator);
	}
	void Read_File(const std::string & i_szFilename, bool i_bWhitespace_Separated = false, char i_chColumn_Seperator = ',');
	void Parse_String(const std::string & i_szFilename, bool i_bWhitespace_Separated = false, char i_chColumn_Seperator = ',');

	size_t Get_Num_Columns(void) const {return m_tMax_Cols;}
	size_t Get_Num_Columns_of_Row(size_t i_tRow) const {size_t tRet = 0; if (i_tRow < m_vvsData.size()) tRet = m_vvsData[i_tRow].size(); return tRet;}
	size_t Get_Num_Rows(void) const {return m_vvsData.size();}
	bool	All_Rows_Same_Size(void) const {return m_bAll_Rows_Same_Size;}
	std::string Get_Element(size_t i_tRow, size_t i_tColumn) const
	{
		std::string sRet;
		if (i_tRow < m_vvsData.size() && i_tColumn < m_vvsData[i_tRow].size())
			sRet = m_vvsData[i_tRow][i_tColumn];
		return sRet;
	}
	xstdlib::datatype Get_Element_Type(size_t i_tRow, size_t i_tColumn) const
	{
		xstdlib::datatype eRet = xstdlib::empty;
		if (i_tRow < m_vvsDatatypes.size() && i_tColumn < m_vvsDatatypes[i_tRow].size())
			eRet = m_vvsDatatypes[i_tRow][i_tColumn];
		return eRet;
	}
};

class xdataset_improved : public xdataparse
{
private:
	std::vector<std::vector<std::string> > m_szHeader_Lines;
public:
	xstdlib::datatype Get_Column_Type(size_t i_uiColumn);

	void Allocate(size_t i_tNum_Rows, size_t i_tNum_Columns);
	void Read_Data_File(std::string i_lpszFilename, bool i_bWhitespace_Separated_Columns = false, char i_chColumn_Separator = ',', size_t i_tHeader_Lines = 1);
	void Read_xdataset(std::string i_lpszFilename);

	void Zero(void); // make all integer and double fields zero, make all string fields empty 

	double Get_Element_Double(size_t i_tRow, size_t i_tColumn) const;
	int Get_Element_Int(size_t i_tRow, size_t i_tColumn) const;

	void Set_Element(size_t i_tRow, size_t i_tColumn, const std::string &i_szString);
	void Set_Element(size_t i_tRow, size_t i_tColumn, const double & i_dValue);
	void Set_Element(size_t i_tRow, size_t i_tColumn, int i_iValue);
	void Set_Element(size_t i_tRow, size_t i_tColumn, unsigned int i_uiValue);
	void Set_Element(size_t i_tRow, size_t i_tColumn, bool i_bValue);

	inline void Set_Header_Lines(std::vector<std::vector<std::string> > i_vvszHeader_Lines) { m_szHeader_Lines = i_vvszHeader_Lines;}
	inline std::vector<std::vector<std::string> > Get_Header_Lines(void) const {return m_szHeader_Lines;}
	std::string Get_Header_Line_Entry(size_t i_tRow, size_t i_tColumn) const
	{
		std::string szRet;
		if (i_tRow < m_szHeader_Lines.size() && i_tColumn < m_szHeader_Lines[i_tRow].size())
			szRet = m_szHeader_Lines[i_tRow][i_tColumn];
		return szRet;
	}

	void Save_Data_File_CSV(const std::string & i_szFilename, char i_chColumn_Separator = ',', char i_chString_Delimiter = '\"', const std::string i_szNumberFormatString = "%.17e") const;
	void Save_Data_File_Binary(const std::string & i_szFilename) const;

	std::vector<double> operator[] (size_t i_tColumn_Index) const
	{
		std::vector<double> vtRet;
		if (m_bAll_Rows_Same_Size && i_tColumn_Index < Get_Num_Columns())
		{
			vtRet.resize(m_vvsData.size());
			for (size_t tI = 0; tI < m_vvsData.size(); tI++)
			{
				if (m_vvsDatatypes[tI][i_tColumn_Index] == xstdlib::floating_point || m_vvsDatatypes[tI][i_tColumn_Index] == xstdlib::integer)
					vtRet[tI] = std::stod(m_vvsData[tI][i_tColumn_Index]);
				else
					vtRet[tI] = std::nan("");
			}
		}
		return vtRet;
	}

};


	


//
// XDATASET and XDATASET_ADVANCED
// Are containers for matrix-like data (e.g. a spreadsheet)
// XDATASET contains only DOUBLE type data,
// XDATASET_ADVANCED contains multiple data types
// Both use 0 based indexing for columns/rows

class XDATASET_ADVANCED_STRING_INDEX_DATA
{
public:
	unsigned int 	m_uiTable_Offset;
	unsigned int	m_uiString_Allocated_Length;
	unsigned int	m_uiString_Used_Length;
	unsigned int	m_uiString_Data_Length;
};

class XDATASET_ADVANCED
{
public:
	enum DATATYPE {XDT_ERR,XDT_LOGICAL,XDT_INT,XDT_UINT,XDT_SHORT,XDT_USHORT,XDT_CHAR,XDT_UCHAR,XDT_STRING,XDT_FLOAT,XDT_DOUBLE};

private:
	// String lookup table
	XDATASET_ADVANCED_STRING_INDEX_DATA	*	m_lpcString_Lookup_Table;
	unsigned int	m_uiString_Lookup_Table_Size_Alloc;
	unsigned int	m_uiString_Lookup_Table_Entry_Cursor;

	// String data table
	char *			m_lpchString_Data_Table;
	unsigned int	m_uiString_Data_Cursor;
	unsigned int	m_uiString_Data_Size_Alloc;
	unsigned int	m_uiString_Data_Dead_Space;

	// Record information
	DATATYPE *		m_lpeRecord_Descriptor;
	unsigned int * 	m_lpuiData_Element_Offsets;
	unsigned int * 	m_lpuiData_Element_Sizes;
	unsigned int 	m_uiRecord_Length;
	// Data table
	unsigned char *	m_lpData;
	unsigned int 	m_uiNum_Columns;
	unsigned int 	m_uiNum_Rows;
	unsigned int 	m_uiSize_Alloc;

	char **			m_lpszHeader_Lines;
	unsigned int	m_uiHeader_Lines;

	// internal use methods
	unsigned int Add_String(const char * i_lpszString, unsigned int i_uiString_Length);
	unsigned int Add_String_Lookup_Entry(unsigned int i_uiString_Table_Offset, unsigned int i_uiString_Length);
	unsigned int Add_String_Table_Entry(const char * i_lpszString, unsigned int i_uiString_Length);
	void Modify_String(unsigned int i_uiString_Lookup_Index, const char * i_lpszString, unsigned int i_uiString_Length);
	char * Get_String(unsigned int i_uiLookup_Table_Index, unsigned int &o_uiString_Length);
	void Regenerate_String_Table(void);

	void Realloc_String_Storage(unsigned int i_uiNew_Size);
	void Realloc_String_Lookup(unsigned int i_uiNew_Size);
	void ReadDataFileInternal(FILE * file, bool i_bWhitespace_Separated_Columns, char i_chColumn_Separator, char i_chString_Identifier, unsigned int i_uiHeader_Lines, unsigned int uiMax_Line_Length);
	unsigned int CountColumns(const char * lpszBuffer,bool i_bWhitespace_Separated_Columns, char i_chColumn_Separator, char i_chString_Identifier) const;

	void Copy(const XDATASET_ADVANCED & i_cRHO);

public:
	XDATASET_ADVANCED(void);
	XDATASET_ADVANCED(const XDATASET_ADVANCED & i_cRHO);
	~XDATASET_ADVANCED(void);

	XDATASET_ADVANCED & operator =(const XDATASET_ADVANCED &i_cRHO);

	void Allocate(unsigned int i_uiNum_Rows);
	void Add_Rows(unsigned int i_uiNum_Rows);

	void DescribeRecord(DATATYPE * i_lpeColumn_Descriptors,unsigned int i_uiNum_Columns);
	
	inline unsigned int GetNumColumns(void) {return m_uiNum_Columns;}
	inline unsigned int GetNumRows(void) {return m_uiNum_Rows;}
	DATATYPE GetColumnType(unsigned int i_uiColumn);


	void GetElement(unsigned int i_uiColumn, unsigned int i_uiRow_Num,void * o_lpvReturn_Value);
	double	GetElementDbl(unsigned int i_uiColumn, unsigned int i_uiRow_Num);
	int		GetElementInt(unsigned int i_uiColumn, unsigned int i_uiRow_Num);
	const char *	GetElementString(unsigned int i_uiColumn, unsigned int i_uiRow_Num); //Warning: DO NOT MODIFY THE STRING THAT IS OUTPUT!  Doing so may screw up the internal storage.
	void SetElement(unsigned int i_uiColumn, unsigned int i_uiRow_Num,const void * i_lpvValue); // Warning:setting a string value multiple times may
	void SetElementDbl(unsigned int i_uiColumn, unsigned int i_uiRow, const double &i_dValue);
	void SetElementInt(unsigned int i_uiColumn, unsigned int i_uiRow, int i_iValue);
	void SetHeaderLines(unsigned int i_uiNum_Header_Lines, const char ** i_lpszHeader_Lines);

	// warning: ReadDataFile requires prior specification of the record format.  ReadDataFileAutomated will try to figure out what type of data each column holds; it will save numbers as either integer or double
	void ReadDataFile(const char * i_lpszFilename, bool i_bWhitespace_Separated_Columns, char i_chColumn_Separator = 0, char i_chString_Identifier = '\"', unsigned int i_uiHeader_Lines = 1);
	void ReadDataFileAutomated(const char * i_lpszFilename, bool i_bWhitespace_Separated_Columns, char i_chColumn_Separator = 0, char i_chString_Identifier = '\"', unsigned int i_uiHeader_Lines = 1);

	// recommended floating point format is "%.17e", but any form is valid
	void SaveFile(const char * i_lpszFilename, char i_chColumn_Separator, char i_chString_Delimiter, const char * lpszFloatingPointFormat);

#ifdef PARALLEL
#ifdef OPEN_MPI
	void TransferMPIBCast(ompi_communicator_t * i_lpCommunicator, int i_iSource_Rank);
	void TransferMPI(ompi_communicator_t * i_lpCommunicator, int i_iSource_Rank, int i_iDest_Rank);
#else // ifndef OPEN_MPI
	void TransferMPIBCast(int i_iCommunicator, int i_iSource_Rank);
	void TransferMPI(int i_iCommunicator, int i_iSource_Rank, int i_iDest_Rank);
#endif // ifndef OPEN_MPI
#endif // ifdef PARALLEL
};
typedef XDATASET_ADVANCED xdataset_advanced;

class XDATASET
{
private:

	bool	*		m_lpbEmpty_Cell;
	double *		m_lplpdDataset;
	unsigned int	m_uiNum_Columns;
	unsigned int	m_uiSize_Alloc;
	unsigned int	m_uiNum_Elements;
	char **			m_lpszHeader_Lines;
	unsigned int	m_uiHeader_Lines;

	void Initialize_Pointers(void); // warning: sets all pointers and all size variables to 0 regardless of allocation state.
	void Copy(const XDATASET & i_cRHO);
	void Reallocate(unsigned int i_uiNum_Columns, unsigned int i_uiNum_Elements);
public:
	XDATASET(void);
	XDATASET(const XDATASET & i_cRHO);
	~XDATASET(void);

	XDATASET & operator =(const XDATASET &i_cRHO);
	void				Zero(void); // sets header lines to null and all data to zero
	void 				Allocate(unsigned int i_uiNum_Columns, unsigned int i_uiNum_Elements);
	void				Deallocate(bool i_bHard = false); // Hard deallocate physically deallocates table, soft retains allocation but sets # columns and rows to 0.
	void 				Add_Rows(unsigned int i_uiNum_Rows);
	void				Add_Columns(unsigned int i_uiNum_Columns);

	void 				SetElement(unsigned int i_uiColumn, unsigned int i_uiElement_Num, const double & dValue);
	void				SetHeaderLines(unsigned int i_uiNum_Header_Lines, const char ** i_lpszHeader_Lines);
	void				SetHeaderLine(const char * i_lpszHeader_Line);
	inline unsigned int GetNumColumns(void) const {return m_uiNum_Columns;}
	inline unsigned int GetNumElements(void) const {return m_uiNum_Elements;}
	inline unsigned int GetNumRows(void) const {return m_uiNum_Elements;}
	double 				GetElement(unsigned int i_uiColumn, unsigned int i_uiElement_Num) const;
	double * 			GetElementArray(unsigned int i_uiColumn) const;
	bool * 				GetElementEmptyArray(unsigned int i_uiColumn) const;

	const char *		GetHeaderLine(unsigned int i_uiHeader_Line_Row) const;

	bool				IsElementEmpty(unsigned int i_uiColumn, unsigned int i_uiRow) const;

	// warning: ReadDataFile does not have strings fully implemented.
	unsigned int		TestDataFileBin(const char * i_lpszFilename); // if file is an xdataset, returns the version # of the file (low order bits (0-7) = sub version, bits 8-15 = major version; otherwise returns 0.
	void 				ReadDataFile(const char * i_lpszFilename, bool i_bWhitespace_Separated_Columns, bool i_bAllow_Strings, char i_chColumn_Separator = 0, unsigned int i_uiHeader_Lines = 1);
	void				ReadDataFileBin(const char * i_lpszFilename, bool i_bQuiet = false);
	void				SaveDataFileCSV(const char * i_lpszFilename, const char * lpszColumn_Names[], char i_chColumn_Separator = ',', const char i_lpszNumberFormatString[] = "%.17e");
	void				SaveDataFileBin(const char * i_lpszFilename, bool i_bQuiet = false);
	void 				PrintLine(unsigned int i_uiElement) const;
#ifdef PARALLEL
#ifdef OPEN_MPI
	void 				TransferMPIBCast(ompi_communicator_t * i_lpCommunicator, int i_iSource_Rank);
	void 				TransferMPI(ompi_communicator_t * i_lpCommunicator, int i_iSource_Rank, int i_iDest_Rank);
#else // ifndef OPEN_MPI
	void 				TransferMPIBCast(int i_iCommunicator, int i_iSource_Rank);
	void 				TransferMPI(int i_iCommunicator, int i_iSource_Rank, int i_iDest_Rank);
#endif // ifndef OPEN_MPI
#endif // ifdef PARALLEL
};
typedef XDATASET xdataset;

class XMAP // read a file containing key values (such as "a=b")
{
private:
	std::map< const std::string, std::string>	m_cMap;
public:
	void	Read_File(const char * i_lpszFilename);

//	bool Key_Exists(const char * i_lpszKey) const;
//	double	Get_Value_Double(const char * i_lpszKey) const;
//	bool	Get_Value_Bool(const char * i_lpszKey) const;
//	int	Get_Value_Int(const char * i_lpszKey) const;
//	unsigned int	Get_Value_Uint(const char * i_lpszKey) const;
//	int64_t	Get_Value_Int64(const char * i_lpszKey) const;
//	uint64_t	Get_Value_Uint64(const char * i_lpszKey) const;
//	std::string	Get_Value_String(const char * i_lpszKey) const;

	bool Key_Exists(const std::string & i_strKey) const;
	double	Get_Value_Double(const std::string & i_strKey) const;
	bool	Get_Value_Bool(const std::string & i_strKey) const;
	int	Get_Value_Int(const std::string & i_strKey) const;
	unsigned int	Get_Value_Uint(const std::string & i_strKey) const;
	int64_t	Get_Value_Int64(const std::string & i_strKey) const;
	uint64_t	Get_Value_Uint64(const std::string & i_strKey) const;
	std::string	Get_Value_String(const std::string & i_strKey) const;
};
typedef XMAP xmap;


