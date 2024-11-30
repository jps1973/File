// FileListViewWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define FILE_LIST_VIEW_WINDOW_CLASS_NAME										WC_LISTVIEW

#define FILE_LIST_VIEW_WINDOW_EXTENDED_STYLE									LVS_EX_FULLROWSELECT
#define FILE_LIST_VIEW_WINDOW_STYLE												( WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT )
#define FILE_LIST_VIEW_WINDOW_TEXT												NULL

#define FILE_LIST_VIEW_WINDOW_INITIAL_COLUMN_WIDTH								100

#define FILE_LIST_VIEW_WINDOW_POPULATE_STATUS_MESSAGE_FORMAT_STRING				"%d items"

#define FILE_LIST_VIEW_WINDOW_ITEM_TEXT_IN_INVERTED_COMMAS_FORMAT_STRING		"\"%s\""

#define FILE_LIST_VIEW_WINDOW_UNABLE_TO_SAVE_FILE_WARNING_MESSAGE_FORMAT_STRING	"Unable to save file.\r\n\r\nDo you want to close?"

#define FILE_LIST_VIEW_WINDOW_COLUMN_TITLES										{ "Name", "Modified" }

typedef enum
{
	FILE_LIST_VIEW_WINDOW_NAME_COLUMN_ID = 0,
	FILE_LIST_VIEW_WINDOW_MODIFIED_COLUMN_ID,

	FILE_LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS

} FILE_LIST_VIEW_WINDOW_COLUMNS;

BOOL IsFileListViewWindow( HWND hWnd );

int FileListViewWindowAdditem( LPCTSTR lpszItemText, DWORD dwMaximumItemTextLength = STRING_LENGTH );

void FileListViewWindowAutoSizeAllColumns();

int CALLBACK FileListViewWindowCompare( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort );

BOOL FileListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance );

int FileListViewWindowDeleteAllItems();

int FileListViewWindowGetItemCount();

int FileListViewWindowGetItemText( int nWhichItem, int nWhichSubItem, LPTSTR lpszItemText, DWORD dwMaximumItemTextLength = STRING_LENGTH );

BOOL FileListViewWindowGetRect( LPRECT lpRect );

BOOL FileListViewWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam, void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ), void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ) );

BOOL FileListViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

int FileListViewWindowPopulate( LPCTSTR lpszFolderPath );

BOOL FileListViewWindowSave( LPCTSTR lpszFileName, LPCTSTR lpszItemSeparator );

HWND FileListViewWindowSetFocus();

void FileListViewWindowSetFont( HFONT hFont );

int FileListViewWindowSetItemText( int nWhichItem, int nWhichSubItem, LPCTSTR lpszItemText, DWORD dwMaximumItemTextLength = STRING_LENGTH );
