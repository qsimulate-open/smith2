//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: tensor.cc
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

#include <boost/regex.hpp>
#include <iostream>
#include <cassert>
#include <src/dg/tensor.h>

using namespace std;
using namespace smith2::DG;

Tensor::Tensor(string input) {
  boost::regex symbl("(\\S+)\\(");

  boost::smatch what;
  string::const_iterator start = input.begin(); 
  string::const_iterator end = input.end(); 

  bool found; 
  found = boost::regex_search(start, end, what, symbl);
  assert(found);

  string tmp(what[1].first, what[1].second);
  symbol_ = tmp;

  start = what[0].second;

  boost::regex reg("\\s(\\S+?)\\s");
  int count = 0;
  while ( boost::regex_search(start, end, what, reg) ) {
    string indx(what[1].first,what[1].second);
    Indices tmp(indx, count);
    listindices_.push_back(tmp); 
    start = what[0].second;
    --start;
  } 

}


Tensor::~Tensor() {

}


const string Tensor::show() const {
  string out;
  out += symbol();
  out += " ( ";
  for (auto i = listindices_.begin(); i != listindices_.end(); ++i)
    out += i->show();
  out += ") ";
  return out;
}


void Tensor::assign_dagger() {
  int size = listindices_.size();
  // TODO this is generally not true... (but perhaps there is a way to avoid changing this function as well...)
  assert(size % 2 == 0);

  auto iiter = listindices_.begin();
  for (int i = 0; i < size/2; ++i, ++iiter) iiter->set_dagger(true);

}


const bool Tensor::connected(const Tensor& other, const pair<string, list<string> > rule) const {
  bool out = true;
  const string first = rule.first;
  const list<string> second = rule.second;
  /// quick return if possible
  if (symbol() != first) return out; 
  const string osymbol = other.symbol(); 
  auto ofind = find(second.begin(), second.end(), osymbol);
  if (ofind == second.end()) return out; 

  if (!connected_(other)) out = false;

  return out;
}


const bool Tensor::connected_(const Tensor& other) const {
  bool out = false;
  
  list<Indices> tmp = listindices();
  list<Indices> otmp = other.listindices();
  list<int> listnums, olistnums;

  for (auto i = tmp.begin(); i != tmp.end(); ++i) {
    list<int> tmp_nums = i->nums();
    listnums.merge(tmp_nums);
  }
  
  for (auto j = otmp.begin(); j != otmp.end(); ++j) {
    list<int> tmp_nums = j->nums();
    olistnums.merge(tmp_nums);
  }

  for (auto i = listnums.begin(); i != listnums.end(); ++i) {
    for (auto j = olistnums.begin(); j != olistnums.end(); ++j) {
      if (*i == *j) { 
        out = true;
        break;
      }
    }
    if (out) break;
  }

  return out;
}


