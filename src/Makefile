#
# Created by gmakemake (Ubuntu Sep  7 2011) on Fri Feb 17 21:39:32 2012
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Default flags (redefine these with a header.mak file if desired)
CXXFLAGS =	-ggdb
CFLAGS =	
CLIBFLAGS =	
CCLIBFLAGS =	
########## End of default flags


CPP_FILES =	
C_FILES =	main-opt.c main-orig.c serpent-opt.c serpent-orig.c test-opt.c test-orig.c
PS_FILES =	
S_FILES =	
H_FILES =	serpent-opt.h serpent-orig.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILESORIG =	serpent-orig.o 
OBJFILESOPT =	serpent-opt.o

#
# Main targets
#

all:	main-opt main-orig test-opt test-orig 

main-opt:	main-opt.o $(OBJFILESOPT)
	$(CC) $(CFLAGS) -o main-opt main-opt.o $(OBJFILESOPT) $(CLIBFLAGS)

main-orig:	main-orig.o $(OBJFILESORIG)
	$(CC) $(CFLAGS) -o main-orig main-orig.o $(OBJFILESORIG) $(CLIBFLAGS)

test-opt:	test-opt.o $(OBJFILESOPT)
	$(CC) $(CFLAGS) -o test-opt test-opt.o $(OBJFILESOPT) $(CLIBFLAGS)

test-orig:	test-orig.o $(OBJFILESORIG)
	$(CC) $(CFLAGS) -o test-orig test-orig.o $(OBJFILESORIG) $(CLIBFLAGS)

#
# Dependencies
#

main-opt.o:	serpent-opt.h
main-orig.o:	serpent-orig.h
serpent-opt.o:	serpent-opt.h
serpent-orig.o:	serpent-orig.h
test-opt.o:	serpent-opt.h
test-orig.o:	serpent-orig.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILESORIG) $(OBJFILESOPT) main-opt.o main-orig.o test-opt.o test-orig.o core 2> /dev/null

realclean:        clean
	-/bin/rm -rf main-opt main-orig test-opt test-orig 
