// $Id: main.cpp,v 1.8 2015-04-28 19:23:13-07 - - $
// util, trace, main, listmap, need changed. (xpair, xlist are good.)
// Look at catfile.cpp: to incorporate ifstream in main.cpp and what if it does not work.
//

#include <cstdlib>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            traceflags::setflags (optarg);
            break;
         default:
            complain() << "-" << (char) optopt << ": invalid option"
                       << endl;
            break;
      }
   }
}

int main (int argc, char** argv) {
   sys_info::set_execname (argv[0]);
   scan_options (argc, argv);

   const string cin_name = "-";
   string progName = sys_info::get_execname();
   string lineArray[1024]; // Lenient allocation of possible number of lines in a file.
   if (argc == 1) {
      cin >> lineArray[0];
   }
   string filenames[argc-1];
   for (int i = 1; i < argc; ++i) {
      filenames[i-1] = argv[i];
   }
   int lineCount = 0;
   for (const auto& it : filenames) {

      if (it == cin_name) {
         readFile(cin, it, lineArray, lineCount);
      }
      else {
         ifstream inFile (it);
         if (inFile.fail()) {
            complain() << it << ": failed to open." << endl;
         }
         else {
            readFile(inFile, it, lineArray, lineCount);
            inFile.close();
            for (int i = 0; i < lineCount; ++i) {
               string currentLine = lineArray[i];
               str_str_map myMap;
               if (currentLine[0] == '#' or currentLine.length() == 0) {
                  continue;
               }
               if (currentLine.find('=') == string::npos) {
                  str_str_map::iterator itor = myMap.find(currentLine);
                  str_str_pair pair = *itor;
                  cout << pair.first << " = " << pair.second << endl;
               }

            }
         }
      }
   }
   for (int i = 0; i < lineCount; ++i) {
      cout << lineArray[i] << endl;
   }



   str_str_map test;
   for (char** argp = &argv[optind]; argp != &argv[argc]; ++argp) {
      str_str_pair pair (*argp, to_string<int> (argp - argv));
      cout << "Before insert: " << pair << endl;
      test.insert (pair);
   }

   for (str_str_map::iterator itor = test.begin();
        itor != test.end(); ++itor) {
      cout << "During iteration: " << *itor << endl;
   }

   str_str_map::iterator itor = test.begin();
   test.erase (itor);

   cout << "EXIT_SUCCESS" << endl;
   return EXIT_SUCCESS;
}

