//
// Author : Toru Shiozaki
// Date   : Mar 2009
//

#include <boost/regex.hpp>
#include <iostream>
#include <src/dg/diagram.h>
/// this is included only in the boost_devel
#include <boost/algorithm/combination.hpp>

using namespace std;
using namespace boost;
using namespace DG;

Diagram::Diagram(const std::list<Operator>& o, const std::list<Tensor>& t) 
 : operators_(o), tensors_(t) {

}


Diagram::Diagram(string input) {
  regex tensor_reg("(\\S+?)\\(.*?\\)");
  regex operator_reg("\\{(.*?)\\}");
  smatch what;
  string::const_iterator start = input.begin();
  string::const_iterator end   = input.end();

  while ( regex_search(start, end, what, tensor_reg) ) {
    string symbol(what[1].first,what[1].second);
    if (symbol != "Sum" && symbol != "P" ) { 
      string tensor_str(what[0].first,what[0].second);
      Tensor tnsr(tensor_str);
      tensors_.push_back(tnsr);
    }
    start = what[0].second;
  } 

  start = input.begin();
  int count = 0;
  while ( regex_search(start, end, what, operator_reg) ) {
    string op_str(what[0].first,what[0].second);
    Operator op(op_str, count); 
    operators_.push_back(op);
    start = what[0].second;
    ++count;
  } 

  assign_dagger();
}


void Diagram::assign_dagger() {
  // assuming number conserving operators 
  // TODO this should be updated
  for (auto titer = tensors_.begin(); titer != tensors_.end(); ++titer)
    titer->assign_dagger();
}


Diagram::~Diagram() {

}


const string Diagram::show() const { 
  string out;
  for (auto i = tensors_.begin();   i != tensors_.end();   ++i) out += i->show();
  for (auto i = operators_.begin(); i != operators_.end(); ++i) out += i->show();
  return out;
}


const string Diagram::show_only_tensors() const {
  string out;
  const string project("_");
  
  for (auto i = tensors_.begin(); i != tensors_.end(); ++i) {
    if (i->symbol() != project) out += i->show();
  }

  return out;
}


const int Diagram::count_creation_indices() const {
  int out = 0;
  for (auto i = operators_.begin(); i != operators_.end(); ++i) { 
    list<Indices> iindices = i->listindices();
    for (auto j = iindices.begin(); j != iindices.end(); ++j) { 
      if (j->dagger()) ++out; 
    }
  }
  return out;
}


const list<Diagram> Diagram::contract_one() const {

  list<Diagram> out;

  vector<Indices> creation, annihilation;
  for (auto i = operators_.begin(); i != operators_.end(); ++i) { 
    list<Indices> iindices = i->listindices();
    for (auto j = iindices.begin(); j != iindices.end(); ++j) { 
      if (j->dagger()) creation.push_back(*j);
      else annihilation.push_back(*j);
    }
  }

  {
    int dim_index = creation.front().nums().size();
    int dim_boxes = annihilation.size();
    vector<int> combination; 
    for (int ii = 0; ii < dim_boxes + dim_index - 1; ++ii) combination.push_back(ii);
    int count = 0;
    auto middle = combination.end();
    for (auto ii = combination.begin(); ii != combination.end(); ++ii, ++count) {
      if (count == dim_index) middle = ii; 
    }
  
    while (1) {
      list<int> cnums = creation.front().nums();
      vector<Indices> an = annihilation;

      // see if possible or not, 
      bool possible = true;
      list<pair<int, int> > index_map;
      vector<string> ctype;

      count = 0;
      for (auto ii = combination.begin(); ii != middle; ++ii, ++count) {
        const int block = *ii - count;
        if (an.at(block).nums().empty() 
         || an.at(block).source() == creation.front().source()
         || !an.at(block).contractable(creation.front()) ) {
          possible = false;
          break;
        } else {
          index_map.push_back(make_pair(an.at(block).nums().front(), cnums.front()));
          ctype.push_back(an.at(block).cindex(creation.front())); 

          an.at(block).pop_front_nums();
          cnums.pop_front();
        }
      }

      if (possible) {
        // if possible, copy-construct the diagram 
        Diagram tmp(*this); 

        // match numbers and replace if hit & remove operators 
        tmp.update_contracted_indices(index_map, ctype);

        // then add to the output list 
        out.push_back(tmp);
      }

      if (dim_boxes == 1 || !next_combination(combination.begin(), middle, combination.end())) break; 
    }
     
  }

  return out;
}


void Diagram::update_contracted_indices(const list<pair<int, int> > index_map, const vector<string> ctype) {
  auto j = ctype.begin();
  for (auto i = index_map.begin(); i != index_map.end(); ++i, ++j) {
    const string current_type = *j;
    const int first  = i->first;
    const int second = i->second;

    for (auto titer = tensors_.begin(); titer != tensors_.end(); ++titer) 
      titer->replace_indices(first, second, current_type);

    for (auto oiter = operators_.begin(); oiter != operators_.end(); ++oiter)
      oiter->delete_indices(first, second);
  } 
}


const bool Diagram::connected(const pair<string, list<string> > rule) const {
  bool out = true;
  string source = rule.first;
  list<string> target = rule.second;
 
  for (auto i = tensors_.begin(); i != tensors_.end(); ++i) {
    for (auto j = tensors_.begin(); j != tensors_.end(); ++j) {
      if (!i->connected(*j, rule)) out = false;
    }
  } 
  return out;
}

