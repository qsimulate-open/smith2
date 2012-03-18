//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: inputgenerator.cc
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

 
#include <src/ig/inputgenerator.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <src/ig/operator.h>

using namespace std;
using namespace boost;
using namespace IG;

/// utility functions
const std::list<Operator> extract_operator(const std::string input, const int type);
const std::list<std::list<Operator> > make_explist(const std::list<Operator>& init, const std::list<Operator>& expop, 
                                                   const unsigned int high, const unsigned int low);
const std::string print_input(std::list<Operator> lo, const int bra, const int ket);


InputGenerator::InputGenerator(string filenm) : filename_(filenm) {

}

InputGenerator::~InputGenerator() {

}

const list<string> InputGenerator::generate() {
  /// first, read the input
  list<string> out;

  ifstream ifs(filename_.c_str());
  if (!ifs.is_open()) {
    const string error_message = "\"" + filename_ + "\" cannot be opened in file: " + __FILE__ + " line: " + lexical_cast<string>(__LINE__);
    throw std::runtime_error(error_message);
  }

  while (!ifs.eof()) {
    string stmp;
    getline(ifs,stmp);
    if (stmp.empty()) continue;

    cout << stmp << endl;

    regex bra("<(\\S)\\|");
    regex ket("\\|(\\S)>");
    regex lin_deexcitation("d\\((.+?)\\)");
    regex lin_excitation("e\\((.+?)\\)");
    regex exp_excitation("x\\((.+?)\\)");
    regex lin_general("g\\((.+?)\\)");
    
    smatch what;
    string::const_iterator start = stmp.begin();
    string::const_iterator end   = stmp.end();

    const string error_message = "incompatible input format in \"" + filename_ + ".\" File: " + __FILE__ + " line: ";
    if (!regex_search(start, end, what, bra)) throw runtime_error(error_message + lexical_cast<string>(__LINE__)); 
    const string bra_str(what[1].first,what[1].second);

    if (!regex_search(start, end, what, ket)) throw runtime_error(error_message + lexical_cast<string>(__LINE__));
    const string ket_str(what[1].first,what[1].second);

    const int bra_rank = lexical_cast<int>(bra_str); 
    const int ket_rank = lexical_cast<int>(ket_str); 

    if (!regex_search(start, end, what, lin_deexcitation))
      cout << "  ***  no linear deexcitation operator found   ***" << endl;
    const string lin_deexcitation_str(what[1].first,what[1].second);

    if (!regex_search(start, end, what, lin_excitation))
      cout << "  ***   no linear excitation operator found    ***" << endl;
    const string lin_excitation_str(what[1].first,what[1].second);
    
    if (!regex_search(start, end, what, exp_excitation))
      cout << "  *** no exponential excitation operator found ***" << endl;
    const string exp_excitation_str(what[1].first,what[1].second);
    
    regex_search(start, end, what, lin_general);
    const string lin_general_str(what[1].first,what[1].second);

    const list<Operator> lin_deexcitations = extract_operator(lin_deexcitation_str, -1);
    const list<Operator> lin_excitations = extract_operator(lin_excitation_str, 1);;
    const list<Operator> exp_excitations = extract_operator(exp_excitation_str, 1);;
    const list<Operator> lin_generals = extract_operator(lin_general_str, 0);

    const int target = bra_rank - ket_rank; 
    list<list<Operator> > totallist;
    for (auto i = lin_deexcitations.begin(); i != lin_deexcitations.end(); ++i) {
      for (auto j = lin_generals.begin(); j != lin_generals.end(); ++j) {
        for (auto k = lin_excitations.begin(); k != lin_excitations.end(); ++k) {
          list<Operator> outlist;
          outlist.push_back(*i);
          outlist.push_back(*j);
          outlist.push_back(*k);
          const int high = target + i->rank() + j->rank() - k->rank();
          const int low  = target + i->rank() - j->rank() - k->rank();
          list<list<Operator> > currentlist = make_explist(outlist, exp_excitations, high, (low > 0 ? low : 0)); 
          totallist.insert(totallist.end(), currentlist.begin(), currentlist.end());
        }
      }
    } 

    for (auto i = totallist.begin(); i != totallist.end(); ++i) { 
      const string tmp_str = print_input(*i, bra_rank, ket_rank);
      cout << tmp_str << endl;
      out.push_back(tmp_str);
    }
    
  }

  return out;
}


const list<list<Operator> > make_explist(const list<Operator>& init, const list<Operator>& expop, 
                                         const unsigned int high, const unsigned int low) {
  /// Assuming that we use the similarity-transformed ansatz; therefore,
  /// the maximun number of operators will be 4.
  /// TODO this is not an optimal algorithm. Now looking up from (0000) to (4444)
  list<list<Operator> > out;

  const unsigned int m = high < 4 ? high+1 : 5;
  unsigned int max = 1;
  for (unsigned int i = 0; i != expop.size(); ++i) max *= m;

  for (unsigned int i = 0; i != max; ++i) {
    list<Operator> tmp = init;
    unsigned int combination = i;
    unsigned int ex = 0;
    for (auto j = expop.begin(); j != expop.end(); ++j) {
      ex += (combination % 5) * j->rank(); 
      combination /= 5;
    }
    if (ex > high || ex < low) continue; 

    combination = i;
    for (auto j = expop.begin(); j != expop.end(); ++j) {
      unsigned int dup = combination % 5; 
      for (unsigned int k = 0; k != dup; ++k) {
        tmp.push_back(*j);
      }
      combination /= 5;
    }
    out.push_back(tmp);
  }

  return out;
}


const list<Operator> extract_operator(const string input, const int type) {
  /// type = 0  : general
  /// type = 1  : excitation 
  /// type = -1 : deexcitation 
  list<Operator> out;
  regex op("\\s(\\S+?)([0-9])\\s");

  smatch what;
  string::const_iterator start = input.begin();
  string::const_iterator end   = input.end();
  while (regex_search(start, end, what, op)) { 
    string symb(what[1].first, what[1].second);
    string num_str(what[2].first, what[2].second);
    int num = lexical_cast<int>(num_str); 
    bool deexcitation = false;
    if (type < 0) deexcitation = true;;
    bool general = false;
    if (type == 0) general = true; 
    Operator tmp_op(symb, num, general, deexcitation);
    out.push_back(tmp_op);
    start = what[0].second;
    --start;
  }

  if (out.empty()) {
    string dummy("dummy");
    Operator tmp_op(dummy, 0, false, false);
    out.push_back(tmp_op);
  }
  return out;
}


const string print_input(list<Operator> lo, const int bra, const int ket) {
  string tens;
  string op; 

  int count = 1;
  if (bra > 0) { 
    op += "{ ";
    for (int i = 0; i != bra; ++i, ++count) op += "h" + lexical_cast<string>(count) + "+ "; 
    for (int i = 0; i != bra; ++i, ++count) op += "p" + lexical_cast<string>(count) + " "; 
    op += "} ";
  }

  for (auto i = lo.begin(); i != lo.end(); ++i) {
    if (i->symbol() == "dummy") continue;
    if (i->general()) {
      op += "{ ";
      tens += i->symbol() + "( ";
      for (int j = 0; j != i->rank(); ++j, ++count) {
        op += "g" + lexical_cast<string>(count) + "+ "; 
        tens += "g" + lexical_cast<string>(count) + " ";      
      }
      for (int j = 0; j != i->rank(); ++j, ++count) {
        op += "g" + lexical_cast<string>(count) + " "; 
        tens += "g" + lexical_cast<string>(count) + " ";
      }
      op += "} ";
      tens += ") ";
    } else if (i->deexcitation()) {
      op += "{ ";
      tens += i->symbol() + "( ";
      for (int j = 0; j != i->rank(); ++j, ++count) {
        op += "h" + lexical_cast<string>(count) + "+ "; 
        tens += "h" + lexical_cast<string>(count) + " ";      
      }
      for (int j = 0; j != i->rank(); ++j, ++count) {
        op += "p" + lexical_cast<string>(count) + " "; 
        tens += "p" + lexical_cast<string>(count) + " ";
      }
      op += "} ";
      tens += ") ";
    } else {
      op += "{ ";
      tens += i->symbol() + "( ";
      for (int j = 0; j != i->rank(); ++j, ++count) {
        op += "p" + lexical_cast<string>(count) + "+ "; 
        tens += "p" + lexical_cast<string>(count) + " ";      
      }
      for (int j = 0; j != i->rank(); ++j, ++count) {
        op += "h" + lexical_cast<string>(count) + " "; 
        tens += "h" + lexical_cast<string>(count) + " ";
      }
      op += "} ";
      tens += ") ";
    }
  }

  if (ket > 0) { 
    op += "{ ";
    for (int i = 0; i != ket; ++i, ++count) op += "p" + lexical_cast<string>(count) + "+ "; 
    for (int i = 0; i != ket; ++i, ++count) op += "h" + lexical_cast<string>(count) + " "; 
    op += "} ";
  }


  return tens + op;
} 
