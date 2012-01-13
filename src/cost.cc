//
// Author:: Toru Shiozaki
// Date  :: Feb 2009
//
#include <algorithm>
#include "cost.h"

using namespace std;
using namespace boost;


void PCost::add(int i, int j, int k) {
  get<0>(pcost_) += i; 
  get<1>(pcost_) += j;
  get<2>(pcost_) += k;
} 


const string PCost::show() const { 
  string out;
  out += "h" + lexical_cast<string>(get<0>(pcost()))
       + "p" + lexical_cast<string>(get<1>(pcost())) 
       + "P" + lexical_cast<string>(get<2>(pcost())); 
  return out; 
} 


void Cost::sort_pcost() {
  sort(cost_.rbegin(), cost_.rend());
}


const string Cost::show() const { 
  string out;
  for (auto i = cost_.begin(); i != cost_.end(); ++i) out += i->show() + " ";
  return out;
}


