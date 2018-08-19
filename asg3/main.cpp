#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>
#include <regex>
#include <fstream>

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
            debugflags::setflags (optarg);
            break;
         default:
            complain() << "-" << char (optopt) << ": invalid option"
                       << endl;
            break;
      }
   }
}

int main (int argc, char** argv) {
   sys_info::execname (argv[0]);
   scan_options (argc, argv);

   for(int i = 1; i < argc; i++){ //main for loop for files
   ifstream inFile(argv[i]);
   regex comment_regex {R"(^\s*(#.*)?$)"};
   regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
   regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};
   string line;
   str_str_map dalist;
   int linenum = 0;
   while (getline (inFile , line)) {
      linenum++;
      cout<< argv[i] << ": " << linenum  << ": "<< line;
      if (cin.eof()) break;
      cout << endl;
      smatch result;
      if (regex_search (line, result, comment_regex)) {
         //do nothing on comments
         continue;
      }
      if (regex_search (line, result, key_value_regex)) { 
        //anyline with '=', most of the work is done here
        string fir = result[1]; int fs = fir.size();
        string sec = result[2]; int ss = sec.size();
        str_str_pair dapair(fir,sec);
        if((fs != 0) and (ss != 0)){
          //case where a new key is inserted
          cout << result[1] << " = " << result[2] << endl;
          dalist.insert ( dapair );
        }
        if((fs != 0) and (ss == 0)){
          //delete an existing key
          str_str_map::iterator temp = dalist.find(fir);
          if(temp != dalist.end()){ 
            dalist.erase(temp);
         }else{cout << "key not found" << endl;}
        }
        if((fs == 0) and (ss == 0)){
          //print the list
          str_str_map::iterator temp = dalist.begin(); 
          while(temp != dalist.end() ){
            cout << *temp << endl;
            ++temp;
         }
        }
        if((fs == 0) and (ss != 0)){
          //print all pairs with respective value
          str_str_map::iterator temp = dalist.begin();
          while(temp != dalist.end() ){
            if(temp->second == sec) { cout<< *temp << endl;}
            ++temp;
          }
        }
      }else if (regex_search (line, result, trimmed_regex)) {
         //final case, check for key in list
         string key1 = result[1];
         str_str_map::iterator temp = dalist.find(key1);
         if(temp == dalist.end() ){
           cout << key1 << ": key not found" << endl;
         }else{
           cout << *temp << endl;
         }
      }else {
         cout <<  "This can not happen." << endl;
      }
   }
   inFile.close();
   }
   return 0;
}


