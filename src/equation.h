//
// Author : Toru Shiozaki
// Date   : Feb 2009
//
#ifndef _smith_equation_h
#define _smith_equation_h

#include <list>
#include <src/vectensor.h>
#include <src/tree.h>
#include <memory>

class Equation {
  typedef std::shared_ptr<Tree> RefTree;

  protected:
    std::list<RefVecTensor> vectensor_;
    RefTree tree_root_;

  public:
    Equation(const std::list<RefVecTensor>);
    ~Equation();

    // performs strength reduction to achieve (quasi)optimal operation counts
    void strength_reduction(const bool); 
    
    // returns private member with const
    const std::list<RefVecTensor> vectensor() const { return vectensor_; }; 

    // returns operation tree at the root
    RefTree tree_root() { return tree_root_; }; 

    // forms tree...
    void form_tree(); 

    // factorizes tree...
    void factorize() { assert(tree_root_); tree_root_->factorize(); };

    //// for implementation
    // startup some prequisites
    void startup() {
      set_outerloop();
      set_innerloop();
    };

    // set tile loop for the target tensor
    void set_outerloop() { assert(tree_root_); tree_root_->set_outerloop(); };
    // set tile loop for contraction
    void set_innerloop() { assert(tree_root_); tree_root_->set_innerloop(); };
};

#endif

