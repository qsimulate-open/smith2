//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: dg.cc
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
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <src/dg/dg.h>

using namespace std;
using namespace smith2::DG;

list<string> DiagramGenerator::generate() {
  list<string> out;
 
  list<Diagram> listD;
  if (filename_ == "") 
    listD = read_input(stringinput_);
  else 
    listD = read_input(filename_);

  list<pair<string, list<string> > > rules;
  list<string> general;
  general.push_back("f");
  general.push_back("v");
  string tamp("t");
  rules.push_back(make_pair(tamp, general));

  for (auto i = listD.begin(); i != listD.end(); ++i) {
    bool disconnected = false; 
    for (auto j = rules.begin(); j != rules.end(); ++j) {
      if (!(i->connected(*j))) disconnected = true;
      break;
    }
    if (!disconnected) 
      out.push_back(i->show_only_tensors());
  }

  return out;
}



list<Diagram> DiagramGenerator::read_input(string filen) {

  list<Diagram> out;

  ifstream ifs(filen.c_str());
  if (!ifs.is_open()) {
    stringstream ss; ss << "\"" + filen + "\" cannot be opened in file: " + __FILE__ + " line: " << __LINE__; 
    throw std::runtime_error(ss.str());
  }
  
  while (!ifs.eof()) {
    string stmp;
    getline(ifs,stmp);
    if (stmp.empty()) continue;

    Diagram dgrm(stmp);
    list<Diagram> current, next;
    current.push_back(dgrm);

    const int length = dgrm.count_creation_indices(); 
    int i = 0;

    while (i < length) {
      for (auto diter = current.begin(); diter != current.end(); ++diter) {
        list<Diagram> tmp = diter->contract_one();
        next.insert(next.end(), tmp.begin(), tmp.end()); 
      }
      current = next;
      next.clear();
      ++i;
    }
    out.insert(out.end(), current.begin(), current.end());
  }

  return out;
}


list<Diagram> DiagramGenerator::read_input(list<string> inp) {

  list<Diagram> out;

  for (auto siter = inp.begin(); siter != inp.end(); ++siter) {
    string stmp(*siter);

    Diagram dgrm(stmp);
    list<Diagram> current, next;
    current.push_back(dgrm);

    const int length = dgrm.count_creation_indices(); 
    int i = 0;

    while (i < length) {
      for (auto diter = current.begin(); diter != current.end(); ++diter) {
        list<Diagram> tmp = diter->contract_one();
        next.insert(next.end(), tmp.begin(), tmp.end()); 
      }
      current = next;
      next.clear();
      ++i;
    }
    out.insert(out.end(), current.begin(), current.end());
  }

  return out;
}


