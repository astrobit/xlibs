#include <xastroline.h>
#include <stdio.h>
#include <stdlib.h>


void XASTROLINE_ELEMENTDATA::WriteDataFile(void)
{
	char lpszFilename[8];
	unsigned int uiI;
	FILE * fileOut;

	sprintf(lpszFilename,"%s.dat",lpszElement_ID);
	fileOut = fopen(lpszFilename,"wb");
	WriteBinaryFileHeader(fileOut);
	for (uiI = 0; uiI < uiZ; uiI++)
	{
		WriteBinaryDataFileIonData(lpcIonData[uiI],fileOut);
	}
}

void XASTROLINE_ELEMENTDATA::WriteBinaryFileHeader(FILE * i_fileOut)
{
	const char lpszFileType[] = {"Element Data File"};
	unsigned int uiSize;
        size_t iJunk; 
	iJunk = fwrite(lpszFileType,sizeof(char),sizeof(lpszFileType),i_fileOut);
	//Store class sizes
	uiSize = sizeof(XASTROLINE_ELEMENTDATA);
	fwrite(&uiSize,sizeof(unsigned int), 1, i_fileOut);
	uiSize = sizeof(XASTROLINE_IONDATA);
	fwrite(&uiSize,sizeof(unsigned int), 1, i_fileOut);
	uiSize = sizeof(XASTROLINE_LINEDATA);
	iJunk = fwrite(&uiSize,sizeof(unsigned int), 1, i_fileOut);
	uiSize = sizeof(XASTROLINE_STATEDATA);
	iJunk = fwrite(&uiSize,sizeof(unsigned int), 1, i_fileOut);
	//Store header version
	uiSize = XASTROLINE_LINEDATAVERSION;
	iJunk = fwrite(&uiSize,sizeof(unsigned int), 1, i_fileOut);
	uiSize = XASTROLINE_LINEDATASUBVERSION;
	iJunk = fwrite(&uiSize,sizeof(unsigned int), 1, i_fileOut);

	//Actual data for element
	iJunk = fwrite(lpszElement_ID,sizeof(char), 3, i_fileOut);
	iJunk = fwrite(&uiA,sizeof(unsigned int), 1, i_fileOut);
	iJunk = fwrite(&uiZ,sizeof(unsigned int), 1, i_fileOut);
}

bool XASTROLINE_ELEMENTDATA::ReadBinaryFileHeader(FILE * i_fileIn)
{
	const char lpszFileType[] = {"Element Data File"};
	char lpszFileTypeReadBuffer[32];
	bool bSuccess = true;
        size_t iJunk;

	unsigned int uiSize;
	iJunk = fread(lpszFileTypeReadBuffer,sizeof(char),sizeof(lpszFileType),i_fileIn);
	if (strcmp(lpszFileTypeReadBuffer,lpszFileType) != 0)
	{
		printf("XASTROLINE_ELEMENTDATA: File type error.\n");
		bSuccess = false;
		return false;
	}
	//Store class sizes
	iJunk = fread(&uiSize,sizeof(unsigned int), 1, i_fileIn);
	if (uiSize != sizeof(XASTROLINE_ELEMENTDATA))
	{
		printf("XASTROLINE_ELEMENTDATA: XASTROLINE_ELEMENTDATA size mismatch.\n");
		bSuccess = false;
		return false;
	}
	iJunk = fread(&uiSize,sizeof(unsigned int), 1, i_fileIn);
	if (uiSize != sizeof(XASTROLINE_IONDATA))
	{
		printf("XASTROLINE_ELEMENTDATA: XASTROLINE_IONDATA size mismatch.\n");
		bSuccess = false;
		return false;
	}

	iJunk = fread(&uiSize,sizeof(unsigned int), 1, i_fileIn);
	if (uiSize != sizeof(XASTROLINE_LINEDATA))
	{
		printf("XASTROLINE_ELEMENTDATA: XASTROLINE_LINEDATA size mismatch.\n");
		bSuccess = false;
		return false;
	}
	iJunk = fread(&uiSize,sizeof(unsigned int), 1, i_fileIn);
	if (uiSize != sizeof(XASTROLINE_STATEDATA))
	{
		printf("XASTROLINE_ELEMENTDATA: XASTROLINE_STATEDATA size mismatch.\n");
		bSuccess = false;
		return false;
	}

	//Store header version
	iJunk = fread(&uiSize,sizeof(unsigned int), 1, i_fileIn);
	if (uiSize != XASTROLINE_LINEDATAVERSION)
	{
		printf("XASTROLINE_ELEMENTDATA: warning: version mismatch.  File version %i, Header version %i.\n",uiSize,XASTROLINE_LINEDATAVERSION);
	}

	iJunk = fread(&uiSize,sizeof(unsigned int), 1, i_fileIn);
	if (uiSize != XASTROLINE_LINEDATASUBVERSION)
	{
		printf("XASTROLINE_ELEMENTDATA: warning: subversion mismatch.  File version %i, Header version %i.\n",uiSize,XASTROLINE_LINEDATASUBVERSION);
	}


	iJunk = fread(lpszElement_ID,sizeof(char), 3, i_fileIn);
	iJunk = fread(&uiA,sizeof(unsigned int), 1, i_fileIn);
	iJunk = fread(&uiZ,sizeof(unsigned int), 1, i_fileIn);

//	printf("%s\t%i\t%i\n",lpszElement_ID,uiA,uiZ);
	lpcIonData = new XASTROLINE_IONDATA[uiZ];

	return bSuccess;
}

void XASTROLINE_ELEMENTDATA::WriteBinaryDataFileIonData(const XASTROLINE_IONDATA & i_cIonData, FILE * i_fileOut)
{
        size_t iJunk;
	iJunk = fwrite(&i_cIonData.uiIon_State,sizeof(unsigned int),1,i_fileOut);
	iJunk = fwrite(&i_cIonData.uiNum_Lines,sizeof(unsigned int),1,i_fileOut);
	if (i_cIonData.uiNum_Lines != 0)
		iJunk = fwrite(i_cIonData.lpcLine_Data,sizeof(XASTROLINE_LINEDATA),i_cIonData.uiNum_Lines,i_fileOut);
}

void XASTROLINE_ELEMENTDATA::ReadBinaryDataFileIonData(XASTROLINE_IONDATA & i_cIonData, FILE * i_fileIn)
{
        size_t iJunk;
	iJunk = fread(&i_cIonData.uiIon_State,sizeof(unsigned int),1,i_fileIn);
	iJunk = fread(&i_cIonData.uiNum_Lines,sizeof(unsigned int),1,i_fileIn);
//	printf("%i\t%i\n",i_cIonData.uiIon_State,i_cIonData.uiNum_Lines);
	if (i_cIonData.uiNum_Lines != 0)
	{
		i_cIonData.lpcLine_Data = new XASTROLINE_LINEDATA[i_cIonData.uiNum_Lines];
		iJunk = fread(i_cIonData.lpcLine_Data,sizeof(XASTROLINE_LINEDATA),i_cIonData.uiNum_Lines,i_fileIn);
	}
}

void XASTROLINE_ELEMENTDATA::WriteCSVDataFileIonData(const XASTROLINE_IONDATA & i_cIonData, FILE * i_fileOut)
{
	unsigned int uiI;
	fprintf(i_fileOut,"------------------------------ %i -----------------------------------------\n",i_cIonData.uiIon_State);
	for (uiI = 0; uiI < i_cIonData.uiNum_Lines; uiI++)
	{
		fprintf(i_fileOut,"%.1f,%.3e,%.3f,%s,%.3f,%.1f,%i,%s,%.3f,%.1f,%i\n",
			i_cIonData.lpcLine_Data[uiI].dWavelength_A,
			i_cIonData.lpcLine_Data[uiI].dTransisiton_Probability_sm1,
			i_cIonData.lpcLine_Data[uiI].dOscillator_Strength,
			i_cIonData.lpcLine_Data[uiI].cLower_State.lpszState,
			i_cIonData.lpcLine_Data[uiI].cLower_State.dEnergy_eV,
			i_cIonData.lpcLine_Data[uiI].cLower_State.dAngular_Momentum_J,
			i_cIonData.lpcLine_Data[uiI].cLower_State.uiStatistical_Weight,
			i_cIonData.lpcLine_Data[uiI].cUpper_State.lpszState,
			i_cIonData.lpcLine_Data[uiI].cUpper_State.dEnergy_eV,
			i_cIonData.lpcLine_Data[uiI].cUpper_State.dAngular_Momentum_J,
			i_cIonData.lpcLine_Data[uiI].cUpper_State.uiStatistical_Weight);
	}

}

void XASTROLINE_ELEMENTDATA::WriteCSVFile()
{
	char lpszFilename[8];
	unsigned int uiI;
	FILE * fileOut;

	sprintf(lpszFilename,"%s.csv",lpszElement_ID);
	fileOut = fopen(lpszFilename,"wt");
	fprintf(fileOut,"Element = %s\nA = %i\nZ = %i\n",lpszElement_ID,uiA,uiZ);

	for (uiI = 0; uiI < uiZ; uiI++)
	{
		WriteCSVDataFileIonData(lpcIonData[uiI],fileOut);
	}
}

void XASTROLINE_ELEMENTDATA::ReadDataFile(const char * i_lpszFile_Path, const char * i_lpszElement_ID)
{
	char * lpszFilename = NULL;
	unsigned int uiFilename_Len = 0;
	unsigned int uiI;
	FILE * fileIn = NULL;
	const char * lpszElement_ID_Temp = i_lpszElement_ID;

	if (!i_lpszElement_ID)
		lpszElement_ID_Temp = lpszElement_ID;


	
	if (i_lpszFile_Path && i_lpszFile_Path[0] != 0)
	{
		uiFilename_Len = strlen(i_lpszFile_Path) + strlen(lpszElement_ID_Temp) + 7;
		lpszFilename = new char[uiFilename_Len];
		sprintf(lpszFilename,"%s%s.dat",i_lpszFile_Path,lpszElement_ID_Temp);
		fileIn = fopen(lpszFilename,"rb");
	}
	if (!fileIn && i_lpszFile_Path && i_lpszFile_Path[0] != 0)
	{
		sprintf(lpszFilename,"%s/%s.dat",i_lpszFile_Path,lpszElement_ID_Temp);
		fileIn = fopen(lpszFilename,"rb");
	}
	if (!fileIn)
	{
		char * lpData_Path;
		lpData_Path = getenv("XASTRO_DATA_PATH");
		if (lpData_Path)
		{
			unsigned int uiReq_Len = strlen(lpData_Path) + strlen(lpszElement_ID_Temp) + 7;
			if (uiReq_Len > uiFilename_Len)
			{
				if (lpszFilename)
					delete [] lpszFilename;
				uiFilename_Len = uiReq_Len;
				lpszFilename = new char[uiFilename_Len];
			}
			sprintf(lpszFilename,"%s/%s.dat",lpData_Path,lpszElement_ID_Temp);
			fileIn = fopen(lpszFilename,"rb");
		}
	}

#ifdef DATADIR
	if (!fileIn)
	{
		unsigned int uiReq_Len = strlen(DATADIR) + strlen(lpszElement_ID_Temp) + 7;
		if (uiReq_Len > uiFilename_Len)
		{
			if (lpszFilename)
				delete [] lpszFilename;
			uiFilename_Len = uiReq_Len;
			lpszFilename = new char[uiFilename_Len];
		}
		sprintf(lpszFilename,"%s/%s.dat",DATADIR,lpszElement_ID_Temp);
		fileIn = fopen(lpszFilename,"rb");
	}
#endif

	if (!fileIn)
	{
		unsigned int uiReq_Len = strlen(lpszElement_ID_Temp) + 7;
		if (uiReq_Len > uiFilename_Len)
		{
			if (lpszFilename)
				delete [] lpszFilename;
			uiFilename_Len = uiReq_Len;
			lpszFilename = new char[uiFilename_Len];
		}
		sprintf(lpszFilename,"%s.dat",lpszElement_ID_Temp);
		fileIn = fopen(lpszFilename,"rb");
	}

	printf("XASTRO: (line - element data) Reading file %s\n",lpszFilename);
	if (fileIn)
	{
		if (ReadBinaryFileHeader(fileIn))
		{
//			printf("Header read complete\n");
			for (uiI = 0; uiI < uiZ; uiI++)
			{
//				printf("Read ion data %i\n",uiI);
				ReadBinaryDataFileIonData(lpcIonData[uiI],fileIn);
			}
		}
		fclose(fileIn);
	}
	else
		printf("XASTRO: (line - element data) Unable to open file %s\n",lpszFilename);

	
	delete [] lpszFilename;
}

