// ComboBoxWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define COMBO_BOX_WINDOW_CLASS_NAME												WC_COMBOBOX

#define COMBO_BOX_WINDOW_EXTENDED_STYLE											WS_EX_WINDOWEDGE
#define COMBO_BOX_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_SORT )
#define COMBO_BOX_WINDOW_TEXT													NULL

BOOL IsComboBoxWindow( HWND hWndSupplied );

int ComboBoxWindowAddString( LPCTSTR lpszString );

BOOL ComboBoxWindowCreate( HWND hWndParent, HINSTANCE hInstance, HFONT hFont );

BOOL ComboBoxWindowGetRect( LPRECT lpRect );

BOOL ComboBoxWindowHandleCommandMessage( WPARAM wParam, LPARAM lParam, void( *lpSelectionChangeFunction )( LPCTSTR lpszItemText ) );

BOOL ComboBoxWindowLoad( LPCTSTR lpszFilePath );

BOOL ComboBoxWindowMove( int nLeft, int nTop, int nWidth, int nHeight );

BOOL ComboBoxWindowSave( LPCTSTR lpszFilePath );

int ComboBoxWindowSelectItem( int nWhichItem, void( *lpSelectionChangeFunction )( LPCTSTR lpszItemText ) );
