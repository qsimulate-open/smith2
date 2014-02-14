//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: vectensor.cc
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
#include <src/vectensor.h>
#include <src/cost.h>

using namespace std;
using namespace smith2;

VecTensor::VecTensor(const vector<shared_ptr<Tensor>>& tensr): tensor_(tensr) {

}


VecTensor::~VecTensor() {

}


const string VecTensor::show() const {
  string out;
  for (auto& t : tensor_)
    out += t->show() + " ";
  out.erase(out.size()-1,1);
  out += "\n";
  out += "  Operation: " + mincost_.first.show() + "  Memory: " + mincost_.second.show();
  return out;
}


void VecTensor::strength_reduction(const bool opt_memory) {

  vector<shared_ptr<Tensor>> perm_tensor = tensor();
  sort(perm_tensor.begin(), perm_tensor.end());
  vector<shared_ptr<Tensor>> save_tensor;
  const int length = perm_tensor.size();
  const bool notone = length > 1;

  int perm_length = 1;
  for (int i = 0; i < length; ++i) perm_length *= i + 1;

  Cost tmp;
  {
    Index_map ind;
    vector<int> tt(ind.size());
    tt[ind.type("p")] = 100;
    PCost t(tt);
    if (notone) tmp.add_pcost(t); /// must be large enough
  }
  mincost_ = make_pair(tmp, tmp);

  while(1) {

    bool redundant = false;
    if (notone) {
      const Tensor a0 = *(perm_tensor[0]);
      const Tensor a1 = *(perm_tensor[1]);
      redundant = a0 > a1;
    }

    if (notone && !redundant) {

      pair<Cost, Cost> eval = cost_evaluater(perm_tensor);
      if (!opt_memory) {
        if (eval.first < mincost_.first) {
          mincost_ = eval;
          save_tensor = perm_tensor;
        } else if (eval.first == mincost_.first && eval.second < mincost_.second) {
          mincost_ = eval;
          save_tensor = perm_tensor;
        }
      } else {
        if (eval.second < mincost_.second) {
          mincost_ = eval;
          save_tensor = perm_tensor;
        } else if (eval.second == mincost_.second && eval.first < mincost_.first) {
          mincost_ = eval;
          save_tensor = perm_tensor;
        }
      }

    }
    if(!next_permutation(perm_tensor.begin(), perm_tensor.end())) break;
  }

  if (notone) {
    reverse(save_tensor.begin(), save_tensor.end());
    tensor_ = save_tensor;
    tensor_ = save_tensor;
  }
  refresh_tensor_depth();
  refresh_indices();
  refresh_sign();
  refresh_factor();

}


const pair<Cost, Cost> VecTensor::cost_evaluater(const vector<shared_ptr<Tensor>> tensors) const {

  list<SmartIndex> current;
  vector<shared_ptr<Tensor>> finished;
  Cost cop;
  Cost cmem;
  for (auto& t : tensors) {
    if (t != tensors.front()) {
      // evaluate cost here
      PCost pcop;
      PCost pcmem;
      list<SmartIndex> next;

      for (auto& s : current) {
        if (s.target_tensor().get() != t.get()) { // if not contraction indices
          next.push_back(s);
          s.add_to_pcost(pcmem);
          s.add_to_pcost(pcop);
        }
      }
      list<SmartIndex> newlist = t->smartindices()->si();
      for (auto& s : newlist) {
        s.add_to_pcost(pcop);
        if (!s.target_tensor() || !s.target_tensor()->in_list(finished)) { // if not contraction indices
          next.push_back(s);
          s.add_to_pcost(pcmem);
        }
      }

      cop.add_pcost(pcop);
      cmem.add_pcost(pcmem);

      current = next;
    } else {
      current = t->smartindices()->si();
    }
    finished.push_back(t);
  }
  cop.sort_pcost();
  cmem.sort_pcost();
  pair<Cost, Cost> out = make_pair(cop, cmem);


  return out;
}


void VecTensor::refresh_tensor_depth() {
  int depth = 0;
  for (auto& t : tensor_)
    t->set_depth(depth++);
}


void VecTensor::refresh_indices() {
/// this function is driven by the pointers to num of indices...
/// pretty complicated; but works. // info for number matching will be
/// storeted in map<shared_ptr<int>, vector<shared_ptr<int>>>

  map<shared_ptr<int> , vector<shared_ptr<int>>> dupl;
  // external indices (target indices)
  map<string, vector<shared_ptr<int>>> eindices;
  // internal indices (summation indices)
  map<string, vector<pair<shared_ptr<int> , shared_ptr<Tensor>>>> iindices;

  for (auto& current_tensor : tensor_) {
    current_tensor->sort_indices();

    for (auto& s : *current_tensor->smartindices()) {

      if (!s.target_tensor()) {
        vector<shared_ptr<int>> tmp = s.num_pointers();
        if (eindices.find(s.type_str()) == eindices.end())
          eindices[s.type_str()] = tmp;
        else
          eindices.at(s.type_str()).insert(eindices.at(s.type_str()).end(), tmp.begin(), tmp.end());
      } else {
       // internal indices come here
        vector<shared_ptr<int>> tmp = s.num_pointers();
        vector<pair<shared_ptr<int> , shared_ptr<Tensor>>> add;
        for (auto& t : tmp) {
          bool find_ = false;
          if (iindices.find(s.type_str()) != iindices.end()) {
            for (auto& i : iindices.at(s.type_str())) {
              if (*(i.first) == *t) {
                auto mapiter = dupl.find(i.first);
                if (mapiter == dupl.end()) {
                  vector<shared_ptr<int>> tmpv(1, t);
                  dupl.insert(make_pair(i.first, tmpv));
                } else {
                  mapiter->second.push_back(t);
                }
                find_ = true;
                break;
              }
            }
          }
          if (!find_) add.push_back(make_pair(t, current_tensor));
        }
        if (iindices.find(s.type_str()) == iindices.end())
          iindices[s.type_str()] = add;
        else
          iindices.at(s.type_str()).insert(iindices.at(s.type_str()).end(), add.begin(), add.end());
      }
    }
  }

  int count = 1;
  {
    /// numbering external indices
    for (auto& e : eindices)
      for (auto& i : e.second)   { *i = count++; }
  }

  /// numbering internal indices
  for (auto& t : tensor_) {
    for (auto& ii : iindices) {
      for (auto& i : ii.second)   {
        if (t != i.second) continue;

        *(i.first) = count;
        auto mapiter = dupl.find(i.first);
        if (mapiter != dupl.end()) {
          for (auto& ii : mapiter->second) *ii = count;
        }
        ++count;
      }
    }
  }

}


void VecTensor::refresh_sign() {
/// sign will be put to the last tensor
  const int numloops = count_loops();
  const int numholes = count_holes();

  const double factor = ((numloops + numholes) % 2 == 0) ? 1.0 : -1.0;
  tensor_.back()->set_factor(factor);
}


void VecTensor::refresh_factor() {

  /// TODO currently I only perform the tensor-tensor matching that is
  /// sufficient for the CC-R12 family of methods. In more general cases,
  /// we need to account for i) tensors-tensors matching,
  /// ii) 3-, 4-, .. folded symmetry etc. Those will be easy.

  double factor = 1.0;

  vector<shared_ptr<Tensor>> tensors = tensor();

  for (auto titer = tensors.begin(); titer != tensors.end(); ++titer) {
    auto tmp = titer;
    for (auto titer2 = (++tmp); titer2 != tensors.end(); ++titer2) {

      /// most of the time, **titer == **titer2 is sufficient
      /// but in case if **titer has an index that point **titer and those are
      /// apart in the VecTensor exression, that will be a problem.
      /// To avoid this, another vector is set up below.
      vector<shared_ptr<Tensor>> v;
      v.push_back(*titer);
      v.push_back(*titer2);
      for (auto t = tensors.begin(); t != tensors.end(); ++t) {
        if (t != titer && t != titer2) v.push_back(*t);
      }
      (*titer)->sort_indices();
      (*titer2)->sort_indices();
      if ( *(v.at(0)) == *(v.at(1)) )
        factor *= 0.5;
    }
  }

  tensor_.back()->mul_factor(factor);
}


int VecTensor::count_loops() {
  int out = 0;
  map<int, int> indexpair = index_num_pair();

  while (!indexpair.empty()) {

    auto a = indexpair.begin();
    vector<map<int, int>::iterator> used(1, a);
    const int target = (*a).first;
    int current = (*a).second;

    while (current != target) {
      a = indexpair.find(current);
      assert(a != indexpair.end());

      current = (*a).second;
      used.push_back(a);
    }

    for (auto& mi : used)
      indexpair.erase(mi);

    ++out;
  }

  return out;
}


int VecTensor::count_holes() {
  vector<shared_ptr<Tensor>> tensors = tensor();
  vector<int> temp;
  for (auto& t : tensors) {
    vector<int> holelines = t->num_values_hole();

    for (auto& ii : holelines) {
      if (find(temp.begin(), temp.end(), ii) == temp.end())
        temp.push_back(ii);
    }

  }
  return temp.size();
}


const map<int, int> VecTensor::index_num_pair() const {
  map<int, int> out;

  vector<shared_ptr<Tensor>> tensors = tensor();
  vector<int> edag;
  vector<int> endag;
  for (auto& t : tensors) {
    vector<int> dag = t->num_values(1);
    vector<int> ndag = t->num_values(-1);
    vector<int> tmp_ed = t->num_values_external(1);
    vector<int> tmp_en = t->num_values_external(-1);
    if (!tmp_ed.empty())
      edag.insert(edag.end(), tmp_ed.begin(), tmp_ed.end());
    if (!tmp_en.empty())
      endag.insert(endag.end(), tmp_en.begin(), tmp_en.end());
    assert(dag.size() == ndag.size());

    for (auto i = dag.begin(), j = ndag.begin(); i != dag.end(); ++i, ++j) {
      pair<map<int, int>::iterator, bool> a = out.insert(make_pair(*i, *j));
      assert(a.second);
    }
  }

  // probably need to modify in case of non-number-conserving theory such as IP and EA
  assert(edag.size() == endag.size());
  for (auto i = edag.begin(), j = endag.begin(); i != edag.end(); ++i, ++j) {
    // adding dummy indices that makes diagrams closed
    pair<map<int, int>::iterator, bool> a = out.insert(make_pair(*j, *i));
    assert(a.second);
  }

  return out;
}


bool VecTensor::identical(const VecTensor& o) const {
  const vector<shared_ptr<Tensor>> otensor_ = o.tensor();
  if (tensor_.size() != otensor_.size()) return false;

  for (auto i = tensor_.begin(), j = otensor_.begin(); i != tensor_.end(); ++i, ++j) {
    if (!((*i)->identical(*j))) return false;
  }

  return true;
}

