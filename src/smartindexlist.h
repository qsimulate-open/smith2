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

// TODO I haven't understood fully the dependency and friendship... so far it works...

class SmartIndex;
class Tensor;

class SmartIndexList {

  friend class SmartIndex;
  friend class Tensor;
 
  protected:
    std::list<SmartIndex> si_;

  public:
    SmartIndexList(const std::list<SmartIndex>& inp) : si_(inp) {};
    SmartIndexList() {};
    ~SmartIndexList() {};

    const SmartIndexList operator=(const std::list<SmartIndex>& inp) { si_ = inp; };
    const SmartIndexList operator=(const SmartIndexList& inp) { si_ = inp.si(); };
    const std::list<SmartIndex> si() const { return si_; };
    std::list<SmartIndex>* si_pointer() { return &si_; };

    // intrinsic functions in list<SmartIndex>
    void sort();
    std::list<SmartIndex>::iterator begin() { return si_.begin(); };
    std::list<SmartIndex>::const_iterator begin() const { return si_.begin(); };
    std::list<SmartIndex>::iterator end() { return si_.end(); };
    std::list<SmartIndex>::const_iterator end() const { return si_.end(); };
    int size() const { return si_.size(); };
    bool empty() const { return si_.empty(); };

    // for implementation
    std::vector<std::vector<Block> > create_loop(const SmartIndexList&);
    // easy way to grub all the Index object as list<Index>
    const std::list<Index> indexlist() const;

};

#endif

