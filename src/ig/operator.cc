//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: operator.cc
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

 
#include <src/ig/operator.h>
#include <sstream>

using namespace std;
using namespace smith2::IG;

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

