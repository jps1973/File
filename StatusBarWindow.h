// StatusBarWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define STATUS_BAR_WINDOW_CLASS_NAME											STATUSCLASSNAME

#define STATUS_BAR_WINDOW_EXTENDED_STYLE										0
#define STATUS_BAR_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP )
#define STATUS_BAR_WINDOW_TEXT													NULL

typedef enum
{
	STATUS_BAR_WINDOW_LEFT_PART_ID = 0,
	STATUS_BAR_WINDOW_RIGHT_PART_ID,

	STATUS_BAR_WINDOW_PART_COUNT

} STATUS_BAR_WINDOW_PARTS;

BOOL IsStatusBarWindow( HWND hWnd );

BOOL StatusBarWindowCreate( HWND hWndParent, HINSTANCE hInstance );

BOOL StatusBarWindowGetRect( LPRECT lpRect );

BOOL StatusBarWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

HWND StatusBarWindowSetFocus();

void StatusBarWindowSetFont( HFONT hFont );

BOOL StatusBarWindowSetParts( int nLeftWidth );

BOOL StatusBarWindowSetText( BYTE bWhichPart, LPCTSTR lpszStatusText );

LRESULT StatusBarWindowSize();
