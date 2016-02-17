// $Id: util.cpp,v 1.7 2015-05-01 17:06:25-07 - - $

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <stdexcept>
#include <string>
using namespace std;

#include "util.h"

int sys_info::exit_status = EXIT_SUCCESS;
string sys_info::execname; // Must be initialized from main().

void sys_info_error (const string& condition) {
   throw logic_error ("main() has " + condition
               + " called sys_info::set_execname()");
}

void sys_info::set_execname (const string& argv0) {
   if (execname != "") sys_info_error ("already");
   int slashpos = argv0.find_last_of ('/') + 1;
   execname = argv0.substr (slashpos);
   cout << boolalpha;
   cerr << boolalpha;
   TRACE ('u', "execname = " << execname);
}

const string& sys_info::get_execname () {
   if (execname == "") sys_info_error ("not yet");
   return execname;
}

void sys_info::set_exit_status (int status) {
   if (execname == "") sys_info_error ("not yet");
   exit_status = status;
}

int sys_info::get_exit_status () {
   if (execname == "") sys_info_error ("not yet");
   return exit_status;
}

const string datestring () {
   time_t clock = time (nullptr);
   struct tm *tm_ptr = localtime (&clock);
   char timebuf[256];
   strftime (timebuf, sizeof timebuf,
             "%a %b %e %H:%M:%S %Z %Y", tm_ptr);
   return timebuf;
}


list<string> split (const string& line, const string& delimiters) {
   list<string> words;
   size_t end = 0;
   // Loop over the string, splitting out words, and for each word
   // thus found, append it to the output list<string>.
   for (;;) {
      size_t start = line.find_first_not_of (delimiters, end);
      if (start == string::npos) break;
      end = line.find_first_of (delimiters, start);
      words.push_back (line.substr (start, end - start));
   }
   TRACE ('u', words);
   return words;
}

ostream& complain() {
   sys_info::set_exit_status (EXIT_FAILURE);
   cerr << sys_info::get_execname () << ": ";
   return cerr;
}

void syscall_error (const string& object) {
   complain() << object << ": " << strerror (errno) << endl;
}

void readFile (istream& infile, string lineArray[], int& lineCount) {
   for (; ;) {
      string space = " \t";
      string line;
      getline(infile, line);
      if (infile.eof()) break;
      //cout << filename << ": " << lineCount << ": " << line << endl;
      string lineTrimmed;
      trim (line, space, lineTrimmed);
      //cout << lineTrimmed << endl;
      lineArray[lineCount] = lineTrimmed;
      //cout << lineArray[lineCount-1] << endl;
      ++lineCount;
   }
}

void trim (string& line, string space, string& lineTrimmed) {
   unsigned long lineBegin = line.find_first_not_of(space);
   if (lineBegin == string::npos) {
      lineTrimmed = "";
      return;
   }
   unsigned long lineEnd = line.find_last_not_of(space);
   lineTrimmed = line.substr(lineBegin, lineEnd - lineBegin + 1);
   if (lineTrimmed == "=") {
      return;
   }
   auto foundEqual = lineTrimmed.find('=');
   if (foundEqual != string::npos) {
      //if (foundEqual == 0)
      string keyName = lineTrimmed.substr(0, foundEqual);
      keyName =
              keyName.substr(0, (keyName.find_last_not_of(space) + 1));
      string valueName = lineTrimmed.substr(foundEqual + 1);
      if (valueName.length() == 0) {
         lineTrimmed = keyName + "=";
         return;
      }
      valueName = valueName.substr(valueName.find_first_not_of(space));
      lineTrimmed = keyName + "=" + valueName;
   }
}

