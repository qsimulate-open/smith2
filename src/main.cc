//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: main.cc
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
#include <list>
#include <string>
#include <stdexcept>
#include <boost/regex.hpp>
#include <src/pretensor.h>
#include <src/equation.h>
#include <src/dg/dg.h>
#include <src/ig/inputgenerator.h>

using namespace std;
using namespace DG;
using namespace IG;

list<RefListPreTensor>  read_input(string filename);
list<RefListPreTensor>  read_input(list<string> content);

int main(int argc, char** argv) {

  try {
    
    if (argc < 2) {
      throw std::runtime_error("Please specify an input file");
    } 
    string filename = argv[1];
  
    // those will be controlled by the suffix of the input files
    InputGenerator ig(filename);
    DiagramGenerator dg(ig.generate());
    //DiagramGenerator dg(filename);
    list<RefListPreTensor> listpretensor=read_input(dg.generate());
    //list<RefListPreTensor> listpretensor=read_input(filename);
  
    if (listpretensor.empty()) return 1;
    list<RefVecTensor> listvectensor;
 
    for (auto iter = listpretensor.begin(); iter != listpretensor.end() ;++iter) { 
      RefVecTensor newvectensor = (*iter)->analyze();
      listvectensor.push_back(newvectensor);
    }
    
    Equation equation(listvectensor);
    const bool opt_memory = false;
    equation.strength_reduction(opt_memory);
    equation.form_tree();
    equation.factorize();
    cout << equation.tree_root()->show() << endl;

  } catch (const runtime_error& e) {
    cout << "  ----" << endl;
    cout << "  Runtime error: " << e.what() << endl;
    cout << endl;
  } catch (const logic_error& e) {
    cout << "  ----" << endl;
    cout << "  Logic error: " << e.what() << endl;
    cout << endl;
  } catch (...) {
    cout << "  ----" << endl;
    cout << "  Error" << endl;
    cout << endl;
  }  

  return 0;
}



list<RefListPreTensor> read_input(string filename) {

  list<RefListPreTensor> out;
 
  ifstream ifs(filename.c_str());
  boost::regex tensor_reg("(\\S+?)\\s\\(.*?\\)");
  if (!ifs.is_open()) {
    cout <<  "FILE " << filename << " CANNOT BE OPENED" << endl;
    return out;
  }
   
  while (!ifs.eof()) {
    string stmp;
    getline(ifs,stmp);
    if (stmp.empty()) continue;
 
    boost::smatch what;
 
    string::const_iterator start = stmp.begin();
    string::const_iterator end   = stmp.end();
 
    list<RefPreTensor> pretensors;
    while ( boost::regex_search(start, end, what, tensor_reg) ) {
 
      string symbol(what[1].first,what[1].second);
 
      if (symbol != "Sum" && symbol != "P" ) { 
        string tensor(what[0].first,what[0].second);
        pretensors.push_back(RefPreTensor(new PreTensor(tensor)));
      }
      start = what[0].second;
      
    } 
 
    out.push_back(RefListPreTensor(new ListPreTensor(pretensors)));
 
  }
 
  return out;
}


list<RefListPreTensor> read_input(list<string> content) {

  list<RefListPreTensor> out;
  boost::regex tensor_reg("(\\S+?)\\s\\(.*?\\)");
   
  for(auto i = content.begin(); i != content.end(); ++i) {
    const string stmp = *i;
 
    boost::smatch what;
 
    string::const_iterator start = stmp.begin();
    string::const_iterator end   = stmp.end();
 
    list<RefPreTensor> pretensors;
    while ( boost::regex_search(start, end, what, tensor_reg) ) {
 
      string symbol(what[1].first,what[1].second);
 
      if (symbol != "Sum" && symbol != "P" ) { 
        string tensor(what[0].first,what[0].second);
        pretensors.push_back(RefPreTensor(new PreTensor(tensor)));
      }
      start = what[0].second;
      
    } 
 
    out.push_back(RefListPreTensor(new ListPreTensor(pretensors)));
 
  }
 
  return out;
}

