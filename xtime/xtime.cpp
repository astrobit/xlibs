#include <sys/time.h>
#include <time.h>
#include <xtime.h>
#include <unistd.h>
// TIMER_RESOLUTION is the integer number of timer ticks per second
#define TIMER_RESOLUTION 1000000
#define TIMER_RESOLUTION_NS (TIMER_RESOLUTION * 1000)
#define TIMER_RESOLUTION_US (TIMER_RESOLUTION)
#define TIMER_RESOLUTION_MS (TIMER_RESOLUTION / 1000)
#define TIMER_RESOLUTION_S (TIMER_RESOLUTION / 1000000)
#define TIMER_RESOLUTION_FLOAT 1.0e-6

inline uint64_t xTimeClock() 
{
	unsigned int lo, hi;
	asm volatile (
		"cpuid \n"
		"rdtsc" 
		: "=a"(lo), "=d"(hi) /* outputs */
		: "a"(0)             /* inputs */
		: "%ebx", "%ecx");     /* clobbers*/
	return ((uint64_t)lo) | (((uint64_t)hi) << 32);
}


bool				XTIMER::m_bInit = false;
UINT64TYPE	XTIMER::m_iOverhead = 0;
UINT64TYPE	XTIMER::m_iProcessor_Speed_s = 0;
UINT64TYPE	xTime_iProcessor_Speed_us = 0;
UINT64TYPE	xTime_iProcessor_Speed_s = 0;

XTIMER	xtimer_main_instantiation_for_initialization;

XTIMER::XTIMER(void)
{
	UINT64TYPE iSpeed_Accum;
	if (!m_bInit)
	{
		Start();
		m_iOverhead = Stop();
		Start();
		sleep(1);
		Stop();
		iSpeed_Accum = m_iLastCycle;
		Start();
		sleep(1);
		Stop();
		iSpeed_Accum += m_iLastCycle;

		m_iProcessor_Speed_s = (iSpeed_Accum >> 1); // in cycles per second
		xTime_iProcessor_Speed_s = m_iProcessor_Speed_s;
		xTime_iProcessor_Speed_us = m_iProcessor_Speed_s / 1000000;
		m_bInit = true;
	}
}

void XTIMER::Start(void) // starts the timer
{
	m_iStartCycle = xTimeClock();
}

UINT64TYPE XTIMER::Stop(void) // stops the timer, returns the time of the start/stop cycle in clock ticks
{
	m_iStopCycle = xTimeClock();
	m_iLastCycle = m_iStopCycle - m_iStartCycle;
	if (m_iLastCycle <= m_iOverhead)
		m_iLastCycle = 1;
	else
		m_iLastCycle -= m_iOverhead;
	return m_iLastCycle;
}

unsigned int XTIMER::GetLastTime(void) // returns the time in nanoseconds of the last timer start/stop cycle (already adjusted for call overhead)
{
	UINT64TYPE uiTime;
	UINT64TYPE uiRetVal = m_iLastCycle / m_iProcessor_Speed_s * TIMER_RESOLUTION;

	uiTime = m_iProcessor_Speed_s / (m_iLastCycle % m_iProcessor_Speed_s);
	if (uiTime > 0)
		uiRetVal += (TIMER_RESOLUTION / uiTime);
	return (unsigned int)uiRetVal;
}

UINT64TYPE XTIMER::GetLastTime_ticks(void) // returns the time in clock ticks of the last timer start/stop cycle (already adjusted for call overhead)
{
	return m_iLastCycle;
}

UINT64TYPE XTIMER::GetLastTime_ns(void) // returns the time in nanoseconds of the last timer start/stop cycle (already adjusted for call overhead)
{
	UINT64TYPE uiTime;
	UINT64TYPE uiRetVal = m_iLastCycle / m_iProcessor_Speed_s * TIMER_RESOLUTION_NS;

	uiTime = m_iProcessor_Speed_s / (m_iLastCycle % m_iProcessor_Speed_s);
	if (uiTime > 0)
		uiRetVal += (unsigned int)(TIMER_RESOLUTION_NS / uiTime);
	return uiRetVal;
}

unsigned int XTIMER::GetLastTime_us(void) // returns the time in microseconds of the last timer start/stop cycle (already adjusted for call overhead)
{
	UINT64TYPE uiTime;
	unsigned int uiRetVal = (unsigned int)(m_iLastCycle / m_iProcessor_Speed_s * TIMER_RESOLUTION_US);

	uiTime = m_iProcessor_Speed_s / (m_iLastCycle % m_iProcessor_Speed_s);
	if (uiTime > 0)
		uiRetVal += (unsigned int)(TIMER_RESOLUTION_US / uiTime);
	return uiRetVal;
}

unsigned int XTIMER::GetLastTime_ms(void) // returns the time in milliseconds of the last timer start/stop cycle (already adjusted for call overhead)
{
	UINT64TYPE uiTime;
	unsigned int uiRetVal = (unsigned int)(m_iLastCycle / m_iProcessor_Speed_s * TIMER_RESOLUTION_MS);

	uiTime = m_iProcessor_Speed_s / (m_iLastCycle % m_iProcessor_Speed_s);
	if (uiTime > 0)
		uiRetVal += (unsigned int)(TIMER_RESOLUTION_MS / uiTime);
	return uiRetVal;
}

unsigned int XTIMER::GetLastTime_s(void) // returns the time in milliseconds of the last timer start/stop cycle (already adjusted for call overhead)
{
	UINT64TYPE uiTime;
	unsigned int uiRetVal;

	uiTime = m_iLastCycle / m_iProcessor_Speed_s;
	uiRetVal = (unsigned int)uiTime;
	return uiRetVal;
}

double XTIMER::GetProcessorSpeed(void) // returns processor speed in MHz
{
	unsigned int	iRetVal;
	iRetVal = (unsigned int) m_iProcessor_Speed_s;
	return (double)(iRetVal * TIMER_RESOLUTION_FLOAT);
}

UINT64TYPE XTIMER::GetCallOverhead(void) // returns overhead of calling timer functions in clock ticks
{
	return m_iOverhead;
}


void xSleep(unsigned int i_uiTime_s)
{
	uint64_t uiStart = xTimeClock();
	uint64_t uiDelta = (i_uiTime_s * xTime_iProcessor_Speed_s);
	if (uiDelta > 100)
		uiDelta -= 100;
	uint64_t uiEnd = uiStart + uiDelta;
	uint64_t uiJunk = 0;
	while (xTimeClock() < uiEnd)
		uiJunk++;
	
}

void xuSleep(unsigned int i_uiTime_us)
{
	uint64_t uiStart = xTimeClock();
	uint64_t uiDelta = (i_uiTime_us * xTime_iProcessor_Speed_s) / 1000000;
	if (uiDelta > 100)
		uiDelta -= 100;
	uint64_t uiEnd = uiStart + uiDelta;
	uint64_t uiJunk = 0;
	while (xTimeClock() < uiEnd)
		uiJunk++;
}
