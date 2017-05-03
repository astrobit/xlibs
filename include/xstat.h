#include <xmath.h>
#include <xlinalg.h>

// Statistics functions
void XMATH_Compute_Stats(const xvector & i_vData, double & o_dMean, double & o_dVariance);

// Lomb-Scargle power: similar to a Fourier transform, but not limited to fixed sampling frequency
// can return Fourier parameters if requested (o_vA and o_vOa parameters are the A,B and variance of A,B, respectively)
double	XMATH_LombScarglePower(const double &i_dFrequency, const xvector &i_vX, const xvector &i_vY, xvector *o_vA = NULL, xvector *o_vOa = NULL);
