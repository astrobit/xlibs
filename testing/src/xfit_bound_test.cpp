#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <xmath.h>
#include <xlinalg.h>
// This is a fitting routine to find the minimum within a bound space.  The User must specify the bounds and the function to fit

#include <xio.h>


bool Convergence_Point(const XVECTOR &i_lpVector, const XVECTOR &i_cThreshold)
{
	XVECTOR cTest;
	bool bRet = (i_cThreshold.Get_Size() == i_lpVector.Get_Size());
	if (!bRet)
		fprintf(stderr,"xfit: Convergence threshold size not the same as parameter vector size!\n");

	for (unsigned int uiK = 0; uiK < i_lpVector.Get_Size() && bRet; uiK++)
	{
		bRet = (fabs(i_lpVector.Get(uiK)) < i_cThreshold.Get(uiK));
	}
	return bRet;
}

bool Convergence(const XVECTOR * i_lpVectors, unsigned int i_uiNum_Points,const XVECTOR &i_cThreshold, XVECTOR * o_lpcConvergence_Fault = NULL)
{
	XVECTOR cTest;
	bool bRet = true;
	for(unsigned int uiI = 0; uiI < i_uiNum_Points; uiI++)
		bRet &= (i_cThreshold.Get_Size() == i_lpVectors[uiI].Get_Size());
	if (!bRet)
		fprintf(stderr,"xfit: Convergence threshold size not the same as parameter vector size!\n");

	for(unsigned int uiI = 0; uiI < i_uiNum_Points && bRet; uiI++)
	{
		for (unsigned int uiJ = uiI + 1; uiJ < i_uiNum_Points && bRet; uiJ++)
		{
			cTest = i_lpVectors[uiJ] - i_lpVectors[uiI];
			for (unsigned int uiK = 0; uiK < cTest.Get_Size() && bRet; uiK++)
			{
				bRet = (fabs(cTest.Get(uiK)) < i_cThreshold.Get(uiK));
				if (!bRet && o_lpcConvergence_Fault)
				{
					o_lpcConvergence_Fault[0] = cTest;
				}
			}
		}
	}
	return bRet;
}
bool BoundTest(const XVECTOR & i_cTest_Point, const XVECTOR * i_lpcBounds)
{
	bool bRet;
	bRet = (i_cTest_Point.Get_Size() == i_lpcBounds[0].Get_Size() && i_cTest_Point.Get_Size() == i_lpcBounds[1].Get_Size());
	if (!bRet)
		fprintf(stderr,"xfit: Bounds size not the same as test point size!\n");
	XVECTOR cTest = i_cTest_Point - i_lpcBounds[0];
	for (unsigned int uiI = 0; uiI < i_cTest_Point.Get_Size() && bRet; uiI++)
	{
		bRet &= (cTest.Get(uiI) >= 0.0);
	}
	cTest = i_lpcBounds[1] - i_cTest_Point;
	for (unsigned int uiI = 0; uiI < i_cTest_Point.Get_Size() && bRet; uiI++)
	{
		bRet &= (cTest.Get(uiI) >= 0.0);
	}
	return bRet;
}

void SavePointsCache(const char * i_lpszFilename, const XVECTOR * lpvPoints, unsigned int uiNum_Points, unsigned int uiNum_Parameters, const double * lpdFit)
{
	if (i_lpszFilename && i_lpszFilename[0] != 0)
	{
		XDATASET	cDataset;
		cDataset.Allocate(uiNum_Parameters + 1,uiNum_Points);
		for (unsigned int uiI = 0; uiI < uiNum_Parameters; uiI++)
			for (unsigned int uiJ = 0; uiJ < uiNum_Points; uiJ++)
				cDataset.SetElement(uiI,uiJ,lpvPoints[uiJ].Get(uiI));
		for (unsigned int uiJ = 0; uiJ < uiNum_Points; uiJ++)
			cDataset.SetElement(uiNum_Parameters,uiJ,lpdFit[uiJ]);
		cDataset.SaveDataFileBin(i_lpszFilename,true);
	}
}
bool ReadPointsCache(const char * i_lpszFilename, XVECTOR * lpvPoints, unsigned int uiNum_Points, unsigned int uiNum_Parameters, double * lpdFit)
{
	bool	bRet = false;
	if (i_lpszFilename && i_lpszFilename[0] != 0)
	{
		XDATASET	cDataset;
		cDataset.ReadDataFileBin(i_lpszFilename,true);
		if (uiNum_Points == cDataset.GetNumElements() && (uiNum_Parameters + 1) == cDataset.GetNumColumns())
		{
			bRet = true;
			for (unsigned int uiI = 0; uiI < uiNum_Parameters; uiI++)
				for (unsigned int uiJ = 0; uiJ < uiNum_Points; uiJ++)
				{
					//printf("%.1f\t",cDataset.GetElement(uiI,uiJ));
					lpvPoints[uiJ].Set(uiI,cDataset.GetElement(uiI,uiJ));
				}
			for (unsigned int uiJ = 0; uiJ < uiNum_Points; uiJ++)
				lpdFit[uiJ] = cDataset.GetElement(uiNum_Parameters,uiJ);
		}
	}
	return bRet;
}


void xPerform_Fit_Bound(const XVECTOR &i_vLower_Bound,const XVECTOR &i_vUpper_Bound, const XVECTOR & i_lpvFit_Threshold, QFunctionV i_fUser_Function, XVECTOR & o_vResult, void * i_lpvUser_Data, const char * i_lpszCache_Filename)
{
	// use the following define to set the numberr of dimensions to test - value should be # dim + 1 (e.g. 3-d = 4 points)
	
	unsigned int uiNum_Parameters = i_vLower_Bound.Get_Size();
	if (uiNum_Parameters != i_vUpper_Bound.Get_Size() || uiNum_Parameters != i_lpvFit_Threshold.Get_Size())
		fprintf(stderr,"xPerform_Fit_Bound: bound and theshold size doesn't match\n");
	else
	{
		double	dFit_Min = DBL_MAX;
		double	*lpdFit;
		// define tetrahedron spanning the search space
		XVECTOR *lpcTest_Points;
		XVECTOR cCentroid(uiNum_Parameters);
		XVECTOR cTestOuter(uiNum_Parameters);
		XVECTOR cTestInner(uiNum_Parameters);
		XVECTOR cTest(uiNum_Parameters);
		XVECTOR cDelta(uiNum_Parameters);
		double dTest_Fit = 0.0;
		unsigned int uiNum_Vertices;

		uiNum_Vertices = 1 << uiNum_Parameters;
		lpcTest_Points = new XVECTOR[uiNum_Vertices];
		lpdFit = new double[uiNum_Vertices];

		for (unsigned int uiI = 0; uiI < uiNum_Vertices; uiI++)
			lpcTest_Points[uiI].Set_Size(uiNum_Parameters);

		double	dFit_Sum = 0.0;
		if (i_lpszCache_Filename == NULL || !ReadPointsCache(i_lpszCache_Filename,lpcTest_Points,uiNum_Vertices,uiNum_Parameters,lpdFit))
		{
			// create a cuboid that spans the search space
			for (unsigned int uiI = 0; uiI < uiNum_Vertices; uiI++)
			{
				lpcTest_Points[uiI] = i_vLower_Bound;
				for (unsigned int uiJ = 0; uiJ < uiNum_Parameters; uiJ++)
				{
					if (uiI & (1 << uiJ)) 
						lpcTest_Points[uiI].Set(uiJ,i_vUpper_Bound.Get(uiJ));
				}
			}
			for (unsigned int uiI = 0; uiI < uiNum_Vertices; uiI++)
			{
				cCentroid += lpcTest_Points[uiI];
				dFit_Sum ++;
			}
			cCentroid /= dFit_Sum;
	//		printf("Centroid\n");
		//	cCentroid.Print();
	
			// Make sure the centroid produces a valid spectrum - if not, randomly generate a point within the search space to act as the centroid for the time being...
			// This centroid is going to be used to adjust edges of the cuboid to have a valid fit
			double dCentroid_Fit = 0.0;
			do
			{
				fprintf(stdout,".");
				fflush(stdout);
				if (isnan(dCentroid_Fit) || isinf(dCentroid_Fit)) // need a different point - try random point within the grid
				{
					cDelta = i_vUpper_Bound - i_vLower_Bound;
					for (unsigned int uiI = 0; uiI < uiNum_Parameters; uiI++)
					{
						cCentroid.Set(uiI,i_vLower_Bound.Get(uiI) + xrand_d() * cDelta.Get(uiI));
					}
				}
				dCentroid_Fit = i_fUser_Function(cCentroid,i_lpvUser_Data);
			}
			while (isnan(dCentroid_Fit) || isinf(dCentroid_Fit));
	
			for (unsigned int uiI = 0; uiI < uiNum_Vertices; uiI++)
			{
				fprintf(stdout,"-");
				fflush(stdout);
				lpdFit[uiI] = i_fUser_Function(lpcTest_Points[uiI],i_lpvUser_Data);
				cTestOuter = lpcTest_Points[uiI];
				cTest = lpcTest_Points[uiI];
				while(isnan(lpdFit[uiI]) || isinf(lpdFit[uiI]))
				{
					fprintf(stdout,"_");
					fflush(stdout);
					XVECTOR cDelta = cTestOuter - cCentroid;
					cDelta *= 0.5;
					cTest = cCentroid + cDelta;
		//			cTest.Print();
					lpdFit[uiI] = i_fUser_Function(cTest,i_lpvUser_Data);
					cTestOuter = cTest;
				}
				lpcTest_Points[uiI] = cTest;
			}

			SavePointsCache(i_lpszCache_Filename,lpcTest_Points,uiNum_Vertices,uiNum_Parameters,lpdFit);
		}
		unsigned int uiFit_Max_Point = uiNum_Vertices;
		unsigned int uiFit_Min_Point = uiNum_Vertices;
	
		while (!Convergence(lpcTest_Points, uiNum_Vertices , i_lpvFit_Threshold))
		{
			fprintf(stdout,"+");
			fflush(stdout);
	//		fprintf(stdout,"F: %.1f %.1f %.1f %.1f %.1f %.1f %.1f\n",cConvergence_Fault.Get(0),cConvergence_Fault.Get(1),cConvergence_Fault.Get(2),cConvergence_Fault.Get(3),cConvergence_Fault.Get(4),cConvergence_Fault.Get(5),cConvergence_Fault.Get(6));

			// Find point with worst fit
			double	dFit_Max = 0.0;
			double	dFit_Near_Max = 0.0;
			double	dFit_Min = DBL_MAX;
			double	dFit_Variance = 0.0;
			double	dFit_Mean = 0.0;
			bool	bClose_Fit = true;
			uiFit_Max_Point = uiNum_Vertices;
			uiFit_Min_Point = uiNum_Vertices;
			for (unsigned int uiI = 0; uiI < uiNum_Vertices; uiI++)
			{
	//			printf("%i : %.2e\n",uiI,lpdFit[uiI]);
				dFit_Variance += lpdFit[uiI] * lpdFit[uiI];
				dFit_Mean += lpdFit[uiI];
				if (lpdFit[uiI] > dFit_Max)
				{
					dFit_Max = lpdFit[uiI];
					uiFit_Max_Point = uiI;
				}
				if (lpdFit[uiI] < dFit_Min)
				{
					dFit_Min = lpdFit[uiI];
					uiFit_Min_Point = uiI;
				}
			}
			if (dFit_Min <= 0.5)
			{
				for (unsigned int uiI = 0; uiI < uiNum_Vertices; uiI++)
				{
					lpdFit[uiI] = lpdFit[uiFit_Min_Point];
					lpcTest_Points[uiI] = lpcTest_Points[uiFit_Min_Point];
				}
			}
			else
			{
				dFit_Variance /= uiNum_Vertices;
				dFit_Mean /= uiNum_Vertices;
				double dFit_Std_Dev = dFit_Variance - dFit_Mean * dFit_Mean;
	//			for (unsigned int uiI = 0; uiI < uiNum_Vertices && bClose_Fit; uiI++)
	//			{
	//				bClose_Fit = (fabs(lpdFit[uiI] - dFit_Mean) < (2.0 * dFit_Std_Dev));
	//			}
	//			bClose_Fit |= (dFit_Std_Dev < 10.0);
				bClose_Fit &= (dFit_Std_Dev < 10.0);
				if (dFit_Near_Max == 0.0)
				{
					for (unsigned int uiI = 0; uiI < uiNum_Vertices; uiI++)
					{
						if (lpdFit[uiI] > dFit_Near_Max && uiI != uiFit_Max_Point)
							dFit_Near_Max = lpdFit[uiI];
					}
				}
		//		fprintf(stdout,"%i",uiFit_Max_Point);
		//		fprintf(stdout,"F: %.8e %.8e %.8e %.8e %.8e %.8e %.8e\n",dFit[0],dFit[1],dFit[2],dFit[3],dFit[4],dFit[5],dFit[6],dFit[7]);
		//		if (uiFit_Max_Point < uiNum_Vertices)
				{
					// Compute weighted centroid of remaining parameters
					for (unsigned int uiI = 0; uiI < cCentroid.Get_Size(); uiI++)
						cCentroid.Set(uiI,0.0);
					dFit_Sum = 0.0;
					for (unsigned int uiI = 0; uiI < uiNum_Vertices; uiI++)
					{
						if (uiI != uiFit_Max_Point)
						{
							XVECTOR cTemp = lpcTest_Points[uiI];
		//					lpcTest_Points[uiI].Print(NULL);
		//					cTemp.Print(NULL);
							if (lpdFit[uiI] > 1.0)
								cTemp *= lpdFit[uiI];
							cCentroid += cTemp;
		//					cCentroid.Print(NULL);
							if (lpdFit[uiI] > 1.0)
								dFit_Sum += lpdFit[uiI];
							else
								dFit_Sum += 1.0;
						}
					}
					cCentroid /= dFit_Sum;
		//			printf("Centroid\n");
		//			for (unsigned int uiI = 0; uiI < uiNum_Parameters; uiI++)
		//			{
		//				printf("%.1f\n",cCentroid.Get(uiI));
		//			}
					cTestOuter = lpcTest_Points[uiFit_Max_Point];
					cTestInner = cCentroid;
	//				printf("\n");
	//				cCentroid.Print();
					dTest_Fit = i_fUser_Function(cCentroid,i_lpvUser_Data);
					if (bClose_Fit && !isnan(dTest_Fit) && !isinf(dTest_Fit))
					{
						for (unsigned int uiI = 0; uiI < uiNum_Vertices; uiI++)
						{
	//						fprintf(stdout,"-%.0f\t%.2e\t%.0f\t%.0f\t%c\t%.1f\n",dTest_Fit,dFit_Min,dFit_Max,dFit_Near_Max,bClose_Fit ? 't' : 'f',dFit_Std_Dev);
							fprintf(stdout,"-");
							fflush(stdout);
							cDelta = cCentroid - lpcTest_Points[uiI];
							cDelta *= 0.5;
							lpcTest_Points[uiI] += cDelta;
							lpdFit[uiI] = i_fUser_Function(lpcTest_Points[uiI],i_lpvUser_Data);
						}
					}
					else if (dTest_Fit > dFit_Near_Max)
					{
						cTestOuter = cCentroid;
						cTestInner = lpcTest_Points[uiFit_Min_Point];
						cTest = cTestInner;
						while (isnan(dTest_Fit) || isinf(dTest_Fit) || dTest_Fit > dFit_Near_Max)
						{
							//cCentroid.Print();
	//						fprintf(stdout,"@%.0f\t%.2e\t%.0f\t%.0f\t%c\t%.1f\n",dTest_Fit,dFit_Min,dFit_Max,dFit_Near_Max,bClose_Fit ? 't' : 'f',dFit_Std_Dev);
							fprintf(stdout,"@");
							fflush(stdout);
							cDelta = cTestOuter - cTestInner;
							cDelta *= 0.5;
							cTest = cTestInner + cDelta;
	//						cTest.Print();
							dTest_Fit = i_fUser_Function(cTest,i_lpvUser_Data);
							cTestOuter = cTest;
	/*						lpcTest_Points[uiFit_Max_Point] = cCentroid;
							lpdFit[uiFit_Max_Point] = dTest_Fit;
							for (unsigned int uiI = 0; uiI < cCentroid.Get_Size(); uiI++)
								cCentroid.Set(uiI,0.0);
							dFit_Sum = 0.0;
							for (unsigned int uiI = 0; uiI < uiNum_Vertices; uiI++)
							{
								XVECTOR cTemp = lpcTest_Points[uiI];
								if (lpdFit[uiI] > 1.0)
									cTemp *= lpdFit[uiI];
								cCentroid += cTemp;
								if (lpdFit[uiI] > 1.0)
									dFit_Sum += lpdFit[uiI];
								else
									dFit_Sum == 1.0;
							}
							cCentroid /= dFit_Sum;
							Generate_Synow_Spectra(i_cTarget,i_cOpacity_Map_A,i_cOpacity_Map_B,i_uiIon,cCentroid,cOutput);
							dTest_Fit = Fit(i_cTarget, cOutput, i_dMin_WL, i_dMax_WL, i_bChi2);
	*/
						}
						lpcTest_Points[uiFit_Max_Point] = cTest;
						lpdFit[uiFit_Max_Point] = dTest_Fit;
					}
					else
					{
						cTest = lpcTest_Points[uiFit_Max_Point];
						double dTest_Fit = 0.0;
						while(isnan(dTest_Fit) || isinf(dTest_Fit) || (!bClose_Fit && dTest_Fit < dFit_Min))
						{
	//						fprintf(stdout,"*%.0f\t%.2e\t%.0f\t%.0f\t%c\t%.1f\n",dTest_Fit,dFit_Min,dFit_Max,dFit_Near_Max,bClose_Fit ? 't' : 'f',dFit_Std_Dev);
							fprintf(stdout,"*");
							fflush(stdout);
							cDelta = cTestOuter - cTestInner;
							cDelta *= 0.5;
							cTest = cTestInner + cDelta;
	//						cTest.Print();
							dTest_Fit = i_fUser_Function(cTest,i_lpvUser_Data);
							cTestInner = cTest;
						}
						cTestOuter = cTest;
						cTestInner = cCentroid;
						while(isnan(dTest_Fit) || isinf(dTest_Fit) || dTest_Fit == 0.0 || dTest_Fit > dFit_Near_Max)
						{
	//						fprintf(stdout,"%%%.0f\t%.2e\t%.0f\t%.0f\t%c\t%.1f\n",dTest_Fit,dFit_Min,dFit_Max,dFit_Near_Max,bClose_Fit ? 't' : 'f',dFit_Std_Dev);
							fprintf(stdout,"%%");
							fflush(stdout);
							cDelta = cTestOuter - cTestInner;
							cDelta *= 0.5;
							cTest = cTestInner + cDelta;
	//						cTest.Print();
							dTest_Fit = i_fUser_Function(cTest,i_lpvUser_Data);
							cTestOuter = cTest;
						}
						lpcTest_Points[uiFit_Max_Point] = cTest;
						lpdFit[uiFit_Max_Point] = dTest_Fit;
					}
				}
			}
			SavePointsCache(i_lpszCache_Filename,lpcTest_Points,uiNum_Vertices,uiNum_Parameters,lpdFit);
		}

	//	printf("Convergence\n");
	//	for (unsigned int uiI = 0; uiI < uiNum_Parameters; uiI++)
	//	{
	//		printf("%.1f\n",cConvergence_Fault.Get(uiI));
	//	}
		for (unsigned int uiI = 0; uiI < cCentroid.Get_Size(); uiI++)
			cCentroid.Set(uiI,0.0);
		for (unsigned int uiI = 0; uiI < uiNum_Vertices; uiI++)
		{
			cCentroid += lpcTest_Points[uiI];
		}
		cCentroid /= (double)(uiNum_Vertices);

		o_vResult = cCentroid;
	}
}


void xPerform_Fit_Bound_Simplex(const XVECTOR &i_vLower_Bound,const XVECTOR &i_vUpper_Bound, const XVECTOR & i_lpvFit_Threshold, QFunctionV i_fUser_Function, XVECTOR & o_vResult, void * i_lpvUser_Data, const char * i_lpszCache_Filename)
{
	unsigned int uiNum_Parameters = i_vLower_Bound.Get_Size();
	if (uiNum_Parameters != i_vUpper_Bound.Get_Size() || uiNum_Parameters != i_lpvFit_Threshold.Get_Size())
		fprintf(stderr,"xPerform_Fit_Bound: bound and theshold size doesn't match\n");
	else
	{
		double	dFit_Min = DBL_MAX;
		double	*lpdFit;
		unsigned int uiNum_Points = uiNum_Parameters + 1;
		// define tetrahedron spanning the search space
		XVECTOR *lpcTest_Points;
		XVECTOR cCentroid(uiNum_Parameters);
		XVECTOR cTestOuter(uiNum_Parameters);
		XVECTOR cTestInner(uiNum_Parameters);
		XVECTOR cTest(uiNum_Parameters);
		XVECTOR cDelta(uiNum_Parameters);
		XVECTOR cBounds[2];
		double dTest_Fit = 0.0;
		unsigned int uiNum_Vertices;

		lpcTest_Points = new XVECTOR[uiNum_Points];
		lpdFit = new double[uiNum_Points];

		if (i_lpszCache_Filename && !ReadPointsCache(i_lpszCache_Filename,lpcTest_Points,uiNum_Vertices,uiNum_Parameters,lpdFit))
		{
			lpcTest_Points[0] = i_vLower_Bound;
			for (unsigned int uiI = 1; uiI < uiNum_Points; uiI++)
			{
				lpcTest_Points[uiI] = i_vLower_Bound;
				lpcTest_Points[uiI].Set(uiI - 1,i_vUpper_Bound.Get(uiI - 1));
			}
			cBounds[0] = i_vLower_Bound;
			cBounds[1] = i_vUpper_Bound;

			for (unsigned int uiI = 0; uiI < uiNum_Points; uiI++)
			{
				fprintf(stdout,"-");
				fflush(stdout);
				lpdFit[uiI] = i_fUser_Function(lpcTest_Points[uiI],i_lpvUser_Data);
			}
			SavePointsCache(i_lpszCache_Filename,lpcTest_Points,uiNum_Points,uiNum_Parameters,lpdFit);
		}
		unsigned int uiFit_Max_Point = 0;
		// The method used below is basically simplex optimization, with a few modifications to handle bad data points and a contrained parameter space
//		while ((uiFit_Max_Point < uiNum_Parameters) && !Convergence(lpcTest_Points, uiNum_Parameters , i_lpvFit_Threshold))
		while (!Convergence(lpcTest_Points, uiNum_Parameters , i_lpvFit_Threshold))
		{
			fprintf(stdout,".");
			fflush(stdout);
	//		fprintf(stdout,"F: %.1f %.1f %.1f %.1f %.1f %.1f %.1f\n",cConvergence_Fault.Get(0),cConvergence_Fault.Get(1),cConvergence_Fault.Get(2),cConvergence_Fault.Get(3),cConvergence_Fault.Get(4),cConvergence_Fault.Get(5),cConvergence_Fault.Get(6));

			// Test for uniform field
			bool bIdentical = true;
			for (unsigned int uiI = 0; uiI < uiNum_Points && bIdentical; uiI++)
			{
				for (unsigned int uiJ = uiI; uiJ < uiNum_Points && bIdentical; uiJ++)
				{
					bIdentical &= fabs(lpdFit[uiI] - lpdFit[uiJ]) < 1.0e-8;
				}
			}
			if(bIdentical)
				uiFit_Max_Point++;
			else
			{
				// Find point with worst fit
				double	dFit_Max = 0.0;
				for (unsigned int uiI = 0; uiI < uiNum_Points; uiI++)
				{
					if (lpdFit[uiI] > dFit_Max)
					{
						dFit_Max = lpdFit[uiI];
						uiFit_Max_Point = uiI;
					}
				}
			}
	//		fprintf(stdout,"%i",uiFit_Max_Point);
	//		fprintf(stdout,"F: %.8e %.8e %.8e %.8e %.8e %.8e %.8e\n",dFit[0],dFit[1],dFit[2],dFit[3],dFit[4],dFit[5],dFit[6],dFit[7]);
			if (uiFit_Max_Point < uiNum_Points)
			{
				// Compute centroid of remaining parameters
				for (unsigned int uiI = 0; uiI < cCentroid.Get_Size(); uiI++)
					cCentroid.Set(uiI,0.0);

				for (unsigned int uiI = 0; uiI < uiNum_Points; uiI++)
				{
					if (uiI != uiFit_Max_Point)
						cCentroid += lpcTest_Points[uiI];
				}
				cCentroid /= (double)(uiNum_Points - 1);
				// Generate new test position
				double	dFit_Min = DBL_MAX;
				for (unsigned int uiI = 0; uiI < uiNum_Points; uiI++)
				{
					if (lpdFit[uiI] < dFit_Min)
					{
						dFit_Min = lpdFit[uiI];
					}
				}

				double	dMod = -0.5;
				double	dMod_Lcl;
				XVECTOR cDelta = lpcTest_Points[uiFit_Max_Point] - cCentroid;
				XVECTOR cDeltaTemp;
				double dFit_Test;
				XVECTOR cTest;
				if (Convergence_Point(cDelta , i_lpvFit_Threshold))
				{
	//				fprintf(stdout,"C");
					fflush(stdout);
					do
					{
						fprintf(stdout,"*");
						fflush(stdout);
						// The test point has moved to the centroid.  This is bad.  Let's try jitter
						for(unsigned int uiI = 0; uiI < uiNum_Points; uiI++)
						{
							lpcTest_Points[uiFit_Max_Point].Set(uiI,(i_vUpper_Bound.Get(uiI) + i_vLower_Bound.Get(uiI)) * xrand_d());
						}
	//					fprintf(stdout,"S: %.1f %.1f %.1f %.1f %.1f %.1f %.1f\n",cTest_Points[uiFit_Max_Point].Get(0),cTest_Points[uiFit_Max_Point].Get(1),cTest_Points[uiFit_Max_Point].Get(2),cTest_Points[uiFit_Max_Point].Get(3),cTest_Points[uiFit_Max_Point].Get(4),cTest_Points[uiFit_Max_Point].Get(5),cTest_Points[uiFit_Max_Point].Get(6));
						fflush(stdout);
						lpdFit[uiFit_Max_Point] = i_fUser_Function(lpcTest_Points[uiFit_Max_Point],i_lpvUser_Data);
					} while (isnan(lpdFit[uiFit_Max_Point]) || isinf(lpdFit[uiFit_Max_Point]));
				}
				else
				{
					do
					{
						dMod *= 2; // this is intended - first test point is -1
						dMod_Lcl = dMod;
						do
						{
		//					fprintf(stdout,",");
		//					fflush(stdout);
							cDeltaTemp = cDelta;
		//					fprintf(stdout,"S: %.1f %.1f %.1f %.1f %.1f %.1f %.1f\n",cTest_Points[uiFit_Max_Point].Get(0),cTest_Points[uiFit_Max_Point].Get(1),cTest_Points[uiFit_Max_Point].Get(2),cTest_Points[uiFit_Max_Point].Get(3),cTest_Points[uiFit_Max_Point].Get(4),cTest_Points[uiFit_Max_Point].Get(5),cTest_Points[uiFit_Max_Point].Get(6));
		//					fprintf(stdout,"C: %.1f %.1f %.1f %.1f %.1f %.1f %.1f\n",cCentroid.Get(0),cCentroid.Get(1),cCentroid.Get(2),cCentroid.Get(3),cCentroid.Get(4),cCentroid.Get(5),cCentroid.Get(6));
		//					fprintf(stdout,"D: %.1f %.1f %.1f %.1f %.1f %.1f %.1f\n",cDeltaTemp.Get(0),cDeltaTemp.Get(1),cDeltaTemp.Get(2),cDeltaTemp.Get(3),cDeltaTemp.Get(4),cDeltaTemp.Get(5),cDeltaTemp.Get(6));
							cDeltaTemp *= dMod_Lcl;
		//					fprintf(stdout,"D: %.1f %.1f %.1f %.1f %.1f %.1f %.1f\n",cDeltaTemp.Get(0),cDeltaTemp.Get(1),cDeltaTemp.Get(2),cDeltaTemp.Get(3),cDeltaTemp.Get(4),cDeltaTemp.Get(5),cDeltaTemp.Get(6));
							cTest = cCentroid + cDeltaTemp;
		//					fprintf(stdout,"T: %.1f %.1f %.1f %.1f %.1f %.1f %.1f\n",cTest.Get(0),cTest.Get(1),cTest.Get(2),cTest.Get(3),cTest.Get(4),cTest.Get(5),cTest.Get(6));
		//					sleep(1);
							dMod_Lcl += .125;
							if (dMod_Lcl == 0.0)
								dMod_Lcl += 0.50; // make sure we don't use the centroid itself!
						}
						while (!BoundTest(cTest,cBounds) && dMod_Lcl < 1.0);
						dMod_Lcl += 0.125; // raise back to previous test value
						fprintf(stdout,"%%");
						fflush(stdout);
						dFit_Test = i_fUser_Function(cTest,i_lpvUser_Data);
					}
					while (dFit_Test < dFit_Min && dMod_Lcl == dMod);
					if (dFit_Test >= lpdFit[uiFit_Max_Point])
					{
						cTest = cCentroid + cDelta * 0.5;
						dFit_Test = i_fUser_Function(cTest,i_lpvUser_Data);
						fprintf(stdout,"#");
						fflush(stdout);
					}

					if (dFit_Test <= lpdFit[uiFit_Max_Point])
					{
		//				fprintf(stdout,";");
	//					fflush(stdout);
						lpdFit[uiFit_Max_Point] = dFit_Test;
						lpcTest_Points[uiFit_Max_Point] = cTest;
					}
					else
					{
						// no success; contract the simplex
						// Compute centroid 
						for (unsigned int uiI = 0; uiI < cCentroid.Get_Size(); uiI++)
							cCentroid.Set(uiI,0.0);

						for (unsigned int uiI = 0; uiI < uiNum_Points; uiI++)
						{
							cCentroid += lpcTest_Points[uiI];
						}
						cCentroid /= (double)(uiNum_Points);
						// Scale the simplex by 1/2
						for (unsigned int uiI = 0; uiI < uiNum_Points; uiI++)
						{
							cDelta = lpcTest_Points[uiI] - cCentroid;
							lpcTest_Points[uiI] = cCentroid + 0.5 * cDelta;
							lpdFit[uiI] = i_fUser_Function(lpcTest_Points[uiI],i_lpvUser_Data);
							fprintf(stdout,"-");
							fflush(stdout);
						}
					}
				}
			}
			SavePointsCache(i_lpszCache_Filename,lpcTest_Points,uiNum_Points,uiNum_Parameters,lpdFit);
		}

		for (unsigned int uiI = 0; uiI < cCentroid.Get_Size(); uiI++)
			cCentroid.Set(uiI,0.0);
		for (unsigned int uiI = 0; uiI < uiNum_Points; uiI++)
		{
			cCentroid += lpcTest_Points[uiI];
		}
		cCentroid /= (double)(uiNum_Points);

		o_vResult = cCentroid;
	}
}


double	Test_Function(const XVECTOR &i_vX, const void * i_lpvData)
{
	XVECTOR * lpvExpected = (XVECTOR *)i_lpvData;
	double	dXr = i_vX.Get(0) - lpvExpected->Get(0);
	double	dX2 = dXr * dXr + 1.0;
	double	dYr = i_vX.Get(1) - lpvExpected->Get(1);
	double	dExp = exp(-dYr * dYr * 10.0);
	double	dRes = (dX2 - dExp) * 1000.0;
	printf("\n%.1f\n",dRes);
	i_vX.Print();
	return dRes;
}

int main(void)
{
	XVECTOR	vLower(2);
	XVECTOR	vUpper(2);
	XVECTOR	vThreshold(2);
	XVECTOR	vExpected(2);
	XVECTOR	vResult(2);

	vExpected.Set(0,0.75);
	vExpected.Set(1,0.6);

	vLower.Set(0,0.0);
	vLower.Set(1,0.0);

	vUpper.Set(0,1.0);
	vUpper.Set(1,0.75);

	vThreshold.Set(0,0.0001);
	vThreshold.Set(1,0.0001);


	xPerform_Fit_Bound_Simplex(vLower,vUpper,vThreshold,Test_Function,vResult,&vExpected,"xfit.cache");
	printf("\n");
	vResult.Print();

}
