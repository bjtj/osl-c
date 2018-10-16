#ifndef __OSL_H__
#define __OSL_H__

#include "platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>

void idle(unsigned long timeout);
unsigned long tick_milli(void);

#endif
