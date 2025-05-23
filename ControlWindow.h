// ControlWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define CONTROL_WINDOW_CLASS_NAME												WC_LISTBOX

#define CONTROL_WINDOW_EXTENDED_STYLE											0
#define CONTROL_WINDOW_STYLE													( WS_CHILD | WS_HSCROLL | WS_VSCROLL | WS_BORDER | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY )
#define CONTROL_WINDOW_TEXT														NULL

#define CONTROL_WINDOW_DRIVE_FORMAT_STRING										"%C Drive"

#define CONTROL_WINDOW_POPULATE_STATUS_MESSAGE_FORMAT_STRING					"%s (%d files)"

HWND ControlWindowCreate( HWND hWndParent, HINSTANCE hInstance );

BOOL ControlWindowHandleCommandMessage( WPARAM wParam, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) );

BOOL ControlWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) );

int ControlWindowPopulate( HWND hWndControl, LPCTSTR lpszFolderPath, LPCTSTR lpszFileFilter );
