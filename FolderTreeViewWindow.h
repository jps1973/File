// FolderTreeViewWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define FOLDER_TREE_VIEW_WINDOW_CLASS_NAME										WC_TREEVIEW

#define FOLDER_TREE_VIEW_WINDOW_EXTENDED_STYLE									0
#define FOLDER_TREE_VIEW_WINDOW_STYLE											( WS_CHILD | WS_VISIBLE | WS_BORDER | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT )
#define FOLDER_TREE_VIEW_WINDOW_TEXT											NULL

#define FOLDER_TREE_VIEW_WINDOW_DUMMY_ITEM_TEXT									"[dummy]"

BOOL IsFolderTreeViewWindow( HWND hWnd );

BOOL FolderTreeViewWindowCreate( HWND hWndParent, HINSTANCE hInstance );

BOOL FolderTreeViewWindowGetRect( LPRECT lpRect );

BOOL FolderTreeViewWindowHandleCommandMessage( WPARAM wParam, LPARAM lParam, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ) );

HTREEITEM FolderTreeViewWindowInsertItem( LPCTSTR lpszItemText, HTREEITEM htiParent = TVI_ROOT, HTREEITEM htiInsertAfter = TVI_SORT );

int FolderTreeViewWindowLoad( LPCTSTR lpszFileName, HTREEITEM htiParent = TVI_ROOT, HTREEITEM htiInsertAfter = TVI_SORT );

BOOL FolderTreeViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

HWND FolderTreeViewWindowSetFocus();

void FolderTreeViewWindowSetFont( HFONT hFont );
