//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: indices.h
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

#ifndef _dg_indices_h
#define _dg_indices_h

#include <string>
#include <list>

namespace smith2 {
namespace DG {

class Indices {
  protected:
    std::string type_;
    std::list<int> nums_;
    bool dagger_;
    int source_;

  public:
    Indices(const std::string, const int);
    Indices(const Indices& o) : type_(o.type()), nums_(o.nums()), dagger_(o.dagger()), source_(o.source()) {};
    ~Indices();

    const std::string type() const { return type_; }; 
    const std::list<int> nums() const { return nums_; };
    const bool dagger() const { return dagger_; };
    const int source() const { return source_; };

    const std::string show() const;

    bool operator==(const Indices& other) const {
      return (type_ == other.type()) && (dagger_ == other.dagger()) && (source_ == other.source()); 
    };

    void add_nums(std::list<int> other) {nums_.merge(other); };
    void pop_front_nums() { nums_.pop_front(); };

    const bool contractable(const Indices&) const;

    const std::string cindex(const Indices&) const;

    void replace_index(const int, const int, const std::string);
    void delete_index(const int, const int);

    void set_dagger(const bool dag) { dagger_ = dag; };

};

}
}

#endif

