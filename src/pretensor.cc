//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: pretensor.cc
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

#include <cassert>
#include <algorithm>
#include <iostream>
#include <boost/regex.hpp>
#include <src/pretensor.h>

using namespace std;
using namespace smith2;

PreTensor::PreTensor(const string tensor_str) {

  boost::regex symbol_reg("(\\S+?)\\s\\(");
  boost::smatch what;
  boost::regex_search(tensor_str.begin(), tensor_str.end(), what, symbol_reg);
  string symb(what[1].first, what[1].second);
  symbol_ = symb;

  auto start = what[0].second;
  boost::regex index_reg("(\\S+?)\\s");
  while (regex_search(start, tensor_str.end(), what, index_reg)) {
    string indx_str(what[1].first, what[1].second);
    Index indx(indx_str);
    indices_.push_back(indx);
    start = what[0].second;
  }

}

PreTensor::~PreTensor() {

}


void PreTensor::find_com_num(RefPreTensor o) {
  for (auto& ii : indices_) {
    for (auto& jj : o->indices_) {
      if (ii.num() == jj.num()) {
        shared_ptr<int> num2 = jj.num_pointer();
        num2 = ii.num_pointer();
      }
    }
  }
}


//////////////////////////////////////////////////////////////////////////////////


ListPreTensor::ListPreTensor(const list<RefPreTensor>& pretensr): pretensor_(pretensr) {

  find_com_num();

}


void ListPreTensor::find_com_num() {
  for (auto ptiter = pretensor_.begin(); ptiter != pretensor_.end(); ++ptiter) {
    auto ptiter2 = ptiter;
    ++ptiter2;
    const auto ptiter_start = ptiter2;

    for (ptiter2 = ptiter_start; ptiter2 != pretensor_.end(); ++ptiter2) {
      (*ptiter)->find_com_num(*ptiter2);
    }
  }

}


ListPreTensor::~ListPreTensor() {

}


RefVecTensor ListPreTensor::analyze() {

  /// first create List<shared_ptr<Tensor> >
  vector<shared_ptr<Tensor> > vectensor;
  int depth = 0;
  for (auto iter = pretensor_.begin(); iter != pretensor_.end(); ++iter, ++depth) {
    auto newtensor = make_shared<Tensor>((*iter)->symbol(),depth);
    newtensor->push_back_regtensors(newtensor);
    vectensor.push_back(newtensor);
  }

  /// then initialize smartindex
  /// loop for self
  auto newiter = vectensor.begin();
  for (auto iter = pretensor_.begin(); iter != pretensor_.end(); ++iter, ++newiter) {
    shared_ptr<Tensor> mytensor = *newiter;

    list<Index> listindex = (*iter)->indices();

    list<SmartIndex> smartindex;

    for (auto& index : listindex) {
      const int current_num = index.num();

      list<Index> list_by_current_index;
      list_by_current_index.push_back(index);

      /// loop for other
      auto newiter2 = vectensor.begin();

      bool found = false;
      for (auto iter2 = pretensor_.begin(); iter2 != pretensor_.end(); ++iter2, ++newiter2) {
        if (iter == iter2) continue;

        shared_ptr<Tensor> othertensor = *newiter2;

        list<Index> listindex2 = (*iter2)->indices();

        for (auto& index2 : listindex2) {
          if (index2.num() == current_num) {
            found = true;
            break;
          }
        }
        if (found) {
          SmartIndex current_smartindex(list_by_current_index, mytensor, othertensor);
          smartindex.push_back(current_smartindex);
          break;
        }
      }
      if (!found) {
        /// NULL means that it is external
        shared_ptr<Tensor> null_shared_ptr;
        SmartIndex current_smartindex(list_by_current_index, mytensor, null_shared_ptr);
        smartindex.push_back(current_smartindex);
      }
    }

    /// merge if possible
    vector<list<SmartIndex>::iterator> removelist;
    for (auto siter = smartindex.begin(); siter != smartindex.end(); ++siter) {
      if ((*siter).indices().empty()) removelist.push_back(siter);

      auto siter_now = siter;
      ++siter_now;
      for (auto siter2 = siter_now; siter2 != smartindex.end(); ++siter2) {
        if(siter->permutable(*siter2)) siter->merge(*siter2);
      }
    }
    for (auto& i : removelist)
      smartindex.erase(i);

    mytensor->index_init(smartindex);
  }

  return make_shared<VecTensor>(vectensor);
}

