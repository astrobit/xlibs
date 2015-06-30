#include <xastro.h>
#include <xstdlib.h>
#include <xastroion.h>
#include <stdio.h>



void Display_Ion_Data(unsigned int i_uiZ)
{
	char lpszRoman_Numeral[16];
	unsigned int uiI;

	XASTRO_ATOMIC_IONIZATION_DATA * lpIon_Data = g_xAstro_Ionization_Energy_Data.Get_Ionization_Data_Ptr(i_uiZ);


	for (uiI = 0; uiI < lpIon_Data->m_uiIonization_Energies_Count; uiI++)
	{
		xRomanNumeralGenerator(lpszRoman_Numeral,uiI + 1);
		printf("%i %s: %.2e (%.2e)\n",i_uiZ,lpszRoman_Numeral,lpIon_Data->m_lpIonization_energies_erg[uiI],lpIon_Data->m_lpIonization_energies_erg[uiI] * g_XASTRO.k_deV_erg);
	}
}

int main(void)
{
	Display_Ion_Data(1); // Hydrogen
	Display_Ion_Data(20); // Calcium

	return 0;
}

