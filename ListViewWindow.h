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

#define LIST_VIEW_WINDOW_DEFAULT_COLUMN_WIDTH									100

#define LIST_VIEW_WINDOW_FOLDER_TEXT_FORMAT_STRING								"%c%s"
#define LIST_VIEW_WINDOW_FOLDER_PREFIX											ASCII_SPACE_CHARACTER

#define LIST_VIEW_WINDOW_MODIFIED_TEXT_FORMAT_STRING							"%04d/%02d/%02d %02d:%02d:%02d"

#define LIST_VIEW_WINDOW_POPULATE_STATUS_MESSAGE_FORMAT_STRING					"%s (%d items )"

#define LIST_VIEW_WINDOW_UNABLE_TO_SAVE_WARNING_MESSAGE							"Unable to save.\r\n\r\nDo you want to close?"

#define LIST_VIEW_WINDOW_COLUMN_TITLES											{ "Name", "Modified" }

typedef enum
{
	LIST_VIEW_WINDOW_NAME_COLUMN_ID = 0,
	LIST_VIEW_WINDOW_MODIFIED_COLUMN_ID,

	LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS

} LIST_VIEW_WINDOW_COLUMN_IDS;

BOOL IsListViewWindow( int nWhichListViewWindow, HWND hWnd );

int ListViewWindowAddColumn( int nWhichListViewWindow, LPCTSTR lpszTitle, int nWidth = LIST_VIEW_WINDOW_DEFAULT_COLUMN_WIDTH );

int ListViewWindowAddFile( int nWhichListViewWindow, LPCTSTR lpszFileName, DWORD dwMaximumFileNameLength = STRING_LENGTH );

int ListViewWindowAutoSizeAllColumns( int nWhichListViewWindow );

int CALLBACK ListViewWindowCompare( int nWhichListViewWindow, LPARAM lParam1, LPARAM lParam2, LPARAM lParamColumn );

BOOL ListViewWindowCreate( int nWhichListViewWindow, HWND hWndParent, HINSTANCE hInstance );

void ListViewWindowFreeMemory( int nWhichListViewWindow );

BOOL ListViewWindowGetFilePath( int nWhichListViewWindow, int nWhichFile, LPTSTR lpszFilePath );

BOOL ListViewWindowGetRect( int nWhichListViewWindow, LPRECT lpRect );

BOOL ListViewWindowHandleNotifyMessage( int nWhichListViewWindow, WPARAM wParam, LPARAM lParam, BOOL( *lpSelectionChangedFunction )( int nWhichListViewWindow, LPCTSTR lpszItemText ), BOOL( *lpDoubleClickFunction )( int nWhichListViewWindow, LPCTSTR lpszItemText ) );

BOOL ListViewWindowMove( int nWhichListViewWindow, int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

int ListViewWindowPopulate( int nWhichListViewWindow, LPCTSTR lpszParentFolderPath );

HWND ListViewWindowSetFocus( int nWhichListViewWindow );

void ListViewWindowSetFont( int nWhichListViewWindow, HFONT hFont );
