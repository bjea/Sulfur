// $Id: main.cpp,v 1.8 2015-04-28 19:23:13-07 - - $
// util, main, listmap, need changed. (xpair, xlist are good.)
// Look at catfile.cpp: to incorporate ifstream in main.cpp
// and what if it does not work.

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
   // Lenient allocation of possible number of lines in a file.
   string lineArray[1024];
   if (argc == 1) {
      cin >> lineArray[0];
   }
   string filenames[argc-1];
   for (int i = 1; i < argc; ++i) {
      filenames[i-1] = argv[i];
   }

   str_str_map myMap;
   for (const auto& it : filenames) {
      int lineCount = 0;
      if (it == cin_name) {
         readFile(cin, /*it,*/ lineArray, lineCount);
      }
      else {
         ifstream inFile(it);
         if (inFile.fail()) {
            complain() << it << ": failed to open." << endl;
         }
         else {
            readFile(inFile, /*it,*/ lineArray, lineCount);
            inFile.close();
         }
      }
      for (int i = 0; i < lineCount; ++i) {
         string currentLine = lineArray[i];
         cout << it << ": " << i+1 << ": " << currentLine << endl;

         if (currentLine[0] == '#' or currentLine.length() == 0) {
            continue;
         }

         auto foundEqual = currentLine.find('=');

         if (foundEqual == string::npos) {
            str_str_map::iterator itor =
                    myMap.find(currentLine);
            if (itor != myMap.end()) {
               str_str_pair pair = *itor;
               cout << pair.first << " = " << pair.second << endl;
            }
            else {
               cout << currentLine << ": key not found" << endl;
            }
         }
         else {
            // Handling case: nothing after "key=",
            // erase the {key, value} pair.
            if (foundEqual == currentLine.length()-1) {
               string keyName = currentLine.substr(0, foundEqual);
               str_str_map::iterator itor =
                       myMap.find(keyName);
               if (itor != myMap.end()) {
                  myMap.erase(itor);
               }
            }
            // Handling case: only "=" in a line,
            // print out entire listmap.
            if (currentLine == "=") {
               for (str_str_map::iterator itor = myMap.begin();
                    itor != myMap.end(); ++itor) {
                  str_str_pair pair = *itor;
                  cout << pair.first << " = "
                       << pair.second << endl;
               }
            }
            // Handling case: only "=value" in a line,
            // no key specified, print out all the
            // {key, value} pairs with target "value".
            if (foundEqual == 0 && currentLine.length() > 1) {
               string valueName =
                       currentLine.substr(foundEqual+1);
               for (str_str_map::iterator itor = myMap.begin();
                    itor != myMap.end(); ++itor) {
                  str_str_pair pair = *itor;
                  if (pair.second == valueName) {
                     cout << pair.first << " = "
                          << pair.second << endl;
                  }
               }
            }
            // Handling case: "key=value", if "key" not found,
            // add {key, value} to the listmap; if key found,
            // replace existing value with "value".
            if (foundEqual != 0 && currentLine.length() > 2 &&
                    foundEqual != currentLine.length()-1) {
               string keyName = currentLine.substr(0, foundEqual);
               string valueName =
                       currentLine.substr(foundEqual+1);
               str_str_map::iterator itor =
                       myMap.find(keyName);
               if (itor == myMap.end()) {
                  str_str_pair pair(keyName, valueName);
                  itor = myMap.insert(pair);
                  cout << itor->first << " = "
                       << itor->second << endl;
               }
               else {
                  itor->second = valueName;
                  cout << itor->first << " = "
                       << itor->second << endl;
               }
            }
         }
      }
   }

   myMap.~listmap();
   //cout << "EXIT_SUCCESS" << endl;
   return EXIT_SUCCESS;
}

