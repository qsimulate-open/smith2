//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: equation.h
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

#ifndef _smith_equation_h
#define _smith_equation_h

#include <list>
#include <src/vectensor.h>
#include <src/tree.h>
#include <memory>

namespace smith2 {

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

}

#endif

