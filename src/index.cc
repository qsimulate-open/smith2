//
// Author : Toru Shiozaki
// Date   : Feb 2009
//

#include <iostream>
#include <string>
#include <stdexcept>
#include <src/index.h>
#include <sstream>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

Index::Index(const string indx) {

  const boost::regex dagger_reg("\\+");
  boost::smatch what;
  if (boost::regex_search(indx.begin(), indx.end(), what, dagger_reg)) 
    dagger_=true;
  else
    dagger_=false;

  const boost::regex type_reg("([^0-9]+)([0-9]+)");
  if (boost::regex_search(indx.begin(), indx.end(), what, type_reg)) {
    const string num_str(what[2].first,what[2].second); 
    shared_ptr<int> tnum(new int(boost::lexical_cast<int>(num_str)));
    num_ = tnum;

    const string type_str(what[1].first,what[1].second); 
    type_=type_str;
  } else {
    stringstream error_message; error_message << "Something is wrong...: " <<  __FILE__ << " line: " << __LINE__;
    throw std::invalid_argument(error_message.str());
  }

}

Index::~Index() {

}

const string Index::show() const {

  ostringstream buffer;
  buffer << type() << num() << (dagger() ? "+" : "");

  return buffer.str();
}
 
