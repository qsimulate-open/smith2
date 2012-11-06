//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: smartindex.cc
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


#include <cassert>
#include <algorithm>
#include <iostream>
#include <src/cost.h>
#include <src/smartindex.h>
#include <src/_boost/combination.hpp>

using namespace std;
using namespace smith2;

SmartIndex::SmartIndex(const list<Index> indx, shared_ptr<Tensor> mytensr, shared_ptr<Tensor> targettensr)
  : indices_(indx), my_tensor_(mytensr), target_tensor_(targettensr) {

}


SmartIndex::~SmartIndex() {

}


bool SmartIndex::operator<(const SmartIndex& o) const {
  // in a canonical ordering, daggered quantity comes first
  if (dagger() != o.dagger()) return dagger();

  // next, we use the orbital orderings
  // hole < active < particle < complete-particle
  if (type() != o.type()) {
    return type() < o.type();
#if 0
    if (type() == "h" || type() == "a" && o.type() != "h" || type() == "p" && o.type() == "P")
      return true;
    else
      return false;
#endif
  }

  // and then, sort with "target tensors"
  // 1) external lines come first
  if (target_tensor().get() == NULL && o.target_tensor().get() != NULL)  return true;
  if (target_tensor().get() != NULL && o.target_tensor().get() == NULL) return false;
  // 2) then tensor depths
  if (target_tensor().get() != o.target_tensor().get()) return target_tensor()->depth() < o.target_tensor()->depth();

  // if everything is the same, sort by numbers (rarely happens)
  return num() < o.num();
}


bool SmartIndex::operator==(const SmartIndex& other) const {

  bool out = true;
  if (type() != other.type() || dagger() != other.dagger() || length() != other.length()) out = false;

  if ((target_tensor().get() != other.target_tensor().get())
   && (target_tensor().get() != other.my_tensor().get() || other.target_tensor().get() != my_tensor().get())) out = false;

  return out;
}


const bool SmartIndex::identical(const SmartIndex& other) const {

  return type() == other.type()
      && dagger() == other.dagger()
      && length() == other.length()
      && num_values() == other.num_values();
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
  const int norbcl = indices_.front().indmap().num_orb_class();
  vector<int> out(norbcl);
  out[type()] = length();
  pcost.add(out);
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


