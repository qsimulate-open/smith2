//
// Author:: Toru Shiozaki
// Date  :: Feb 2009
//

#include <cassert>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <src/equation.h>
#include <src/tree.h>

using namespace std;

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
  for (auto i = remove.begin(); i != remove.end(); ++i)
    vectensor_.erase(*i);
}


void Equation::form_tree() {

  /// first, make a unfactorized tree (which has multiple edges at root_, 
  /// while having only one elsewhere) 

  list<RefTree> listtmptree, a;

  shared_ptr<Tensor> target(new Tensor("D"));
  target->push_back_regtensors(target); 

  for (auto vtiter = vectensor_.begin(); vtiter != vectensor_.end(); ++vtiter){ 
    RefVecTensor current = *vtiter;
    RefTree tmptree(new Tree(current, target));
    listtmptree.push_back(tmptree);
  }

  vector<shared_ptr<Tensor> > rb(1, target);
  BinaryContraction rootb(rb, target);

  RefTree root(new Tree(rootb, make_pair(a, listtmptree)));
  list<RefTree> child = listtmptree; 
  for (auto titer = child.begin(); titer != child.end(); ++titer) 
    (*titer)->set_parent(root);


  tree_root_ = root;
}


