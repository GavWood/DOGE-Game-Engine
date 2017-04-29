////////////////////////////////////////////////////////////////////////////////
// BtPrint.cpp

#include "BtPrint.h"
#include <string.h>
#include <cctype>
#include <stdarg.h>
#include <stdio.h>

#ifdef DX11
#define DX11_LEAN_AND_MEAN
#define INITGUID
#include <windows.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// Printf

void BtPrint( const BtChar* format, ... )
{
	char    buf[4096], *p = buf;
	va_list args;
	int     n;

	va_start(args, format);
	n = vsprintf(p, format, args); // buf-3 is room for CR/LF/NUL
	va_end(args);

	p += (n < 0) ? sizeof buf - 3 : n;

	while ( p > buf  &&  isspace(p[-1]) )
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p   = '\0';

	printf( "%s", buf );
}
