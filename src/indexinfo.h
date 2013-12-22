//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: indexinfo.h
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

#ifndef _smith_indexinfo_h
#define _smith_indexinfo_h

#include <list>
#include <vector>
#include <memory>
#include <src/smartindexlist.h>

namespace smith2 {

class IndexInfo {
  protected:
    std::shared_ptr<SmartIndexList> si_;
    std::shared_ptr<SmartIndexList> perm_;

  public:
    IndexInfo(std::shared_ptr<SmartIndexList> a, std::shared_ptr<SmartIndexList> b) { si_ = a; perm_ = b; }
    IndexInfo(std::shared_ptr<SmartIndexList> a) { si_ = a; }
    ~IndexInfo() {}

    void set_smartindices(std::shared_ptr<SmartIndexList>& inp) { si_ = inp; }
    void set_permutables(std::shared_ptr<SmartIndexList>& inp) { perm_ = inp; }

    std::shared_ptr<SmartIndexList> smartindices() { assert(si_); return si_; }
    std::shared_ptr<SmartIndexList> permutables() { assert(perm_); return perm_; }

};

}

#endif

