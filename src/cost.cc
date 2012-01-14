//
// Author : Toru Shiozaki
// Date   : Feb 2009
//

#include <algorithm>
#include <sstream>
#include <src/cost.h>

using namespace std;


void PCost::add(int i, int j, int k) {
  get<0>(pcost_) += i;
  get<1>(pcost_) += j;
  get<2>(pcost_) += k;
}


const string PCost::show() const {
  stringstream out; out << "h" << get<0>(pcost()) << "p" << get<1>(pcost()) << "P" << get<2>(pcost());
  return out.str();
}


void Cost::sort_pcost() {
  sort(cost_.rbegin(), cost_.rend());
}


const string Cost::show() const {
  string out;
  for (auto i = cost_.begin(); i != cost_.end(); ++i) out += i->show() + " ";
  return out;
}


