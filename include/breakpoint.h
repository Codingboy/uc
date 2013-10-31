#ifndef BREAKPOINT_H
#define BREAKPOINT_H

#include "typedefs.h"

/**
 * Halts the system until BREAKPOINT is cleared by an ISR.
 */
void breakpointDebug(void);

/**
 * Resumes the system when halted by breakpoint.
 */
void resumeBreakpoint(void);

#endif
