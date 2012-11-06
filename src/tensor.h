//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: tensor.h
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

#ifndef _smith_tensor_h
#define _smith_tensor_h

#include <string>
#include <list>
#include <vector>
#include <src/index.h>
#include <src/cost.h>
#include <src/indexinfo.h>
#include <src/smartindexlist.h>
#include <src/smartindex.h>
#include <memory>
#include <algorithm>

namespace smith2 {

class SmartIndex;
class SmartIndexList;

class Tensor {

  friend class SmartIndex;
  friend class SmartIndexList;

  protected:
    // A symbol for this tensor such as t, v, f, and so forth.
    std::string symbol_;  
    // Contains IndexInfo which is essentially a pair of SmartIndexList.
    std::shared_ptr<IndexInfo> indexinfo_;
    // This number reflects the localtion of the tensor in tree (0 at bottom)
    int depth_;
    // A factor that is attached to this tensor.
    double factor_;
    int rank_;

    bool index_initialized_;

    std::vector<std::shared_ptr<Tensor> > regtensors_;

  public:
    Tensor(const std::string symb, const std::list<SmartIndex> indx, const int dep=-1, const double fac=1.0 ); 
    Tensor(const std::string symb, const int dep=-1, const double fac=1.0 ); 
    Tensor(const Tensor&);
    Tensor(){};
    ~Tensor();

    /// "==" returns true, when two tensors are diagrammatically indistinguishable (for exact matching, use pointers)
    bool operator==(const Tensor& other) const;
    bool operator!=(const Tensor& other) const {return !((*this) == other);};
    bool operator<(const Tensor&) const;
    bool operator>(const Tensor& other) const { return !((*this) < other) ;};//&& !((*this) == other);};

    /// Late initialization of smartindices
    void index_init(const std::list<SmartIndex> indx);
    /// returns index_initialized_
    const bool index_initialized() const { return index_initialized_; };

    /// Returns the generation in which this tensor is placed
    const int depth() const {return depth_;};
    /// Set the generation in which this tensor is placed
    void set_depth(const int d) {depth_ = d; };

    /// The last tensor in VecTensor will be responsible to the overall factors and signs. 
    const double factor() const {return factor_;};
    void set_factor(const double fac) {factor_  = fac; };
    void mul_factor(const double fac) {factor_ *= fac; };
    /// Returns the rank of the tensor (e.g. t3.rank() = 3)
    const int rank() const {return rank_;};
    /// set rank (for target_tensor_ of BinaryContraction)
    void set_rank(const int r) { rank_ = r; };
    /// Returns the symbol of the tensor
    const std::string symbol() const {return symbol_;};

    /// Returns indexinfo_
    const std::shared_ptr<IndexInfo> indexinfo() const { return indexinfo_; };
    /// Returns the copy of smartindices_.
    const std::shared_ptr<SmartIndexList> smartindices() const { return indexinfo_->smartindices();};
    /// Returns the pointer of the smartindices of this object.
    std::shared_ptr<std::list<SmartIndex> > smartindices_pointer() { return indexinfo_->smartindices()->si_pointer();};
    /// Substitute into smartindices_
    void set_smartindices(const std::list<SmartIndex>& si) {
                                                             std::shared_ptr<SmartIndexList> tmp(new SmartIndexList(si));
                                                             std::shared_ptr<IndexInfo> tmp2(new IndexInfo(tmp));
                                                             indexinfo_ = tmp2;
                                                             set_permutables_for_input(si);
                                                           };

    /// Returns the copy of the permutables_
    const std::shared_ptr<SmartIndexList> permutables() const { return indexinfo_->permutables(); };
    /// Substitute into permutables_
    void set_permutables(const std::list<SmartIndex>& si) {
                                                            std::shared_ptr<SmartIndexList> tmp(new SmartIndexList(si));
                                                            indexinfo_->set_permutables(tmp);
                                                          }; 
    /// Set permutables for input tensors
    void set_permutables_for_input(const std::list<SmartIndex>& si);

    /// For printing out.
    const std::string show() const;
    /// Utility for tensor matching to the vector of tensors.
    const bool in_list(const std::vector<std::shared_ptr<Tensor> > vect) const {
      std::vector<const Tensor*> tmp;
      for (auto iter = vect.begin(); iter != vect.end(); ++iter)
        tmp.push_back(iter->get());
      return std::find(tmp.begin(), tmp.end(), this) != tmp.end();
    };
    /// For intermediate tensor---retruns a list of tensors that have been used to form this tensor
    const std::vector<std::shared_ptr<Tensor> > regtensors() const { return regtensors_; };
    /// Utility for tensor maching with memory
    const bool in_regtensors() const { return in_list(regtensors_); }; 
    /// add a tensor to regtensors_
    void push_back_regtensors(std::shared_ptr<Tensor> a) { regtensors_.push_back(a); };
    /// add tensors to regtensors_
    void push_back_regtensors(std::vector<std::shared_ptr<Tensor> > a) { regtensors_.insert(regtensors_.end(), a.begin(), a.end()); };

    /// Sort smartindices
    void sort_indices();

    /// Collect all the "num_" of the indices; dagger is a toggle.
    const std::vector<int> num_values(const int dagger = 0) const;
    /// Collect all the "num_" of the external indices; dagger is a toggle.
    const std::vector<int> num_values_external(const int dagger = 0) const;
    /// Collect all the "num_" of the hole indices.
    const std::vector<int> num_values_hole() const;

    /// returns if two tensors can be seen as identical (in factorization step)
    const bool identical(std::shared_ptr<Tensor>) const;

    //////////////////////////////////////////////// for implementation /////////////////////////////////////////////////
    /// returns the required memory size to store/restore this tensor 
    size_t memory_size_needed() const;


};

}
     
#endif
