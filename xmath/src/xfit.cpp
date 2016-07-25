#include <xfit.h>
#include <xpolynomial.h>
#include <cmath>

void PolynomialFit(const XVECTOR &i_vX, const XVECTOR &i_vY, const XVECTOR &i_vWeights, unsigned int i_uiDegree, XPOLYNOMIAL & o_polyFit, XSQUARE_MATRIX &o_cCovariance_Matrix)
{
	if (i_vX.Get_Size() == i_vY.Get_Size() && i_vX.Get_Size() == i_vWeights.Get_Size() && i_uiDegree >= 1 && i_vX.Get_Size() >= (i_uiDegree + 1))
	{
		unsigned int uiMatrix_Size = i_uiDegree + 1;
		unsigned int uiData_Size = i_vX.Get_Size();
		o_polyFit.Set_Allocated_Memory(i_uiDegree);
		o_cCovariance_Matrix.Set_Size(uiMatrix_Size);
		XVECTOR	vParameters(uiMatrix_Size);
		XVECTOR	vY(uiMatrix_Size);
		XVECTOR	vWX(uiData_Size);
		XVECTOR	vWY(uiData_Size);
		for (unsigned int uiI = 0; uiI < uiMatrix_Size; uiI++)
		{
			vY.Set(uiI,0.0);
		}
		vWX = i_vWeights;
		vWY = i_vWeights;
		vWY.Vector_Multiply(i_vY);

		for (unsigned int uiI = 0; uiI < uiMatrix_Size; uiI++)
		{
			double	dValWX = 0.0;
			double	dValWY = 0.0;
			for (unsigned int uiJ = 0; uiJ < uiData_Size; uiJ++)
			{
				dValWX += vWX.Get(uiJ);
				dValWY += vWY.Get(uiJ);
			}
			vWX.Vector_Multiply(i_vX);
			vWY.Vector_Multiply(i_vX);
			vY.Set(uiI,dValWY);
			for (unsigned int uiJ = 0; uiJ <= uiI; uiJ++)
			{
				o_cCovariance_Matrix.Set(uiJ,uiI - uiJ,dValWX);
			}
		}
		o_cCovariance_Matrix.Invert_LUD();
		vParameters = o_cCovariance_Matrix * vY;
		for (unsigned int uiI = 0; uiI < i_uiDegree; uiI++)
			o_polyFit.SetCoefficient(vParameters.Get(uiI),uiI);
		double dSMin = 0.0;
		for (unsigned int uiI = 0; uiI < uiData_Size; uiI++)
			dSMin +=  i_vWeights.Get(uiI) * pow(i_vY.Get(uiI) - o_polyFit.Eval(i_vX.Get(uiI)),2.0);
		dSMin /= (uiData_Size - i_uiDegree - 1);
		for (unsigned int uiI = 0; uiI < uiMatrix_Size; uiI++)
		{
			o_cCovariance_Matrix.Scale_Row(uiI,dSMin);
		}
	}
}

bool GeneralFit(const XVECTOR &i_cX, const XVECTOR &i_cY,const XVECTOR &i_cW, QFunctionDA Fit_Function, XVECTOR_FNA i_vFit_Function_dA, XVECTOR & io_vFit_Parameters, XSQUARE_MATRIX &o_cCovariance_Matrix, double &o_dSmin, unsigned int i_uiMax_Iterations, int i_iConvergence_Criterion, unsigned int *o_lpiIterations)
{
	bool			bDone = false;
	if (i_cX.Get_Size() == i_cY.Get_Size() && i_cX.Get_Size() == i_cW.Get_Size() && io_vFit_Parameters.Get_Size() == i_vFit_Function_dA.Get_Size())
	{
		XSQUARE_MATRIX	mN;
		XVECTOR			vY;
		XVECTOR			vDelta_A;
		XVECTOR			vA = io_vFit_Parameters;
		unsigned int	uiI_Max = io_vFit_Parameters.Get_Size();
		unsigned int	uiK_Max = i_cX.Get_Size();
		unsigned int	uiIterations = 0;
		int iDelta_Max = 0;
		int iE_A;
		int iE_dA;
		int iE_oA;
		unsigned int uiI;

		mN.Set_Size(uiI_Max);
		vY.Set_Size(uiI_Max);
		vDelta_A.Set_Size(uiI_Max);

		for (unsigned int uiI = 0; uiI < uiI_Max; uiI++)
			vDelta_A.Set(uiI,0.0);

		while (!bDone && uiIterations < i_uiMax_Iterations)
		{
			uiIterations++;
			vA += vDelta_A;
			mN.Identity();
			for (uiI = 0; uiI < uiI_Max; uiI++)
			{
				QFunctionDA lpfD1 = i_vFit_Function_dA.Get(uiI);
				for (unsigned int uiJ = 0; uiJ <= uiI; uiJ++)
				{
					double	dHSum = 0.0;
					QFunctionDA lpfD2 = i_vFit_Function_dA.Get(uiJ);
					for (unsigned int uiK = 0; uiK < uiK_Max; uiK++)
					{
						dHSum += i_cW.Get(uiK) * lpfD1(i_cX.Get(uiK),vA) * lpfD2(i_cX.Get(uiK),vA);
					}
					mN.Set(uiI,uiJ,dHSum);
					mN.Set(uiJ,uiI,dHSum);
				}
				double	dYSum = 0.0;
				for (unsigned int uiK = 0; uiK < uiK_Max; uiK++)
				{
					
					dYSum += i_cW.Get(uiK) * (i_cY.Get(uiK) - Fit_Function(i_cX.Get(uiK),vA)) * lpfD1(i_cX.Get(uiK),vA);
				}
				vY.Set(uiI,dYSum);
			}
			mN.Invert_LUD();
			vDelta_A = mN * vY;
			iDelta_Max = -40;
			for (uiI = 0; uiI < uiI_Max; uiI++)
			{
				double	dAi = vA.Get(uiI);
				double	dDeltaAi = vDelta_A.Get(uiI);
				double	doAi = sqrt(mN.Get(uiI,uiI));
				frexp(dAi,&iE_A);
				frexp(dDeltaAi,&iE_dA);
				frexp(doAi,&iE_oA);
				if ((iE_dA - iE_A) > iDelta_Max)
					iDelta_Max = iE_dA - iE_A;
				if ((iE_dA - iE_oA) > iDelta_Max)
					iDelta_Max = (iE_dA - iE_oA);
			}

			if (iDelta_Max < i_iConvergence_Criterion)
			{
				bDone = true;
			}
		}
		if (o_lpiIterations)
			o_lpiIterations[0] = uiIterations;
		if (bDone)
		{
			vA += vDelta_A; // update A
			mN.Identity(); // generate final N, C
			for (uiI = 0; uiI < uiI_Max; uiI++)
			{
				QFunctionDA lpfD1 = i_vFit_Function_dA.Get(uiI);
				for (unsigned int uiJ = 0; uiJ <= uiI; uiJ++)
				{
					double	dHSum = 0.0;
					QFunctionDA lpfD2 = i_vFit_Function_dA.Get(uiJ);
					for (unsigned int uiK = 0; uiK < uiK_Max; uiK++)
					{
						dHSum += i_cW.Get(uiK) * lpfD1(i_cX.Get(uiK),vA) * lpfD2(i_cX.Get(uiK),vA);
					}
					mN.Set(uiI,uiJ,dHSum);
					mN.Set(uiJ,uiI,dHSum);
				}
				double	dYSum = 0.0;
				for (unsigned int uiK = 0; uiK < uiK_Max; uiK++)
				{
					
					dYSum += i_cW.Get(uiK) * (i_cY.Get(uiK) - Fit_Function(i_cX.Get(uiK),vA)) * lpfD1(i_cX.Get(uiK),vA);
				}
				vY.Set(uiI,dYSum);
			}
			mN.Invert_LUD();
			double	dS = 0.0;
			for (unsigned int uiK = 0; uiK < uiK_Max; uiK++)
			{
				dS += i_cW.Get(uiK) * pow(i_cY.Get(uiK) - Fit_Function(i_cX.Get(uiK),vA),2.0);
			}
			io_vFit_Parameters = vA;
			o_cCovariance_Matrix = mN;
			o_cCovariance_Matrix *= dS / (uiK_Max - uiI_Max - 1.0);
			o_dSmin = dS;
		}
	}
	return bDone;
}


bool GeneralFit(const XVECTOR &i_cX, const XVECTOR &i_cY,const XVECTOR &i_cW, QFunctionVA Fit_Function, XVECTOR & io_vFit_Parameters, XSQUARE_MATRIX &o_cCovariance_Matrix, double &o_dSmin, void * io_lpvData, unsigned int i_uiMax_Iterations, int i_iConvergence_Criterion, unsigned int *o_lpiIterations, XVECTOR * o_lpvEstimated_Fit_Parameters)
{
	bool			bDone = false;
	if (i_cX.Get_Size() == i_cY.Get_Size() && i_cX.Get_Size() == i_cW.Get_Size())
	{
		XSQUARE_MATRIX	mN;
		XSQUARE_MATRIX	mDel_N;
		XVECTOR			vY;
		XVECTOR			vDel_Y;
		XVECTOR			vDelta_A;
		XVECTOR			vA = io_vFit_Parameters;
		XVECTOR			vF;
		unsigned int	uiI_Max = io_vFit_Parameters.Get_Size();
		unsigned int	uiK_Max = i_cX.Get_Size();
		unsigned int	uiIterations = 0;
		int iDelta_Max = 0;
		int iE_A;
		int iE_dA;
		int iE_oA;
		bool			bNan_Inf = false;

		mN.Set_Size(uiI_Max);
		mDel_N.Set_Size(uiI_Max);
		vY.Set_Size(uiI_Max);
		vDel_Y.Set_Size(uiI_Max);
		vDelta_A.Set_Size(uiI_Max);

		for (unsigned int uiI = 0; uiI < uiI_Max; uiI++)
			vDelta_A.Set(uiI,0.0);
//		FILE * fileOut = fopen("testfile.csv","wt");
		while (!bDone && !bNan_Inf && uiIterations < i_uiMax_Iterations)
		{
			uiIterations++;
			vA += vDelta_A;
//			printf("A\n");
//			vA.Print();
//			printf("%i %i %lu %lu %lu	\n",uiK_Max,uiI_Max,mN.size(),vY.size(),vA.size());
//			fflush(stdout);
			mN.Zero();
//			printf("N.zero\n");
//			fflush(stdout);
			vY.Zero();
//			printf("Y.zero\n");
//			fflush(stdout);
			for (unsigned int uiK = 0; uiK < uiK_Max && !bNan_Inf; uiK++)
			{
//				printf("F(%i)\n",uiK);
//				fflush(stdout);
				vF = Fit_Function(i_cX.Get(uiK),vA,io_lpvData);
				bNan_Inf |= vF.is_nan() || vF.is_inf();
//				if (uiIterations == 1)
//				{
//					fprintf(fileOut,"%.17e, %.17e, %.17e\n",i_cX.Get(uiK),i_cY.Get(uiK),vF.Get(0));
//				}
//				vF.Print();
//				fflush(stdout);
				for (unsigned int uiI = 0; uiI < uiI_Max && !bNan_Inf; uiI++)
				{
					for (unsigned int uiJ = 0; uiJ <= uiI && !bNan_Inf; uiJ++)
					{
						mDel_N.Set(uiI,uiJ,i_cW.Get(uiK) * vF.Get(uiI + 1) * vF.Get(uiJ + 1));
						mDel_N.Set(uiJ,uiI,i_cW.Get(uiK) * vF.Get(uiI + 1) * vF.Get(uiJ + 1));
						bNan_Inf |= mDel_N.is_nan() || mDel_N.is_inf();
					}
					if (!bNan_Inf)
						vDel_Y.Set(uiI,i_cW.Get(uiK) * (i_cY.Get(uiK) - vF.Get(0)) * vF.Get(uiI + 1));
					bNan_Inf |= vDel_Y.is_nan() || vDel_Y.is_inf();
				}
				if (!bNan_Inf)
					vY += vDel_Y;
/*				for (unsigned int uiI = 0; uiI < uiI_Max; uiI++)
				{
					for (unsigned int uiJ = 0; uiJ <= uiI; uiJ++)
					{
						printf("%.2f ",mDel_N.Get(uiI,uiJ));
					}
					printf("\n");
				}*/
//				printf("N\n");
				if (!bNan_Inf)
					mN += mDel_N;
//				printf("N done\n");
//				fflush(stdout);
			}
//			for (unsigned int uiI = 0; uiI < uiI_Max; uiI++)
//			{
//				for (unsigned int uiJ = 0; uiJ <= uiI; uiJ++)
//				{
//					printf("%.2e ",mN.Get(uiI,uiJ));
//				}
//				printf("\n");
//			}
//			printf("LUD\n");
//			fflush(stdout);
			bNan_Inf |= mN.is_nan() || mN.is_inf();
			if (!bNan_Inf)
			{
				mN.Invert_LUD();
//				printf("LUD done\n");
				bNan_Inf = mN.is_nan() || mN.is_inf();
				if (!bNan_Inf)
				{
					vDelta_A = mN * vY;
					bNan_Inf = vDelta_A.is_nan() || vDelta_A.is_inf();
//					printf("del a\n");
//					vDelta_A.Print();
					iDelta_Max = -40;
					for (unsigned int uiI = 0; uiI < uiI_Max && !bNan_Inf; uiI++)
					{
						double	dAi = vA.Get(uiI);
						double	dDeltaAi = vDelta_A.Get(uiI);
						double dNi = mN.Get(uiI,uiI);
						double	doAi = DBL_MAX;
						if (dNi > 0.0 && !std::isnan(dNi) && !std::isinf(dNi))
							doAi = sqrt(dNi);
						frexp(dAi,&iE_A);
						frexp(dDeltaAi,&iE_dA);
						frexp(doAi,&iE_oA);
						if ((iE_dA - iE_A) > iDelta_Max)
							iDelta_Max = iE_dA - iE_A;
						if ((iE_dA - iE_oA) > iDelta_Max)
							iDelta_Max = (iE_dA - iE_oA);
					}
//					printf("conv test done\n");
					if (!bNan_Inf && iDelta_Max < i_iConvergence_Criterion)
					{
						bDone = true;
					}
				}
			}
		}
		if (o_lpiIterations)
			o_lpiIterations[0] = uiIterations;
		if (bDone)
		{
			bDone = false;
			XVECTOR vAsave = vA;
			double dS;
			vA += vDelta_A; // update A
			unsigned int uiCount = 0;
			while (!bDone && uiCount < 2)
			{
				mN.Zero();
				vY.Zero();
				dS = 0.0;
				for (unsigned int uiK = 0; uiK < uiK_Max && !bNan_Inf; uiK++)
				{
	//				printf("F(k) %i\n",uiK);
					vF = Fit_Function(i_cX.Get(uiK),vA,io_lpvData);
					bNan_Inf |= vF.is_nan() || vF.is_inf();
	//				if (uiIterations == 1)
	//				{
	//					fprintf(fileOut,"%.17e, %.17e, %.17e\n",i_cX.Get(uiK),i_cY.Get(uiK),vF.Get(0));
	//				}
	//				vF.Print();
	//				fflush(stdout);
					for (unsigned int uiI = 0; uiI < uiI_Max && !bNan_Inf; uiI++)
					{
						for (unsigned int uiJ = 0; uiJ <= uiI && !bNan_Inf; uiJ++)
						{
							mDel_N.Set(uiI,uiJ,i_cW.Get(uiK) * vF.Get(uiI + 1) * vF.Get(uiJ + 1));
							mDel_N.Set(uiJ,uiI,i_cW.Get(uiK) * vF.Get(uiI + 1) * vF.Get(uiJ + 1));
							bNan_Inf |= mDel_N.is_nan() || mDel_N.is_inf();
						}
						if (!bNan_Inf)
							vDel_Y.Set(uiI,i_cW.Get(uiK) * (i_cY.Get(uiK) - vF.Get(0)) * vF.Get(uiI + 1));
						bNan_Inf |= vDel_Y.is_nan() || vDel_Y.is_inf();
					}
					if (!bNan_Inf)
					{
						vY += vDel_Y;
						mN += mDel_N;

						dS += i_cW.Get(uiK) * pow(i_cY.Get(uiK) - vF.Get(0),2.0);
					}
				}
	//			printf("Inverrt LUD\n");
				if (!bNan_Inf)
				{
					mN.Invert_LUD();
					bNan_Inf = mN.is_nan() || mN.is_inf();
				}
				if (!bNan_Inf)
					bDone = true;
				else
					vA = vAsave;
				uiCount++;
			}
			if (bDone)
			{
				io_vFit_Parameters = vA;
				o_cCovariance_Matrix = mN;
				o_cCovariance_Matrix *= dS / (uiK_Max - uiI_Max - 1.0);
				o_dSmin = dS;
			}
		}
		if (o_lpvEstimated_Fit_Parameters)
			o_lpvEstimated_Fit_Parameters[0] = vA;
//		fclose(fileOut);
	}
//	printf("--------------------------------------------------------------------\n");
	return bDone;
}


double	UsePolynomialFit(const double &i_dX, const XPOLYNOMIAL & o_polyFit, const XSQUARE_MATRIX &o_cCovariance_Matrix, double &o_dError)
{
	double	dY = o_polyFit.Eval(i_dX);
	unsigned int uiDegree = o_polyFit.GetDegree();
	double	dpowX = 1.0;
	o_dError = 0.0;
	for (unsigned int uiI = 0; uiI < uiDegree + 1; uiI++)
	{
		unsigned int uiI2 = uiI / 2;
		for (unsigned int uiJ = 0; uiJ <= uiI2; uiJ++)
		{
			unsigned int uiRow = uiI2 - uiJ;
			unsigned int uiCol = uiI2 + uiJ;
			double dSigma = o_cCovariance_Matrix.Get(uiI2 - uiJ,uiI2 + uiJ);
			double dTerm = dSigma * dpowX;
			if (uiRow != uiCol)
				dTerm *= 2.0;
			o_dError += dTerm;
		}
		dpowX *= i_dX;
	}
	return dY;
}



bool XFIT_Simplex_Epsilon_Test(const XVECTOR * i_lpvParameters, const XVECTOR & i_vEpsilon)
{
	bool bEpsilon = true;
	if (i_lpvParameters)
	{
	// Test to see if all vertices of the simplex are within epsilon of each other
		unsigned int uiNum_Parameters = i_lpvParameters->Get_Size();
		for (unsigned int uiI = 0; uiI < uiNum_Parameters + 1 && bEpsilon; uiI++)
			for (unsigned int uiJ = uiI; uiJ < uiNum_Parameters + 1 && bEpsilon; uiJ++)
				for (unsigned int uiK = uiI; uiK < uiNum_Parameters && bEpsilon; uiK++)
					bEpsilon &= fabs(i_lpvParameters[uiI].Get(uiK) - i_lpvParameters[uiJ].Get(uiK)) < fabs(i_vEpsilon.Get(uiK));
	}
	return bEpsilon;

}

class XFIT_SIMPLEX_MEMORY
{
public:
	double	* m_lpdNew_Chi_Squared_Fits;
	XVECTOR	* m_lpvNew_Parameters;
	XVECTOR *	m_lpvCurrent_Parameters;
	double *	m_lpdChi_Squared_Fits;
//	unsigned int * m_lpuiExclusions;
	unsigned int m_uiNum_Parameters;
	unsigned int m_uiNum_Parameters_Allocated;
private:
	void	Null(void)
	{
		m_lpdNew_Chi_Squared_Fits = NULL;
		m_lpvNew_Parameters = NULL;
		m_lpvCurrent_Parameters = NULL;
		m_lpdChi_Squared_Fits = NULL;
//		m_lpuiExclusions = NULL;
		m_uiNum_Parameters = 0;
		m_uiNum_Parameters_Allocated = 0;
	}
	void	Deallocate(void)
	{
		if (m_lpdNew_Chi_Squared_Fits)
			delete [] m_lpdNew_Chi_Squared_Fits;
		if (m_lpvNew_Parameters)
			delete [] m_lpvNew_Parameters;
		if (m_lpvCurrent_Parameters)
			delete [] m_lpvCurrent_Parameters;
		if (m_lpdChi_Squared_Fits)
			delete [] m_lpdChi_Squared_Fits;
//		if (m_lpuiExclusions)
//			delete [] m_lpuiExclusions;
		Null();
	}
public:
	XFIT_SIMPLEX_MEMORY(void)
	{
		Null();
	}

	~XFIT_SIMPLEX_MEMORY(void)
	{
		Deallocate();
	}

	void	Allocate(unsigned int i_uiNum_Parameters)
	{
		if (i_uiNum_Parameters > m_uiNum_Parameters_Allocated)
		{
			Deallocate();
			m_lpdNew_Chi_Squared_Fits = new double[i_uiNum_Parameters + 1];
			m_lpvNew_Parameters = new XVECTOR[i_uiNum_Parameters + 1];
			m_lpvCurrent_Parameters = new XVECTOR[i_uiNum_Parameters + 1];
			m_lpdChi_Squared_Fits = new double[i_uiNum_Parameters + 1];
//			m_lpuiExclusions = new unsigned int [i_uiNum_Parameters + 1];
			m_uiNum_Parameters_Allocated = i_uiNum_Parameters;
		}
		m_uiNum_Parameters = i_uiNum_Parameters;
		for (unsigned int uiI = 0; uiI <= i_uiNum_Parameters; uiI++) // <= is correct; there are n+1 points
		{
			m_lpvNew_Parameters[uiI].Set_Size(i_uiNum_Parameters);
			m_lpvCurrent_Parameters[uiI].Set_Size(i_uiNum_Parameters);
		}
	}
};

bool XFIT_Simplex_Planar_Test(XVECTOR * io_lpvParameters, const XVECTOR & i_vEpsilon, unsigned int i_uiSuggested_Change = -1)
{
	bool bChanged = false;
	
	if (io_lpvParameters)
	{
		bool bBad_Value = false;
		unsigned int uiNum_Parameters = io_lpvParameters->Get_Size();
		for (unsigned int uiJ = 0; uiJ < uiNum_Parameters + 1 && !bBad_Value; uiJ++)
		{
			bBad_Value = (io_lpvParameters[uiJ].is_nan() || io_lpvParameters[uiJ].is_inf());
		}
		if (!bBad_Value)
		{
		// Test to see if all vertices of the simplex are within epsilon of each other
			for (unsigned int uiK = 0; uiK < uiNum_Parameters; uiK++)
			{
				double	dDelta_Max = 0.0;
				for (unsigned int uiI = 0; uiI < uiNum_Parameters + 1; uiI++)
				{
					for (unsigned int uiJ = uiI + 1; uiJ < uiNum_Parameters + 1; uiJ++)
					{
						double	dDelta = fabs(io_lpvParameters[uiI].Get(uiK) - io_lpvParameters[uiJ].Get(uiK));
						if (dDelta_Max < dDelta)
							dDelta_Max = dDelta;
					}
				}
				double	dMin_Offset = max(min(1.0e-7,i_vEpsilon.Get(uiK)*1e-7),i_vEpsilon.Get(uiK)*1e-14);
				if (dDelta_Max < dMin_Offset)
				{
					unsigned char uiI = 0;
					if (i_uiSuggested_Change != (unsigned int)(-1))
						uiI = i_uiSuggested_Change;
					else if (uiNum_Parameters <= 254)
					{
						// randomly select a vertex to move
						FILE * fileRand = fopen("/dev/random","rb");
						if (fileRand) 
						{
							fread(&uiI,sizeof(unsigned char),1,fileRand);
							fclose(fileRand);
						}
						uiI %= (uiNum_Parameters + 1);
					}
					io_lpvParameters[uiI].Set(uiK,io_lpvParameters[uiI].Get(uiK) + i_vEpsilon.Get(uiK));
					bChanged = true;
				}
			}
		}
	}
	return bChanged;
}
//------------------------------------------------------------------------------
//
// XFIT_Simplex_Planar_Hold
//
//------------------------------------------------------------------------------
//
// Goes though all vertices of the simplex, evaluates if the simplex is planar
// in one or more 
//
//------------------------------------------------------------------------------

void XFIT_Simplex_Planar_Hold(XVECTOR * io_lpvParameters, const XVECTOR & i_vEpsilon, std::vector<bool> & i_vbFixed_Points)
{
	unsigned int uiNum_Parameters = io_lpvParameters->Get_Size();
	if (i_vbFixed_Points.size() != uiNum_Parameters + 1) 
		i_vbFixed_Points.resize(uiNum_Parameters + 1,false);
	std::vector<bool> vbPlanar_Paramter;
	vbPlanar_Paramter.resize(uiNum_Parameters,false);
	bool bReduce = false;
	bool bBad_Value = false;
	for (unsigned int uiJ = 0; uiJ < uiNum_Parameters + 1 && !bBad_Value; uiJ++)
	{
		bBad_Value = (io_lpvParameters[uiJ].is_nan() || io_lpvParameters[uiJ].is_inf());
	}
	if (!bBad_Value)
	{
		// go through list and see if any of the ranges are within epsilon of each other
		for (unsigned int uiK = 0; uiK < uiNum_Parameters; uiK++)
		{
			double	dDelta_Max = 0.0;
			double	dMin_Offset = max(min(1.0e-7,i_vEpsilon.Get(uiK)*1e-7),i_vEpsilon.Get(uiK)*1e-14);
			for (unsigned int uiI = 0; uiI < uiNum_Parameters + 1; uiI++)
			{
				for (unsigned int uiJ = uiI + 1; uiJ < uiNum_Parameters + 1; uiJ++)
				{
					if (uiI != uiJ && !i_vbFixed_Points[uiI] && !i_vbFixed_Points[uiJ])
					{
						double	dDelta = fabs(io_lpvParameters[uiI].Get(uiK) - io_lpvParameters[uiJ].Get(uiK));
						if (dDelta_Max < dDelta)
							dDelta_Max = dDelta;
					}
				}
			}
	//		printf("%i %f %f\n",uiK,dDelta_Max , dMin_Offset);
			vbPlanar_Paramter[uiK] = (dDelta_Max < dMin_Offset);
			bReduce |= (dDelta_Max < dMin_Offset);
		}
		if (bReduce) // there is at least one parameter in which the simplex has reduced to a plane
		{
	//		printf("Reducing\n");
			std::vector<bool> vbHasMinMax;
			vbHasMinMax.resize(uiNum_Parameters + 1,false);
			// go through each parameter and determine what the max and min values are of each parameter other than those that are planar
			for (unsigned int uiK = 0; uiK < uiNum_Parameters; uiK++)
			{
				if (!vbPlanar_Paramter[uiK])
				{
					double dMin = DBL_MAX;
					double dMax = DBL_MIN;
					// first find the min and max
					for (unsigned int uiJ = 0; uiJ < uiNum_Parameters + 1; uiJ++)
					{
						if (io_lpvParameters[uiJ][uiK] < dMin)
							dMin = io_lpvParameters[uiJ][uiK];
						if (io_lpvParameters[uiJ][uiK] > dMax)
							dMax = io_lpvParameters[uiJ][uiK];
					}
					// for each parameter, flag vertices that have a minimum or maximum of any parameter
					for (unsigned int uiJ = 0; uiJ < uiNum_Parameters + 1; uiJ++)
					{
						if (io_lpvParameters[uiJ][uiK] == dMin || io_lpvParameters[uiJ][uiK] == dMax)
							vbHasMinMax[uiJ] = true;
					}
				}
			}
			// for any vertex that has a maximum or minimum in the non-planar parameters, continue adjusting that paramter, otherwise
			// flag it as a static vertex - it is effectively interior to the other vertices
			for (unsigned int uiK = 0; uiK < uiNum_Parameters + 1; uiK++)
			{
				i_vbFixed_Points[uiK] = !vbHasMinMax[uiK];
				if (!vbHasMinMax[uiK])
					printf("%i Fixed\n",uiK);
			}
		}
	}
}
XFIT_SIMPLEX_MEMORY	g_cXfit_Simplex_Memory;

class XFIT_SIMPLEX_BOUNDS
{
public:
	XVECTOR * lpvLower_Bounds;
	std::vector<bool>	* lpvbLower_Bounds_Valid;
	XVECTOR * lpvUpper_Bounds;
	std::vector<bool>	* lpvbUpper_Bounds_Valid;
};

bool XFIT_Simplex_Bounds(const XFIT_SIMPLEX_BOUNDS &i_cBounds, const XVECTOR & i_vCentroid, XVECTOR & io_vX)
{
	bool bChange = false;
	unsigned int uiError = 0;
	unsigned int uiSize = 0;
	if (i_cBounds.lpvLower_Bounds)
		uiSize = i_cBounds.lpvLower_Bounds[0].Get_Size();
	else if (i_cBounds.lpvUpper_Bounds)
		uiSize = i_cBounds.lpvUpper_Bounds[0].Get_Size();

	if (i_cBounds.lpvLower_Bounds && i_cBounds.lpvbLower_Bounds_Valid && i_cBounds.lpvbLower_Bounds_Valid[0].size() != uiSize)
	{
		fprintf(stderr,"XFIT_Simplex_Bounds: lower bound validity vector size does not match lower bound vector size.\n");
		uiError = 1;
	}
	if (uiError == 0 && i_cBounds.lpvUpper_Bounds && i_cBounds.lpvbUpper_Bounds_Valid && i_cBounds.lpvUpper_Bounds[0].Get_Size() != uiSize)
	{
		fprintf(stderr,"XFIT_Simplex_Bounds: upper bound vector size does not match lower bound vector size.\n");
		uiError = 2;
	}
	if (uiError == 0 && i_cBounds.lpvUpper_Bounds && i_cBounds.lpvbUpper_Bounds_Valid && i_cBounds.lpvbUpper_Bounds_Valid[0].size() != uiSize)
	{
		fprintf(stderr,"XFIT_Simplex_Bounds: upper bound validity vector size does not match upper bound vector size.\n");
		uiError = 3;
	}
	if (uiError == 0 && i_vCentroid.Get_Size() != uiSize)
	{
		fprintf(stderr,"XFIT_Simplex_Bounds: centroid vector size does not match bound vector size.\n");
		uiError = 4;
	}
	if (uiError == 0 && io_vX.Get_Size() != uiSize)
	{
		fprintf(stderr,"XFIT_Simplex_Bounds: X vector size does not match bound vector size.\n");
		uiError = 5;
	}
	bool bBad_Value = false;
	for (unsigned int uiJ = 0; uiJ < uiSize && !bBad_Value; uiJ++)
	{
		bBad_Value = (i_cBounds.lpvbLower_Bounds_Valid[0][uiJ] && (std::isnan(i_cBounds.lpvLower_Bounds[0][uiJ]) || std::isinf(i_cBounds.lpvLower_Bounds[0][uiJ])));
	}
	if (bBad_Value)
	{
		fprintf(stderr,"XFIT_Simplex_Bounds: Lower bound constains invalid value (nan or inf).\n");
		uiError = 6;
	}
	bBad_Value = false;
	for (unsigned int uiJ = 0; uiJ < uiSize && !bBad_Value; uiJ++)
	{
		bBad_Value = (i_cBounds.lpvbUpper_Bounds_Valid[0][uiJ] && (std::isnan(i_cBounds.lpvUpper_Bounds[0][uiJ]) || std::isinf(i_cBounds.lpvUpper_Bounds[0][uiJ])));
	}
	if (bBad_Value)
	{
		fprintf(stderr,"XFIT_Simplex_Bounds: Upper bound constains invalid value (nan or inf).\n");
		uiError = 7;
	}

	if (uiError == 0)
	{
		XVECTOR	vDelta = io_vX - i_vCentroid;
		XVECTOR	vLB_Delta;
		XVECTOR	vUB_Delta;
		double	dScalar = 1.0;

		if (i_cBounds.lpvLower_Bounds && i_cBounds.lpvbLower_Bounds_Valid)
			vLB_Delta = i_cBounds.lpvLower_Bounds[0] - i_vCentroid;
		if (i_cBounds.lpvUpper_Bounds && i_cBounds.lpvbUpper_Bounds_Valid)
			vUB_Delta = i_cBounds.lpvUpper_Bounds[0] - i_vCentroid;

		for (unsigned int uiI = 0; uiI < uiSize; uiI++)
		{
			if (i_cBounds.lpvLower_Bounds && i_cBounds.lpvbLower_Bounds_Valid && i_cBounds.lpvbLower_Bounds_Valid[0].at(uiI) && io_vX.Get(uiI) < i_cBounds.lpvLower_Bounds[0].Get(uiI))
			{
				double dTest = vLB_Delta.Get(uiI) / vDelta.Get(uiI);
				if (dTest < dScalar)
					dScalar = dTest;
			}
			if (i_cBounds.lpvUpper_Bounds && i_cBounds.lpvbUpper_Bounds_Valid && i_cBounds.lpvbUpper_Bounds_Valid[0].at(uiI) && io_vX.Get(uiI) > i_cBounds.lpvUpper_Bounds[0].Get(uiI))
			{
				double dTest = vUB_Delta.Get(uiI) / vDelta.Get(uiI);
				if (dTest < dScalar)
					dScalar = dTest;
			}
		}
		if (dScalar < 1.0 && !std::isnan(dScalar) && !std::isinf(dScalar))
		{
			vDelta *= dScalar;
			io_vX = i_vCentroid + vDelta;
			bChange = true;
		}
	}
	return bChange;
}


bool XFIT_Simplex_Roll(XVECTOR * io_lpvCurrent_Parameters, unsigned int i_uiNum_Parameters, double * io_lpdChi_Squared_Fits, const XVECTOR &i_vEpsilon, QFunctionV i_lpfvChi_Squared_Function, void * i_lpvData, const XFIT_SIMPLEX_BOUNDS & i_cBounds, bool i_bQuiet)
{
	if (!i_bQuiet)
		printf("Rolling\n");
	XVECTOR vCentroid(io_lpvCurrent_Parameters[0]);
	XVECTOR	vDelta;
	bool bImproved = false;
//	unsigned int uiLast_Idx = 0;
	unsigned int uiLast_Improved_Count = 0;
	unsigned int uiChange_Idx = 0;
	std::vector<bool> vbFixed_Points;
	bool	bAll_Fixed = false;
	unsigned int uiError = 0;
	while (!bAll_Fixed && uiLast_Improved_Count < (i_uiNum_Parameters + 1))
	{
		uiLast_Improved_Count++;
		XFIT_Simplex_Planar_Hold(io_lpvCurrent_Parameters,i_vEpsilon,vbFixed_Points);
		if (!vbFixed_Points[uiChange_Idx])
		{
			// generate vector describing the centroid of remaining points
			vCentroid.Zero();
			unsigned int uiCentroid_Count = 0;
			for (unsigned int uiJ = 0; uiJ < i_uiNum_Parameters + 1; uiJ++)
			{
				if (uiChange_Idx != uiJ && !vbFixed_Points[uiJ])
				{
					uiCentroid_Count++;
					vCentroid += io_lpvCurrent_Parameters[uiJ];
				}
			}
			if (uiCentroid_Count != 0) // if 0, the point being processed is the last remaining point that is not fixed
			{
				vCentroid /= (double)uiCentroid_Count;
				if (vCentroid.is_nan() || vCentroid.is_inf())
				{
					uiError = 1;
				}
				if (uiError == 0)
				{
					vDelta = io_lpvCurrent_Parameters[uiChange_Idx] - vCentroid;
					g_cXfit_Simplex_Memory.m_lpvNew_Parameters[uiChange_Idx] = vCentroid - vDelta;
					if (XFIT_Simplex_Bounds(i_cBounds, vCentroid, g_cXfit_Simplex_Memory.m_lpvNew_Parameters[uiChange_Idx])) // make sure test point is not out of bounds
					{
						XFIT_Simplex_Planar_Hold(g_cXfit_Simplex_Memory.m_lpvNew_Parameters,i_vEpsilon,vbFixed_Points);
					}
				}

				if (!vbFixed_Points[uiChange_Idx])
				{
					// Find chi^2 at test point
					if (!i_bQuiet)
					{
						printf("Trying %i:",uiChange_Idx);
						for (unsigned int uiJ = 0; uiJ < g_cXfit_Simplex_Memory.m_lpvNew_Parameters[uiChange_Idx].Get_Size(); uiJ++)
							printf("\t%f",g_cXfit_Simplex_Memory.m_lpvNew_Parameters[uiChange_Idx].Get(uiJ));
						fflush(stdout);
					}
					double dFit = i_lpfvChi_Squared_Function(g_cXfit_Simplex_Memory.m_lpvNew_Parameters[uiChange_Idx], i_lpvData);
					if (!i_bQuiet)
						printf("\t%e",dFit);

					if (!std::isnan(dFit) && dFit < io_lpdChi_Squared_Fits[uiChange_Idx])
					{
						bImproved = true;
						uiLast_Improved_Count = 0;
						io_lpdChi_Squared_Fits[uiChange_Idx] = dFit;
						io_lpvCurrent_Parameters[uiChange_Idx] = g_cXfit_Simplex_Memory.m_lpvNew_Parameters[uiChange_Idx];
						if (!i_bQuiet)
							printf("\tI");
					}
					if (!i_bQuiet)
						printf("\n");
				}
			}
		}
		uiChange_Idx++;
		if (uiChange_Idx >= i_uiNum_Parameters + 1)
			uiChange_Idx %= i_uiNum_Parameters + 1;
		unsigned int uiFixed_Count = 0;
		for (unsigned int uiI = 0; uiI < vbFixed_Points.size(); uiI++)
			if (vbFixed_Points[uiI])
				uiFixed_Count++;
		bAll_Fixed = (uiFixed_Count >= (vbFixed_Points.size() - 2)); // if only one point is  not fixed, we won't be able to compute a centroid
	}
	return bImproved;
}
// This funciton is similar to fit_scale, but will only change any vertex which shows an improvement after scaling
bool XFIT_Simplex_Scale_Individual(XVECTOR * io_lpvCurrent_Parameters, unsigned int i_uiNum_Parameters, double * io_lpdChi_Squared_Fits, const XVECTOR &i_vEpsilon, QFunctionV i_lpfvChi_Squared_Function, void * i_lpvData, const double & i_dScaling_Factor, const XFIT_SIMPLEX_BOUNDS & i_cBounds, bool i_bQuiet)
{
	if (!i_bQuiet)
		printf("Scaling (individual) %f\n",i_dScaling_Factor);
	bool bImproved = false;
	XVECTOR vCentroid(io_lpvCurrent_Parameters[0]);
	XVECTOR	vDelta;
	XVECTOR vTest;
	double	dFit;

	for (unsigned int uiJ = 0; uiJ < i_uiNum_Parameters; uiJ++)
		vCentroid.Set(uiJ,0.0);
	for (unsigned int uiJ = 0; uiJ < i_uiNum_Parameters + 1; uiJ++)
	{
		vCentroid += io_lpvCurrent_Parameters[uiJ];
	}
	vCentroid /= (double)(i_uiNum_Parameters + 1);

	for (unsigned int uiI = 0; uiI < i_uiNum_Parameters + 1; uiI++)
	{
		vDelta = io_lpvCurrent_Parameters[uiI] - vCentroid;
		g_cXfit_Simplex_Memory.m_lpvNew_Parameters[uiI] = vCentroid + vDelta * i_dScaling_Factor;
		if (XFIT_Simplex_Bounds(i_cBounds, vCentroid, g_cXfit_Simplex_Memory.m_lpvNew_Parameters[uiI])) // make sure test point is not out of bounds
			if (XFIT_Simplex_Planar_Test(g_cXfit_Simplex_Memory.m_lpvNew_Parameters,i_vEpsilon, uiI)) // make sure we didn't collapse the simplex
			{
				for (unsigned int uiI = 0; uiI < i_uiNum_Parameters + 1; uiI++)
				{
					if (!i_bQuiet)
					{
						printf("Planar %i:",uiI);
						for (unsigned int uiJ = 0; uiJ < g_cXfit_Simplex_Memory.m_lpvNew_Parameters[uiI].Get_Size(); uiJ++)
							printf("\t%f",g_cXfit_Simplex_Memory.m_lpvNew_Parameters[uiI].Get(uiJ));
						fflush(stdout);
					}
					io_lpdChi_Squared_Fits[uiI] = i_lpfvChi_Squared_Function(g_cXfit_Simplex_Memory.m_lpvNew_Parameters[uiI], i_lpvData);
					if (!i_bQuiet)
					{
						printf("\t%e",io_lpdChi_Squared_Fits[uiI]);
						printf("\n");
					}
				}
			}
		if (!i_bQuiet)
		{
			printf("Trying %i:",uiI);
			for (unsigned int uiJ = 0; uiJ < g_cXfit_Simplex_Memory.m_lpvNew_Parameters[uiI].Get_Size(); uiJ++)
				printf("\t%f",g_cXfit_Simplex_Memory.m_lpvNew_Parameters[uiI].Get(uiJ));
			fflush(stdout);
		}
		dFit = i_lpfvChi_Squared_Function(g_cXfit_Simplex_Memory.m_lpvNew_Parameters[uiI], i_lpvData);
		if (!i_bQuiet)
			printf("\t%e",dFit);
		if (!std::isnan(dFit) && dFit < io_lpdChi_Squared_Fits[uiI])
		{
			io_lpvCurrent_Parameters[uiI] = g_cXfit_Simplex_Memory.m_lpvNew_Parameters[uiI];
			io_lpdChi_Squared_Fits[uiI] = dFit;
			bImproved |= true;
			if (!i_bQuiet)
				printf("\tI");
		}
		if (!i_bQuiet)
			printf("\n");
	}

	return bImproved;
}

bool XFIT_Simplex_Scale(XVECTOR * io_lpvCurrent_Parameters, unsigned int i_uiNum_Parameters, double * io_lpdChi_Squared_Fits, const XVECTOR &i_vEpsilon, QFunctionV i_lpfvChi_Squared_Function, void * i_lpvData, const double & i_dScaling_Factor, bool i_bForce, const XFIT_SIMPLEX_BOUNDS & i_cBounds, bool i_bQuiet)
{
	if (!i_bQuiet)
		printf("Scaling %f\n",i_dScaling_Factor);
	bool bImproved = false;
	double	* lpdNew_Chi_Squared_Fits = g_cXfit_Simplex_Memory.m_lpdNew_Chi_Squared_Fits;
	XVECTOR * lpvNew_Parameters  = g_cXfit_Simplex_Memory.m_lpvNew_Parameters;
	XVECTOR vCentroid(io_lpvCurrent_Parameters[0]);
	XVECTOR	vDelta;

	for (unsigned int uiJ = 0; uiJ < i_uiNum_Parameters; uiJ++)
		vCentroid.Set(uiJ,0.0);
	for (unsigned int uiJ = 0; uiJ < i_uiNum_Parameters + 1; uiJ++)
	{
		vCentroid += io_lpvCurrent_Parameters[uiJ];
	}
	vCentroid /= (double)(i_uiNum_Parameters + 1);
	if (vCentroid.is_nan() || vCentroid.is_inf())
	{
		fprintf(stderr,"XFIT_Simplex_Scale: centroid is invalid.\n");
		fflush(stderr);
	}
	else
	{
		for (unsigned int uiI = 0; uiI < i_uiNum_Parameters + 1; uiI++)
		{
			vDelta = io_lpvCurrent_Parameters[uiI] - vCentroid;
			lpvNew_Parameters[uiI] = vCentroid + vDelta * i_dScaling_Factor;
			if (XFIT_Simplex_Bounds(i_cBounds, vCentroid, lpvNew_Parameters[uiI])) // make sure test point is not out of bounds
			{
				if (XFIT_Simplex_Planar_Test(lpvNew_Parameters,i_vEpsilon, uiI)) // make sure we didn't collapse the simplex
				{
					for (unsigned int uiI = 0; uiI < i_uiNum_Parameters + 1; uiI++)
					{
						if (!i_bQuiet)
						{
							printf("Planar %i:",uiI);
							for (unsigned int uiJ = 0; uiJ < lpvNew_Parameters[uiI].Get_Size(); uiJ++)
								printf("\t%f",lpvNew_Parameters[uiI].Get(uiJ));
							fflush(stdout);
						}
						lpdNew_Chi_Squared_Fits[uiI] = i_lpfvChi_Squared_Function(lpvNew_Parameters[uiI], i_lpvData);
						if (!i_bQuiet)
						{
							printf("\t%e",io_lpdChi_Squared_Fits[uiI]);
							printf("\n");
						}
					}
				}
			}
			if (!i_bQuiet)
			{
				printf("Trying %i:",uiI);
				for (unsigned int uiJ = 0; uiJ < lpvNew_Parameters[uiI].Get_Size(); uiJ++)
					printf("\t%f",lpvNew_Parameters[uiI].Get(uiJ));
				fflush(stdout);
			}
			lpdNew_Chi_Squared_Fits[uiI] = i_lpfvChi_Squared_Function(lpvNew_Parameters[uiI], i_lpvData);
			if (!i_bQuiet)
				printf("\t%e",lpdNew_Chi_Squared_Fits[uiI]);
			bImproved |= (!std::isnan(lpdNew_Chi_Squared_Fits[uiI]) && lpdNew_Chi_Squared_Fits[uiI] < io_lpdChi_Squared_Fits[uiI]);
			if (!i_bQuiet)
			{
				if (!std::isnan(lpdNew_Chi_Squared_Fits[uiI]) && lpdNew_Chi_Squared_Fits[uiI] < io_lpdChi_Squared_Fits[uiI])
					printf("\tI");
				printf("\n");
			}
		}
		if (bImproved || i_bForce)
		{
			for (unsigned int uiI = 0; uiI < i_uiNum_Parameters + 1; uiI++)
			{
				if (!std::isnan(lpdNew_Chi_Squared_Fits[uiI]))
				{
					io_lpvCurrent_Parameters[uiI] = lpvNew_Parameters[uiI];
					io_lpdChi_Squared_Fits[uiI] = lpdNew_Chi_Squared_Fits[uiI];
				}
			}
		}
	}
	return bImproved;
}


void XFIT_Simplex(XVECTOR & io_vParameters, const XVECTOR & i_vParameters_Range_Start, const XVECTOR & i_vEpsilon, QFunctionV i_lpfvChi_Squared_Function, void * i_lpvData, bool i_bQuiet, XVECTOR * i_lpvLower_Bounds, std::vector<bool> * i_lpvbLower_Bounds_Valid, XVECTOR * i_lpvUpper_Bounds, std::vector<bool> * i_lpvbUpper_Bounds_Valid)
{
		
	unsigned int uiNum_Parameters = io_vParameters.Get_Size();
	XFIT_SIMPLEX_BOUNDS cBounds;

	cBounds.lpvLower_Bounds = i_lpvLower_Bounds;
	cBounds.lpvbLower_Bounds_Valid = i_lpvbLower_Bounds_Valid;
	cBounds.lpvUpper_Bounds = i_lpvUpper_Bounds;
	cBounds.lpvbUpper_Bounds_Valid = i_lpvbUpper_Bounds_Valid;

	g_cXfit_Simplex_Memory.Allocate(uiNum_Parameters); // prepare memory allocation

	XVECTOR * lpvCurrent_Parameters = g_cXfit_Simplex_Memory.m_lpvCurrent_Parameters;
	double * lpdChi_Squared_Fits = g_cXfit_Simplex_Memory.m_lpdChi_Squared_Fits;
	XVECTOR vCentroid(uiNum_Parameters);
	XVECTOR vDelta(uiNum_Parameters);
//	unsigned int * lpuiExclusions = g_cXfit_Simplex_Memory.m_lpuiExclusions;
//	unsigned int uiExclusion_Count = 0;
//	for (unsigned int uiI = 0; uiI < uiNum_Parameters; uiI++)
//		lpuiExclusions[uiI] = uiNum_Parameters + 2;
	// Generate starting simplex:
	// X_ij = <x0_i + xr_i delta_ij> (i,j<dof)
	// X_i(dof+1) = <x0_i + 0.5 xr_i>
	// where x0 = io_vParameters, xr = i_vParameters_Range_Start, i is the vector index, and j is the vertex index
	// This generates a simplex which spans the suggested range and has the suggested starting point internal ot the simplex
	for (unsigned int uiI = 0; uiI < uiNum_Parameters; uiI++)
	{
		lpvCurrent_Parameters[uiI] = io_vParameters;
		lpvCurrent_Parameters[uiI].Set(uiI,io_vParameters.Get(uiI) + i_vParameters_Range_Start.Get(uiI));
		if (!i_bQuiet)
		{
			printf("Starting %i:",uiI);
			for (unsigned int uiJ = 0; uiJ < lpvCurrent_Parameters[uiI].Get_Size(); uiJ++)
				printf("\t%f",lpvCurrent_Parameters[uiI].Get(uiJ));
			fflush(stdout);
		}
		lpdChi_Squared_Fits[uiI] = i_lpfvChi_Squared_Function(lpvCurrent_Parameters[uiI], i_lpvData);
		printf("\t%.2e",lpdChi_Squared_Fits[uiI]);
		printf("\n");
	}
	lpvCurrent_Parameters[uiNum_Parameters].Set_Size(uiNum_Parameters);
	for (unsigned int uiI = 0; uiI < uiNum_Parameters; uiI++)
		lpvCurrent_Parameters[uiNum_Parameters].Set(uiI,io_vParameters.Get(uiI) - 0.5 * i_vParameters_Range_Start.Get(uiI));
	if (!i_bQuiet)
	{
		printf("Starting %i:",uiNum_Parameters);
		for (unsigned int uiI = 0; uiI < lpvCurrent_Parameters[uiNum_Parameters].Get_Size(); uiI++)
			printf("\t%f",lpvCurrent_Parameters[uiNum_Parameters].Get(uiI));
		fflush(stdout);
	}
	lpdChi_Squared_Fits[uiNum_Parameters] = i_lpfvChi_Squared_Function(lpvCurrent_Parameters[uiNum_Parameters], i_lpvData);
	if (!i_bQuiet)
	{
		printf("\t%.2e",lpdChi_Squared_Fits[uiNum_Parameters]);
		printf("\n");
	}
	// Initialize fit data
	if (!i_bQuiet)
		printf("Starting Fit\n");
	// fitting routine
//	unsigned int uiLast_Worst_Point = uiNum_Parameters + 2;
//	unsigned int uiLast_Worst_Point_Counter = 0;
	// "roll" the simplex around until no reflections cause a improved fit

	do 
	{
		XFIT_Simplex_Roll(lpvCurrent_Parameters, uiNum_Parameters, lpdChi_Squared_Fits, i_vEpsilon, i_lpfvChi_Squared_Function, i_lpvData, cBounds, i_bQuiet);
		XFIT_Simplex_Scale(lpvCurrent_Parameters, uiNum_Parameters, lpdChi_Squared_Fits, i_vEpsilon, i_lpfvChi_Squared_Function, i_lpvData,0.5,true, cBounds,i_bQuiet);
	} while (!XFIT_Simplex_Epsilon_Test(lpvCurrent_Parameters, i_vEpsilon));

/*	unsigned int uiRoll_Fail = 0;
	unsigned int uiDouble_Count = 0;
	do
	{
		bool bDouble = XFIT_Simplex_Scale_Individual(lpvCurrent_Parameters, uiNum_Parameters, lpdChi_Squared_Fits, i_vEpsilon, i_lpfvChi_Squared_Function, i_lpvData,2.0, cBounds,i_bQuiet);
		if (bDouble)
			uiDouble_Count++;
		else
			uiDouble_Count = 0;
		if (!bDouble)
			XFIT_Simplex_Scale(lpvCurrent_Parameters, uiNum_Parameters, lpdChi_Squared_Fits, i_vEpsilon, i_lpfvChi_Squared_Function, i_lpvData,0.5,true, cBounds,i_bQuiet);
		if (!XFIT_Simplex_Roll(lpvCurrent_Parameters, uiNum_Parameters, lpdChi_Squared_Fits, i_vEpsilon, i_lpfvChi_Squared_Function, i_lpvData, cBounds, i_bQuiet))
			uiRoll_Fail++;
		else
			uiRoll_Fail = 0;
		if (uiRoll_Fail >= 2)
		{
			double dScalar = 0.5;
			if (bDouble)
				dScalar = 0.5 / uiDouble_Count;

			XFIT_Simplex_Scale(lpvCurrent_Parameters, uiNum_Parameters, lpdChi_Squared_Fits, i_vEpsilon, i_lpfvChi_Squared_Function, i_lpvData,dScalar,true, cBounds, i_bQuiet);
			uiRoll_Fail = 0;
		}
	} while (!XFIT_Simplex_Epsilon_Test(lpvCurrent_Parameters, i_vEpsilon));*/
	// do one more
	XFIT_Simplex_Scale(lpvCurrent_Parameters, uiNum_Parameters, lpdChi_Squared_Fits, i_vEpsilon, i_lpfvChi_Squared_Function, i_lpvData, 0.5, true, cBounds, i_bQuiet);
	XFIT_Simplex_Roll(lpvCurrent_Parameters, uiNum_Parameters, lpdChi_Squared_Fits, i_vEpsilon, i_lpfvChi_Squared_Function, i_lpvData, cBounds, i_bQuiet);

		
	for (unsigned int uiI = 0; uiI < uiNum_Parameters; uiI++)
		vCentroid.Set(uiI,0.0);
	// generate vector describing the centroid of remaining points
	for (unsigned int uiI = 0; uiI < uiNum_Parameters + 1; uiI++)
	{
		vCentroid += lpvCurrent_Parameters[uiI];
	}
	vCentroid /= (double)(uiNum_Parameters + 1);
	double	dBest_Vertex_Fit = i_lpfvChi_Squared_Function(vCentroid, i_lpvData);
	io_vParameters = vCentroid;
	for (unsigned int uiI = 0; uiI < uiNum_Parameters; uiI++)
	{
		if (dBest_Vertex_Fit > lpdChi_Squared_Fits[uiI])
		{
			dBest_Vertex_Fit = lpdChi_Squared_Fits[uiI];
			io_vParameters = lpvCurrent_Parameters[uiI];
		}
	}
}

