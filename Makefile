all :
	@echo Please do 'make PLATFORM' where PLATFORM is one of these : linux mingw

CFLAGS := -g -Wall
LDFLAGS := --shared

mingw : TARGET := lbitmap.dll
mingw : CFLAGS += -I/usr/local/include
mingw : LDFLAGS += -L/usr/local/bin -llua53

mingw : lbitmap

linux : TARGET := lbitmap.so
linux : CFLAGS += -I/usr/local/include
linux : LDFLAGS += -fPIC

linux : lbitmap

lbitmap : lbitmap.c
	gcc -o $(TARGET) $(CFLAGS) $^ $(LDFLAGS)

clean :
	rm -f lbitmap.dll lbitmap.so
