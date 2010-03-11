//
// Author:: Toru Shiozaki
// Date  :: Feb 2009
//
#ifndef _smith_index_h
#define _smith_index_h

#include <string>
#include <iostream>
#include <vector>
#include <src/spaces.h>
#include <boost/shared_ptr.hpp>

class Index {

  protected:
    // basic info for deriving equations.
    bool dagger_;
    std::string type_;
    boost::shared_ptr<int> num_; 

    // runtime-information for CC etc. IndexSpace is basically vector<Block>
    IndexSpace indexspace_;

  public:
    Index(const std::string, const Spaces&);
    ~Index();

    // returns dagger_
    bool dagger() const { return dagger_; };
    // reutrns type_
    const std::string type() const { return type_; };
    // returns num_
    int num() const { return *num_; };
    // pointer to num_, which is used for updating num_ in Wick's theorem
    boost::shared_ptr<int> num_pointer() { return num_; };

    // returns maximum block size
    size_t max_block_size() const { return indexspace_.max_block_size(); };

    // show function
    const std::string show() const;

    // for implementation
    bool operator<(const Index& o) const { return num() < o.num(); };
    bool operator>(const Index& o) const { return num() > o.num(); };
    bool operator==(const Index& o) const { return num() == o.num(); };
    bool operator<=(const Index& o) const { return num() <= o.num(); };
    bool operator>=(const Index& o) const { return num() >= o.num(); };

    // returns index space
    IndexSpace indexspace() const { return indexspace_; };
};

#endif

