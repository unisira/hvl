#include "../../include/hvl_print.h"
#include <ntifs.h>
#include <stdarg.h>


void hvl_print(const char* fmt, ...)
{
    va_list args;
	va_start(args, fmt);

	vDbgPrintExWithPrefix("[HVL] ", DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, fmt, args);

	va_end(args);
}
