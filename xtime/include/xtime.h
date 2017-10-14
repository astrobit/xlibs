#pragma once
#include <stdint.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>


#define UINT64TYPE	uint64_t
// return effective timer in ns

class XTIMER
{
public:
	UINT64TYPE	m_iStartCycle;
	UINT64TYPE	m_iStopCycle;
	UINT64TYPE	m_iLastCycle;
	static UINT64TYPE	m_iOverhead;
	static UINT64TYPE	m_iProcessor_Speed_s;
	static bool				m_bInit;

	XTIMER(void);
	void Start(void); // starts the timer
	UINT64TYPE Stop(void); // stops the timer, returns the time of the start/stop cycle in clock ticks
	unsigned int GetLastTime(void); // returns the time in nanoseconds of the last timer start/stop cycle (already adjusted for call overhead)
	UINT64TYPE GetLastTime_ticks(void); // returns the time in clock ticks of the last timer start/stop cycle (already adjusted for call overhead)
	UINT64TYPE GetLastTime_ns(void); // returns the time in nanoseconds of the last timer start/stop cycle (already adjusted for call overhead)
	unsigned int GetLastTime_us(void); // returns the time in microseconds of the last timer start/stop cycle (already adjusted for call overhead)
	unsigned int GetLastTime_ms(void); // returns the time in milliseconds of the last timer start/stop cycle (already adjusted for call overhead)
	unsigned int GetLastTime_s(void); // returns the time in milliseconds of the last timer start/stop cycle (already adjusted for call overhead)
	double GetProcessorSpeed(void); // returns processor speed in MHz
	UINT64TYPE GetCallOverhead(void); // returns overhead of calling timer functions in clock ticks
};

// The sleep functions here have much higher accuracy than do the unix sleep functions, but do not release processing back to the OS
void xSleep(unsigned int i_uiTime_s);
void xuSleep(unsigned int i_uiTime_us);
