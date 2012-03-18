//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: vectensor.h
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

#ifndef _smith_vectensor_h
#define _smith_vectensor_h

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include <src/cost.h>
#include <src/tensor.h>

class VecTensor {

  friend class Tensor;

  protected:
    std::vector<std::shared_ptr<Tensor> > tensor_; 

    const int count_loops();
    const int count_holes();
    const std::map<int, int> index_num_pair() const;

  public:
    VecTensor(const std::vector<std::shared_ptr<Tensor> >&);
    ~VecTensor();

    const std::vector<std::shared_ptr<Tensor> > tensor() const {return tensor_;};

    /// For printing out.
    const std::string show() const;
    /// Perform strength reduction; tensor_ will be reordered to minimize the cost.
    void strength_reduction(const bool);
    /// Replace tensor_.
    void set_tensor(const std::vector<std::shared_ptr<Tensor> >& tensor) {tensor_ = tensor;};
    /// Returns operation and memory cost.
    const std::pair<Cost, Cost> cost_evaluater(const std::vector<std::shared_ptr<Tensor> >) const;

    /// Refresh tensor::depth_ of tensors.
    void refresh_tensor_depth();
    /// Re-number all the indices after sorting.
    void refresh_indices();
    /// Determine the sign by counting the holes and loops; tensor_.back() will have it
    void refresh_sign();
    /// Determine the factor by looking at the topological symmetry; tensor_.back() will have it
    void refresh_factor();

    const bool identical(const VecTensor&) const;

};

typedef std::shared_ptr<VecTensor> RefVecTensor;

#endif
