//
// Author:: Toru Shiozaki
// Date  :: Feb 2009
//
#ifndef _smith_cost_h
#define _smith_cost_h

#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <boost/lexical_cast.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>

#include <iostream>

// may be substituted by the actual numbers...
#define NUM_COMP_PARTICLE 888
#define NUM_PARTICLE 232
#define NUM_HOLE 28


class PCost {
  
  protected: 
    boost::tuple<int,int,int> pcost_;// hole, particle, complete particle 
 
  public:
    PCost(const boost::tuple<int,int,int>& pcst): pcost_(pcst) { };
    PCost(int i, int j, int k): pcost_(boost::make_tuple(i,j,k)) {};
    PCost() { };
    ~PCost() { };

    bool operator<(const PCost& other) const { return pcost_total() < other.pcost_total(); };
    bool operator>(const PCost& other) const  { return pcost_total() > other.pcost_total(); };
    bool operator==(const PCost& other) const { return pcost() == other.pcost(); };
    bool operator!=(const PCost& other) const { return !(*this == other);};

    const double pcost_total() const { return
       ::log(static_cast<double>(NUM_HOLE         ))*boost::get<0>(pcost_) 
     + ::log(static_cast<double>(NUM_PARTICLE     ))*boost::get<1>(pcost_)
     + ::log(static_cast<double>(NUM_COMP_PARTICLE))*boost::get<2>(pcost_);
    }
    const boost::tuple<int,int,int> pcost() const { return pcost_;};

    void add(int, int, int); 

    const std::string show() const;

};


class Cost {

  protected: 
    std::vector<PCost> cost_; 

  public:
    Cost(const std::vector<PCost>& cst) : cost_(cst) { };
    Cost() { };
    ~Cost() {};

    bool operator<(const Cost& other) const {
      std::vector<PCost> otherc = other.cost();
      std::vector<PCost> myc = cost();
      for (auto i = myc.begin(), j = otherc.begin(); i != myc.end(); ++i, ++j) {
        if (j == otherc.end()) return false;
        if      (*i < *j)      return true; 
        else if (*i > *j)      return false;
      }
      return true; 
    };

#define use_old_code_cost 0
#if use_old_code_cost
    bool operator==(const Cost& other) const { 
      std::vector<PCost> otherc = other.cost();
      std::vector<PCost> myc = cost();
      if (myc.size() != otherc.size()) return false;
      for (auto i = myc.begin(), j = otherc.begin(); i != myc.end(); ++i, ++j) {
        if (*i != *j ) return false;
      }
      return true;
    }
#else
    bool operator==(const Cost& other) const { return cost()==other.cost(); };
#endif
    bool operator!=(const Cost& other) const { return !(*this == other);};
    bool operator>(const Cost& other)  const { return !(*this < other);};

    const std::vector<PCost> cost() const {return cost_;};

    void add_pcost(const PCost& p) { cost_.push_back(p); };
    void add_pcost(int i, int j, int k) { PCost a(i, j, k); cost_.push_back(a); };

    const std::string show() const;

    void sort_pcost();

};


#endif

