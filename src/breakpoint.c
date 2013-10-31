#include "breakpoint.h"

static volatile u8 BREAKPOINT;

void breakpoint(void)
{
	BREAKPOINT = 1;
	while (BREAKPOINT)
	{

	}
}

void resumeBreakpoint(void)
{
	BREAKPOINT = 0;
}
