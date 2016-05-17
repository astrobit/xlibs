#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <unistd.h>
#include <ctype.h>
#include "xflash.h"


/* sfocu(argc, argv)

   Compares two flash checkpoint files, to decide wether or not they have
   the same data. Does not load all the file into memory at once, so should
   be able to deal with large datasets.
*/
int main(int iArgC, char ** lpszArgV)
{
	XFLASH_File	cFile;
	XFLASH_Block ** lpBlocks;
	FILE * fileOut = fopen("out.csv","wt");
	unsigned int uiI, iVar, i, j, k,  cellIdx;
	unsigned int iLeafBlocks;
	double	ddx = 0.0, ddy = 0.0, ddz = 0.0;
	double x0 = 0.0, y0 = 0.0, z0 = 0.0;

  /* Open files */
	cFile.Open(lpszArgV[1]);
	
	printf("Found %i blocks in file.\n",cFile.m_uiNum_Blocks);

  /* Now use coordinate and size arrays to build a table translating
     leaf block numbers from one file to the other. We also use this chance
     to dismiss A's non-leaf blocks.

     No need for fancy algorithms since the mapping is close to the identity?
  */
	if (cFile.m_uiBlock_Dimensions[1] > 1 || cFile.m_uiBlock_Dimensions[2] > 1)
	{
		printf("Error: dataset is not 1-d.  Exiting.\n");
		return 0;
	}
	// Count leaf blocks
	iLeafBlocks = 0;
	for(uiI = 0; uiI < cFile.m_uiNum_Blocks; uiI++)
	{
		if (cFile.m_lpeBlock_Node_Type[uiI] == FR_LEAF_NODE)
			iLeafBlocks++;
	}

	// Allocate block pointers for variables
	lpBlocks = new XFLASH_Block * [cFile.m_uiNum_Vars];

	// Output header line	
	fprintf(fileOut,"x, y, z");
		// Variable names
	for(iVar = 0; iVar < cFile.m_uiNum_Vars; iVar++)
	{
		fprintf(fileOut,", %s",cFile.m_lpszVar_Names[iVar]);
	}
	fprintf(fileOut,"\n");

	// Finally, output the data, in block order
	for(uiI = 0; uiI < cFile.m_uiNum_Blocks; uiI++)
	{
		if (cFile.m_lpeBlock_Node_Type[uiI] == FR_LEAF_NODE)
		{
			int iOffset = 2 * cFile.m_uiNum_Dimensions * uiI;
//			for (d = 0; d < 2 * cFile.m_uiNum_Dimensions; d++)
//			{
//				printf("m_lpdBlock_Bounding_Box[%i] %.2e\n",d,cFile.m_lpdBlock_Bounding_Box[iOffset + d]);
//			}
			// compute the cell 
			ddx = (cFile.m_lpdBlock_Bounding_Box[iOffset + 1] - cFile.m_lpdBlock_Bounding_Box[iOffset + 0]);
			if (cFile.m_uiNum_Dimensions >= 2)
				ddy = cFile.m_lpdBlock_Bounding_Box[iOffset + 3] - cFile.m_lpdBlock_Bounding_Box[iOffset + 2];
			if (cFile.m_uiNum_Dimensions >= 3)
				ddz = cFile.m_lpdBlock_Bounding_Box[iOffset + 5] - cFile.m_lpdBlock_Bounding_Box[iOffset + 4];
//			printf("%.2e\t%.2e\t%.2e\n",ddx,ddy,ddz);

			for(iVar = 0; iVar < cFile.m_uiNum_Vars; iVar++)
			{
				lpBlocks[iVar] = cFile.GetBlock(iVar, uiI);
//				printf("Got block %i of %i\n",ivar,cFile.nvar);
//				cellCount = blA[ivar]->size[0] * blA[ivar]->size[1] * blA[ivar]->size[2];
			}
            x0 = cFile.m_lpdBlock_Bounding_Box[iOffset + 0];
			if (cFile.m_uiNum_Dimensions >= 2)
	            y0 = cFile.m_lpdBlock_Bounding_Box[iOffset + 2];
			if (cFile.m_uiNum_Dimensions >= 3)
	            z0 = cFile.m_lpdBlock_Bounding_Box[iOffset + 4];
			ddx /= (double)lpBlocks[0]->m_uiSize[0];
			ddy /= (double)lpBlocks[0]->m_uiSize[1];
			ddz /= (double)lpBlocks[0]->m_uiSize[2];
//			printf("%.2e\t%.2e\t%.2e\n",ddx,ddy,ddz);

			// Parse through the data and output it to file
			for (i = 0; i < lpBlocks[0]->m_uiSize[0]; i++)
				for (j = 0; j < lpBlocks[0]->m_uiSize[1]; j++)
					for (k = 0; k < lpBlocks[0]->m_uiSize[2]; k++)
					{
						cellIdx = i + j * lpBlocks[0]->m_uiSize[0] + k * lpBlocks[0]->m_uiSize[0]  * lpBlocks[0]->m_uiSize[1];
			
//						printf("%.2e\t%.2e\t%.2e\n",(i + 0.5) * ddx + x0, (j * 0.5) * ddy + y0, (k * 0.5) * ddz + z0);
						fprintf(fileOut,"%.17e, %.17e, %.17e",(i + 0.5) * ddx + x0, (j * 0.5) * ddy + y0, (k * 0.5) * ddz + z0);
						for(iVar = 0; iVar < cFile.m_uiNum_Vars; iVar++)
						{
						/*this is where the acutal grid data is read*/
							fprintf(fileOut,", %.17e",lpBlocks[iVar]->m_lpdData[cellIdx]);
						}
						fprintf(fileOut,"\n");
					}

			for(iVar = 0; iVar < cFile.m_uiNum_Vars; iVar++)
				cFile.ReleaseBlock(lpBlocks[iVar]);
		}
	}
	delete [] lpBlocks;

	/* Cleanup */
	cFile.Close();
 
	return 0;
}

