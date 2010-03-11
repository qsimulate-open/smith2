//
// Author:: Toru Shiozaki
// Date  :: Jan 2010
//
#ifndef _smith_indexinfo_h
#define _smith_indexinfo_h

#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <src/smartindexlist.h>

class IndexInfo {
  protected:
    boost::shared_ptr<SmartIndexList> si_;
    boost::shared_ptr<SmartIndexList> perm_;

  public:
    IndexInfo(boost::shared_ptr<SmartIndexList> a, boost::shared_ptr<SmartIndexList> b) { si_ = a; perm_ = b; };
    IndexInfo(boost::shared_ptr<SmartIndexList> a) { si_ = a; };
    ~IndexInfo() {};

    void set_smartindices(boost::shared_ptr<SmartIndexList>& inp) { si_ = inp; };
    void set_permutables(boost::shared_ptr<SmartIndexList>& inp) { perm_ = inp; };

    boost::shared_ptr<SmartIndexList> smartindices() { return si_; };
    boost::shared_ptr<SmartIndexList> permutables() { return perm_; };

};

#endif

