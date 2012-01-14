//
// Author:: Toru Shiozaki
// Date  :: Mar 2009
//
#ifndef _dg_tensor_h
#define _dg_tensor_h

#include <string>
#include <list>
#include "indices.h"

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

#endif

