TARGET    = fractal
SRC_FILES = fracral.cpp
HDR_FILES =
OBJ       = fractal.o
######################################################
CC        =  g++
CFLAGS    = -g  -Wall
IPATH     = 
LFLAGS    = -L/usr/lib/x86_64-linux-gnu/ 
LIBS      =  -lplotter  -lX11
COMPILE   = $(CC) $(CFLAGS) $(IPATH) $(LFLAGS) $(LIBS)
######################################################
target: $(OBJ) $(HDR_FILES)
	@echo 'makeing target'
	$(COMPILE) -o $(TARGET) $(OBJ) 
	./$(TARGET) 857x512 X M -0.75 0 0 0 1.25 453 534 345 4096

.PHONY:clean
clean:
	rm -f $(TARGET) $(OBJ)
	ls -lA

edit: 
	vim makefile
