// ComboBoxWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define COMBO_BOX_WINDOW_CLASS_NAME												WC_COMBOBOX

#define COMBO_BOX_WINDOW_EXTENDED_STYLE											0
#define COMBO_BOX_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_SORT )
#define COMBO_BOX_WINDOW_TEXT													NULL

#define COMBO_BOX_WINDOW_POPULATE_STATUS_MESSAGE_FORMAT_STRING					"%s (%d items )"

#define COMBO_BOX_WINDOW_UNABLE_TO_SAVE_WARNING_MESSAGE							"Unable to save.\r\n\r\nDo you want to close?"

BOOL IsComboBoxWindow( HWND hWnd );

int ComboBoxWindowAddString( LPCTSTR lpszString, BOOL( *lpSelectionChangeFunction )( LPCTSTR lpszItemText ) );

BOOL ComboBoxWindowCreate( HWND hWndParent, HINSTANCE hInstance );

int ComboBoxWindowGetItemCount();

BOOL ComboBoxWindowGetRect( LPRECT lpRect );

BOOL ComboBoxWindowHandleCommandMessage( WPARAM wParam, LPARAM lParam, BOOL( *lpSelectionChangeFunction )( LPCTSTR lpszItemText ) );

BOOL ComboBoxWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

int ComboBoxWindowLoad( LPCTSTR lpszFileName );

int ComboBoxWindowPopulate( LPCTSTR lpszFileName );

int ComboBoxWindowSave( LPCTSTR lpszFileName );

BOOL ComboBoxWindowSelectItem( int nWhichItem, BOOL( *lpSelectionChangeFunction )( LPCTSTR lpszItemText ) );

HWND ComboBoxWindowSetFocus();

void ComboBoxWindowSetFont( HFONT hFont );
