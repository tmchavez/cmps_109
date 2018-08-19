#include <cctype>
#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
#include <bits/stdc++.h>
#include <sstream>
using namespace std;

#include "ubigint.h"
#include "debug.h"

ubigint::ubigint (unsigned long that): ubig_value (that) {
   DEBUGF ('~', this << " -> " << ubig_value[0])
}

ubigint::ubigint (const string& that): ubig_value(0) {
   DEBUGF ('~', "that = \"" << that << "\"");
   for (char digit: that) {
      if (not isdigit (digit)) {
         throw invalid_argument ("ubigint::ubigint(" + that + ")");
      }
      //uvalue = uvalue * 10 + digit - '0';
      //cout << digit << " is pushed back to vector " << endl;
      ubig_value.push_back (digit);
   }
}

ubigint ubigint::operator+ (const ubigint& that) const {
  std::string rs;       //rs for return string
  int carry = 0;
  int msize = ubig_value.size() - 1;     //size of called upon bigint
  int diff = ubig_value.size() - that.ubig_value.size(); 
 //used to itterate through second ubigvalue properly
  //cout << "diff in size is " << diff << endl;             
  //cout << "msize = " << msize << endl;
  for(int i = msize; i >= 0; i--){  //larger int must be used first
    int adig = ubig_value[i] - 48;   //sub 48 since ascii
    int bdig = that.ubig_value[i-diff] - 48;
    if( bdig < 0 ){
      bdig = 0;
    }
    int cdig = adig + bdig + carry;
    //cout << "a = " << adig << " b = " << bdig << endl;
    //cout << "c = " << cdig << endl;
    if(cdig > 9){
      carry = 1;
      cdig = cdig - 10;
      char fdig = cdig+48;    //add 48 to return to ascii values
      rs += fdig;
      if(i == 0)
       rs += '1';            //in case where a new digit is neccesary 
    }else{
      carry = 0;
      char gdig = cdig+48;
      rs += gdig;
      //cout << cdig << " "  << rs << " got pushed" << endl;
    }
  }
  reverse(rs.begin(),rs.end());
  cout << "rs is " << rs << endl;
  return ubigint(rs);
}
  //return ubigint (ubig_value[0]);
  // return ubigint (uvalue + that.uvalue);


ubigint ubigint::operator- (const ubigint& that) const {
  if (*this < that) throw domain_error ("ubigint::operator-(a<b)");
  std::string rs; 
  int carry = 0;
  int msize = ubig_value.size() - 1;
  int diff = ubig_value.size() - that.ubig_value.size();
  for(int i = msize; i >= 0; i--){  //larger int must be used first
    int adig = ubig_value[i] - 48;
    int bdig = that.ubig_value[i-diff] - 48;
    if( bdig < 0 ){
      bdig = 0;
    }
    int cdig = adig - bdig; 
    cdig = cdig - carry;
    //cout << "a = " << adig << " b = " << bdig << endl;
    //cout << "c = " << cdig << endl;
    if(cdig < 0){
      carry = 1;
      cdig = cdig + 10;
      char fdig = cdig+48;
      rs += fdig;
      }else{
      carry = 0;
      char gdig = cdig+48;
      rs += gdig;
      cout << cdig << " "  << rs << " got pushed" << endl;
    }
  }
  reverse(rs.begin(),rs.end());
  cout << "rs is " << rs << endl;
  return ubigint(rs);
}


ubigint ubigint::operator* (const ubigint& that) const {
  std::string rs;
  int size = that.ubig_value.size()-1;
  for(int i = size; i >= 0; i--){  
    char num = ubig_value[i];
    rs += num;
  }
  reverse(rs.begin(),rs.end());
  //cout << rs << endl;
  stringstream conv(rs);
  int actnum = 0;
  conv >> actnum;
  return that; 
  //return ubigint (uvalue * that.uvalue);
}

void ubigint::multiply_by_2() {
  cout << "in mult by 2" << endl;
  int msize = ubig_value.size() - 1;
  int carry = 0;
  for(int i = msize; i >= 0; i--){
     int current = ubig_value[i] - 48;
     cout << "before" << current <<  endl;
     current = current * 2;
     current = current + carry;
     carry = 0;
     while(current > 9){
       current = current -10;
       carry = carry +1;
     }
     cout << "after" << current <<  endl;
     ubig_value[i] = current + 48;
  }
}

void ubigint::divide_by_2() {
  cout << "indiv by 2" << endl;
  int msize = ubig_value.size() - 1;
  for(int i = msize; i >= 0; i--){
    int current = ubig_value[i] - 48;
    if(current % 2 == 1){
      if(i != msize){
        ubig_value[i] += 5;
      }
    }
    current /= 2;
    ubig_value[i] = current +48;
  }
}

struct quo_rem { ubigint quotient; ubigint remainder; };
quo_rem udivide (const ubigint& dividend, ubigint divisor) {
  // Note: divisor is modified so pass by value (copy).
   ubigint zero {0};
   if (divisor == zero) throw domain_error ("udivide by zero");
   ubigint power_of_2 {1};
   ubigint quotient {0};
   ubigint remainder {dividend}; // left operand, dividend
   while (divisor < remainder) {
      divisor.multiply_by_2();
      power_of_2.multiply_by_2();
   }
   while (power_of_2 > zero) {
      if (divisor <= remainder) {
         remainder = remainder - divisor;
         quotient = quotient + power_of_2;
      }
      divisor.divide_by_2();
      power_of_2.divide_by_2();
   }
   return {.quotient = quotient, .remainder = remainder};
}

ubigint ubigint::operator/ (const ubigint& that) const {
  return that;
  //return udivide (*this, that).quotient;
}

ubigint ubigint::operator% (const ubigint& that) const {
  return that; 
  //return udivide (*this, that).remainder;
}

bool ubigint::operator== (const ubigint& that) const {
  if(ubig_value.size() != that.ubig_value.size()){
    return 0;
  }else{
    int msize = ubig_value.size() - 1;
    for(int i = 0; i < msize; i++){  //larger int must be used first
      int adig = ubig_value[i] - 48;
      int bdig = that.ubig_value[i] - 48;
      if(adig != bdig){
        return 0;
      }
  }
  return 1;
  }
}

bool ubigint::operator< (const ubigint& that) const {
  //cout << "size 1 = " << ubig_value.size();
  //cout << "size 2 = " << that.ubig_value.size() << endl;
  if(ubig_value.size() < that.ubig_value.size()){ 
    return 1; //return true if size is <
  }else if(ubig_value.size() > that.ubig_value.size()){
    return 0; //return false if size is >
  }else{
   int msize = ubig_value.size() - 1;
   for(int i = 0; i <= msize; i++){  //larger int must be used first
     int adig = ubig_value[i] - 48;
     int bdig = that.ubig_value[i] - 48;
     //cout << adig << " compared to " << bdig << endl;
     if(adig < bdig){
       //cout << "this return " << endl;
       return 1; //return true at first digit that is <
     }
     if(adig > bdig){
      return 0; //vice versa
    }
  }

  return 0; //return false if they are same
  }
}

ostream& operator<< (ostream& out, const ubigint& that) { 
  int msize = that.ubig_value.size() - 1;
  for(int i = 0; i <= msize; i++){  
   cout << that.ubig_value[i];
  }
  return out;// << "ubigint(" << that.ubig_value.back() << ")";
}

