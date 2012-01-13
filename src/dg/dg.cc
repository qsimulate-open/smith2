//
// Author:: Toru Shiozaki
// Date  :: Mar 2009
//
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/lexical_cast.hpp>
#include <stdexcept>
#include <src/dg/dg.h>

using namespace std;
using namespace boost;
using namespace DG;

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
    const string error_message = "\"" + filen + "\" cannot be opened in file: " + __FILE__ + " line: " + lexical_cast<string>(__LINE__);
    throw std::runtime_error(error_message);
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


