#
# Created by makemake (Ubuntu Oct  3 2011) on Mon Oct  3 14:48:28 2011
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
		$(COMPILE.s) -o $@ $<
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

AS =		as
CC =		cc
CXX =		c++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.s = $(AS) $(ASFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Flags from header.mak

CXXFLAGS =	-ggdb -Wall
CFLAGS =	-ggdb -Wall
CLIBFLAGS =	-lm
CCLIBFLAGS =	

########## End of flags from header.mak


CPP_FILES =	dirlist.cpp makemake.cpp write.cpp
C_FILES =	
PS_FILES =	
S_FILES =	
H_FILES =	dirlist.h write.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	dirlist.o write.o 

#
# Main targets
#

all:	makemake 

makemake:	makemake.o $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o makemake makemake.o $(OBJFILES) $(CCLIBFLAGS)

#
# Dependencies
#

dirlist.o:	dirlist.h
makemake.o:	dirlist.h write.h
write.o:	dirlist.h write.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) makemake.o core

realclean:        clean
	-/bin/rm -f makemake 
