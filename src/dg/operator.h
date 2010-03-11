//
// Author:: Toru Shiozaki
// Date  :: Mar 2009
//
#ifndef _dg_operator_h
#define _dg_operator_h

#include <string>
#include <list>
#include "indices.h"

namespace DG {

class Operator {
  protected:
    std::list<Indices> listindices_;

    void merge_indices();

  public:
    Operator(std::string, int);
    ~Operator();

    const std::list<Indices> listindices() const { return listindices_; }; 
    std::list<Indices>* listindices_pointer() { return &listindices_; }; 
    const std::string show() const;

    void delete_indices(const int, const int);
};

}

#endif

