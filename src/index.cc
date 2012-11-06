//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: index.cc
// Copyright (C) 2009 Toru Shiozaki
//
// Author: Toru Shiozaki <shiozaki@northwestern.edu>
// Maintainer: Shiozaki group
//
// This file is part of the SMITH2 package (to be renamed).
//
// The SMITH2 package is free software; you can redistribute it and\/or modify
// it under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// The SMITH2 package is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with the SMITH2 package; see COPYING.  If not, write to
// the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
//


#include <iostream>
#include <string>
#include <stdexcept>
#include <src/index.h>
#include <sstream>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace smith2;

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
  buffer << type_str() << num() << (dagger() ? "+" : "");

  return buffer.str();
}
 
