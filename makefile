TARGET    = fractal
SRC_FILES = fractal.cpp
HDR_FILES =
OBJ       = fractal.o
######################################################
CC        =  g++
CFLAGS    = -g -std=c++14 -Wall
IPATH     = 
LFLAGS    = -L/usr/lib/x86_64-linux-gnu/ 
LIBS      =  -lplotter  -lX11
COMPILE   = $(CC) $(CFLAGS) $(IPATH) $(LFLAGS) $(LIBS)
######################################################
target: $(OBJ) $(HDR_FILES)
	@echo 'makeing target'
	$(COMPILE) -o $(TARGET) $(OBJ) 
	time ./$(TARGET) 1010x1010 X M -0.7562 0.0639 0 0 0.0010 266 367 489 8197

.PHONY:clean
clean:
	rm -f $(TARGET) $(OBJ)
	ls -lA

edit: 
	vim makefile
