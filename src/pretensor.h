//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: pretensor.h
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

#ifndef _smith_pretensor_h
#define _smith_pretensor_h

#include <string>
#include <vector>
#include <list>
#include <memory>
#include <src/index.h>
#include <src/vectensor.h>

namespace smith2 {

class PreTensor {
  /// temporary object which interfaces the input strings and the tensor objects

  typedef std::shared_ptr<PreTensor> RefPreTensor;

  protected:
    std::string symbol_;
    std::list<Index> indices_;
    int rank_;

  public:
    PreTensor(const std::string);
    ~PreTensor();

    const std::list<Index> indices() const {return indices_;}
    const std::string symbol() const {return symbol_;}
    int rank() const {return rank_;}

    void find_com_num(RefPreTensor);
};
typedef std::shared_ptr<PreTensor> RefPreTensor;


class ListPreTensor {

  protected:
    std::list<RefPreTensor> pretensor_;

  public:
    ListPreTensor(const std::list<RefPreTensor>&);
    ~ListPreTensor();

    const std::list<RefPreTensor> pretensor() const { return pretensor_;}

    RefVecTensor analyze();
    void find_com_num();
};

typedef std::shared_ptr<ListPreTensor> RefListPreTensor;

}

#endif

