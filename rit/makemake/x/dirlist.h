// File: dirlist.h
// Author: K. Reek
// Contributors: S. Marshall
// Description: A class that manages the various lists of filenames
//  needed to create a makefile.

#ifndef _DIRLIST_H
#define _DIRLIST_H

#include <set>
#include <map>

using namespace std;

/* Inserter object
 *
 * Writes a space-separated list of file names in f (with a leading space).
 * If the suffix is the null pointer then writes the file names exactly.
 * If the suffix is non-null then the suffix of each file is replaced with
 * this suffix.
 */
struct FileListInserter {
  const set<string> &files;
  const char * suffix;

  FileListInserter( const set<string> &f, const char *suf = 0 );
};

/* Insertion operator for the previous inserter object. Writes a
 * space-separated list of file names in f (with a leading space).
 */
ostream & operator<<( ostream &os, const FileListInserter &ins );

/* Class to hold sets of file names. There are several methods that scan
 * a directory and classify files and manipulate file names.
 */
class DirList {
public: // Constructors

  enum FileType { C, CPP, H, ARCHIVE, ASSEMBLY, OTHER }; // file type
 
  DirList( int ac, char **av ); // Constructor to initialize sets
  static const string basename( const string &name ); // get basename of name
  set<string> includeFiles( const string &name ); // get include files
  set<string> dependencies( const string &name ); // recursive dependencies

  // File name lists, publically accessible for traversals etc.

  set<string> cpp;       // All C++ files
  set<string> cpp_main;  // All C++ files with main
  set<string> cpp_other; // Non-main C++ files
  set<string> c;         // All C files
  set<string> c_main;    // All C files with main
  set<string> c_other;   // Non-main C files
  set<string> ass;         // All assembly files
  set<string> ass_main;    // All assembly files with main
  set<string> ass_other;   // Non-main assembly files
  set<string> h;         // All .H and .h files
  set<string> archive;   // All .a files

 private: // Helper functions
 public: // for debug

  static bool matchString( ifstream &in, char &ch, const string &s );
  static bool skipComments( ifstream &in, char &ch );
  static bool skipWhitespace( ifstream &in, char &ch );
  static string getString( ifstream &in, char &ch );

  void insert_source( bool (* mainfn)( const string &name ),
                      const string &name, set<string> &both,
                      set<string> &main, set<string> &other );
  static bool has_main_function1( const string &name );
  static bool has_main_label1( const string &name );
  static set<string> include_files1( const string &name );

  static bool hasMainFunction( const string &name );
  static bool hasMainLabel( const string &name );
  static set<string> getIncludeFiles( const string &name );

  static bool hasMainFunction1( const string &name );
  static bool hasMainLabel1( const string &name );
  static set<string> includeFiles1( const string &name );

  FileType file_type( const string &name ) const;

  // file properties

  map<string, set<string> > _includeFiles;
  map<string, set<string> > _dependencies;

}; // DirList

#endif
