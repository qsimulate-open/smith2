//
// Author:: Toru Shiozaki
// Date  :: Apr 2009
//
 
#include <src/ig/operator.h>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;
using namespace IG;

Operator::Operator(const string symbl, const int rnk, const bool gnrl, const bool de)
 : symbol_(symbl), rank_(rnk), general_(gnrl), deexcitation_(de) {

}

Operator::~Operator() {

}

const string Operator::show() const {
  string out; 
  out += symbol_ + lexical_cast<string>(rank_);
  return out;
}

