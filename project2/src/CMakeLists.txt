cmake_minimum_required (VERSION 2.8)

PROJECT( Specter )

# Find the source code and build the program.  Note that for the command
# line tools, when you add new source files, you may want to re-run cmake
# to scan for them and update the Makefile.  The generated Visual Studio
# project should do this and update itself automatically -- click yes to
# let it reload.
FILE( GLOB SRC_FILES *.cc *.h )
ADD_EXECUTABLE( specter ${SRC_FILES} )
