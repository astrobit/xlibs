#include <xmath.h>
#include <xlinalg.h>

// Statistics functions
void XMATH_Compute_Stats(const XVECTOR & i_vData, double & o_dMean, double & o_dVariance);

// Lomb-Scargle power: similar to a Fourier transform, but not limited to fixed sampling frequency
// can return Fourier parameters if requested (o_vA and o_vOa parameters are the A,B and variance of A,B, respectively)
double	XMATH_LombScarglePower(const double &i_dFrequency, const XVECTOR &i_vX, const XVECTOR &i_vY, XVECTOR *o_vA = NULL, XVECTOR *o_vOa = NULL);
