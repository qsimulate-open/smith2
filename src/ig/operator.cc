//
// Author:: Toru Shiozaki
// Date  :: Apr 2009
//
 
#include <src/ig/operator.h>
#include <sstream>

using namespace std;
using namespace IG;

Operator::Operator(const string symbl, const int rnk, const bool gnrl, const bool de)
 : symbol_(symbl), rank_(rnk), general_(gnrl), deexcitation_(de) {

}

Operator::~Operator() {

}

const string Operator::show() const {
  stringstream out; 
  out << symbol_ << rank_;
  return out.str();
}

