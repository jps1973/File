// ListViewWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#include "SystemImageList.h"

#define LIST_VIEW_WINDOW_CLASS_NAME												WC_LISTVIEW

#define LIST_VIEW_WINDOW_EXTENDED_STYLE											LVS_EX_FULLROWSELECT
#define LIST_VIEW_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE | LVS_REPORT )
#define LIST_VIEW_WINDOW_TEXT													NULL

#define LIST_VIEW_WINDOW_DEFAULT_COLUMN_WIDTH									100

#define LIST_VIEW_WINDOW_FOLDER_DISPLAY_TEXT_FORMAT_STRING						"%c%s"
#define LIST_VIEW_WINDOW_FOLDER_DISPLAY_TEXT_PREFIX								ASCII_SPACE_CHARACTER

#define LIST_VIEW_WINDOW_MODIFIED_TEXT_FORMAT_STRING							"%04d/%02d/%02d %02d:%02d:%02d"

#define LIST_VIEW_WINDOW_POPULATE_STATUS_MESSAGE_FORMAT_STRING					"%s (%d items)"

#define LIST_VIEW_WINDOW_TITLES													{ "Name", "Modified" }

typedef enum
{
	LIST_VIEW_WINDOW_NAME_COLUMN_ID = 0,
	LIST_VIEW_WINDOW_MODIFIED_COLUMN_ID,

	LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS

} LIST_VIEW_WINDOW_COLUMNS;

BOOL IsListViewWindow( HWND hWndSupplied );

int ListViewWindowAutoSizeAllColumns();

int CALLBACK ListViewWindowCompare( LPARAM lParam1, LPARAM lParam2, LPARAM lParamColumn );

BOOL ListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance, HFONT hFont );

BOOL ListViewWindowGetItemPath( int nWhichItem, int nWhichSubItem, LPTSTR lpszItemPath );

BOOL ListViewWindowHandleNotifyMessage( WPARAM, LPARAM lParam, BOOL( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) );

BOOL ListViewWindowMove( int nLeft, int nTop, int nWidth, int nHeight );

int ListViewWindowPopulate();
