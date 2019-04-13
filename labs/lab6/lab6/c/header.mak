#
# This header.mak file sets up all the necessary options for compiling
# and linking C and C++ programs which use OpenGL, GLFW, and/or GLEW on
# the CS Ubuntu systems.  To use it, place it in the same directory as
# your source code, and run the command
#
#	gmakemake > Makefile
#
# To compile and link your program, just run "make".
#

# locations of important directories if the header files
# and library files aren't in the standard places
INCLUDE = -I/usr/include/SOIL
LIBDIRS =

# uncomment this if you get "-lGL not found"
# errors when linking on the CS machines
# LIBDIRS = -L/home/course/cscix10/lib/links

# common linker options
LDLIBS = -lSOIL -lGL -lm -lGLEW -lglfw

# language-specific linker options
CLDLIBS = -lgsl -lgslcblas
CCLDLIBS =

# common compiler flags
COMMONFLAGS = -g $(INCLUDE) -DGL_GLEXT_PROTOTYPES

# language-specific compiler flags
CFLAGS = -std=c99 $(COMMONFLAGS)
CXXFLAGS = $(COMMONFLAGS)

# linker flags
LIBFLAGS = -g $(LIBDIRS) $(LDLIBS)
CLIBFLAGS = $(LIBFLAGS) $(CLDLIBS)
CCLIBFLAGS = $(LIBFLAGS) $(CCLDLIBS)
