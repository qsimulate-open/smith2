//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: tree.h
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

#ifndef _src_tree_h
#define _src_tree_h

#include <list>
#include <src/binarycontraction.h>
#include <src/vectensor.h>
#include <memory>

namespace smith2 {

class Tree {
  typedef std::shared_ptr<Tree> RefTree;

  protected:
    /// A node corresponds to a binary contraction that is programmable.
    BinaryContraction binc_;
    /// Subtrees for forming the input tensors 
    /// In the sequenctial optimization, first is empty 
    std::pair<std::list<RefTree>, std::list<RefTree> > subtrees_;
    /// Pointer to the parent node
    std::weak_ptr<Tree> parent_;
    /// if root or not 
    bool root_;

  public:

    /// A constructer
    Tree(const BinaryContraction&, const std::pair<std::list<RefTree>, std::list<RefTree> >&, RefTree);
    /// A constructer with late initialization of parent
    Tree(const BinaryContraction&, const std::pair<std::list<RefTree>, std::list<RefTree> >&);
    /// A constructer for the bottom node; there is no contraction; no parent. 
    //Tree(std::pair<std::list<RefTree>, std::list<RefTree> >);
    /// for the first construction from the VecTensor;
    Tree(RefVecTensor, std::shared_ptr<Tensor>); 
    /// deallocater
    ~Tree();
    /// for Tree(RefVecTensor)
    void init(std::vector<std::shared_ptr<Tensor> >&, std::shared_ptr<Tensor>);

    /// returns binarycontraction 
    BinaryContraction binc() { return binc_; }; 
    /// returns subtrees
    const std::pair<std::list<RefTree>, std::list<RefTree> > subtrees() const { return subtrees_; };

    /// returns if this is the root or not
    const bool is_root() const { return root_; };
    /// returns if this is the last generation
    const bool is_leaf() const { return subtrees_.first.empty() && subtrees_.second.empty(); };
    /// returns if the first tensor is an input tensor
    const bool first_is_input()  const { return subtrees_.first.empty(); };
    /// returns if the second tensor is an input tensor
    const bool second_is_input() const { return subtrees_.second.empty(); };
    /// set parent (used with Tree(BinaryContraction&, std::pair<std::list<RefTree>, std::list<RefTree> >))
    void set_parent(RefTree par) { parent_ = par; };
    /// returns a pointer to the parent node
    RefTree parent() { return parent_.lock(); }; 

    /// renew the target tensor of binc_;
    void renew_binc_target(std::shared_ptr<Tensor> t) { binc_.set_target_tensor(t); }; 

    /// factirize
    void factorize();
    /// factorize with the first tensor
    void merge_second_subtree(RefTree);
    /// preparation for the factorization
    void prepare_merge_second();
    /// determine the permutability of the target tensor
    const std::list<SmartIndex> permutables_in_factorize(RefTree);

    /// count the number of nodes below
    const int num_nodes() const;

    /// printing out 
    const std::string show(const int i = 0) const;  

    /// returns target tensor of binc_
    std::shared_ptr<Tensor> target_tensor() { return binc_.target_tensor(); };

    ////// functions for implementation
    /// set outerloop
    void set_outerloop();
    /// set innerloop
    void set_innerloop();
};

}

#endif

