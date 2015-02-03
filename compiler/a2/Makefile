CC          =	gcc
# Flags that are sent to the compiler
# Do not change these
CFLAGS      = -g -pedantic -Wall -std=c99 -D_POSIX_C_SOURCE=199309L
LDFLAGS     =

#Directories where the compiler can find things
INCLUDES    =
LIBDIRS     =  
LIBS        = -lm -lpthread -lfl

BINDIR = ./
SRCDIR = ./

#Put the names of your source code files in the lines below.  You will need both your
# Solution source code file and the source code with the main program in it

SOURCE = $(SRCDIR)parser.c $(SRCDIR)dlxlib.c $(SRCDIR)scanner.c


#The list of object files for each program is below.  
# The object file names here must match the file names of your source code 
# except with .o instead of .c.   Make changes as appropriate


OBJS    = parser.o dlxlib.o scanner.o


PROGNAME = $(BINDIR)parser

default : all

all : scanner prog

prog : build
	$(CC) $(LIBDIRS) $(LDFLAGS) -o $(PROGNAME)  $(OBJS) $(LIBS)
build: $(SOURCE)
	$(CC) $(CFLAGS) -c $(SOURCE) $(INCLUDES)

clean:
	@ rm *.o

scanner:
	flex --header-file=scanner.h --outfile=scanner.c scanner.l;

parser: prog 

run: prog 
	$(PROGNAME)
 
val: prog
	valgrind $(PROGNAME)