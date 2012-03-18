//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: indices.cc
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


#include <src/dg/indices.h>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

using namespace std;
using namespace boost;
using namespace DG;

Indices::Indices(const string input, const int s) : source_(s) {
  regex tp("[A-Za-z]+"); 
  smatch what;  
  string::const_iterator start = input.begin(); 
  string::const_iterator end = input.end(); 
  bool find_tp = regex_search(start, end, what, tp);
  assert(find_tp);

  string tmp_t(what[0].first, what[0].second);
  type_ = tmp_t;

  regex nm("[0-9]+");
  start = what[0].second;
  bool find_nm = regex_search(start, end, what, nm);
  assert(find_nm);

  string tmp_n(what[0].first, what[0].second); 
  nums_.push_back(lexical_cast<int>(tmp_n));

  regex dag("\\+");
  start = what[0].second;
  dagger_ = regex_search(start, end, what, dag);
  
}


Indices::~Indices() {

}


const string Indices::show() const {
  string out;
  for (auto i = nums_.begin(); i != nums_.end(); ++i) { 
    out += type() + lexical_cast<string>(*i);
    if (dagger()) out += "+"; 
  }
  out += " ";

  return out;
}


const bool Indices::contractable(const Indices& other) const { 
  string mine = type();
  string yours = other.type();

  if (mine == "g" || yours == "g") return true;
  if (mine == yours) return true;

  if (mine == "P" && yours == "p") return true;
  if (mine == "p" && yours == "P") return true;

  return false;
}


const std::string Indices::cindex(const Indices& other) const { 
  string mine = type();
  string yours = other.type();
  string out;

  if (mine == yours) out += mine;
  else if (mine == "g" && yours != "g") out += yours; 
  else if (mine != "g" && yours == "g") out += mine; 
  else if (mine == "P" && yours == "p") out += yours;
  else if (mine == "p" && yours == "P") out += mine;

  else abort();

  return out;
}


void Indices::replace_index(const int first, const int second, const string ctype) {

  for (auto iiter = nums_.begin(); iiter != nums_.end(); ++iiter) {
    const int num = *iiter;
    if (num == first) {
      type_ = ctype; 
      *iiter = second;  
    } else if (num == second) {
      type_ = ctype;
    }
  } 
}


void Indices::delete_index(const int first, const int second) {

  vector<list<int>::iterator> remove;
  for (auto iiter = nums_.begin(); iiter != nums_.end(); ++iiter) {
    const int num = *iiter;
    if (num == first || num == second) remove.push_back(iiter); 
  } 

  for (auto i = remove.begin(); i != remove.end(); ++i) 
    nums_.erase(*i);
}


