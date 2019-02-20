// File: write.h
// Author: K. Reek
// Contributors: S. Marshall
// Description: A class that writes the makefile.

#ifndef _WRITE_H
#define _WRITE_H

#include <string>
#include <iostream>

#include "dirlist.h"

using namespace std;

class Write {
public: // Constructor

  Write( DirList &dirlist, bool noDebugFlag, bool useGCC, const string &header_file, const string &programs_file );

private: // Helper functions

  void writeHeader(bool noDebugFlag, bool useGCC, string header_file);
  void writeLists( DirList &dirlist );
  void writeMainTargets( DirList &dirlist, string programs_file );
  void writeMainTargetList( const set<string> &list,
                               string compile,
                               string local_libs );
  void writeDependencies( DirList &dirlist );
  void writeDependencyList( const set<string> &list, DirList &dirlist );
  void writeTrailer( DirList &dirlist, bool useGCC  );

  set<string> products;

}; // Write

#endif
