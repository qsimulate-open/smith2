//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: tree.cc
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

#include <src/tree.h>
#include <cassert>
#include <stdexcept>
#include <vector>

using namespace std;
using namespace smith2;

Tree::Tree(const BinaryContraction& bc, const pair<list<RefTree>, list<RefTree>>& sub, RefTree par)
 : binc_(bc), subtrees_(sub), parent_(par) {

  root_ = false;
}


Tree::Tree(const BinaryContraction& bc, const pair<list<RefTree>, list<RefTree>>& sub)
 : binc_(bc), subtrees_(sub) {

  root_ = false;
}


Tree::Tree(RefVecTensor input, shared_ptr<Tensor> par)
{

  vector<shared_ptr<Tensor>> vectensor = input->tensor();
  init(vectensor, par);
  root_ = false;

}


void Tree::init(vector<shared_ptr<Tensor>>& vectensor, shared_ptr<Tensor> par) {
  shared_ptr<Tensor> current;
  shared_ptr<Tensor> previous = vectensor.back();
  list<RefTree> a;
  pair<list<RefTree>, list<RefTree>> prevtree = make_pair(a, a);

  auto titer_end = vectensor.rend();
  --titer_end;

  for (auto titer = vectensor.rbegin(); titer != titer_end; ++titer) {
    if (titer != vectensor.rbegin()) {
      current = *titer;
      auto target = make_shared<Tensor>("i");
      target->push_back_regtensors(target);
      vector<shared_ptr<Tensor>> btensors;
      btensors.push_back(current);
      btensors.push_back(previous);
      BinaryContraction bc(btensors, target);
      target->push_back_regtensors(current->regtensors());
      target->push_back_regtensors(previous->regtensors());

      RefTree tmptree = make_shared<Tree>(bc, prevtree);

      list<RefTree> first = prevtree.first;
      for (auto& t : first)
        t->set_parent(tmptree);
      list<RefTree> second = prevtree.second;
      for (auto& t : second)
        t->set_parent(tmptree);

      list<RefTree> b(1, tmptree);
      prevtree = make_pair(a, b);
      previous = target;
    }
  }

  if (vectensor.size() > 1) {
    vector<shared_ptr<Tensor>> btensors;
    btensors.push_back(*titer_end);
    btensors.push_back(previous);
    BinaryContraction binc(btensors, par);
    binc_ = binc;
  } else {
    vector<shared_ptr<Tensor>> btensors;
    btensors.push_back(previous);
    BinaryContraction binc(btensors, par);
    binc_ = binc;
  }

  subtrees_ = prevtree;
}


Tree::~Tree() {

}


void Tree::factorize() {

  /// So far, we are assuming that the first tensor be an input tensor.
  /// therefore, we are now starting with the second subtrees

  vector<list<RefTree>::iterator> remove_list;

  for (auto i = subtrees_.second.begin(); i != subtrees_.second.end(); ++i) {
    if (find(remove_list.begin(), remove_list.end(), i) != remove_list.end()) continue;
    auto tmpi = i;
    ++tmpi;
    RefTree mysubtree = *i;
    BinaryContraction mybinc = mysubtree->binc();

    for (auto j = tmpi; j != subtrees_.second.end(); ++j) {
      if (find(remove_list.begin(), remove_list.end(), j) != remove_list.end()) continue;
      RefTree othersubtree = *j;
      BinaryContraction otherbinc = othersubtree->binc();

      if (mybinc.factorizable(otherbinc)) {
        mysubtree->merge_second_subtree(othersubtree);
        remove_list.push_back(j);
      }
    }
  }

  for (auto& k : remove_list)
    subtrees_.second.erase(k);


  /// recursive calls
  for (auto& i : subtrees_.second)  i->factorize();
  assert(subtrees_.first.empty());


}


void Tree::merge_second_subtree(RefTree other) {

  if (second_is_input())
    prepare_merge_second();
  if (other->second_is_input())
    other->prepare_merge_second();

  list<RefTree> othersub = other->subtrees_.second;

  subtrees_.second.merge(othersub);
  shared_ptr<Tensor> target = subtrees_.second.front()->target_tensor();

  list<SmartIndex> target_permutables = permutables_in_factorize(other);
  target->set_permutables(target_permutables);

  for (auto& i : subtrees_.second)
    i->renew_binc_target(target);

}


const list<SmartIndex> Tree::permutables_in_factorize(RefTree other) {
  list<RefTree> othersub = other->subtrees_.second;
  list<SmartIndex> permutables2 = othersub.front()->target_tensor()->permutables()->si();
  list<SmartIndex> permutables1 = subtrees_.second.front()->target_tensor()->permutables()->si();


  list<SmartIndex> primitive1;
  for (auto& i : permutables1) {
    list<SmartIndex> ps = i.extract();
    primitive1.insert(primitive1.end(), ps.begin(), ps.end());
  }

  list<SmartIndex> primitive2;
  for (auto& i : permutables2) {
    list<SmartIndex> ps = i.extract();
    primitive2.insert(primitive2.end(), ps.begin(), ps.end());
  }

  list<SmartIndex> target_permutables;

  assert(primitive1.size() == primitive2.size());

  SmartIndex s1 = primitive1.front();
  SmartIndex s2 = primitive2.front();
  auto ia = primitive1.begin(); ++ia;
  auto ja = primitive2.begin(); ++ja;
  for (auto i = ia, j = ja; i != primitive1.end(); ++i, ++j) {
    if (s1.permutable(*i) && s2.permutable(*j)) {
      s1.merge(*i);
      s2.merge(*j);
    } else {
      target_permutables.push_back(s1);
      s1 = *i;
      s2 = *j;
    }
  }
  target_permutables.push_back(s1);

  return target_permutables;
}


void Tree::prepare_merge_second() {

  vector<shared_ptr<Tensor>> b(1, binc_.second());
  auto target = make_shared<Tensor>("i");
  target->push_back_regtensors(target);
  BinaryContraction bc(b, target);

  list<RefTree> dummy;
  RefTree parent(this);
  RefTree sub = make_shared<Tree>(bc, make_pair(dummy, dummy), parent);

  binc_.set_second(target);
  subtrees_.second.push_back(sub);

}


int Tree::num_nodes() const {
  int out = 1;
//if (!is_root())
//  cout << binc_.show() << endl;
  list<RefTree> subtree1 = subtrees_.first;
  list<RefTree> subtree2 = subtrees_.second;

  for (auto& t : subtree1) out += t->num_nodes();
  for (auto& t : subtree2) out += t->num_nodes();

  return out;
}


const string Tree::show(const int i) const {
  string out;
  string indent(i*2, ' ');

  out += indent;
  out += binc_.show();
  out += "\n";

  list<RefTree> s1, s2;
  s1 = subtrees_.first;
  s2 = subtrees_.second;

  int next = i;
  ++next;
  for (auto& t : s1) out += t->show(next);
  for (auto& t : s2) out += t->show(next);

  return out;
}

