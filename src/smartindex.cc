//
// Author : Toru Shiozaki
// Date   : Feb 2009
//
#include <cassert>
#include <algorithm>
#include <iostream>
#include <src/cost.h>
#include <src/smartindex.h>
#include <boost/algorithm/combination.hpp>

using namespace std;

SmartIndex::SmartIndex(const list<Index> indx, shared_ptr<Tensor> mytensr, shared_ptr<Tensor> targettensr)
  : indices_(indx), my_tensor_(mytensr), target_tensor_(targettensr) {

}


SmartIndex::~SmartIndex() {

}


bool SmartIndex::operator<(const SmartIndex& o) const { 
  // in a canonical ordering, daggered quantity comes first
  if (dagger() != o.dagger()) return dagger(); 
  // next, we use the orbital orderings
  // closed < hole < particle < complete-particle
  if (type() != o.type()) {
    if (type() == "h")        return true;
    else if (type() == "P")   return false;
    if (o.type() == "h")      return false;
    else if (o.type() == "P") return true;
  }
  if (target_tensor().get() == NULL && o.target_tensor().get() != NULL)  return true; 
  if (target_tensor().get() != NULL && o.target_tensor().get() == NULL) return false; 
  if (target_tensor().get() != o.target_tensor().get()) return target_tensor()->depth() < o.target_tensor()->depth(); 
  return num() < o.num();
}


bool SmartIndex::operator==(const SmartIndex& other) const {

  if (type() != other.type()) return false;
  if (dagger() != other.dagger()) return false;
  if (length() != other.length()) return false;

  if ((target_tensor().get() != other.target_tensor().get()) 
   && (target_tensor().get() != other.my_tensor().get() || other.target_tensor().get() != my_tensor().get())) return false; 

  return true;
}


const bool SmartIndex::identical(const SmartIndex& other) const {

  if (type() != other.type()) return false;
  if (dagger() != other.dagger()) return false;
  if (length() != other.length()) return false;

  if (num_values() != other.num_values()) return false;
  return true;
}


const bool SmartIndex::permutable(const SmartIndex& other) const {
  bool perm = true;

  const bool bothnull = !target_tensor() && !other.target_tensor();
  const bool onenull = !bothnull && (!target_tensor() || !other.target_tensor()); 
  const bool nonull = !bothnull && !onenull;

  if (nonull) { 
    if (target_tensor() != other.target_tensor() || dagger() != other.dagger() || type() != other.type())  perm = false;
  } else if (onenull) {
    perm = false;
  } else {
    if (indices_.empty() || other.indices_.empty() || dagger() != other.dagger() || type() != other.type()) perm = false;
  } 

  return perm;
}


void SmartIndex::merge(SmartIndex& other) { 

  assert(other.type()==type());

  // add indices of other to self
  indices_.merge(other.indices_); 
}


const list<SmartIndex> SmartIndex::extract() const {
  list<SmartIndex> out;
  for (auto i = indices_.begin(); i != indices_.end(); ++i) {
    list<Index> li(1, *i);
    SmartIndex s(li, my_tensor(), target_tensor());
    out.push_back(s); 
  }

  return out;
}


const string SmartIndex::show() const {
  string out;
  for (auto iiter = indices_.begin(); iiter != indices_.end(); ++iiter) 
    out += iiter->show(); 
  return out;
}


void SmartIndex::add_to_pcost(PCost& pcost){
  if (type() == "p") {
    pcost.add(0, length(), 0);    
  } else if (type() == "h") {
    pcost.add(length(), 0, 0);
  } else if (type() == "P") {
    pcost.add(0, 0, length());
  } else {
    cout << "SmartIndex::add_to_pcost Please define the index type" << endl; 
    abort();
  } 
}


vector<shared_ptr<int> > SmartIndex::num_pointers(const int dagger) {
/// dagger = 1 returns only those with dagger,
/// dagger = -1 returns only those without dagger
  vector<shared_ptr<int> > out;
  for (auto iiter = indices_.begin(); iiter != indices_.end(); ++iiter)
    if (dagger == 0 || (dagger == 1 && (*iiter).dagger()) || (dagger == -1 && !(*iiter).dagger()))
      out.push_back(iiter->num_pointer());    
  return out;
}


const vector<int> SmartIndex::num_values(const int dagger) const {
/// dagger = 1 returns only those with dagger,
/// dagger = -1 returns only those without dagger
  vector<int> out;
  for (auto iiter = indices_.begin(); iiter != indices_.end(); ++iiter) {
    if (dagger == 0 || (dagger == 1 && (*iiter).dagger()) || (dagger == -1 && !(*iiter).dagger()))
      out.push_back(iiter->num());    
  }
  return out;
}


