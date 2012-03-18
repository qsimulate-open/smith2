//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: operator.h
// Copyright (C) 2009 Toru Shiozaki
//
// Author: Toru Shiozaki <shiozaki.toru@gmail.com>
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

#ifndef _ig_operator_h
#define _ig_operator_h

#include <string>

namespace IG {

class Operator {
  protected:
    std::string symbol_;
    int rank_;
    bool general_;
    bool deexcitation_;

  public:
    Operator(const std::string, const int, const bool, const bool);
    ~Operator();

    const std::string symbol() const { return symbol_; };
    const unsigned int rank() const { return rank_; };
    const bool general() const { return general_; };
    const bool deexcitation() const { return deexcitation_; };
    const std::string show() const;

    bool operator<(const Operator& o) const { if (rank() != o.rank()) return rank() < o.rank(); else return symbol() < o.symbol(); };
    bool operator>(const Operator& o) const { if (rank() != o.rank()) return rank() > o.rank(); else return symbol() > o.symbol(); };
};

}

#endif
