#include <xio.h>
#include <string.h>

int main(int i_iArg_Count, char * i_lpszArg_Values[])
{
	XDATASET	cDataset;
	char		lpszFilename[64];
	for (unsigned int uiI = 1; uiI < (unsigned int)(i_iArg_Count); uiI++)
	{
		cDataset.ReadDataFileBin(i_lpszArg_Values[uiI]);
		if (strchr(i_lpszArg_Values[uiI],'.'))
		{
			strcpy(lpszFilename,i_lpszArg_Values[uiI]);
			char * lpszCursor = lpszFilename;
			lpszCursor += strlen(lpszFilename);
			while (lpszCursor[0] != '.')
				lpszCursor--;
			strcpy(lpszCursor,".csv");
		}
		else
			sprintf(lpszFilename,"%s.csv",i_lpszArg_Values[uiI]);
		cDataset.SaveDataFileCSV(lpszFilename,NULL);
	}
	return 0;
	
}
