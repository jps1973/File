cls

del *.exe

windres Resource.rc -o Resource.o

g++ -mwindows -m64 -static -Wall -Wextra ^
 ^
 File.cpp ^
 ^
 ControlWindow.cpp ^
 StatusBarWindow.cpp ^
 TabControlWindow.cpp ^
 ^
 Resource.o ^
 ^
 -o File.exe
