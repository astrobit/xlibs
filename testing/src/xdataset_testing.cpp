#include <xio.h>
#include <cstdio>
#include <string>
#include <cmath>

int main(void)
{
	std::string strFail = "...\033[0;31mFAIL\033[0m";
	std::string strPass = "...\033[0;32mPASS\033[0m";
	XDATASET	cData;
	cData.ReadDataFile("xdataset_test.csv",false,false,',',0);

	printf("Number of rows %i...%s\n",cData.GetNumElements(),cData.GetNumElements() == 10 ? strPass.c_str() : strFail.c_str());
	printf("Number of columns %i...%s\n",cData.GetNumColumns(),cData.GetNumColumns() == 9 ? strPass.c_str() : strFail.c_str());
	for (unsigned int uiRow = 0; uiRow < 10; uiRow++)
	{
		printf("Data line %i: ", uiRow);
		bool bPass = true;
		bool bOne_Test = false;
		for (unsigned int uiI = 0; uiI < 9; uiI++)
		{
			if (uiI > 0)
				printf(", ");
			if (cData.IsElementEmpty(uiI,uiRow))
				printf("---");
			else
			{
				bOne_Test = true;
				printf("%.1f",cData.GetElement(uiI,uiRow));
				double dExpected_Value = (uiRow + 1) + (uiI + 1) * 0.1;
				bool bTest = fabs(cData.GetElement(uiI,uiRow) - dExpected_Value) < 0.01;
				bPass &= bTest;
				if (!bTest)
					printf("f");
			}
		}
		printf("...%s\n",(bPass && bOne_Test) ? strPass.c_str() : strFail.c_str());
		printf("\n");
	}
	return 0;
};
