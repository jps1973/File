// ListViewWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#include "ImageList.h"

#define LIST_VIEW_WINDOW_CLASS_NAME												WC_LISTVIEW

#define LIST_VIEW_WINDOW_EXTENDED_STYLE											LVS_EX_FULLROWSELECT
#define LIST_VIEW_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER | LVS_REPORT )
#define LIST_VIEW_WINDOW_TEXT													NULL

#define LIST_VIEW_WINDOW_MODIFIED_TIME_FORMAT_STRING							"%04d/%02d/%02d %02d:%02d:%02d"

#define LIST_VIEW_WINDOW_DEFAULT_COLUMN_WIDTH									100

#define LIST_VIEW_WINDOW_COLUMN_TITLES											{ "Name", "Modified" }

typedef enum ListViewWindowColumns
{
	LIST_VIEW_WINDOW_NAME_COLUMN_ID = 0,
	LIST_VIEW_WINDOW_MODIFIED_COLUMN_ID,

	LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS

} LIST_VIEW_WINDOW_COLUMNS;

BOOL IsListViewWindow( HWND hWnd );

int ListViewWindowAddFiles( LPCTSTR lpszFolderPath, LPCTSTR lpszFileFilter );

int ListViewWindowAutoSizeAllColumns();

BOOL ListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance );

int ListViewWwidowGetFilePath( int nWhichItem, LPTSTR lpszFilePath );

int ListViewWwidowGetItemText( int nWhichItem, int nWhichSubItem, LPTSTR lpszItemText );

BOOL ListViewWindowGetRect( LPRECT lpRect );

BOOL ListViewWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam, void( *lpDoubleClickFunction )( LPCTSTR lpszFilePath ), void( *lpSelectionChangedFunction )( LPCTSTR lpszFilePath ) );

BOOL ListViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

HWND ListViewWindowSetFocus();

void ListViewWindowSetFont( HFONT hFont );

void ListViewWindowSetImageList( HIMAGELIST hImageList );
