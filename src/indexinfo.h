//
// Author:: Toru Shiozaki
// Date  :: Jan 2010
//
#ifndef _smith_indexinfo_h
#define _smith_indexinfo_h

#include <list>
#include <vector>
#include <memory>
#include <src/smartindexlist.h>

class IndexInfo {
  protected:
    std::shared_ptr<SmartIndexList> si_;
    std::shared_ptr<SmartIndexList> perm_;

  public:
    IndexInfo(std::shared_ptr<SmartIndexList> a, std::shared_ptr<SmartIndexList> b) { si_ = a; perm_ = b; };
    IndexInfo(std::shared_ptr<SmartIndexList> a) { si_ = a; };
    ~IndexInfo() {};

    void set_smartindices(std::shared_ptr<SmartIndexList>& inp) { si_ = inp; };
    void set_permutables(std::shared_ptr<SmartIndexList>& inp) { perm_ = inp; };

    std::shared_ptr<SmartIndexList> smartindices() { return si_; };
    std::shared_ptr<SmartIndexList> permutables() { return perm_; };

};

#endif

