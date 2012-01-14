//
// Author : Toru Shiozaki
// Date   : Mar 2009
//
#ifndef _dg_indices_h
#define _dg_indices_h

#include <string>
#include <list>

namespace DG {

class Indices {
  protected:
    std::string type_;
    std::list<int> nums_;
    bool dagger_;
    int source_;

  public:
    Indices(const std::string, const int);
    Indices(const Indices& o) : type_(o.type()), nums_(o.nums()), dagger_(o.dagger()), source_(o.source()) {};
    ~Indices();

    const std::string type() const { return type_; }; 
    const std::list<int> nums() const { return nums_; };
    const bool dagger() const { return dagger_; };
    const int source() const { return source_; };

    const std::string show() const;

    bool operator==(const Indices& other) const {
      return (type_ == other.type()) && (dagger_ == other.dagger()) && (source_ == other.source()); 
    };

    void add_nums(std::list<int> other) {nums_.merge(other); };
    void pop_front_nums() { nums_.pop_front(); };

    const bool contractable(const Indices&) const;

    const std::string cindex(const Indices&) const;

    void replace_index(const int, const int, const std::string);
    void delete_index(const int, const int);

    void set_dagger(const bool dag) { dagger_ = dag; };

};

}

#endif

