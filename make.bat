cls

del *.exe

windres Resource.rc -o Resource.o

g++ -mwindows -m64 -static -Wall -Wextra File.cpp ComboBoxWindow.cpp ListViewWindow.cpp StatusBarWindow.cpp SystemImageList.cpp Resource.o -o File.exe
