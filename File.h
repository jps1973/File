// File.h

#pragma once

#include <windows.h>

#include "Ascii.h"
#include "Common.h"

#include "Resource.h"

#include "ComboBoxWindow.h"
#include "ListViewWindow.h"
#include "StatusBarWindow.h"

#include "BrowseForFolder.h"

#define MAIN_WINDOW_CLASS_NAME													"Main File Window Class"

#define MAIN_WINDOW_CLASS_STYLE													0
#define MAIN_WINDOW_CLASS_ICON_NAME												IDI_APPLICATION
#define MAIN_WINDOW_CLASS_ICON													LoadIcon( NULL, MAIN_WINDOW_CLASS_ICON_NAME )
#define MAIN_WINDOW_CLASS_CURSOR												LoadCursor( NULL, IDC_ARROW )
#define MAIN_WINDOW_CLASS_BACKGROUND											( HBRUSH )( COLOR_APPWORKSPACE + 1 )
#define MAIN_WINDOW_CLASS_MENU_NAME												MAKEINTRESOURCE( IDR_MAIN_MENU )
#define MAIN_WINDOW_CLASS_ICON_SMALL											LoadIcon( NULL, MAIN_WINDOW_CLASS_ICON_NAME )

#define MAIN_WINDOW_EXTENDED_STYLE												WS_EX_CLIENTEDGE
#define MAIN_WINDOW_STYLE														WS_OVERLAPPEDWINDOW
#define MAIN_WINDOW_TEXT														"File"

#define MAIN_WINDOW_MINIMUM_WIDTH												320
#define MAIN_WINDOW_MINIMUM_HEIGHT												200

#define SYSTEM_MENU_SEPARATOR_ITEM_POSITION										5
#define SYSTEM_MENU_ABOUT_ITEM_POSITION											6
#define SYSTEM_MENU_ABOUT_ITEM_TEXT												"About"

#define FOLDERS_FILE_NAME														"Folders.txt"

#define SELECT_FOLDER_TITLE_FILE_COPY_TO										"Select Folder to Copy Files To"
#define SELECT_FOLDER_TITLE_FILE_MOVE_TO										"Select Folder to Move Files To"

#define ABOUT_MESSAGE_TEXT														"File\r\n"							\
																				"\r\n"								\
																				"Written by Jim Smith\r\n"			\
																				"\r\n"								\
																				"January 2026"

#define UNABLE_TO_SAVE_FOLDERS_WARNING_MESSAGE									"Unable to Save Folders.\r\n\r\nDo You Want To Close?"

#define UNABLE_TO_REGISTER_MAIN_WINDOW_CLASS_ERROR_MESSAGE						"Unable to Register Main Window Class"
#define UNABLE_TO_CREATE_MAIN_WINDOW_ERROR_MESSAGE								"Unable to Create Main Window"
