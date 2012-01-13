//
// Author : Toru Shiozaki
// Date   : Feb 2009
//
#include <cassert>
#include <algorithm>
#include <iostream>
#include <boost/regex.hpp>
#include <src/pretensor.h>

using namespace std;

PreTensor::PreTensor(const string tensor_str, const Spaces& sp) {
 
  boost::regex symbol_reg("(\\S+?)\\s\\(");
  boost::smatch what;
  boost::regex_search(tensor_str.begin(), tensor_str.end(), what, symbol_reg);
  string symb(what[1].first, what[1].second);
  symbol_ = symb;
 
  auto start = what[0].second;
  boost::regex index_reg("(\\S+?)\\s");
  while (regex_search(start, tensor_str.end(), what, index_reg)) {
    string indx_str(what[1].first, what[1].second);
    Index indx(indx_str, sp); 
    indices_.push_back(indx);
    start = what[0].second;
  }

}

PreTensor::~PreTensor() {

}


void PreTensor::find_com_num(RefPreTensor o) {
  for (auto iiter = indices_.begin(); iiter != indices_.end(); ++iiter) { 
    for (auto iiter2 = o->indices_.begin(); iiter2 != o->indices_.end(); ++iiter2) { 
      if (iiter->num() == iiter2->num()) {
        shared_ptr<int> num2 = iiter2->num_pointer();
        num2 = iiter->num_pointer();
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
    shared_ptr<Tensor> newtensor(new Tensor((*iter)->symbol(),depth));
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

    for (auto index_iter = listindex.begin(); index_iter != listindex.end(); ++index_iter) {
      const int current_num = (*index_iter).num();

      list<Index> list_by_current_index;
      list_by_current_index.push_back(*index_iter); 

      /// loop for other
      auto newiter2 = vectensor.begin();

      bool found = false;
      for (auto iter2 = pretensor_.begin(); iter2 != pretensor_.end(); ++iter2, ++newiter2) {
        if (iter == iter2) continue;

        shared_ptr<Tensor> othertensor = *newiter2;

        list<Index> listindex2 = (*iter2)->indices();

        for (auto index_iter2 = listindex2.begin(); index_iter2 != listindex2.end(); ++index_iter2) {
          if (index_iter2->num() == current_num) {
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
    for (auto i = removelist.begin(); i != removelist.end(); ++i) 
      smartindex.erase(*i);

    mytensor->index_init(smartindex);
  } 

  return RefVecTensor(new VecTensor(vectensor));
}

