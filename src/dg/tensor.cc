//
// Author:: Toru Shiozaki
// Date  :: Mar 2009
//
#include <boost/regex.hpp>
#include <iostream>
#include <cassert>
#include "tensor.h"

using namespace std;
using namespace boost;
using namespace DG;

Tensor::Tensor(string input) {
  regex symbl("(\\S+)\\(");

  smatch what;
  string::const_iterator start = input.begin(); 
  string::const_iterator end = input.end(); 

  bool found; 
  found = regex_search(start, end, what, symbl);
  assert(found);

  string tmp(what[1].first, what[1].second);
  symbol_ = tmp;

  start = what[0].second;

  regex reg("\\s(\\S+?)\\s");
  int count = 0;
  while ( regex_search(start, end, what, reg) ) {
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
  for (list<Indices>::const_iterator i = listindices_.begin(); i != listindices_.end(); ++i)
    out += i->show();
  out += ") ";
  return out;
}


void Tensor::assign_dagger() {
  int size = listindices_.size();
  assert(size % 2 == 0);

  list<Indices>::iterator iiter = listindices_.begin();
  for (int i = 0; i < size/2; ++i, ++iiter) iiter->set_dagger(true);

}


const bool Tensor::connected(const Tensor& other, const pair<string, list<string> > rule) const {
  bool out = true;
  const string first = rule.first;
  const list<string> second = rule.second;
  /// quick return if possible
  if (symbol() != first) return out; 
  const string osymbol = other.symbol(); 
  list<string>::const_iterator ofind = find(second.begin(), second.end(), osymbol);
  if (ofind == second.end()) return out; 

  if (!connected_(other)) out = false;

  return out;
}


const bool Tensor::connected_(const Tensor& other) const {
  bool out = false;
  
  list<Indices> tmp = listindices();
  list<Indices> otmp = other.listindices();
  list<int> listnums, olistnums;

  for (list<Indices>::const_iterator i = tmp.begin(); i != tmp.end(); ++i) {
    list<int> tmp_nums = i->nums();
    listnums.merge(tmp_nums);
  }
  
  for (list<Indices>::const_iterator j = otmp.begin(); j != otmp.end(); ++j) {
    list<int> tmp_nums = j->nums();
    olistnums.merge(tmp_nums);
  }

  for (list<int>::const_iterator i = listnums.begin(); i != listnums.end(); ++i) {
    for (list<int>::const_iterator j = olistnums.begin(); j != olistnums.end(); ++j) {
      if (*i == *j) { 
        out = true;
        break;
      }
    }
    if (out) break;
  }

  return out;
}


