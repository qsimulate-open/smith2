//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: index.h
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


#ifndef _smith_index_h
#define _smith_index_h

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <list>
#include <stdexcept>

// This defines the index classes. If you want to generalize this generator
// to more general cases (RASPT2, for instance), then just add some entry.
// Indices will be sorted using these numbers when tensors are canonicalized.
class Index_map {
  protected:
    std::list<std::pair<std::string, std::pair<int,int> > > map_;
  public:
    Index_map() { 
      map_.push_back(std::make_pair("h", std::make_pair(0, 28)));
      map_.push_back(std::make_pair("a", std::make_pair(1, 6)));
      map_.push_back(std::make_pair("p", std::make_pair(2, 232)));
      map_.push_back(std::make_pair("P", std::make_pair(3, 888)));
    };
    ~Index_map() {};
    int num_orb_class() const { return map_.size(); };
    int size() const { return num_orb_class(); };

    const int type(const std::string& type_) const {
      auto iter = map_.begin();
      for (; iter != map_.end(); ++iter) if (iter->first == type_) break;
      if (iter == map_.end()) throw std::runtime_error("key is no valid in Index::type()"); 
      return iter->second.first;
    };
    std::list<std::pair<std::string, std::pair<int,int> > >::const_iterator begin() const { return map_.begin(); };
    std::list<std::pair<std::string, std::pair<int,int> > >::const_iterator end() const { return map_.end(); };
};

class Index {

  protected:
    // basic info for deriving equations.
    bool dagger_;
    std::string type_;
    std::shared_ptr<int> num_;
    Index_map indmap_;

  public:
    Index(const std::string);
    ~Index();

    // returns dagger_
    bool dagger() const { return dagger_; };
    // reutrns type_
    const std::string type_str() const { return type_; };
    const int type() const { indmap_.type(type_); };
    // returns num_
    int num() const { return *num_; };
    // pointer to num_, which is used for updating num_ in Wick's theorem
    std::shared_ptr<int> num_pointer() { return num_; };
    // returns indmap_
    Index_map indmap() const { return indmap_; };

    // show function
    const std::string show() const;

    // for implementation
    bool operator<(const Index& o) const { return num() < o.num(); };
    bool operator>(const Index& o) const { return num() > o.num(); };
    bool operator==(const Index& o) const { return num() == o.num(); };
    bool operator<=(const Index& o) const { return num() <= o.num(); };
    bool operator>=(const Index& o) const { return num() >= o.num(); };

};

#endif

