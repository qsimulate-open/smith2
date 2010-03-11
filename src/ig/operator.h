//
// Author:: Toru Shiozaki
// Date  :: Apr 2009
//
#ifndef _ig_operator_h
#define _ig_operator_h

#include <string>

namespace IG {

class Operator {
  protected:
    std::string symbol_;
    int rank_;
    bool general_;
    bool deexcitation_;

  public:
    Operator(const std::string, const int, const bool, const bool);
    ~Operator();

    const std::string symbol() const { return symbol_; };
    const unsigned int rank() const { return rank_; };
    const bool general() const { return general_; };
    const bool deexcitation() const { return deexcitation_; };
    const std::string show() const;

    bool operator<(const Operator& o) const { if (rank() != o.rank()) return rank() < o.rank(); else return symbol() < o.symbol(); };
    bool operator>(const Operator& o) const { if (rank() != o.rank()) return rank() > o.rank(); else return symbol() > o.symbol(); };
};

}

#endif
