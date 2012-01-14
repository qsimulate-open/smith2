//
// Author : Toru Shiozaki
// Date   : Feb 2009
//
#ifndef _smith_index_h
#define _smith_index_h

#include <string>
#include <iostream>
#include <vector>
#include <memory>

class Index {

  protected:
    // basic info for deriving equations.
    bool dagger_;
    std::string type_;
    std::shared_ptr<int> num_; 

  public:
    Index(const std::string);
    ~Index();

    // returns dagger_
    bool dagger() const { return dagger_; };
    // reutrns type_
    const std::string type() const { return type_; };
    // returns num_
    int num() const { return *num_; };
    // pointer to num_, which is used for updating num_ in Wick's theorem
    std::shared_ptr<int> num_pointer() { return num_; };

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

