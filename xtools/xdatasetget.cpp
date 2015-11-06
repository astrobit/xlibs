#include <xio.h>
#include <cmath>

int main(int i_iArg_Count, const char * i_lpszArg_Values[])
{
	XDATASET	cData;
	unsigned int uiRow;
	unsigned int uiCol;
	if (i_iArg_Count == 4 && strstr(i_lpszArg_Values[1],".xdataset") != NULL)
	{
		uiRow = atoi(i_lpszArg_Values[2]);
		uiCol = atoi(i_lpszArg_Values[3]);
		cData.ReadDataFileBin(i_lpszArg_Values[1]);
	}
	else if (i_iArg_Count == 5)
	{
		uiRow = atoi(i_lpszArg_Values[3]);
		uiCol = atoi(i_lpszArg_Values[4]);
		cData.ReadDataFile(i_lpszArg_Values[1],false,false,',',atoi(i_lpszArg_Values[2]));
	}
	else
	{
		printf("Usage: xdatasetget <file> <headerlines> <column> <row>\n");
		printf("If <file> is an .xdataset, <headerlines> is ommitted.\n");
		printf("File is assumed to be comma separated.\n");
	}
	if (cData.GetNumElements() > 0)
	{
		if (uiCol > cData.GetNumColumns() || uiRow > cData.GetNumRows())
			printf("Col %i Row %i not in dataset\n",uiCol,uiRow);
		else if (cData.IsElementEmpty(uiCol,uiRow))
			printf("----\n");
		else
			printf("%f\n",cData.GetElement(uiCol,uiRow));
	}
	return 0;
}
