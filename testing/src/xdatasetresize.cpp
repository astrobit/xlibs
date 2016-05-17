#include <xio.h>


void Reset(XDATASET & cDataset)
{
	cDataset.Allocate(16,16);
	for (unsigned int uiI = 0; uiI < 16; uiI++)
		for (unsigned int uiJ = 0; uiJ < 16; uiJ++)
			cDataset.SetElement(uiI,uiJ,uiI * 100 + uiJ);
}
void Test(const XDATASET & cDataset)
{
	for (unsigned int uiI = 0; uiI < 16; uiI++)
		for (unsigned int uiJ = 0; uiJ < 16; uiJ++)
			if (cDataset.GetElement(uiI,uiJ) != (uiI * 100 + uiJ))
				printf("Error %i %i\n");
}

void PerformTests(unsigned int uiTest_Size)
{
	XDATASET	cDataset;
	printf("Testing %i\n",uiTest_Size);
	Reset(cDataset);
	printf("Columns\n",uiTest_Size);
	cDataset.Add_Columns(uiTest_Size);
	Test(cDataset);
	Reset(cDataset);
	printf("Rows\n",uiTest_Size);
	cDataset.Add_Rows(uiTest_Size);
	Test(cDataset);
	Reset(cDataset);
	printf("Columns/Rows\n",uiTest_Size);
	cDataset.Add_Columns(uiTest_Size);
	cDataset.Add_Rows(uiTest_Size);
	Test(cDataset);
}

int main(void)
{
	PerformTests(1);
	PerformTests(1024);
	return 0;
}

