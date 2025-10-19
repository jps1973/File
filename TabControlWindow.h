// TabControlWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#include "ControlWindow.h"
#include "SelectFolder.h"

#define TAB_CONTROL_WINDOW_CLASS_NAME											WC_TABCONTROL

#define TAB_CONTROL_WINDOW_EXTENDED_STYLE										0
#define TAB_CONTROL_WINDOW_STYLE												( WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE )
#define TAB_CONTROL_WINDOW_TEXT													NULL

#define TAB_CONTROL_WINDOW_FIRST_NEW_TAB_NUMBER									1
#define TAB_CONTROL_WINDOW_NEW_TAB_NAME_FORMAT_STRING							"Tab %d"

#define TAB_CONTROL_WINDOW_UNABLE_TO_SAVE_TABS_WARNING_MESSAGE					"Unable to Save Tabs.\r\n\r\nDo you still want to close?"

typedef struct
{
	TCITEMHEADER tcItemHeader;

	HWND hWndControl;
	LPTSTR lpszParentFolderPath;

} TAB_CONTROL_WINDOW_DATA;

BOOL IsTabControlWindow( HWND hWnd );

int TabControlWindowAddTab( HINSTANCE hInstance );

int TabControlWindowAddTab( HINSTANCE hInstance, LPCTSTR lpszParentFolderPath );

int TabControlWindowCountTabs();

BOOL TabControlWindowCreate( HWND hWndParent, HINSTANCE hInstance );

BOOL TabControlWindowDeleteTab( int nWhichTab );

BOOL TabControlWindowDoesTabExist( LPCTSTR lpszRequiredTabName );

BOOL TabControlWindowGetParentFolderPath( int nWhichTab, LPTSTR lpszParentFolderPath );

BOOL TabControlWindowGetParentFolderPath( LPTSTR lpszParentFolderPath );

BOOL TabControlWindowGetRect( LPRECT lpRect );

int TabControlWindowGetSelectedItem();

BOOL TabControlWindowGetTabName( int nWhichTab, LPTSTR lpszTabName );

BOOL TabControlWindowGetTabName( LPTSTR lpszTabName );

BOOL TabControlWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszTabName ) );

int TabControlWindowLoad( LPCTSTR lpszFileName, HINSTANCE hInstance );

BOOL TabControlWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

BOOL TabControlWindowMoveControlWindow();

BOOL TabControlWindowOnTabSelected( int nWhichTab, BOOL( *lpStatusFunction )( LPCTSTR lpszTabName ) );

int TabControlWindowSave( LPCTSTR lpszFileName );

BOOL TabControlWindowSelectTab( int nWhichTab, BOOL( *lpStatusFunction )( LPCTSTR lpszTabName ) );

HWND TabControlWindowSetFocus();

void TabControlWindowSetFont( HFONT hFont );
