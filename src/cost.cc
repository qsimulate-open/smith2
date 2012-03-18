//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: cost.cc
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


#include <algorithm>
#include <sstream>
#include <src/cost.h>

using namespace std;


const string PCost::show() const {
  stringstream out;
  auto j = pcost_.begin();
  for (auto i = indmap_.begin(); i != indmap_.end(); ++i, ++j)
    out << i->first <<  *j; 
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


