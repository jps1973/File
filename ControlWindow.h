// ControlWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define CONTROL_WINDOW_CLASS_NAME												WC_LISTVIEW

#define CONTROL_WINDOW_EXTENDED_STYLE											LVS_EX_FULLROWSELECT
#define CONTROL_WINDOW_STYLE													( WS_CHILD | WS_BORDER | LVS_REPORT )
#define CONTROL_WINDOW_TEXT														NULL

#define CONTROL_WINDOW_DEFAULT_COLUMN_WIDTH										100
#define CONTROL_WINDOW_COLUMN_TITLES											{ "Name" }

typedef enum
{
	CONTROL_WINDOW_NAME_COLUMN_ID = 0,

	CONTROL_WINDOW_NUMBER_OF_COLUMNS

} CONTROL_WINDOW_COLUMN_IDS;

int ControlWindowAutoSizeAllColumns( HWND hWndControl );

HWND ControlWindowCreate( HWND hWndParent, HINSTANCE hInstance );

BOOL ControlWindowHandleCommandMessage( WPARAM wParam, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) );

BOOL ControlWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) );

int ControlWindowPopulate( HWND hWndControl, LPCTSTR lpszFolderPath );
