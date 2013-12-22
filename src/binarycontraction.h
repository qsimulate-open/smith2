//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: binarycontraction.h
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

#ifndef _smith_binarycontraction_h
#define _smith_binarycontraction_h

#include <src/tensor.h>

namespace smith2 {

class BinaryContraction {
  protected:
    std::vector<std::shared_ptr<Tensor> > tensor_;
    std::shared_ptr<Tensor> target_tensor_;

    std::pair<PCost, PCost> cost_;

    std::pair<PCost, PCost> cost_evaluater(const std::vector<std::shared_ptr<Tensor> >&);
    void determine_target_indices(const std::vector<std::shared_ptr<Tensor> >&);
    void refresh_indices();

  public:
    BinaryContraction(std::vector<std::shared_ptr<Tensor> >&, std::shared_ptr<Tensor>);
    BinaryContraction() {}
    ~BinaryContraction();

    /// returns if this is just an operator (no contraction)
    bool one() const {return tensor_.size() == 1;}

    /// operation count for this contraction
    const PCost op()  const {return cost_.first; }
    /// memory cost for this contraction
    const PCost mem() const {return cost_.second; }

    /// A pair of tensor for this binary contraction
    const std::vector<std::shared_ptr<Tensor> > tensor() const {return tensor_; }
    /// returns tensors
    std::shared_ptr<Tensor> first() { return tensor_.at(0); }
    std::shared_ptr<Tensor> second() { return tensor_.at(1); }
    /// set the second tensor
    void set_second(std::shared_ptr<Tensor> i) { tensor_.at(1) = i; }

    /// pointer to the target tensor
    std::shared_ptr<Tensor> target_tensor() {return target_tensor_; }
    /// set target tensor
    void set_target_tensor(std::shared_ptr<Tensor> t) { target_tensor_ = t; }

    /// for printing out
    const std::string show() const;

    /// returns if two BC can be factorized or not.
    bool factorizable(BinaryContraction&);

};

}

#endif

