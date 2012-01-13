//
// Author:: Toru Shiozaki
// Date  :: Feb 2009
//
#ifndef _smith_pretensor_h
#define _smith_pretensor_h

#include <string>
#include <vector>
#include <list>
#include <memory>
#include <src/index.h>
#include <src/vectensor.h>
#include <src/spaces.h>

class PreTensor {
  /// temporary object which interfaces the input strings and the tensor objects

  typedef std::shared_ptr<PreTensor> RefPreTensor;

  protected:
    std::string symbol_;  
    std::list<Index> indices_;
    int rank_;

  public:
    PreTensor(const std::string, const Spaces&); 
    ~PreTensor();

    const std::list<Index> indices() const {return indices_;};
    const std::string symbol() const {return symbol_;};
    const int rank() const {return rank_;};

    void find_com_num(RefPreTensor);
};
typedef std::shared_ptr<PreTensor> RefPreTensor;
     

class ListPreTensor {

  protected:
    std::list<RefPreTensor> pretensor_;

  public:
    ListPreTensor(const std::list<RefPreTensor>&);
    ~ListPreTensor();

    const std::list<RefPreTensor> pretensor() const { return pretensor_;}; 

    RefVecTensor analyze();
    void find_com_num(); 
};

typedef std::shared_ptr<ListPreTensor> RefListPreTensor;

#endif

