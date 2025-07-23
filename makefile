all:
	gcc src/main.c src/Menu.c src/STATES.c src/Login.c src/sqlite3.c src/level1.c \
  -Iinclude/ -I/home/idir/c/libs/raylib/installed/include \
  -I/home/idir/c/libs/build/ \
  -L/home/idir/c/libs/raylib/installed/lib \
  -Wl,-rpath,/home/idir/c/libs/raylib/installed/lib \
  -o bin/main.exe \
  -lraylib -lGL -lm -lpthread -ldl -lrt -lX11


