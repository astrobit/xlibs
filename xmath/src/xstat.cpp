#include <xmath.h>
#include <xlinalg.h>
#include <xstat.h>

void XMATH_Compute_Stats(const xvector & i_vData, double & o_dMean, double & o_dVariance)
{
	o_dMean = 0.0;
	o_dVariance = 0.0;
	if (i_vData.Get_Size() > 0)
	{
		double	dX;
		double	dSums[2] =  {0,0};
		unsigned int uiI;
		unsigned int uiNum_Items = i_vData.Get_Size();
		for (uiI = 0; uiI < uiNum_Items; uiI++)
		{
			dX = i_vData.Get(uiI);
			dSums[0] += dX;
			dSums[1] += dX * dX;
		}
		o_dMean = dSums[0] / uiNum_Items;
	}
}

//----------------------------------------------------------------------------
// LombScarglePower: This function computes the power in a particular
// frequency using the Lomb-Scargle method.  Inputs are the frequency,
// the time data (X), and the amplitude data (Y).  The A and B parameters
// of the power are optionally output.  Returned value is the
// power in the data set at the user specified frequency.
//----------------------------------------------------------------------------
double	XMATH_LombScarglePower(const double &i_dFrequency, const xvector &i_vX, 
			const xvector &i_vY, xvector *o_vA, xvector *o_vOa)
{
	unsigned int uiI;
	unsigned int uiNum_Data = i_vX.Get_Size();
	double	dSums[7] = {0,0,0,0,0,0,0};
	double	dTau;
	double	dAng_Freq = i_dFrequency * 2.0 * g_cConstants.dPi;
	double	dTwo_Freq = 2.0 * dAng_Freq;
	double	dPls = -1.0;
	double	dF,dY,dSin,dCos;

	if (uiNum_Data > 0 && i_vY.Get_Size()  == uiNum_Data)
	{
		if (o_vA)
			o_vA->Set_Size(2);
		if (o_vOa)
			o_vOa->Set_Size(2);
		// calculate the mean <y> and sin (2 w t),cos(2 w t)
		for (uiI = 0; uiI < uiNum_Data; uiI++)
		{
			double	dF = dTwo_Freq * i_vX.Get(uiI);
			dSums[0] += i_vY.Get(uiI);
			dSums[1] += sin(dF);
			dSums[2] += cos(dF);
		}
		double dMeanY = dSums[0] / uiNum_Data;
		// find tau from tan (2 w tau) = (sum: sin (2wt) / sum: cos(2wt))
		dTau = atan(dSums[1] / dSums[2]) / dTwo_Freq;
		// compute sums of y sin (wx), y cos(wx), sin^2(wx), cos^2(wx)
		// where x = t - tau
		for (uiI = 0; uiI < uiNum_Data; uiI++)
		{
			dF = dAng_Freq * (i_vX.Get(uiI) - dTau);
			dSin = sin(dF);
			dCos = cos(dF);
			dY = i_vY.Get(uiI) - dMeanY;

			dSums[3] += dY * dSin;
			dSums[4] += dSin * dSin;
			dSums[5] += dY * dCos;
			dSums[6] += dCos * dCos;
		}
		// compute parameters A,B
		double dOa = 1.0 / dSums[4];
		double dA = dSums[3] * dOa;
		double dOb = 1.0 / dSums[6];
		double dB = dSums[5] * dOb;
		// compute the power
		dPls = 0.5 * (dSums[3] * dSums[3] * dOa + dSums[5] * dSums[5] * dOb);
		// output A,B, variance of A,B
		if (o_vA)
		{
			o_vA->Set(0,dA);
			o_vA->Set(1,dB);
		}
		if (o_vOa)
		{
			o_vOa->Set(0,dOa);
			o_vOa->Set(1,dOb);
		}
	}
	return dPls;
}
