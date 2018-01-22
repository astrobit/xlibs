#include <xio.h>
#include <cmath>

int main(int i_iArg_Count, const char * i_lpszArg_Values[])
{
	xdataset_improved	cData;
	unsigned int uiRow;
	unsigned int uiCol;
/*	if (i_iArg_Count == 4 && strstr(i_lpszArg_Values[1],".xdataset") != NULL)
	{
		uiRow = atoi(i_lpszArg_Values[3]);
		uiCol = atoi(i_lpszArg_Values[2]);
		cData.ReadDataFileBin(i_lpszArg_Values[1]);
	}
	else*/ if (i_iArg_Count == 5)
	{
		uiRow = atoi(i_lpszArg_Values[4]);
		uiCol = atoi(i_lpszArg_Values[3]);
		cData.Read_Data_File(i_lpszArg_Values[1],false,',',atoi(i_lpszArg_Values[2]));
	}
	else
	{
		printf("Usage: xdatasetget <file> <headerlines> <column> <row>\n");
		printf("If <file> is an .xdataset, <headerlines> is ommitted.\n");
		printf("File is assumed to be comma separated.\n");
		printf("Value of specified element is sent to stdout.  If table element is empty, output is CR.\n");
		printf("If the table is not sufficiently large, a message is sent to stderr\n");
	}
	if (cData.Get_Num_Columns() > 0 && cData.Get_Num_Rows() > 0)
	{
		if (uiCol >= cData.Get_Num_Columns() || uiRow >= cData.Get_Num_Rows())
			fprintf(stderr,"Col %i Row %i not in dataset\n",uiCol,uiRow);
		else if (cData.Get_Element_Type(uiRow,uiCol) == xstdlib::empty)
			printf(" (empty)\n");
		else
		{
			printf("%s",cData.Get_Element(uiRow,uiCol).c_str());
			switch(cData.Get_Element_Type(uiRow,uiCol))
			{
			case xstdlib::logical:
				printf (" (logical)\n");
				break;
			case xstdlib::hex:
				printf (" (hex)\n");
				break;
			case xstdlib::octal:
				printf (" (octal)\n");
				break;
			case xstdlib::binary:
				printf (" (binary)\n");
				break;
			case xstdlib::integer:
				printf (" (integer)\n");
				break;
			case xstdlib::floating_point:
				printf (" (floating point)\n");
				break;
			case xstdlib::string:
				printf (" (string)\n");
				break;
			}
		}
	}
	return 0;
}
