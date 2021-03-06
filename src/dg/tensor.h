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

#ifndef _dg_tensor_h
#define _dg_tensor_h

#include <string>
#include <list>
#include "indices.h"

namespace smith2 {
namespace DG {

class Tensor {
  protected:
    std::string symbol_;
    std::list<Indices> listindices_; 

    const bool connected_(const Tensor&) const;

  public:
    Tensor(std::string);
    ~Tensor();

    // printing function that returns string
    const std::string show() const; 

    // returns a private member
    const std::string symbol() const { return symbol_; }; 

    // returns a private member
    const std::list<Indices> listindices() const { return listindices_; };
    std::list<Indices>* listindices_pointer() { return &listindices_; };

    // set dagger_ to first size/2 indices
    void assign_dagger();

    void replace_indices(const int first, const int second, const std::string type) {
      for (std::list<Indices>::iterator iiter = listindices_.begin(); iiter != listindices_.end(); ++iiter) 
        (*iiter).replace_index(first, second, type); 
    };

    // returns if this tensor is connected to other tensor. 
    const bool connected(const Tensor&, const std::pair<std::string, std::list<std::string> >) const;

};

}
}

#endif

