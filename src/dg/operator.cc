//
// Author:: Toru Shiozaki
// Date  :: Mar 2009
//
#include <boost/regex.hpp>
#include <iostream>
#include "operator.h"

using namespace std;
using namespace boost;
using namespace DG;

Operator::Operator(std::string input, int count){
  regex reg("\\s(\\S+?)\\s");
  smatch what;
  string::const_iterator start = input.begin(); 
  string::const_iterator end = input.end(); 

  while ( regex_search(start, end, what, reg) ) {
    string indx(what[1].first,what[1].second);
    Indices tmp(indx, count);
    listindices_.push_back(tmp); 
    start = what[0].second;
    --start;
  } 

  merge_indices();
}


Operator::~Operator() {

}


void Operator::merge_indices() {
  list<Indices>::iterator i, j, tmp;
  vector<list<Indices>::iterator> removelist;

  for (i = listindices_.begin(); i != listindices_.end(); ++i) {
    if (find(removelist.begin(), removelist.end(), i) != removelist.end()) continue;

    tmp = i;
    ++tmp;
    for (j = tmp; j != listindices_.end(); ++j) {
      if (*i == *j) {
        removelist.push_back(j);
        i->add_nums(j->nums());
      }
    }
  } 

  for (vector<list<Indices>::iterator>::iterator i = removelist.begin(); i != removelist.end(); ++i) 
    listindices_.erase(*i);
}


const string Operator::show() const {
  string out;
  out += "{ ";
  for (list<Indices>::const_iterator i = listindices_.begin(); i != listindices_.end(); ++i)
    out += i->show();
  out += "}";
  return out;
}


void Operator::delete_indices(const int first, const int second) {
  vector<list<Indices>::iterator> remove;
  for (list<Indices>::iterator iiter = listindices_.begin(); iiter != listindices_.end(); ++iiter) {
    iiter->delete_index(first, second);
    if (iiter->nums().empty()) remove.push_back(iiter);
  }
  for (vector<list<Indices>::iterator>::iterator ii = remove.begin(); ii != remove.end(); ++ii)
    listindices_.erase(*ii);
}

