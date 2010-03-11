//
// Author:: Toru Shiozaki
// Date  :: Feb 2009
//
#ifndef _smith_vectensor_h
#define _smith_vectensor_h

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <src/cost.h>
#include <src/tensor.h>

class VecTensor {

  friend class Tensor;

  protected:
    std::vector<boost::shared_ptr<Tensor> > tensor_; 

    const int count_loops();
    const int count_holes();
    const std::map<int, int> index_num_pair() const;

  public:
    VecTensor(const std::vector<boost::shared_ptr<Tensor> >&);
    ~VecTensor();

    const std::vector<boost::shared_ptr<Tensor> > tensor() const {return tensor_;};
    std::vector<boost::shared_ptr<Tensor> >* tensor_pointer() {return &tensor_;};

    /// For printing out.
    const std::string show() const;
    /// Perform strength reduction; tensor_ will be reordered to minimize the cost.
    void strength_reduction(const bool);
    /// Replace tensor_.
    void set_tensor(const std::vector<boost::shared_ptr<Tensor> >& tensor) {tensor_ = tensor;};
    /// Returns operation and memory cost.
    const std::pair<Cost, Cost> cost_evaluater(const std::vector<boost::shared_ptr<Tensor> >) const;

    /// Refresh tensor::depth_ of tensors.
    void refresh_tensor_depth();
    /// Re-number all the indices after sorting.
    void refresh_indices();
    /// Determine the sign by counting the holes and loops; tensor_.back() will have it
    void refresh_sign();
    /// Determine the factor by looking at the topological symmetry; tensor_.back() will have it
    void refresh_factor();

    const bool identical(const VecTensor&) const;

};

typedef boost::shared_ptr<VecTensor> RefVecTensor;

#endif
