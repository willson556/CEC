#include <stdlib.h>
#include "Common.h"

#if defined(WIN32)

static char FormatBuffer[4096]; 
static CRITICAL_SECTION CriticalSection;
static bool CriticalSectionInitialized = false;

void DbgPrint(const char* fmt, ...)
{
	va_list args;

	if (!CriticalSectionInitialized)
	{
		InitializeCriticalSection(&CriticalSection);
		CriticalSectionInitialized = true;
	}

	EnterCriticalSection(&CriticalSection);

	va_start(args, fmt);
	vsprintf_s(FormatBuffer, 4096, fmt, args);
	OutputDebugStringA(FormatBuffer);

	LeaveCriticalSection(&CriticalSection);
}

#elif defined(ESP_PLATFORM)

#include <esp_timer.h>
#include <esp_attr.h>

#define NOP() asm volatile ("nop")

extern "C" IRAM_ATTR unsigned long micros()
{
	return (unsigned long) esp_timer_get_time();
}

extern "C" IRAM_ATTR void delayMicroseconds(unsigned int us)
{
	// Adapted from
	uint32_t m = micros();
	if (us)
	{
		uint32_t e = m + us;
		if (m > e ) // overflow
		{
			while (micros() > e)
			{
				NOP();
			}
		}

		while (micros() < e) 
		{
			NOP();
		}
	}
}

#else

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <Arduino.h>

void DbgPrint(const char* fmt, ...)
{
        char FormatBuffer[128]; 
	va_list args;
	va_start(args, fmt);
        vsprintf(FormatBuffer, fmt, args);
       
        char c;
        char* addr = FormatBuffer;
      
        while ((c = *addr++))
        {
          Serial.print(c);
        }
}

#endif 
