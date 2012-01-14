//
// Author:: Toru Shiozaki
// Date  :: Jan 2010
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

    // for implementation
    std::vector<std::vector<Block> > create_loop(const SmartIndexList&);
    // easy way to grub all the Index object as list<Index>
    const std::list<Index> indexlist() const;

};

#endif

