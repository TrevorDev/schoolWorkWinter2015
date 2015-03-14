CC          =	gcc
# Flags that are sent to the compiler
# Do not change these
CFLAGS      = -g -pedantic -Wall -std=c99 -D_POSIX_C_SOURCE=199309L
LDFLAGS     =

#Directories where the compiler can find things
INCLUDES    =
LIBDIRS     =  
LIBS        = -lm -lpthread

BINDIR = ./
SRCDIR = ./

#Put the names of your source code files in the lines below.  You will need both your
# Solution source code file and the source code with the main program in it

SOURCE = $(SRCDIR)palin.c $(SRCDIR)sudoku.c $(SRCDIR)dlxlib.c


#The list of object files for each program is below.  
# The object file names here must match the file names of your source code 
# except with .o instead of .c.   Make changes as appropriate


OBJS    = dlxlib.o


PROGNAME = $(BINDIR)palin

default : all

all : prog


prog : build
	$(CC) $(LIBDIRS) $(LDFLAGS) -o $(PROGNAME)  $(OBJS) palin.o $(LIBS)
	$(CC) $(LIBDIRS) $(LDFLAGS) -o sudoku  $(OBJS) sudoku.o $(LIBS)
build: $(SOURCE)
	$(CC) $(CFLAGS) -c $(SOURCE) $(INCLUDES)

clean:
	@ rm *.o

run_pali: prog 
	./palin paliTest.txt

run_sudoku: prog 
	./sudoku sudoTest.txt
 
val: prog
	valgrind $(PROGNAME)