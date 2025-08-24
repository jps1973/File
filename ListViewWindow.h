// ListViewWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define LIST_VIEW_WINDOW_CLASS_NAME												WC_LISTVIEW

#define LIST_VIEW_WINDOW_EXTENDED_STYLE											0
#define LIST_VIEW_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER | LVS_REPORT )
#define LIST_VIEW_WINDOW_TEXT													NULL

#define LIST_VIEW_WINDOW_DEFAULT_COLUMN_WIDTH									100

#define LIST_VIEW_WINDOW_POPULATE_STATUS_MESSAGE_FORMAT_STRING					"%s (%d items )"

#define LIST_VIEW_WINDOW_UNABLE_TO_SAVE_WARNING_MESSAGE							"Unable to save.\r\n\r\nDo you want to close?"

#define LIST_VIEW_WINDOW_COLUMN_TITLES											{ "Name", "Modified" }

typedef enum
{
	LIST_VIEW_WINDOW_NAME_COLUMN_ID = 0,
	LIST_VIEW_WINDOW_MODIFIED_COLUMN_ID,

	LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS

} LIST_VIEW_WINDOW_COLUMN_IDS;

BOOL IsListViewWindow( HWND hWnd );

int ListViewWindowAddColumn( LPCTSTR lpszTitle, int nWidth = LIST_VIEW_WINDOW_DEFAULT_COLUMN_WIDTH );

int ListViewWindowAddString( LPCTSTR lpszString );

int ListViewWindowAutoSizeAllColumns();

BOOL ListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance );

BOOL ListViewWindowGetRect( LPRECT lpRect );

BOOL ListViewWindowHandleCommandMessage( WPARAM wParam, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) );

BOOL ListViewWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) );

BOOL ListViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

int ListViewWindowLoad( LPCTSTR lpszFileName );

int ListViewWindowPopulate( LPCTSTR lpszFileName );

int ListViewWindowSave( LPCTSTR lpszFileName );

HWND ListViewWindowSetFocus();

void ListViewWindowSetFont( HFONT hFont );
