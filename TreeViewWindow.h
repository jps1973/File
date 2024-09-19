// TreeViewWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define TREE_VIEW_WINDOW_CLASS_NAME												WC_TREEVIEW

#define TREE_VIEW_WINDOW_EXTENDED_STYLE											0
#define TREE_VIEW_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE | WS_BORDER | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT )
#define TREE_VIEW_WINDOW_TEXT													NULL

#define TREE_VIEW_WINDOW_HEIGHT													100

#define TREE_VIEW_WINDOW_TEXT_COLOR												GetSysColor( COLOR_GRAYTEXT )
#define TREE_VIEW_WINDOW_BACKGROUND_COLOR										GetSysColor( COLOR_WINDOW )

BOOL IsTreeViewWindow( HWND hWnd );

HTREEITEM TreeViewWindowAddItem( LPCTSTR lpszItemText, HTREEITEM htiParent = TVI_ROOT, HTREEITEM htiInsertAfter = TVI_SORT );

BOOL TreeViewWindowCreate( HWND hWndParent, HINSTANCE hInstance );

BOOL TreeViewWindowGetRect( LPRECT lpRect );

BOOL TreeViewWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ) );

BOOL TreeViewWindowLoad( LPCTSTR lpszFileName );

BOOL TreeViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

HWND TreeViewWindowSetFocus();

void TreeViewWindowSetFont( HFONT hFont );
