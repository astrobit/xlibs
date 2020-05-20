#include <xastro.h>
#include <cmath>


XASTRO_CONSTS::XASTRO_CONSTS(void)
{
	// fundamental numeric constants
	k_dpi = acos(-1.0);
	k_dexp = 2.718281828459045235;
	k_dEuler = 0.577215664901532861;

	// defined fundamental constants (CODATA 2018 except AU)
	k_dG = 6.67430e-8;//cgs
	k_dc = 2.99792458e10; // cgs
	k_dCs_Transition = 9.19263177e9;
	k_dh = 6.62607015e-27; // erg s
	k_dAvogadro = 6.0224076e+23;
	k_dAU_cm = 1.49597870700e+13; // cm (IAU Resolution B2 2012)
	k_dQe = 1.602176634000e-19; // Coulomb // fundamental charge in SI units
	k_dKb = 1.380649000000e-16; // erg/K

	// empirical fundamental constants
	k_dYear = 365.2421896698 * 86400.0;// Tropical Year 2000 (McCarthy & Seidelmann 2009)  
	k_dLsun = 3.828e+33;//erg/s (IAU Resolution B3 2015)
	k_dRsun = 6.957e+10; // cm (IAU Resolution B3 2015)
	k_dTsun = 5772; // K  (IAU Resolution B3 2015)
	k_dSsun = 1.361e6; // erg cm^{-2} (IAU Resolution B3 2015)
	k_dH0h = 0.726; // s^{-1} h // WMAP 7

	// semi-empircal derived constants
	k_de = k_dQe * k_dc / 10.0; // esu, now defined in terms of speed of light and fundamental charge. //4.80320451e-10;
	k_dAMU_gm = 1.0 / k_dAvogadro; // AMU in gm
	k_dmp = 1.007276466621 * k_dAMU_gm;//g
	k_dme = 5.48579909065e-4 * k_dAMU_gm;//g (CODATA 2018)

	// derived constants
	// combined mass of Sun and Newton's gravitational constant
	k_dGMsun = 1.3271244e26; // cm^3 s^2 (IAU Resolution B3 2015)
	k_dMsun = k_dGMsun / k_dG; // g (IAU Resolution B3 2015)

	// combined mass of Jupiter and Newton's gravitational constant
	k_dGMjupiter = 1.2668653e23; // cm^3 s^2 (IAU Resolution B3 2015)
	k_dMjupiter = k_dGMjupiter / k_dG; // g (IAU Resolution B3 2015)
	k_dRjupiterPolar = 6.6854e9; // cm  (IAU Resolution B3 2015)
	k_dRjupiterEquatorial = 7.1492e9; // cm  (IAU Resolution B3 2015)
	k_dRjupiter = std::pow(k_dRjupiterPolar * k_dRjupiterEquatorial * k_dRjupiterEquatorial,1.0/3.0); // average radius for volume, cm

	// combined mass of Jupiter and Newton's gravitational constant
	k_dGMearth = 3.986004e20; // cm^3 s^2 (IAU Resolution B3 2015)
	k_dMearth = k_dGMearth / k_dG; // g (IAU Resolution B3 2015)
	k_dRearthPolar = 6.3568e8; // cm  (IAU Resolution B3 2015)
	k_dRearthEquatorial = 6.3781e8; // cm  (IAU Resolution B3 2015)
	k_dRearth = std::pow(k_dRearthPolar * k_dRearthEquatorial * k_dRearthEquatorial,1.0/3.0); // average radius for volume, cm

	k_dpc_cm = k_dAU_cm * 648000.0 / k_dpi; // 1 pc in cm
	k_dMpc_cm = k_dpc_cm * 1.0e6; // 1 Mpc in cm
	k_dGpc_cm = k_dpc_cm * 1.0e9; // 1 Gpc in cm
	k_dH0std = 100.0e5 / (k_dMpc_cm); // s^{-1} h

	k_derg_eV = k_dQe * 1e7;//1. / k_deV_erg;
	k_deV_erg = 1.0e-7 / k_dQe;// eV / erg

	k_dhc = k_dh * k_dc;

	k_derg_cmm1 = k_dhc; // erg / cm^-1
	k_dcmm1_erg = 1.0 /  k_dhc; // cm^-1 / erg

	k_da = 8.0 * k_dpi * k_dpi * k_dpi * k_dpi * k_dpi * k_dKb * k_dKb * k_dKb * k_dKb / (15.0 * k_dhc * k_dhc * k_dhc); //7.5657e-15; // erg cm^{-3} K^{-4}
	k_dSigma_SB = k_da * k_dc * 0.25;////cgs
	k_dWein = k_dhc / k_dKb / 4.965114231e-7;// nm
	k_dKb_eV = k_dKb * k_deV_erg; // eV/K 
	k_dhbar = k_dh / (2.0 * k_dpi); // erg s //@@
	k_dh_eV = k_dh * k_deV_erg;// eV s 
	k_dhbar_eV = k_dhbar * k_deV_erg;// eV s
	k_dhc_eVnm = k_dh_eV * k_dc * 1e7;//eV nm
	k_dH0 = k_dH0std * k_dH0h;
	k_dAlpha = 2.0 * k_dpi * k_de * k_de / k_dhc;
	k_da0 = k_dh * k_dh / (4.0 * k_dpi * k_dpi * k_dme * k_de * k_de); // cm
	k_dRy = (2.0 * k_dpi * k_dpi * k_dme * k_de * k_de * k_de * k_de) / (k_dh * k_dh);
	k_dRy_eV = k_dRy * k_deV_erg;
	k_dRy_cmm1 = k_dRy / k_dhc;
	k_dmh = k_dme + k_dmp - k_dRy / (k_dc * k_dc);
	k_dSigma_Te = k_dQe * k_dQe * k_dQe * k_dQe / (k_dme * k_dme) * 8.0 * k_dpi / 3.0 * 1.0e4;
	k_dSigma_Tp = k_dQe * k_dQe * k_dQe * k_dQe / (k_dmp * k_dmp) * 8.0 * k_dpi / 3.0 * 1.0e4;
}
void	XASTRO_CONSTS::Use_FLASH_Constants(void)
{
	double	dPi = acos(-1.0);
	// flash 4.0 alpha
	k_dc = 2.99792458e+10; 
	k_dG = 6.67428e-8;
	k_de = 4.80320427e-10;
	k_dme = 9.10938215e-28;
	k_dmp = 1.672621637e-24;
	k_dexp = 2.718281828459045235;
	k_dEuler = 0.577215664901532861;
	k_dh = 6.62606896e-27;
	k_dAvogadro = 6.02214179e+23;
	k_dKb = 1.3806504e-16;
	k_dAMU_gm = 1.660538782e-24;

	k_dMsun = 1.9889225e+33;
	k_dRsun = 6.96e+10;
	k_dAU_cm = 1.49597870662e+13;

	k_dpc_cm = 3.0856775807e+18; // 1 pc in cm
	k_dMpc_cm = 3.0856775807e+24; // 1 Mpc in cm
	k_dGpc_cm = 3.0856775807e+27; // 1 Gpc in cm

	k_derg_eV = 1.0e8*k_de/k_dc;//1.602192e-11;//1. / k_deV_erg;
	k_deV_erg = 1.0 / k_derg_eV;//6.2415e+11; // eV / erg

	k_dhc = k_dh * k_dc;
	k_da = 4.0 * k_dSigma_SB / k_dc;
	k_dSigma_SB = 5.670400e-5;
	k_dWein = 0.28977685;
	k_dKb_eV = k_dKb * k_deV_erg;//8.6173324e-5; // eV/K 
	k_dhbar = k_dh / (2.0 * k_dpi); // erg s //@@
	k_dh_eV = k_dh * k_deV_erg;
	k_dhbar_eV = k_dhbar * k_deV_erg;//6.58211928e-16; // eV s
	k_dhc_eVnm = k_dh_eV * k_dc * 1e7;//eV nm
	k_dH0 = k_dH0std * k_dH0h;
	k_dAlpha = 7.2973525376e-3;
	k_da0 = k_dh * k_dh / (4.0 * k_dpi * k_dpi * k_dme * k_de * k_de); // = 5.2917721092e−9; // cm
	k_dRy = (2.0 * k_dpi * k_dpi * k_dme * k_de * k_de * k_de * k_de) / (k_dh * k_dh);
	k_dRy_eV = k_dRy * k_deV_erg;
	k_dRy_cmm1 = k_dRy / k_dhc;
	k_dmh = k_dme + k_dmp - k_dRy / (k_dc * k_dc);
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

