#pragma once
#include <cmath>

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

class calendar_date
{
public:
	int m_iYear;
	unsigned int m_uiMonth; // 1 = January
	unsigned int m_uiDay; // 1 = 1st of Month
	enum calendar_type {Gregorian,Julian};
	calendar_type	m_eCalendar_Type;

	calendar_date(void)
	{
		m_iYear = 2000;
		m_uiMonth = 1;
		m_uiDay = 1;
		m_eCalendar_Type = Gregorian;
	}
};

// Julian date calculators
// proleptic and continuual Julian calendar to JD
double XA_JD_pJulian(int i_iYear, unsigned int i_uiMonth, unsigned int i_uiDay);
// proleptic Gregorian calendar to JD
double XA_JD_pGregorian(int i_iYear, unsigned int i_uiMonth, unsigned int i_uiDay);
// Gregorian on or after 15 Oct 1582, Julian on or before 5 Oct 1582
double XA_JD(int i_iYear, unsigned int i_uiMonth, unsigned int i_uiDay);
double XA_JD(calendar_date & i_cDate);
// Modified Julian dates
double XA_MJD(int i_iYear, unsigned int i_uiMonth, unsigned int i_uiDay);
double XA_MJD(calendar_date & i_cDate);
// Truncated Julian dates
double XA_TJD(int i_iYear, unsigned int i_uiMonth, unsigned int i_uiDay);
double XA_TJD(calendar_date & i_cDate);



double XA_Compute_Magnitude(const double & i_dFlux, const double & i_dReference_Flux, const double & i_dReference_Magnitude);


class XASTRO_FILTER
{
public:
	virtual double operator <<(const double & i_dWavelength_Angstroms)=0;
};

class XASTRO_FILTER_GAUSSIAN : public XASTRO_FILTER
{
private:
	double	m_dCentral_Wavelength;
	double	m_dSigma;
	double	m_dScaling;
public:

	XASTRO_FILTER_GAUSSIAN(void)
	{
		m_dCentral_Wavelength = 5500.0;
		m_dSigma = 100.0;
		m_dScaling = 1.0;
	}
	XASTRO_FILTER_GAUSSIAN(const double & i_dCentral_Wavelength_Angstroms, const double & i_dFull_Width_Half_Max_Angstroms, const double &i_dScaling = 1.0)
	{
		m_dCentral_Wavelength = i_dCentral_Wavelength_Angstroms;
		Set_FWHM(i_dFull_Width_Half_Max_Angstroms);
		m_dScaling = i_dScaling;
	}
	void	Set_Sigma(const double & i_dSigma){m_dSigma = i_dSigma;};
	void	Set_FWHM(const double & i_dFWHM) {m_dSigma = i_dFWHM / (2.0 * sqrt(2.0 * log(2.0)));}
	void	Set_Central_Wavelength(const double & i_dWavelength_Angstroms){m_dCentral_Wavelength = i_dWavelength_Angstroms;}
	void	Set_Scaling(const double & i_dScaling) {m_dScaling = i_dScaling;}
	inline double Get_Transmission(const double &i_dWavelength_Angstroms)
	{
		double dDel_WL = i_dWavelength_Angstroms - m_dCentral_Wavelength;
		double dExp = dDel_WL / m_dSigma;
		return (m_dScaling * exp(-dExp * dExp));
	}

	double operator << (const double &i_dWavelength_Angstroms){return Get_Transmission(i_dWavelength_Angstroms);}
};

extern XASTRO_FILTER_GAUSSIAN	g_XASTRO_Filter_Swift_u;
extern XASTRO_FILTER_GAUSSIAN	g_XASTRO_Filter_Swift_v;
extern XASTRO_FILTER_GAUSSIAN	g_XASTRO_Filter_Swift_b;
extern XASTRO_FILTER_GAUSSIAN	g_XASTRO_Filter_Swift_uvw1;
extern XASTRO_FILTER_GAUSSIAN	g_XASTRO_Filter_Swift_uvw2;
extern XASTRO_FILTER_GAUSSIAN	g_XASTRO_Filter_Swift_uvm2;
extern XASTRO_FILTER_GAUSSIAN	g_XASTRO_Filter_Swift_white;

extern const double g_XASTRO_Vega_u_lum;
extern const double g_XASTRO_Vega_v_lum;
extern const double g_XASTRO_Vega_b_lum;
extern const double g_XASTRO_Vega_uvw1_lum;
extern const double g_XASTRO_Vega_uvw2_lum;
extern const double g_XASTRO_Vega_uvm2_lum;
extern const double g_XASTRO_Vega_white_lum;


