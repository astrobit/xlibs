#include <xastro.h>

double XA_Rest_Frame_Wavelength(const double & i_dObserved_Wavelength, const double & i_dRedshift)
{
	return i_dObserved_Wavelength / (1.0 + i_dRedshift);
}

double XA_Observed_Frame_Wavelength(const double & i_dRest_Frame_Wavelength, const double & i_dRedshift)
{
	return i_dRest_Frame_Wavelength * (1.0 + i_dRedshift);
}

double	XA_Compute_Velocity(const double & i_dObserved_Wavelength, const double & i_dRest_Wavelength)
{
	double	dz = i_dObserved_Wavelength / i_dRest_Wavelength;
	double	dz_sqr = dz * dz;
	return (g_XASTRO.k_dc * (dz_sqr - 1.0) / (dz_sqr + 1.0));
}

