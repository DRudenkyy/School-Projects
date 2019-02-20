#include <sys/types.h>
#include <ctype.h>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "dirlist.h"

using namespace std;

/* Constructor for Inserter object that just initializes fields
 * of struct
 */
FileListInserter::FileListInserter( const set<string> &f, const char *suf )
  : files(f), suffix(suf) {}

/* Insertion operator for the previous inserter object. Writes a
 * space-separated list of file names in f (with a leading space).
 */
ostream & operator<<( ostream &os, const FileListInserter &ins ) {
  for( set<string>::const_iterator it = ins.files.begin();
       it != ins.files.end();
       ++it ) {
    if( it != ins.files.begin() ) {
      os << " "; // check if before first item
    }
    if( ins.suffix ) {
      os << DirList::basename( *it ) << ins.suffix;
    } else {
      os << *it;
    }
  }
  return os;
}

/* DirList
 *
 * Read the current directory, and categorize the files found there
 * according to their suffix, saving them on the appropriate list.
 * If command line arguments were given, read that list to find
 * .C, .cpp, .c, and .s files, and ignore those found in the directory.
 * We always read .h files.
 */
DirList::DirList( int ac, char **av ){
  //
  // Read the directory and store all the files we find there,
  // EXCEPT if arguments were given, those are the only .C, .c, .cpp, and .s
  // files we're interested in, so skip any found here.
  //
  DIR *dir( opendir( "." ) );

  if( dir != 0 ){
    struct dirent *dirent;

    while( ( dirent = readdir( dir ) ) != 0 ) {
      string name( dirent->d_name );

      switch( file_type( name ) ) {
      case CPP:
        if( ac <= 0 ) {
          insert_source( hasMainFunction, name, cpp, cpp_main, cpp_other );
        }
        break;

      case C:
        if( ac <= 0 ) {
          insert_source( hasMainFunction, name, c, c_main, c_other );
        }
        break;

      case ASSEMBLY:
        if( ac <= 0 ) {
          insert_source( hasMainLabel, name, ass, ass_main, ass_other );
        }
        break;

      case H:
        h.insert( name );
        break;

      case ARCHIVE:
        archive.insert( name );
        break;

      default:;
        // nothing to do
      }

    }
    closedir( dir );
  } else {
    cerr << "Error opening current directory\n";
  }

  //
  // Copy the names from the argument list into their vector.
  //
  while( ac-- > 0 ) {
    string name( *av++ );

    switch( file_type( name ) ) {
    case CPP:
      insert_source( hasMainFunction, name, cpp, cpp_main, cpp_other );
      break;

    case C:
      insert_source( hasMainFunction, name, c, c_main, c_other );
      break;

    case ASSEMBLY:
      insert_source( hasMainLabel, name, ass, ass_main, ass_other );
      break;

    default:
      cerr << av[ -1 ] << ": ignored, not C/C++ or assembly" << endl;
    }
  }
}

/* insert_source
 *
 *  Insert a filename into the 'main' or 'other' list, depending on
 * whether it contains a main function.  Also insert it into the
 * 'both' list, too.
 */
void DirList::insert_source( bool (* mainfn)( const string &name ),
                             const string &name,
                             set<string> &both,
                             set<string> &main,
                             set<string> &other ) {
  both.insert( name );
  if( mainfn( name ) ) {
    main.insert( name );
  } else {
    other.insert( name );
  }
}

/* basename
 *
 * Return the basename (without a suffix) of name.
 */
const string DirList::basename( const string &name ) {
  size_t pos = name.rfind('.');
  if( pos == string::npos ) return name;
  return name.substr( 0, pos );
}

/* includeFiles
 *
 * Get a list of include files mentioned in file name. Cache the result
 * so successive calls do no work.
 */
set<string> DirList::includeFiles( const string &name ) {
  map<string,set<string> >::const_iterator it( _includeFiles.find( name ) );
  if( it != _includeFiles.end() ) {
    return it->second;
  } else {
    return _includeFiles[ name ] = getIncludeFiles( name );
  }
}

/*dependencies
 *
 * Do a transitive closure of include files included by file name.
 * Cache the result so successive calls do no work.
 */
set<string> DirList::dependencies( const string &name ) {
  map<string, set<string> >::iterator it = _dependencies.find( name );
  if( it != _dependencies.end() ) {
    return it->second;
  } else {
    set<string> todo( getIncludeFiles( name ) );
    set<string> closure;
    while( !todo.empty() ) {
      string s = *todo.begin();
      todo.erase( todo.begin() );
      if( closure.insert( s ).second ) {
        set<string> newfiles( getIncludeFiles( s ) );
        todo.insert( newfiles.begin(), newfiles.end() );
      }
    }
    _dependencies[ name ] = closure;
    return closure;
  }
}

/*
 * has_main_function
 *
 * Scan a file to see if it contains a main function.
 * This is only a partial job, because to really do it right
 * takes about the same amount of work as compiling the file.
 *
 * 1) ignore C++ style comments,
 * 2) ignore C style comments (fails if the file has strings
 *    that contain / * or * /  ),
 * 3) then look for the pattern:
 *  (^|ws) "int" ws "main" ws* "("
 */
bool DirList::has_main_function1( const string &name ) {
  ifstream in( name.c_str() );

  if( !in ) {
    return 0;
  }

  //
  // The finite state machine
  //
  enum { START_OF_WORD, // at the beginning of a new word
         IN_WORD, // somewhere within a word
         START_COMMENT, // found a slash
         CPP_COMMENT, // within a //-style comment
         C_COMMENT, // within a /* */-style comment
         LEAVING_C_COMMENT, // found a * within a /* /-style comment
         INTMAIN  // finding chars of "int main"
  } state( START_OF_WORD );
  static char intmain[] = "int\001\002main\002(";
  char ch;
  int count = 0;

  while( in.get( ch ) ) {
    switch( state ) {
    case START_OF_WORD: // at the beginning of a new line
      if( ch == '/' ) {
        state = START_COMMENT;
      } else if( ch == 'i' ) {
        state = INTMAIN;
        count = 1;
      } else if( !isspace( ch ) ) {
        state = IN_WORD;
      }
      break;

    case IN_WORD: // somewhere within a line
      if( isspace( ch ) ) {
        state = START_OF_WORD;
      } else if( ch == '/' ) {
        state = START_COMMENT;
      }
      break;

    case START_COMMENT: // found a slash
      if( ch == '/' ) {
        state = CPP_COMMENT;
      } else if( ch == '*' ) {
        state = C_COMMENT;
      } else if( isspace( ch ) ) {
        state = START_OF_WORD;
      } else {
        state = IN_WORD;
      }
      break;

    case CPP_COMMENT: // within a //-style comment
      if( ch == '\n' ) {
        state = START_OF_WORD;
      }
      break;

    case C_COMMENT: // within a /* */-style comment
      if( ch == '*' ) {
        state = LEAVING_C_COMMENT;
      }
      break;

    case LEAVING_C_COMMENT: // found a * within a /* /-style comment
      if( ch == '/' ) {
        state = IN_WORD;
      } else if( ch != '*' ) {
        state = C_COMMENT;
      }
      break;

    case INTMAIN: // finding chars of "int main"
      if( intmain[ count ] == '\002' ) {
        if( !isspace( ch ) ) {
          count += 1;
        }
      }
      if( isprint( intmain[ count ] ) ) {
        //
        // printable chars in this string are
        // the "int" and "main" and "(".  They
        // must match our current character.
        //
        if( ch == intmain[ count ] ) {
          //
          // Matched.  Are we done?
          //
          if( intmain[ count ] == '(' ) {
            return 1;
          }

          //
          // Not done; stay in same state
          //
          count += 1;
        } else {
          if( isspace( ch ) ) {
            state = START_OF_WORD;
          } else {
            state = IN_WORD;
          }
        }
      } else if( intmain[ count ] == '\001' ) {
        //
        // Matches 1 space character
        //
        if( isspace( ch ) ) {
          count += 1;
        } else {
          if( isspace( ch ) ) {
            state = START_OF_WORD;
          } else {
            state = IN_WORD;
          }
        }
      }
      break;
    }
  }
  return 0;
}

/*
 * has_main_label
 *
 * Scan a file to see if it contains an entry point main.
 * This is only a partial job, because to really do it right
 * takes about the same amount of work as compiling the file.
 *
 * 1) ignore C++ style comments,
 * 2) ignore C style comments (fails if the file has strings
 *    that contain / * or * /  ),
 * 3) then look for the pattern:
 *  (^|ws)  "main" ws* ":"
 */
bool DirList::has_main_label1( const string &name ) {
  ifstream in( name.c_str() );

  if( !in ) {
    return 0;
  }

  //
  // The finite state machine
  //
  enum { START_OF_WORD, // at the beginning of a new word
         IN_WORD, // somewhere within a word
         START_COMMENT, // found a slash
         CPP_COMMENT, // within a //-style comment
         C_COMMENT, // within a /* */-style comment
         LEAVING_C_COMMENT, // found a * within a /* /-style comment
         INTMAIN  // finding chars of "main :"
  } state( START_OF_WORD );
  static char mainc[] = "main\002:";
  char ch;
  int count = 0;

  while( in.get( ch ) ) {
    switch( state ) {
    case START_OF_WORD: // at the beginning of a new line
      if( ch == '/' ) {
        state = START_COMMENT;
      } else if( ch == 'm' ) {
        state = INTMAIN;
        count = 1;
      } else if( !isspace( ch ) ) {
        state = IN_WORD;
      }
      break;

    case IN_WORD: // somewhere within a line
      if( isspace( ch ) ) {
        state = START_OF_WORD;
      } else if( ch == '/' ) {
        state = START_COMMENT;
      }
      break;

    case START_COMMENT: // found a slash
      if( ch == '/' ) {
        state = CPP_COMMENT;
      } else if( ch == '*' ) {
        state = C_COMMENT;
      } else if( isspace( ch ) ) {
        state = START_OF_WORD;
      } else {
        state = IN_WORD;
      }
      break;

    case CPP_COMMENT: // within a //-style comment
      if( ch == '\n' ) {
        state = START_OF_WORD;
      }
      break;

    case C_COMMENT: // within a /* */-style comment
      if( ch == '*' ) {
        state = LEAVING_C_COMMENT;
      }
      break;

    case LEAVING_C_COMMENT: // found a * within a /* /-style comment
      if( ch == '/' ) {
        state = IN_WORD;
      } else if( ch != '*' ) {
        state = C_COMMENT;
      }
      break;

    case INTMAIN: // finding chars of "int main"
      if( mainc[ count ] == '\002' ) {
        if( !isspace( ch ) ) {
          count += 1;
        }
      }
      if( isprint( mainc[ count ] ) ) {
        //
        // printable chars in this string are
        // the "int" and "main" and ":".  They
        // must match our current character.
        //
        if( ch == mainc[ count ] ) {
          //
          // Matched.  Are we done?
          //
          if( mainc[ count ] == ':' ) {
            return 1;
          }

          //
          // Not done; stay in same state
          //
          count += 1;
        } else {
          if( isspace( ch ) ) {
            state = START_OF_WORD;
          } else {
            state = IN_WORD;
          }
        }
      } else if( mainc[ count ] == '\001' ) {
        //
        // Matches 1 space character
        //
        if( isspace( ch ) ) {
          count += 1;
        } else {
          if( isspace( ch ) ) {
            state = START_OF_WORD;
          } else {
            state = IN_WORD;
          }
        }
      }
      break;
    }
  }
  return 0;
}

/*
 * include_files
 *
 * Construct a list of all files that are included directly by this one.
 * ********** should ignore comments
 */
set<string> DirList::include_files1( const string &name ) {
  //
  // Read the file, looking for this pattern:
  // ^(ws*)#(ws*)include(ws*)"name"
  // For each entry found, add the name to the filelist.
  // A finite state machine is used to recognize these.
  //
  ifstream in( name.c_str() );
  set<string> list;
  char incl_name[256];
  char ch;
  enum { BEGINNING, GOT_POUND, GOT_INCLUDE, GARBAGE } 
  state( BEGINNING );

  while( in.get( ch ) ) {
    if( ch == '\n' ) {
      state = BEGINNING;
    }
    switch( state ) {
    case BEGINNING:
      //
      // Beginning of the line
      //
      if( ch == '#' ) {
        state = GOT_POUND;
      } else if( !isspace( ch ) ) {
        state = GARBAGE;
      }
      break;

    case GOT_POUND:
      //
      // Found the pound sign.
      //
      if( isspace( ch ) ) {
        continue;
      }
      state = GARBAGE;
      if( ch == 'i' && in.get( ch ) &&
          ch == 'n' && in.get( ch ) &&
          ch == 'c' && in.get( ch ) &&
          ch == 'l' && in.get( ch ) &&
          ch == 'u' && in.get( ch ) &&
          ch == 'd' && in.get( ch ) &&
          ch == 'e' && in.get( ch ) ) {
        if( ch == '"' ) {
          in.putback( ch );
          ch = ' ';
        }
        if( isspace( ch ) ) {
          state = GOT_INCLUDE;
        } 
      }
      break;

    case GOT_INCLUDE:
      //
      // Found #include, now waiting to get the name
      //
      if( isspace( ch ) ) {
        continue;
      }
      if( ch == '"' ) {
        in.get( incl_name, 256, '"' );
        list.insert( incl_name );
      }
      state = GARBAGE;
      break;

    case GARBAGE:
      //
      // Not what we want; skip chars until the end of
      // the line.
      //
      break;
    }
  }
  return list;
}

/*file_type
 *
 * Return a member of the file_type enumeration corresponding to
 * the suffix in name.
 */
DirList::FileType DirList::file_type( const string &name ) const {
  if( name[ 0 ] == '.' ) return OTHER;
  size_t pos = name.rfind('.');
  if( pos == string::npos ) return OTHER;
  string suffix( name.substr( pos+1, name.size() ) );
  FileType ft = OTHER;
  if( suffix == "C" || suffix == "cpp" ) ft = CPP;
  if( suffix == "c" ) ft = C;
  if( suffix == "s" ) ft = ASSEMBLY;
  if( suffix == "H" || suffix == "h" ) ft =  H;
  if( suffix == "a" ) ft = ARCHIVE;
  if( ft == OTHER ) return OTHER;
  if( name.find('#') != string::npos ) {
    cerr << "Warning: ignoring file whose name contains '#': "
         << name << endl;
    return OTHER;
  }
  return ft;
}

/*******************Experimental*********************/

/*
 * Matches a given string or stops on first character of mismatch.
 */
bool DirList::matchString( ifstream &in, char &ch, const string &s ) {
  int n = s.size();
  for( int i = 0; i < n && in; i++, in.get( ch ) ) {
    if( s[i] != ch ) {
      return false;
    }
  }
  return (bool)in; // true if stream still good
}

/*
 * Skips a possible comment.
 *
 * May absorb a '/' and return false.
*/
bool DirList::skipComments( ifstream &in, char &ch ) {
  if( ch == '/' && in ) {
    if( in.get( ch ) && ch == '/' ) {
      while( in.get( ch ) && ch != '\n' ) {/* empty */}
      in.get( ch ); // pass end-of-line
      return true;
    } else if( ch == '*' ) {
      if( in.get( ch ) ) {
        do {
          while( ch != '*' && in.get( ch ) ) {/* empty */}
          while( ch == '*' && in.get( ch ) ) {/* empty */}
        } while( ch != '/' && in );
      }
      return true;
    } else {
      return false;
    }
  }
  return false;
}

/*
 * Skips possible whitespace.
 */
bool DirList::skipWhitespace( ifstream &in, char &ch ) {
  bool skipped = false;
  while( isspace( ch ) && in ) {
    in.get( ch );
    skipped = true;
  }
  return skipped;
}

/*
 * Returns quoted string.
 *
 * Assumes that ch contains the quote character.
 */
string DirList::getString( ifstream &in, char &ch ) {
  char quote = ch;
  ostringstream os;
  while( in.get( ch ) && ch != quote && ch != '\n' ) {
    os << ch;
  }
  if( ch == quote ) {
    return os.str();
  } else {
    return string();
  }
}

/*
 * has_main_function
 *
 * Scan a file to see if it contains a main function.
 * This is only a partial job, because to really do it right
 * takes about the same amount of work as compiling the file.
 *
 * 1) ignore C++ style comments,
 * 2) ignore C style comments (fails if the file has strings
 *    that contain / * or * /  ),
 * 3) then look for the pattern:
 *  (^|ws) "int" ws "main" ws* "("
 */
bool DirList::hasMainFunction1( const string &name ) {
  ifstream in( name.c_str() );
  char ch;

  in.get( ch );
  while( in ) {
    while( skipWhitespace( in, ch ) || skipComments( in, ch ) ) {/* empty */}
    if ( matchString( in, ch, "int" ) ) {
      if( isspace( ch ) ) {
        while( skipWhitespace( in, ch ) ) {/* empty */}
        if( matchString( in, ch, "main" ) ) {
          while( skipWhitespace( in, ch ) ) {/* empty */}
          if( matchString( in, ch, "(" ) ) {
            return true;
          }
        }
      }
    }
    in.get( ch );
  }
  return false;
}

/*
 * has_main_label
 *
 * Scan a file to see if it contains a main label.
 * This is only a partial job, because to really do it right
 * takes about the same amount of work as compiling the file.
 *
 * 1) ignore C++ style comments,
 * 2) ignore C style comments (fails if the file has strings
 *    that contain / * or * /  ),
 * 3) then look for the pattern:
 *  (^|ws) "main" (space|tab)* ":"
 */
bool DirList::hasMainLabel1( const string &name ) {
  ifstream in( name.c_str() );
  char ch;

  in.get( ch );
  while( in ) {
    while( skipWhitespace( in, ch ) || skipComments( in, ch ) ) {/* empty */}
    if ( matchString( in, ch, "main" ) ) {
      while( skipWhitespace( in, ch ) ) {/* empty */}
      if( matchString( in, ch, ":" ) ) {
        return true;
      }
    } else {
      in.get( ch );
    }
  }
  return false;
}

set<string> DirList::includeFiles1( const string &name ) {
  //
  // Read the file, looking for this pattern:
  // ^(ws*)#(ws*)include(ws*)"name"
  // For each entry found, add the name to the filelist.
  // A finite state machine is used to recognize these.
  //
  ifstream in( name.c_str() );
  set<string> list;
  ostringstream os;
  char ch;

  in.get( ch );
  while( in ) { // at beginning of line
    skipWhitespace( in, ch );
    if( matchString( in, ch, "#" ) ) {
      skipWhitespace( in, ch );
      if( matchString( in, ch, "include" ) ) {
        skipWhitespace( in, ch );
        if( ch == '"' ) {
          //in.get( ch );
          string name = getString( in, ch );
          if( name.size() != 0 ) {
            list.insert( name );
          }
        }
      }
    }
    while( ch != '\n' && in.get( ch ) ) {/* empty */}
    in.get(ch);
  }
  return list;
}

bool DirList::hasMainFunction( const string &name ) {
  bool b1 = DirList::has_main_function1( name );
  bool b2 = hasMainFunction1( name );
  if( b1 != b2 ) {
    cerr << "Error: difference between hasMainFunction: "
         << b1 << " " << b2 << endl;
  }
  return b2;
}

bool DirList::hasMainLabel( const string &name ) {
  bool b1 = DirList::has_main_label1( name );
  bool b2 = hasMainLabel1( name );
  if( b1 != b2 ) {
    cerr << "Error: difference between hasMainLabel: "
         << b1 << " " << b2 << endl;
  }
  return b2;
}

set<string> DirList::getIncludeFiles( const string &name ) {
  set<string> s1 = DirList::include_files1( name );
  set<string> s2 = includeFiles1( name );
  if( s1 != s2 ) {
    cerr << "Error: include files: " << s1.size() << " " << s2.size() << endl;
  }
  return s2;
}
