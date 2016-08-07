#include <iostream>
#include <xastro.h>

//*****************************************************************************
//
// xastro_dates
//
// Routines for converting dates from one system to another
//
//*****************************************************************************


// compute Julian date for a Julian calendar date
// Note that this allows dates after the adoption of the Gregorian calendar - 
// it assumes the date is meant to be Julian, and does not convert
// thus, Oct 6, 1582 (OS) is 1d after Oct 5, 1582 (OS), and Oct 15, 1582 (OS) is 10d after 
// Oct 5, 1582 (OS).
// this routine is also Proleptic (allows JD before AD 4)
double XA_JD_pJulian(int i_iYear, unsigned int i_uiMonth, unsigned int i_uiDay)
{
	// convert month and day to day of year
	int iDoY =  i_uiDay - 1; // 0 based
	bool bLeap_Year = (i_iYear % 4 == 0);
	i_uiMonth--; // 0 based
	// add days of months
	switch (i_uiMonth)
	{
	case 11:
		iDoY += 30;
	case 10:
		iDoY += 31;
	case 9:
		iDoY += 30;
	case 8:
		iDoY += 31;
	case 7:
		iDoY += 31;
	case 6:
		iDoY += 30;
	case 5:
		iDoY += 31;
	case 4:
		iDoY += 30;
	case 3:
		iDoY += 31;
	case 2:
		iDoY += 28; // really, why is our calendar so weird?
		if (bLeap_Year)
		{
			iDoY++; // this is a leap year and it's after feb 29.
		}
	case 1:
		iDoY += 31;
	}
	if (i_iYear < 0)
	{
		i_iYear++; // no year 0
		int iDoYmax = 365;

		if (bLeap_Year)
			iDoYmax = 366;
		iDoY -= iDoYmax;
	}
	double dJD = 1721057.5 + i_iYear * 365.0 + (i_iYear / 4) + iDoY;
	return dJD;

}

// compute the JD from the Gregorian date; this routine allows dates before 15 Oct 1582 and assumes that they are Gregorian, not Julian
// So 14 Oct 1582 exists and is 1d before 15 oct 1582; 5 Oct 1582 is 10d before 15 Oct 1582.
double XA_JD_pGregorian(int i_iYear, unsigned int i_uiMonth, unsigned int i_uiDay)
{
	if (i_iYear < 0)
		i_iYear++; // map year 1 BC to 0
	// convert month and day to day of year
	int iDoY =  i_uiDay - 1;
	bool bLeap_Year = (i_iYear % 4 == 0 && (i_iYear % 100 != 0 || i_iYear % 400 == 0));
	i_uiMonth--; // 0 based
	// add days of months
	switch (i_uiMonth)
	{
	case 11:
		iDoY += 30;
	case 10:
		iDoY += 31;
	case 9:
		iDoY += 30;
	case 8:
		iDoY += 31;
	case 7:
		iDoY += 31;
	case 6:
		iDoY += 30;
	case 5:
		iDoY += 31;
	case 4:
		iDoY += 30;
	case 3:
		iDoY += 31;
	case 2:
		iDoY += 28; // really, why is our calendar so weird?
		if (bLeap_Year)
		{
			iDoY++; // this is a leap year and it's after feb 29.
		}
	case 1:
		iDoY += 31;
	}

	int iDelta_Years = (i_iYear - 2000);
	int iLeap_Days = 0;
	if (iDelta_Years >= 0)
	{
		// how many leap days between this date and this year (even though this year is handled by day of year calculation above, we need to account for year 2000 being a leap year)
		iLeap_Days = iDelta_Years / 4 - iDelta_Years / 100 + iDelta_Years / 400 + 1;
		if (bLeap_Year)
			iLeap_Days --;  // don't count leap day for this year
	}
	else
	{
		int iDoYmax = 365;

		if (bLeap_Year)
			iDoYmax = 366;

		iDelta_Years++; //

		iLeap_Days = iDelta_Years / 4 - iDelta_Years / 100 + iDelta_Years / 400;
		iDoY -= iDoYmax;
	}
	double dJD = 2451544.5 + iDelta_Years * 365.0 + iLeap_Days + iDoY;
	return dJD;
}

// this routine assumes gregorian calendar on and after 15 Oct
double XA_JD(int i_iYear, unsigned int i_uiMonth, unsigned int i_uiDay)
{
	if (i_iYear < 1582 || i_iYear == 1582 && i_uiMonth < 10 || i_iYear == 1582 && i_uiMonth == 10 && i_uiDay <= 5)
		return XA_JD_pJulian(i_iYear, i_uiMonth, i_uiDay);
	else if (i_iYear > 1582 || i_iYear == 1582 && i_uiMonth > 10 || i_iYear == 1582 && i_uiMonth == 10 && i_uiDay >= 15)
		return XA_JD_pGregorian(i_iYear, i_uiMonth, i_uiDay);
	else
	{
		std::cerr << "Julian date requested for the period between 5 Oct and 15 Oct 1582. Calendar must be specified for this range." << std::endl;
		return (std::nan(""));
	}
}
// this routine uses the user specified calendar
double XA_JD(calendar_date & i_cDate)
{
	if (i_cDate.m_eCalendar_Type == calendar_date::Julian)
		return XA_JD_pJulian(i_cDate.m_iYear, i_cDate.m_uiMonth, i_cDate.m_uiDay);
	else if (i_cDate.m_eCalendar_Type == calendar_date::Gregorian)
		return XA_JD_pGregorian(i_cDate.m_iYear, i_cDate.m_uiMonth, i_cDate.m_uiDay);
	else
	{
		std::cerr << "Julian date requested for an unknown calendar." << std::endl;
		return (std::nan(""));
	}
}

// modified Julian date
double XA_MJD(int i_iYear, unsigned int i_uiMonth, unsigned int i_uiDay)
{
	return XA_JD(i_iYear,i_uiMonth,i_uiDay) - 2400000.5;
}
double XA_MJD(calendar_date & i_cDate)
{
	return XA_JD(i_cDate) - 2400000.5;
}

// Truncated Julian date
double XA_TJD(int i_iYear, unsigned int i_uiMonth, unsigned int i_uiDay)
{
	return XA_JD(i_iYear,i_uiMonth,i_uiDay) - 2440000.5;
}
double XA_TJD(calendar_date & i_cDate)
{
	return XA_JD(i_cDate) - 2440000.5;
}

