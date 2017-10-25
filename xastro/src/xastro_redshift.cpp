#include <xastro.h>

double XA_Rest_Frame_Wavelength(const double & i_dObserved_Wavelength, const double & i_dRedshift)
{
	return i_dObserved_Wavelength / (1.0 + i_dRedshift);
}

double XA_Observed_Frame_Wavelength(const double & i_dRest_Frame_Wavelength, const double & i_dRedshift)
{
	return i_dRest_Frame_Wavelength * (1.0 + i_dRedshift);
}

