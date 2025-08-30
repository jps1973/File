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

BOOL IsComboBoxWindow( int nWhichComboBoxWindow, HWND hWnd );

int ComboBoxWindowAddString( int nWhichComboBoxWindow, LPCTSTR lpszString, BOOL( *lpSelectionChangeFunction )( int nWhichComboBoxWindow, LPCTSTR lpszItemText ) );

BOOL ComboBoxWindowCreate( int nWhichComboBoxWindow, HWND hWndParent, HINSTANCE hInstance );

int ComboBoxWindowGetItemCount( int nWhichComboBoxWindow );

BOOL ComboBoxWindowGetRect( int nWhichComboBoxWindow, LPRECT lpRect );

BOOL ComboBoxWindowHandleCommandMessage( int nWhichComboBoxWindow, WPARAM wParam, LPARAM lParam, BOOL( *lpSelectionChangeFunction )( int nWhichComboBoxWindow, LPCTSTR lpszItemText ) );

BOOL ComboBoxWindowMove( int nWhichComboBoxWindow, int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

int ComboBoxWindowLoad( int nWhichComboBoxWindow, LPCTSTR lpszFileName );

int ComboBoxWindowPopulate( int nWhichComboBoxWindow, LPCTSTR lpszFileName );

int ComboBoxWindowSave( int nWhichComboBoxWindow, LPCTSTR lpszFileName );

BOOL ComboBoxWindowSelectItem( int nWhichComboBoxWindow, int nWhichItem, BOOL( *lpSelectionChangeFunction )( int nWhichComboBoxWindow, LPCTSTR lpszItemText ) );

HWND ComboBoxWindowSetFocus( int nWhichComboBoxWindow );

void ComboBoxWindowSetFont( int nWhichComboBoxWindow, HFONT hFont );
