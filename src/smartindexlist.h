//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: smartindexlist.h
// Copyright (C) 2010 Toru Shiozaki
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

#ifndef _src_smartindexlist_h
#define _src_smartindexlist_h

#include <vector>
#include <list>
#include <src/smartindex.h>
#include <src/tensor.h>
#include <memory>

class SmartIndex;
class Tensor;

class SmartIndexList {

  friend class SmartIndex;
  friend class Tensor;
 
  protected:
    std::shared_ptr<std::list<SmartIndex> > si_;

  public:
    SmartIndexList(const std::list<SmartIndex>& inp) : si_(new std::list<SmartIndex>(inp)) {};
    SmartIndexList(const SmartIndexList& o) : si_(new std::list<SmartIndex>(o.si())) {};
    SmartIndexList() {};
    ~SmartIndexList() {};

    const SmartIndexList operator=(const std::list<SmartIndex>& inp) {
      std::shared_ptr<std::list<SmartIndex> > tmp(new std::list<SmartIndex>(inp)); si_ = tmp; };
    const SmartIndexList operator=(const SmartIndexList& inp) {
      std::shared_ptr<std::list<SmartIndex> > tmp(new std::list<SmartIndex>(inp.si())); si_ = tmp; };
    std::list<SmartIndex>& si() { return *si_; };
    const std::list<SmartIndex>& si() const { return *si_; };
    std::shared_ptr<std::list<SmartIndex> > si_pointer() { return si_; };

    // intrinsic functions in list<SmartIndex>
    void sort();
    std::list<SmartIndex>::iterator begin() { return si().begin(); };
    std::list<SmartIndex>::const_iterator begin() const { return si().begin(); };
    std::list<SmartIndex>::iterator end() { return si().end(); };
    std::list<SmartIndex>::const_iterator end() const { return si().end(); };
    int size() const { return si().size(); };
    bool empty() const { return si().empty(); };

};

#endif

