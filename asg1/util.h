#ifndef __UTIL_H__
#define __UTIL_H__

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
using namespace std;

#include "debug.h"

//ydc, more comments

class ydc_exn: public runtime_error {
   public:
      explicit ydc_exn (const string& what);
};

//octal

template <typename numeric>
const string octal (numeric number) {
   ostringstream stream;
   stream << showbase << oct << number;
   return stream.str();
}

//main

class exec {
   private:
      static string execname_;
      static int status_;
      static void execname (const string& argv0);
      friend int main (int, char**);
   public:
      static void status (int status);
      static const string& execname() {return execname_; }
      static int status() {return status_; }
};

//complain 

ostream& note();
ostream& error();

#endif

