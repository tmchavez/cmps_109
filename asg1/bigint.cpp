#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "bigint.h"
#include "debug.h"
#include "relops.h"


bigint::bigint (long that): uvalue (that), is_negative (that < 0) {
   DEBUGF ('~', this << " -> " << uvalue)
}

bigint::bigint (const ubigint& uvalue, bool is_negative):
                uvalue(uvalue), is_negative(is_negative) {
}

bigint::bigint (const string& that) {
   is_negative = that.size() > 0 and that[0] == '_';
   uvalue = ubigint (that.substr (is_negative ? 1 : 0));
}

bigint bigint::operator+ () const {
   return *this;
}

bigint bigint::operator- () const {
   return {uvalue, not is_negative};
}

bigint bigint::operator+ (const bigint& that) const {
  if(is_negative == that.is_negative){ //check for signs
    if(uvalue < that.uvalue){  //ensure larger num first
      ubigint result = that.uvalue + uvalue;
      return {result,is_negative};
    }else{
      ubigint result = uvalue + that.uvalue;
      return {result,is_negative};
    }
  }else{  //opposite signs - subtract
    if(uvalue < that.uvalue){
      ubigint result = that.uvalue - uvalue;
      return {result,that.is_negative};
    }else{
      ubigint result = uvalue - that.uvalue;
      return {result,is_negative};
    } 
  }
}

bigint bigint::operator- (const bigint& that) const {
  if(is_negative == that.is_negative){
    if(uvalue < that.uvalue){
      ubigint result = that.uvalue - uvalue;
      return {result,that.is_negative};
    }else{
      ubigint result = uvalue - that.uvalue;
      return {result,is_negative};
    }
  }else{
    if(uvalue < that.uvalue){
      ubigint result = that.uvalue + uvalue;
      return {result,is_negative};
    }else{
      ubigint result = uvalue + that.uvalue;
      return {result,is_negative};
    }
  }
}

bigint bigint::operator* (const bigint& that) const {
  if(is_negative == that.is_negative){
    ubigint result = uvalue * that.uvalue;
    if(is_negative == false){
      return {result,is_negative};
    }else{
      return {result,not is_negative};
    }
  }else {
    ubigint result = uvalue * that.uvalue;
    if(is_negative ==  true){
      return {result,is_negative};
    }else{
      return {result,not is_negative};
    }
  }
}

bigint bigint::operator/ (const bigint& that) const {
  if(is_negative == that.is_negative){
    ubigint result = uvalue / that.uvalue;
    if(is_negative == false){
      return {result,is_negative};
    }else{
      return {result,not is_negative};
    }
  }else {
    ubigint result = uvalue / that.uvalue;
    if(is_negative ==  true){
      return {result,is_negative};
    }else{
      return {result,not is_negative};
    }
  }
}

bigint bigint::operator% (const bigint& that) const {
  if(is_negative == that.is_negative){
    ubigint result = uvalue % that.uvalue;
    if(is_negative == false){
      return {result,is_negative};
    }else{
      return {result,not is_negative};
    }
  }else {
    ubigint result = uvalue % that.uvalue;
    if(is_negative ==  true){
      return {result,is_negative};
    }else{
      return {result,not is_negative};
    }
  }
}

bool bigint::operator== (const bigint& that) const {
   return is_negative == that.is_negative and uvalue == that.uvalue;
}

bool bigint::operator< (const bigint& that) const {
   if (is_negative != that.is_negative) return is_negative;
   return is_negative ? uvalue > that.uvalue
                      : uvalue < that.uvalue;
}

ostream& operator<< (ostream& out, const bigint& that) {
  if(that.is_negative){
    out << "-"; 
  }
  return out << that.uvalue;
}

