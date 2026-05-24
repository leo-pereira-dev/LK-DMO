#pragma once

#include <stdarg.h>
#include <stdio.h>

static inline void ExtraInventoryDebugLog( const char* pFormat, ... )
{
	char szMessage[ 2048 ] = { 0, };
	va_list args;
	va_start( args, pFormat );
	_vsnprintf_s( szMessage, sizeof( szMessage ), _TRUNCATE, pFormat, args );
	va_end( args );

	SYSTEMTIME st;
	GetLocalTime( &st );

	char szLine[ 2304 ] = { 0, };
	_snprintf_s( szLine, sizeof( szLine ), _TRUNCATE,
		"[%04d-%02d-%02d %02d:%02d:%02d.%03d] %s\r\n",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, szMessage );

	OutputDebugStringA( szLine );

	FILE* fp = NULL;
	if( fopen_s( &fp, ".\\ExtraInventory_Debug.log", "a+t" ) == 0 && fp != NULL )
	{
		fputs( szLine, fp );
		fclose( fp );
	}
}
