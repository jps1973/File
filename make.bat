cls

del *.exe

windres Resource.rc -o Resource.o

g++ -mwindows -m64 -static -Wall -Wextra Minimal.cpp Resource.o -o Minimal.exe
