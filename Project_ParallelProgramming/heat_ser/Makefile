MAIN = main
PROG = try_serial
LIBDIRS = 
INCDIRS = 
SRCPATH = ./
COMPILERFLAGS = -O3 -Wall

CC = gcc
CFLAGS = $(COMPILERFLAGS) $(INCDIRS)
LIBS = -lGL -lGLU -lglut -lm -lpthread

try_serial:
	$(CC) $(CFLAGS) -o $(PROG) $(LIBDIRS) $(LIBS) $(SRCPATH)$(MAIN).cpp 

clean:
	rm -f *.o $(PROG) core.*

.PHONY: try_serial
