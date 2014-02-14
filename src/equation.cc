//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: equation.cc
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
#include <iostream>
#include <src/equation.h>
#include <src/tree.h>

using namespace std;
using namespace smith2;

Equation::Equation(const list<RefVecTensor> lvt) : vectensor_(lvt) {

}

Equation::~Equation() {

}

void Equation::strength_reduction(const bool opt_memory) {
  vector<list<RefVecTensor>::iterator> remove;

  // currently we are only looking for the sequential multiplications
  for (auto vtiter = vectensor_.begin(); vtiter != vectensor_.end(); ++vtiter) {
    (*vtiter)->strength_reduction(opt_memory);
    for (auto otiter = vectensor_.begin(); otiter != vtiter; ++otiter) {
      if ((*vtiter)->identical(*(*otiter))) {
        remove.push_back(vtiter);
        break;
      }
    }
  }
  for (auto& i : remove)
    vectensor_.erase(i);

  for (auto& i : vectensor_)
    cout << i->show() << endl << endl;
}


void Equation::form_tree() {

  /// first, make a unfactorized tree (which has multiple edges at root_,
  /// while having only one elsewhere)

  list<RefTree> listtmptree, a;

  auto target = make_shared<Tensor>("D");
  target->push_back_regtensors(target);

  for (auto& vt : vectensor_){
    listtmptree.push_back(make_shared<Tree>(vt, target));
  }

  vector<shared_ptr<Tensor>> rb(1, target);
  BinaryContraction rootb(rb, target);

  auto root = make_shared<Tree>(rootb, make_pair(a, listtmptree));
  list<RefTree> child = listtmptree;
  for (auto& t : child)
    t->set_parent(root);


  tree_root_ = root;
}


