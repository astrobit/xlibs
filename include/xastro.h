#pragma once
class XASTRO_CONSTS
{
public:
	double	k_dAU_cm;  // Astronomical unit in cm
	double	k_dpc_cm;// Parsec in cm
	double	k_dMpc_cm;// Megaparsec in cm
	double	k_dGpc_cm;// Gigaparsec in cm
	double	k_dAMU_gm;// AMU in grams

	double	k_deV_erg;// eV / erg
	double	k_derg_eV;// erg / eV;
	double	k_dc;// Speed of  light, cm/s
	double	k_dG;// Newton's gravitational constant, cgs
	double	k_dYear;// Length of year in seconds
	double	k_dMsun;// Mass of sun, in g
	double	k_dLsun;// Luminosity of sun, erg/s
	double	k_dRsun;// Radius of sun, cm
	double	k_dSigma_SB;// Stephen-Boltzmann constant, erg / cm^2 / K^4 / s
	double	k_dWein;// Wein's law constant, cm K
	double	k_dme;// Electron mass, g
	double	k_dmp;// Proton mass, g
	double	k_dmh;// Hydrogen mass, g
	double	k_dKb;// Boltzmann constant, erg / K
	double	k_dKb_eV;// Boltzmann constant, eV / K
	double	k_de;// electron charge, esu
	double	k_da;// Radiation constant, erg / cm^3 / K^4
	double	k_dpi;// pi
	double	k_dexp; // e
	double	k_dEuler; // Euler's constant
	double	k_dh;// Plank's constant, erg s
	double	k_dhbar;// Planck's constant, erg s
	double	k_dh_eV;// Planck's constant, eV s
	double	k_dhbar_eV;// Planck's constant, eV s
	double	k_dhc;// h * c
	double	k_dhc_eVnm; // h * c, eV nm
	double	k_dH0std;// Hubble parameter, s^-1 (100 km/s/Mpc)
	double	k_dH0h;// Hubble parameter, (litle h)
	double	k_dH0;// Modern Hubble paramter (H0 * h)
	double	k_dAvogadro; // Avogadro's constant, #/mol
	double	k_dAlpha; // Fine strcture constant (e^2 / hc)
	double	k_da0; // Bohr radius (cm)
	double	k_dRy; // Rydberg energy, erg
	double	k_dRy_eV; // Rydberg energy, eV
	XASTRO_CONSTS(void);
	void	Use_FLASH_Constants(void);
};

extern	XASTRO_CONSTS	g_XASTRO;
double	XA_Jeans_Mass(const double &i_dTemp_K, const double &i_dNumber_Density_cmm3, const double &i_dMu);
double	XA_Jeans_Density(const double &i_dTemp_K, const double &i_dMu, const double &i_dMass_g);


