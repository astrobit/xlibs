#include <cstdlib>
#include <xio.h>
#ifdef PARALLEL
#include <mpi.h>
#endif
#include <cstdio>
#include <cmath>

void XDATASET::Initialize_Pointers(void) // warning: sets all pointers and all size variables to 0 regardless of allocation state.
{
	m_lplpdDataset = NULL;
	m_uiNum_Columns = 0;
	m_uiSize_Alloc = 0;
	m_uiNum_Elements = 0;
	m_lpszHeader_Lines = NULL;
	m_uiHeader_Lines = 0;
	m_lpbEmpty_Cell = NULL;
}
XDATASET::XDATASET(void)
{
	Initialize_Pointers();
}

XDATASET::XDATASET(const XDATASET & i_cRHO)
{
	Initialize_Pointers();

	Copy(i_cRHO);
}

XDATASET::~XDATASET(void)
{
	Deallocate(true);

}

void XDATASET::Copy(const XDATASET & i_cRHO)
{
	Allocate(i_cRHO.m_uiNum_Columns,i_cRHO.m_uiNum_Elements);
	if (m_lplpdDataset)
		memcpy(m_lplpdDataset,i_cRHO.m_lplpdDataset,sizeof(double) * i_cRHO.m_uiNum_Columns * i_cRHO.m_uiNum_Elements);
	if (m_lpbEmpty_Cell)
		memcpy(m_lpbEmpty_Cell,i_cRHO.m_lpbEmpty_Cell,sizeof(bool) * i_cRHO.m_uiNum_Columns * i_cRHO.m_uiNum_Elements);
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

XDATASET & XDATASET::operator =(const XDATASET &i_cRHO)
{
	Copy(i_cRHO);
	return *this;
}

void XDATASET::Zero(void)
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
	m_uiHeader_Lines = 0;
	if (m_lplpdDataset)
		memset(m_lplpdDataset,0,sizeof(double) * m_uiSize_Alloc);
	if (m_lpbEmpty_Cell)
		memset(m_lpbEmpty_Cell,true,sizeof(bool) * m_uiSize_Alloc);
		
}
void XDATASET::Deallocate(bool i_bHard)
{
	// set the number of elements and columns to 0.
	m_uiNum_Columns = 0;
	m_uiNum_Elements = 0;
	if (m_uiHeader_Lines > 0)
	{
		for (unsigned int uiI = 0; uiI < m_uiHeader_Lines; uiI++)
		{
			delete [] m_lpszHeader_Lines[uiI];
		}
		delete [] m_lpszHeader_Lines;
		m_lpszHeader_Lines = NULL;
	}
	m_uiHeader_Lines = 0;
	if (i_bHard) // if hard deallocate requested, release the memory
	{
		if (m_lplpdDataset)
			delete [] m_lplpdDataset;
		m_lplpdDataset = NULL;
		if (m_lpbEmpty_Cell)
			delete [] m_lpbEmpty_Cell;
		m_lpbEmpty_Cell = NULL;
		m_uiSize_Alloc = 0;
	}
	else if (m_lplpdDataset)
	{
		memset(m_lplpdDataset,0,sizeof(double) * m_uiSize_Alloc);
		memset(m_lpbEmpty_Cell,true,sizeof(bool) * m_uiSize_Alloc);
	}
}

void XDATASET::Allocate(unsigned int i_uiNum_Columns, unsigned int i_uiNum_Elements)
{
	if (i_uiNum_Columns != m_uiNum_Columns || i_uiNum_Elements != m_uiNum_Elements) // make sure work has actually been requested
	{
		unsigned int i_uiSize = xChoose_Allocation_Size_Word_Aligned<double>(i_uiNum_Columns * i_uiNum_Elements);
		if (i_uiSize > m_uiSize_Alloc)
		{
			if (m_lplpdDataset)
			{
				delete [] m_lplpdDataset;
				delete [] m_lpbEmpty_Cell;
			}

			m_lplpdDataset = new double[i_uiSize];
			m_lpbEmpty_Cell = new bool[i_uiSize];
			memset(m_lpbEmpty_Cell,true,sizeof(bool) * i_uiSize);

			m_uiSize_Alloc = i_uiSize;
		}
		if (i_uiSize > 0)
		{
			m_uiNum_Elements = i_uiNum_Elements;
			m_uiNum_Columns = i_uiNum_Columns;
		}
	}
}
void XDATASET::Reallocate(unsigned int i_uiNum_Columns, unsigned int i_uiNum_Elements)
{
	if (i_uiNum_Columns != m_uiNum_Columns || i_uiNum_Elements != m_uiNum_Elements) // make sure work has actually been requested
	{
		unsigned int i_uiSize = xChoose_Allocation_Size_Word_Aligned<double>(i_uiNum_Columns * i_uiNum_Elements);
		if (i_uiNum_Columns < m_uiNum_Columns)
		{
			fprintf(stderr,"XDATASET: Reallocate called with fewer columns than exist - not sure how to\n");
			fprintf(stderr,"XDATASET: copy the existing data.\n");
			throw(-1);
		}
		if (i_uiSize > 0)
		{
			double	*lpdDataset_New = new double[i_uiSize];
			bool	*lpbEmpty_Cell_New = new bool[i_uiSize];
			memset(lpbEmpty_Cell_New,0,sizeof(bool) * i_uiSize);
			if (m_lplpdDataset && m_uiNum_Elements == i_uiNum_Elements)
			{
				memcpy(lpdDataset_New,m_lplpdDataset,m_uiNum_Elements * m_uiNum_Columns * sizeof(double));
				memcpy(lpbEmpty_Cell_New,m_lpbEmpty_Cell,m_uiNum_Elements * m_uiNum_Columns * sizeof(bool));
				delete [] m_lplpdDataset;
				delete [] m_lpbEmpty_Cell;
			}
			else if (m_lplpdDataset)
			{
				for (unsigned int uiColumn = 0; uiColumn < m_uiNum_Columns; uiColumn++)
				{
					unsigned int uiNew_Idx = uiColumn * i_uiNum_Elements;
					unsigned int uiOld_Idx = uiColumn * m_uiNum_Elements;
					memcpy(&lpdDataset_New[uiNew_Idx],&m_lplpdDataset[uiOld_Idx],m_uiNum_Elements * sizeof(double));
					memcpy(&lpbEmpty_Cell_New[uiNew_Idx],&m_lpbEmpty_Cell[uiOld_Idx],m_uiNum_Elements * sizeof(bool));
				}
				delete [] m_lplpdDataset;
				delete [] m_lpbEmpty_Cell;
			}

			m_lplpdDataset = lpdDataset_New;
			m_lpbEmpty_Cell = lpbEmpty_Cell_New;

			m_uiSize_Alloc = i_uiSize;
		}
		if (i_uiSize > 0)
		{
			m_uiNum_Elements = i_uiNum_Elements;
			m_uiNum_Columns = i_uiNum_Columns;
		}
	}
}

void XDATASET::Add_Rows(unsigned int i_uiNum_Rows)
{
	if (m_uiNum_Columns > 0)
	{
		Reallocate(m_uiNum_Columns, m_uiNum_Elements + i_uiNum_Rows);
	}
	else
		fprintf(stderr,"XDATASET: Add_Rows called with columns not specified.  Call Allocate first.\n");
}
void XDATASET::Add_Columns(unsigned int i_uiNum_Columns)
{
	if (m_uiNum_Elements > 0)
	{
		Reallocate(m_uiNum_Columns + i_uiNum_Columns, m_uiNum_Elements);
	}
	else
	{	
		m_uiNum_Columns += i_uiNum_Columns;
		fprintf(stderr,"XDATASET: warning - Add_Columns called with # of rows = 0.\n");
		fprintf(stderr,"XDATASET: Allocate should be called instead.\n");
	}
}

void XDATASET::SetElement(unsigned int i_uiColumn, unsigned int i_uiElement_Num, const double & dValue)
{
	if (m_lplpdDataset && i_uiColumn < m_uiNum_Columns && i_uiElement_Num < m_uiNum_Elements)
	{
		unsigned int uiIdx = i_uiElement_Num + i_uiColumn * m_uiNum_Elements;
		m_lpbEmpty_Cell[uiIdx] = false;
		m_lplpdDataset[uiIdx] = dValue;
	}
}

bool XDATASET::IsElementEmpty(unsigned int i_uiColumn, unsigned int i_uiRow) const
{
	bool bRet = true;
	if (m_lpbEmpty_Cell && i_uiColumn < m_uiNum_Columns && i_uiRow < m_uiNum_Elements)
		bRet = m_lpbEmpty_Cell[i_uiRow + i_uiColumn * m_uiNum_Elements];
	return bRet;
}

double XDATASET::GetElement(unsigned int i_uiColumn, unsigned int i_uiElement_Num) const
{
	double	dRet = nan("");
	unsigned int uiIdx = i_uiElement_Num + i_uiColumn * m_uiNum_Elements;
	if (m_lplpdDataset && i_uiColumn < m_uiNum_Columns && i_uiElement_Num < m_uiNum_Elements &&
		(m_lpbEmpty_Cell || !m_lpbEmpty_Cell[uiIdx]))
		dRet = m_lplpdDataset[uiIdx];
	return dRet;
}
double * XDATASET::GetElementArray(unsigned int i_uiColumn) const
{
	double	* lpdRet = NULL;
	if (m_lplpdDataset && i_uiColumn < m_uiNum_Columns)
		lpdRet = m_lplpdDataset + i_uiColumn * m_uiNum_Elements;
	return lpdRet;
}
bool * XDATASET::GetElementEmptyArray(unsigned int i_uiColumn) const
{
	bool	* lpdRet = NULL;
	if (m_lpbEmpty_Cell && i_uiColumn < m_uiNum_Columns)
		lpdRet = m_lpbEmpty_Cell + i_uiColumn * m_uiNum_Elements;
	return lpdRet;
}
// warning: ReadDataFile does not have strings fully implemented.
void XDATASET::ReadDataFile(const char * i_lpszFilename, bool i_bWhitespace_Separated_Columns, bool i_bAllow_Strings, char i_chColumn_Separator, unsigned int i_uiHeader_Lines)
{
	if (i_lpszFilename && i_lpszFilename[0] && (i_bWhitespace_Separated_Columns || i_chColumn_Separator != 0))
	{
		FILE * fileIn = fopen(i_lpszFilename,"rt");
		if (fileIn)
		{
			char *lpszBuffer = NULL;
			char *lpszCursor;
			bool bFirst_Data_Line_Processed = false;
			unsigned int uiLine_Count = 0;
			unsigned int uiCurr_Line = 0;
			unsigned int uiFault_Lines = 0;
			unsigned int uiData_Lines = 0;
			unsigned int uiCurr_Column = 0;
			unsigned int uiNum_Columns = 0;
			unsigned int uiBuffer_Size = 0;
			unsigned int uiLine_Size = 0;
			char chChar_Test = 1;
			while (chChar_Test != EOF)
			{
				uiBuffer_Size = 0;
				while (chChar_Test != EOF && chChar_Test != 10 && chChar_Test != 13)
				{
					chChar_Test = fgetc(fileIn);
					uiBuffer_Size++;
				}
				while (chChar_Test != EOF && (chChar_Test == 10 || chChar_Test == 13))
				{
					chChar_Test = fgetc(fileIn);
					uiBuffer_Size++;
				}
				if (uiBuffer_Size > uiLine_Size)
					uiLine_Size = uiBuffer_Size;
			}
			lpszBuffer = new char [uiLine_Size+1];
			fseek(fileIn,0,SEEK_SET);
			while (fgets(lpszBuffer,uiLine_Size + 1,fileIn) != 0)
				uiLine_Count++;
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
			while (fgets(lpszBuffer,uiLine_Size + 1,fileIn) != 0)
			{
				if (uiCurr_Line	>= i_uiHeader_Lines)
				{
					if (!bFirst_Data_Line_Processed)
					{
						uiNum_Columns = 0;
						char * lpszCursor = lpszBuffer;
						if (xIsWhitespace(lpszCursor))
							lpszCursor = xPassWhitespace(lpszCursor);
						if (lpszCursor[0] != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13)
							uiNum_Columns++;
						while (lpszCursor[0] != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13)
						{
							if ((lpszCursor[0] == '\"' || lpszCursor[0] == '\'') && i_bAllow_Strings)
							{
								lpszCursor = xPassString(lpszCursor,lpszCursor[0]);
							}
							else if (i_chColumn_Separator != 0 && lpszCursor[0] == i_chColumn_Separator)
							{
								lpszCursor++;
								//lpszCursor = xPassSeparator(lpszCursor);
//								if (lpszCursor != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13)
									uiNum_Columns++;
							}
							else if (i_bWhitespace_Separated_Columns && xIsWhitespace(lpszCursor))
							{
								lpszCursor = xPassWhitespace(lpszCursor);
//								if (lpszCursor != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13)
									uiNum_Columns++;
							}
							else
							{
								lpszCursor++;
							}
						}
						Allocate(uiNum_Columns, uiLine_Count - i_uiHeader_Lines);
						bFirst_Data_Line_Processed = true;
					}
					lpszCursor = lpszBuffer;
					uiCurr_Column = 0;
					while (uiCurr_Column < uiNum_Columns && lpszCursor[0] != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13)
					{
						if (xIsWhitespace(lpszCursor))
							lpszCursor = xPassWhitespace(lpszCursor);
						if ((lpszCursor[0] == '\"' || lpszCursor[0] == '\'') && i_bAllow_Strings)
						{
							//SetElement(uiCurr_Column,uiData_Lines,0.0);
							lpszCursor = xPassString(lpszCursor,lpszCursor[0]);
						}
						else if ((lpszCursor[0] == 'n' && lpszCursor[1] == 'a' && lpszCursor[2] == 'n') ||
								(lpszCursor[0] == 'N' && lpszCursor[1] == 'a' && lpszCursor[2] == 'N') ||
								(lpszCursor[0] == 'N' && lpszCursor[1] == 'A' && lpszCursor[2] == 'N') ||
								(lpszCursor[0] == '-' && lpszCursor[1] == 'n' && lpszCursor[2] == 'a' && lpszCursor[3] == 'n') ||
								(lpszCursor[0] == '-' && lpszCursor[1] == 'N' && lpszCursor[2] == 'a' && lpszCursor[3] == 'N') ||
								(lpszCursor[0] == '-' && lpszCursor[1] == 'N' && lpszCursor[2] == 'A' && lpszCursor[3] == 'N') ||
								(lpszCursor[0] == 'i' && lpszCursor[1] == 'n' && lpszCursor[2] == 'f') ||
								(lpszCursor[0] == '-' && lpszCursor[1] == 'i' && lpszCursor[2] == 'n' && lpszCursor[3] == 'f') ||
								(lpszCursor[0] == 'I' && lpszCursor[1] == 'N' && lpszCursor[2] == 'F') ||
								(lpszCursor[0] == '-' && lpszCursor[1] == 'I' && lpszCursor[2] == 'N' && lpszCursor[3] == 'F'))
						{ // nan and inf
							if (lpszCursor[0] == '-')
								lpszCursor++;
							lpszCursor+=3; // bypass the field and don't set the value.
							if (!i_bWhitespace_Separated_Columns && xIsWhitespace(lpszCursor))
								lpszCursor = xPassWhitespace(lpszCursor);

							if (i_bWhitespace_Separated_Columns)
							{
								lpszCursor = xPassSeparator(lpszCursor);
							}
							else if (i_chColumn_Separator != 0 && lpszCursor[0] == i_chColumn_Separator)
							{
								lpszCursor++;
//								lpszCursor = xPassWhitespace(lpszCursor);
							}
							uiCurr_Column++;
						}
						else if (xIsANumber(lpszCursor))
						{
							SetElement(uiCurr_Column,uiData_Lines,atof(lpszCursor));
							lpszCursor = xPassNumber(lpszCursor);
							if (!i_bWhitespace_Separated_Columns && xIsWhitespace(lpszCursor))
								lpszCursor = xPassWhitespace(lpszCursor);

							if (i_bWhitespace_Separated_Columns)
							{
								lpszCursor = xPassSeparator(lpszCursor);
							}
							else if (i_chColumn_Separator != 0 && lpszCursor[0] == i_chColumn_Separator)
							{
								lpszCursor++;
//								lpszCursor = xPassWhitespace(lpszCursor);
							}
							uiCurr_Column++;
						}
						else if (!i_bWhitespace_Separated_Columns && i_chColumn_Separator != 0 && lpszCursor[0] == i_chColumn_Separator)
						{
							lpszCursor++;
//							lpszCursor = xPassWhitespace(lpszCursor);
							uiCurr_Column++;
						}
						else if (lpszCursor[0] != 0 && lpszCursor[0] != 10 && lpszCursor[0] != 13)
						{
							uiFault_Lines++;
							uiCurr_Column = uiNum_Columns;
							printf("Column data fault on line %u (column %i, character %c)\n",uiCurr_Line,(unsigned int)(lpszCursor - lpszBuffer + 1),lpszCursor[0]);
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
			if (fileIn)
				fclose(fileIn);
			delete [] lpszBuffer;
			lpszBuffer = NULL;
		}
		else
			printf("Unable to find file %s\n",i_lpszFilename);
	}
	else
		printf("No filename specified.\n");
}
void XDATASET::PrintLine(unsigned int i_uiElement) const
{
	for (unsigned int uiCol = 0; uiCol < m_uiNum_Columns; uiCol++)
	{
		printf("%f,",m_lplpdDataset[uiCol * m_uiNum_Elements + i_uiElement]);
	}
	printf("\n");
}
#ifdef PARALLEL
#ifndef OPEN_MPI
void XDATASET::TransferMPIBCast(int i_lpCommunicator, int i_iSource_Rank)
#else // ifndef OPEN_MPI
void XDATASET::TransferMPIBCast(ompi_communicator_t * i_lpCommunicator, int i_iSource_Rank)
#endif // ifndef OPEN_MPI
{
	unsigned int uiNum_Columns = m_uiNum_Columns, uiNum_Elements = m_uiNum_Elements;
	MPI_Bcast(&uiNum_Columns,1,MPI_UNSIGNED,i_iSource_Rank,i_lpCommunicator);
	MPI_Bcast(&uiNum_Elements,1,MPI_UNSIGNED,i_iSource_Rank,i_lpCommunicator);
	
	if (uiNum_Columns > 0 && uiNum_Elements > 0)
	{
		Allocate(uiNum_Columns,uiNum_Elements);
		MPI_Barrier(MPI_COMM_WORLD);
		if (m_lplpdDataset)
		{
			MPI_Bcast(m_lplpdDataset,m_uiNum_Columns * m_uiNum_Elements,MPI_DOUBLE,i_iSource_Rank,i_lpCommunicator);
			MPI_Bcast(m_lpbEmpty_Cell,m_uiNum_Columns * m_uiNum_Elements,MPI_C_BOOL,i_iSource_Rank,i_lpCommunicator);
		}
		else
			MPI_Abort(MPI_COMM_WORLD,0);
	}
	else
		MPI_Abort(MPI_COMM_WORLD,0);
}
#ifndef OPEN_MPI
void XDATASET::TransferMPI(int i_lpCommunicator, int i_iSource_Rank, int i_iDest_Rank)
#else // ifndef OPEN_MPI
void XDATASET::TransferMPI(ompi_communicator_t * i_lpCommunicator, int i_iSource_Rank, int i_iDest_Rank)
#endif // ifndef OPEN_MPI
{
	int iMy_Rank;
	MPI_Status sStatus;
	MPI_Comm_rank(i_lpCommunicator,&iMy_Rank);
	if (iMy_Rank == i_iSource_Rank)
	{
		MPI_Send(&m_uiNum_Columns,1,MPI_UNSIGNED,i_iDest_Rank,MPI_ANY_TAG,i_lpCommunicator);
		MPI_Send(&m_uiNum_Elements,1,MPI_UNSIGNED,i_iDest_Rank,MPI_ANY_TAG,i_lpCommunicator);
		if (m_uiNum_Columns > 0 && m_uiNum_Elements > 0 && m_lplpdDataset)
		{
			MPI_Send(&m_lplpdDataset,m_uiNum_Columns * m_uiNum_Elements,MPI_DOUBLE,i_iDest_Rank,MPI_ANY_TAG,i_lpCommunicator);
			MPI_Send(&m_lpbEmpty_Cell,m_uiNum_Columns * m_uiNum_Elements,MPI_C_BOOL,i_iDest_Rank,MPI_ANY_TAG,i_lpCommunicator);
		}
		else
			MPI_Abort(MPI_COMM_WORLD,0);
	}
	else
	{
		MPI_Recv(&m_uiNum_Columns,1,MPI_UNSIGNED,i_iSource_Rank,MPI_ANY_TAG,i_lpCommunicator,&sStatus);
		MPI_Recv(&m_uiNum_Elements,1,MPI_UNSIGNED,i_iSource_Rank,MPI_ANY_TAG,i_lpCommunicator,&sStatus);
		Allocate(m_uiNum_Columns,m_uiNum_Elements);
		if (m_uiNum_Columns > 0 && m_uiNum_Elements > 0 && m_lplpdDataset)
		{
			MPI_Recv(&m_lplpdDataset,m_uiNum_Columns * m_uiNum_Elements,MPI_DOUBLE,i_iSource_Rank,MPI_ANY_TAG,i_lpCommunicator,&sStatus);
			MPI_Recv(&m_lpbEmpty_Cell,m_uiNum_Columns * m_uiNum_Elements,MPI_C_BOOL,i_iSource_Rank,MPI_ANY_TAG,i_lpCommunicator,&sStatus);
		}
		else
			MPI_Abort(MPI_COMM_WORLD,0);
	}
}
#endif // ifdef PARALLEL

unsigned int XDATASET::TestDataFileBin(const char * i_lpszFilename)
{
	unsigned int uiRet = 0;
	FILE * fileOut = fopen(i_lpszFilename,"rb");
	if (fileOut)
	{
		char lpszHeader[] = {"XIODATASET"};
		char *lpszReadHeader;
		lpszReadHeader = new char[sizeof(lpszHeader)];
		unsigned char ucVersion;
		unsigned char ucSubversion;
		fread(lpszReadHeader,sizeof(lpszHeader),1,fileOut);
		if (strcmp(lpszHeader,lpszReadHeader) == 0)
		{
			fread(&ucVersion,sizeof(ucVersion),1,fileOut);
			fread(&ucSubversion,sizeof(ucSubversion),1,fileOut);
			uiRet = (ucVersion << 8) + ucSubversion;
		}
		fclose(fileOut);
	}
	return uiRet;
}

void XDATASET::ReadDataFileBin(const char * i_lpszFilename, bool i_bQuiet)
{
	FILE * fileOut = fopen(i_lpszFilename,"rb");
	if (fileOut)
	{
		char lpszHeader[] = {"XIODATASET"};
		char *lpszReadHeader;
		lpszReadHeader = new char[sizeof(lpszHeader)];
		unsigned char ucVersion;
		unsigned char ucSubversion;
		unsigned int uiNum_Cols, uiNum_Elem;
		fread(lpszReadHeader,sizeof(lpszHeader),1,fileOut);
		if (strcmp(lpszHeader,lpszReadHeader) == 0)
		{
			fread(&ucVersion,sizeof(ucVersion),1,fileOut);
			fread(&ucSubversion,sizeof(ucSubversion),1,fileOut);
			if ((ucVersion == 1 && ucSubversion == 0) ||
				(ucVersion == 2 && ucSubversion == 0) ||
				(ucVersion == 2 && ucSubversion == 1))
			{
				if (ucVersion >= 2)
				{
					if (m_uiHeader_Lines != 0) // first delete anything that is already there
					{
						for (unsigned int uiI = 0; uiI < m_uiHeader_Lines; uiI++)
							delete [] m_lpszHeader_Lines[uiI];
						delete [] m_lpszHeader_Lines;
					}
					fread(&m_uiHeader_Lines,sizeof(m_uiHeader_Lines),1,fileOut);
					if (m_uiHeader_Lines > 0)
						m_lpszHeader_Lines = new char *[m_uiHeader_Lines];
					unsigned int * lpuiSizes = new unsigned int[m_uiHeader_Lines];
					for (unsigned int uiI = 0; uiI < m_uiHeader_Lines; uiI++)
					{
						fread(&lpuiSizes[uiI],sizeof(unsigned int),1,fileOut);
						m_lpszHeader_Lines[uiI] = new char[lpuiSizes[uiI]];
					}
					for (unsigned int uiI = 0; uiI < m_uiHeader_Lines; uiI++)
					{
						fread(m_lpszHeader_Lines[uiI],lpuiSizes[uiI],1,fileOut);
					}
					delete [] lpuiSizes;
				}
				fread(&uiNum_Cols,sizeof(m_uiNum_Columns),1,fileOut);
				fread(&uiNum_Elem,sizeof(m_uiNum_Elements),1,fileOut);
				if (uiNum_Cols != 0 && uiNum_Elem != 0)
				{
					Allocate(uiNum_Cols, uiNum_Elem);
					fread(m_lplpdDataset,sizeof(double),m_uiNum_Columns * m_uiNum_Elements,fileOut);
					if (ucVersion > 2 || (ucVersion == 2 && ucSubversion >= 1))
						fread(m_lpbEmpty_Cell,sizeof(bool),m_uiNum_Columns * m_uiNum_Elements,fileOut);
					else
					{
						fprintf(stderr,"XIO: Warning - data file %s is version 2.0 or earlier; filled cell data not available.  Assuming all cells are filled.\n",i_lpszFilename);
						memset(m_lpbEmpty_Cell,true,sizeof(bool) * m_uiSize_Alloc);
					}
				}
				else if (!i_bQuiet)
					fprintf(stderr,"XIO: Data file %s is empty.\n",i_lpszFilename);
			}
			else if (!i_bQuiet)
				fprintf(stderr,"XIO: Data file %s is not version 1.0 or 2.0. Apparent version is %i.%i.\n",i_lpszFilename,ucVersion,ucSubversion);
		}
		else if (!i_bQuiet)
			fprintf(stderr,"XIO: Attempted to read data file %s, but this file is not an XDATASET.\n",i_lpszFilename);
		fclose(fileOut);
	}
	else if (!i_bQuiet)
		printf("XIO: Could not open file %s for read.\n",i_lpszFilename);
}

void XDATASET::SaveDataFileCSV(const char * i_lpszFilename, const char * lpszColumn_Names[], char i_chColumn_Separator, const char i_lpszNumberFormatString[])
{
	FILE * fileOut = fopen(i_lpszFilename,"wt");
	if (fileOut)
	{
		char lpszFormatStringA[16];
		char lpszFormatStringB[16];
		unsigned int uiCol, uiElem;

		strcpy(lpszFormatStringA,i_lpszNumberFormatString);
		sprintf(lpszFormatStringB,"%%c%s",i_lpszNumberFormatString);
		
		for (unsigned int uiI = 0; uiI < m_uiHeader_Lines; uiI++)
		{
			fprintf(fileOut,m_lpszHeader_Lines[uiI]);
		}
		
		if (i_chColumn_Separator == 0)
			i_chColumn_Separator = ',';
		if (lpszColumn_Names)
		{
			fprintf(fileOut,"%s",lpszColumn_Names[0]);
			for (uiCol = 1; uiCol < m_uiNum_Columns; uiCol++)
				fprintf(fileOut,"%c%s",i_chColumn_Separator,lpszColumn_Names[uiCol]);
			fprintf(fileOut,"\n");
		}

		for (uiElem = 0; uiElem < m_uiNum_Elements; uiElem++)
		{
			fprintf(fileOut,lpszFormatStringA,m_lplpdDataset[uiElem]);
			for (uiCol = 1; uiCol < m_uiNum_Columns; uiCol++)
			{
				unsigned int uiIdx = uiElem + uiCol * m_uiNum_Elements;
				if (!m_lpbEmpty_Cell || !m_lpbEmpty_Cell[uiIdx])
					fprintf(fileOut,lpszFormatStringB,i_chColumn_Separator,m_lplpdDataset[uiIdx]);
				else
					fprintf(fileOut,"%c ",i_chColumn_Separator);
			}

			fprintf(fileOut,"\n");
		}
	}
}
void XDATASET::SaveDataFileBin(const char * i_lpszFilename, bool i_bQuiet)
{
	FILE * fileOut = fopen(i_lpszFilename,"wb");
	if (fileOut)
	{
		char lpszHeader[] = {"XIODATASET"};
		unsigned char ucVersion = 2;
		unsigned char ucSubversion = 1;
		fwrite(lpszHeader,sizeof(lpszHeader),1,fileOut);
		fwrite(&ucVersion,sizeof(ucVersion),1,fileOut);
		fwrite(&ucSubversion,sizeof(ucSubversion),1,fileOut);
		fwrite(&m_uiHeader_Lines,sizeof(m_uiHeader_Lines),1,fileOut);
		for (unsigned int uiI = 0; uiI < m_uiHeader_Lines; uiI++)
		{
			unsigned int uiSize = strlen(m_lpszHeader_Lines[uiI]) + 1;
			fwrite(&uiSize,sizeof(uiSize),1,fileOut);
		}
		for (unsigned int uiI = 0; uiI < m_uiHeader_Lines; uiI++)
		{
			unsigned int uiSize = strlen(m_lpszHeader_Lines[uiI]) + 1;
			fwrite(m_lpszHeader_Lines[uiI],uiSize,1,fileOut);
		}
		fwrite(&m_uiNum_Columns,sizeof(m_uiNum_Columns),1,fileOut);
		fwrite(&m_uiNum_Elements,sizeof(m_uiNum_Elements),1,fileOut);
		if (m_lplpdDataset)
			fwrite(m_lplpdDataset,sizeof(double),m_uiNum_Columns * m_uiNum_Elements,fileOut);
		if (m_lpbEmpty_Cell)
			fwrite(m_lpbEmpty_Cell,sizeof(bool),m_uiNum_Columns * m_uiNum_Elements,fileOut);
		fclose(fileOut);
	}	
}

void XDATASET::SetHeaderLines(unsigned int i_uiNum_Header_Lines, const char ** i_lpszHeader_Lines)
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
	if (m_uiHeader_Lines > 0 && i_lpszHeader_Lines)
	{
		m_lpszHeader_Lines = new char *[m_uiHeader_Lines];
		for (unsigned int uiI = 0; uiI < m_uiHeader_Lines; uiI++)
		{
			m_lpszHeader_Lines[uiI] = new char [strlen(i_lpszHeader_Lines[uiI]) + 1];
			strcpy(m_lpszHeader_Lines[uiI],i_lpszHeader_Lines[uiI]);
		}
	}
}
void XDATASET::SetHeaderLine(const char * i_lpszHeader_Line)
{
	if (m_uiHeader_Lines > 0)
	{
		for (unsigned int uiI = 0; uiI < m_uiHeader_Lines; uiI++)
		{
			delete [] m_lpszHeader_Lines[uiI];
		}
		delete [] m_lpszHeader_Lines;
		m_lpszHeader_Lines = NULL;
		m_uiHeader_Lines = 0;
	}
	if (i_lpszHeader_Line && i_lpszHeader_Line[0] != 0)
	{
		m_uiHeader_Lines = 1;
		m_lpszHeader_Lines = new char *[m_uiHeader_Lines];
		m_lpszHeader_Lines[0] = new char [strlen(i_lpszHeader_Line) + 1];
		strcpy(m_lpszHeader_Lines[0],i_lpszHeader_Line);
	}
}


