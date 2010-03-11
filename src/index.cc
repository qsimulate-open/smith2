//
// Author:: Toru Shiozaki
// Date  :: Feb 2009
//

#include <iostream>
#include <string>
#include <stdexcept>
#include <src/index.h>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

Index::Index(const string indx, const Spaces& sp) {

  const regex dagger_reg("\\+");
  smatch what;
  if (regex_search(indx.begin(), indx.end(), what, dagger_reg)) 
    dagger_=true;
  else
    dagger_=false;

  const regex type_reg("([^0-9]+)([0-9]+)");
  if (regex_search(indx.begin(), indx.end(), what, type_reg)) {
    const string num_str(what[2].first,what[2].second); 
    shared_ptr<int> tnum(new int(lexical_cast<int>(num_str)));
    num_ = tnum;

    const string type_str(what[1].first,what[1].second); 
    type_=type_str;
  } else {
    const string error_message = lexical_cast<string>("Something is wrong...: ")
                                 + __FILE__ + " line: " + lexical_cast<string>(__LINE__);
    throw std::invalid_argument(error_message);
  }

  if (type_ == "h") {
    indexspace_ = sp.occ_space();
  } else if (type_ == "p") {
    indexspace_ = sp.vir_space();
  } else {
    const string error_message = lexical_cast<string>("index other than holes and particles hasn't been implemented in file: ")
                                 + __FILE__ + " line: " + lexical_cast<string>(__LINE__);
    throw logic_error(error_message);
  }

}

Index::~Index() {

}

const string Index::show() const {
  string out;

  ostringstream buffer;
  buffer << num();
  string num_str=buffer.str();

  out += type() + num_str;
  if (dagger()) out += "+";

  return out;
}
 
