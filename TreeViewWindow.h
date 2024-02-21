// TreeViewWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define TREE_VIEW_WINDOW_CLASS_NAME												WC_TREEVIEW

#define TREE_VIEW_WINDOW_EXTENDED_STYLE											0
#define TREE_VIEW_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT )
#define TREE_VIEW_WINDOW_TEXT													NULL

#define TREE_VIEW_WINDOW_WIDTH													300

#define TREE_VIEW_WINDOW_DUMMY_ITEM_TEXT										"<dummy>"

BOOL IsTreeViewWindow( HWND hWnd );

HTREEITEM TreeViewWindowAddTopLevelItem( LPCTSTR lpszItemPath );

BOOL TreeViewWindowCreate( HWND hWndParent, HINSTANCE hInstance );

BOOL TreeViewWindowGetItemPath( HTREEITEM htiItem, LPTSTR lpszItemPath );

BOOL TreeViewWindowGetItemText( HTREEITEM htiItem, LPTSTR lpszItemText );

BOOL TreeViewWindowGetRect( LPRECT lpRect );

BOOL TreeViewWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam, void( *lpSelectionChangedFunction )( LPCTSTR lpszItemPath ) );

BOOL TreeViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

HWND TreeViewWindowSetFocus();

void TreeViewWindowSetFont( HFONT hFont );
