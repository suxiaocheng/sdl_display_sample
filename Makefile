
CC      = gcc
EXE     =
CFLAGS  = -g -O2 -I/usr/local/include/SDL2 -DHAVE_OPENGLES -DHAVE_OPENGLES2 -DHAVE_OPENGL -I/home/V01.NET/uidq2055/program/jpeg/x86/jpeg-6b/install/include
LIBS    =  -lSDL2 -lSDL2_test -L/usr/local/lib/ -ljpeg -L/usr/local/lib

hello:main.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

