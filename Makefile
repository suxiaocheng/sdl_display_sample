
CC      = gcc
EXE     =
CFLAGS  = -g -O2 -I/usr/local/include/SDL2 -DHAVE_OPENGLES -DHAVE_OPENGLES2 -DHAVE_OPENGL 
LIBS    =  -lSDL2 -lSDL2_test -L/usr/local/lib/ -ljpeg -L/usr/local/lib

hello:main.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

