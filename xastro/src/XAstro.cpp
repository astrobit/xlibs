#include <xastro.h>
#include <math.h>


XASTRO_CONSTS::XASTRO_CONSTS(void)
{
//	k_dAU_cm = 1.5e13; // cm
//	k_dpc_cm = 3.086e18; // 1 pc in cm
//	k_dMpc_cm = 3.086e24; // 1 Mpc in cm
//	k_dGpc_cm = 3.086e27; // 1 Gpc in cm

	// fundamental constants
	k_dc = 2.99792458e10; // cgs
	k_dG = 6.67384e-8;//cgs
	k_de = 4.80320451e-10;
	k_dme = 9.10938291e-28;//g
	k_dmp = 1.672621777e-24;//g
	k_dpi = acos(-1.0);
	k_dexp = 2.718281828459045235;
	k_dEuler = 0.577215664901532861;
	k_dh = 6.62606957e-27; // erg s
	k_dAvogadro = 6.02214129e+23;
	k_dH0std = 100.e5 / (k_dMpc_cm); // s^{-1} h
	k_dH0h = 0.726; // s^{-1} h // WMAP 7
	k_dKb = 1.3806488e-16; // erg/K
	k_dAMU_gm = 1.660538921e-24; // AMU in gm

	k_dYear = 31556925.9747;//86400 * 365.25;// Tropical Year 1900 (Allen's)  
	k_dMsun = 1.98892e+33;//g
	k_dLsun = 3.8515e+33;//erg/s
	k_dRsun = 6.955e+10; // cm
	k_dAU_cm = 1.49597870662e+13;

	// derived constants
	k_dpc_cm = k_dAU_cm * 648000 / k_dpi;//3.0856775807e+18; // 1 pc in cm
	k_dMpc_cm = k_dpc_cm * 1e6;//3.0856775807e+24; // 1 Mpc in cm
	k_dGpc_cm = k_dpc_cm * 1e12;//3.0856775807e+27; // 1 Gpc in cm

	k_derg_eV = 1.0e8*k_de/k_dc;//1.602192e-11;//1. / k_deV_erg;
	k_deV_erg = 1.0 / k_derg_eV;//6.2415e+11; // eV / erg

	k_dhc = k_dh * k_dc;
	k_da = 8.0 * k_dpi * k_dpi * k_dpi * k_dpi * k_dpi * k_dKb * k_dKb * k_dKb * k_dKb / (15.0 * k_dhc * k_dhc * k_dhc); //7.5657e-15; // erg cm^{-3} K^{-4}
	k_dSigma_SB = k_da * k_dc * 0.25;//5.67051e-5;//cgs
	k_dWein = k_dhc / k_dKb / 4.96511423e-7;//0.2897756e+7; // nm
	k_dKb_eV = k_dKb * k_deV_erg;//8.6173324e-5; // eV/K 
	k_dhbar = k_dh / (2.0 * k_dpi); // erg s //@@
	k_dh_eV = k_dh * k_deV_erg;// 4.135667516e-15; // eV s 
	k_dhbar_eV = k_dhbar * k_deV_erg;//6.58211928e-16; // eV s
	k_dhc_eVnm = k_dh_eV * k_dc * 1e7;//eV nm
	k_dH0 = k_dH0std * k_dH0h;
	k_dAlpha = 2.0 * k_dpi * k_de * k_de / k_dhc;
	k_da0 = k_dh * k_dh / (4.0 * k_dpi * k_dpi * k_dme * k_de * k_de); // = 5.2917721092e−9; // cm
	k_dRy = (2.0 * k_dpi * k_dpi * k_dme * k_de * k_de * k_de * k_de) / (k_dh * k_dh);
	k_dRy_eV = k_dRy * k_deV_erg;
	k_dmh = k_dme + k_dmp - k_dRy / (k_dc * k_dc);
}
void	XASTRO_CONSTS::Use_FLASH_Constants(void)
{
	double	dPi = acos(-1.0);
	// flash 4.0 alpha
	k_dAMU_gm = 1.660538782e-24;
	k_dKb = 1.3806504e-16;
	k_dG = 6.67428e-8;
	k_dpc_cm = 3.0856775807e+18; // 1 pc in cm
	k_dMpc_cm = 3.0856775807e+24; // 1 Mpc in cm
	k_dGpc_cm = 3.0856775807e+27; // 1 Gpc in cm
	k_dRsun = 6.96e+10;
	k_dAU_cm = 1.49597870662e+13;
	k_dMsun = 1.9889225e+33;
	k_dc = 2.99792458e+10; 
	k_dh = 6.62606896e-27;
	k_dhbar = k_dh / (2.0 * dPi);
	k_dh_eV = k_dh * k_deV_erg;
	k_dhbar_eV = k_dhbar * k_deV_erg;
	k_de = 4.80320427e-10;
	k_dme = 9.10938215e-28;
	k_dmp = 1.672621637e-24;
	k_dSigma_SB = 5.670400e-5;
	k_dWein = 0.28977685;
	k_da = 4.0 * k_dSigma_SB / k_dc;
	k_dAvogadro = 6.02214179e+23;
	k_dAlpha = 7.2973525376e-3;
	k_dexp = 2.718281828459045235;
	k_dEuler = 0.577215664901532861;
	k_dmh = k_dAMU_gm + k_dme;
	k_da0 = k_dhbar * k_dhbar / (k_dme * k_de * k_de);
	k_dRy = (k_dme * k_de * k_de * k_de * k_de) / (2.0 * k_dhbar * k_dhbar);
	k_dRy_eV = k_dRy * k_deV_erg;
}

XASTRO_CONSTS	g_XASTRO;

double XA_Jeans_Mass(const double &i_dTemp_K, const double &i_dNumber_Density_cmm3, const double &i_dMu)
{
	return (pow(5. * g_XASTRO.k_dKb * i_dTemp_K / (i_dMu * g_XASTRO.k_dmh * g_XASTRO.k_dG),1.5) * pow(3. / (4. * g_XASTRO.k_dpi * i_dNumber_Density_cmm3 * g_XASTRO.k_dmh * i_dMu),0.5));
}

double	XA_Jeans_Density(const double &i_dTemp_K, const double &i_dMu, const double &i_dMass_g)
{
	return (pow(5. * g_XASTRO.k_dKb * i_dTemp_K / (i_dMu * g_XASTRO.k_dmh * g_XASTRO.k_dG),3.0) * 3.0 / (4.0 * g_XASTRO.k_dpi) * pow(i_dMass_g,-2.));
}


XASTRO_FILTER_GAUSSIAN	g_XASTRO_Filter_Swift_v(5440.0,750.0);
XASTRO_FILTER_GAUSSIAN	g_XASTRO_Filter_Swift_b(4390.0,980.0);
XASTRO_FILTER_GAUSSIAN	g_XASTRO_Filter_Swift_u(3450.0,875.0);
XASTRO_FILTER_GAUSSIAN	g_XASTRO_Filter_Swift_uvw1(2510.0,700.0);
XASTRO_FILTER_GAUSSIAN	g_XASTRO_Filter_Swift_uvm2(2170.0,510.0);
XASTRO_FILTER_GAUSSIAN	g_XASTRO_Filter_Swift_uvw2(1880.0,760.0);
XASTRO_FILTER_GAUSSIAN	g_XASTRO_Filter_Swift_white(3850.0,2600.0);


const double g_XASTRO_Vega_u_lum = 1.26726338539281169e+34;
const double g_XASTRO_Vega_v_lum = 1.11994643111111157e+34;
const double g_XASTRO_Vega_b_lum = 2.48514964875034518e+34;
const double g_XASTRO_Vega_uvw1_lum = 1.05311372501524477e+34;
const double g_XASTRO_Vega_uvw2_lum = 1.54124059960300707e+34;
const double g_XASTRO_Vega_uvm2_lum = 9.19595425657849857e+33;
const double g_XASTRO_Vega_white_lum = 5.08343897550328910e+34;


double XA_Compute_Magnitude(const double & i_dFlux, const double & i_dReference_Flux, const double & i_dReference_Magnitude)
{
	return 2.5 * log10(i_dReference_Flux / i_dFlux) + i_dReference_Magnitude;
}

double XA_Planck_Photon_Flux(const double & i_dX, const void * i_lpvData)
{
	double dTemperature = ((double *)i_lpvData)[0];

	double dPlanck_Coeff = 2.0 * g_XASTRO.k_dpi * g_XASTRO.k_dc / (i_dX * i_dX * i_dX * i_dX);
	double dExp = 1.0 / (exp(g_XASTRO.k_dhc / g_XASTRO.k_dKb / dTemperature / i_dX) - 1.0);
	return (dPlanck_Coeff * dExp);
}

