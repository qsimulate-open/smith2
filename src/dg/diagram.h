//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: diagram.h
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

#ifndef _dg_diagram_h
#define _dg_diagram_h

#include <string>
#include <list>
#include <map>
#include <src/dg/operator.h>
#include <src/dg/tensor.h>

namespace DG {

class Diagram {
  protected:
    // operators
    std::list<Operator> operators_;
    // tensors
    std::list<Tensor> tensors_;

    void update_contracted_indices(const std::list<std::pair<int, int> >, const std::vector<std::string>);
    void assign_dagger();

  public:
    Diagram(const std::list<Operator>&, const std::list<Tensor>&);
    Diagram(const Diagram& o) : operators_(o.operators()), tensors_(o.tensors()) {};
    Diagram(std::string);
    ~Diagram();

    // returns private members
    const std::list<Operator> operators() const { return operators_; };
    const std::list<Tensor> tensors() const { return tensors_; };

    // printing functions that return strings
    const std::string show() const;
    const std::string show_only_tensors() const;

    // count the number of creation operators 
    const int count_creation_indices() const;
    // make a list of contracted diagrams
    const std::list<Diagram> contract_one() const;

    // returns if this diagram is connected under the give rule.
    const bool connected(const std::pair<std::string, std::list<std::string> > rule) const; 

};

}

#endif

