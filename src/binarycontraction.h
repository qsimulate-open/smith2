//
// Author:: Toru Shiozaki
// Date  :: Feb 2009
//
#ifndef _smith_binarycontraction_h
#define _smith_binarycontraction_h

#include <src/tensor.h>

class BinaryContraction {
  protected:
    std::vector<std::shared_ptr<Tensor> > tensor_;
    std::shared_ptr<Tensor> target_tensor_;

    std::pair<PCost, PCost> cost_;

    std::pair<PCost, PCost> cost_evaluater(const std::vector<std::shared_ptr<Tensor> >&);
    void determine_target_indices(const std::vector<std::shared_ptr<Tensor> >&);
    void refresh_indices();

    // For implementation
    std::vector<std::vector<Block> > outerloop_;
    std::vector<std::vector<Block> > innerloop1_;
    std::vector<std::vector<Block> > innerloop2_;
    // Loop indices that reflects which index has come from which tensor etc.
    std::shared_ptr<SmartIndexList> loop_indices_; 
    std::shared_ptr<SmartIndexList> prod_indices_; 
    std::shared_ptr<SmartIndexList> prod_indices2_; 

  public:
    BinaryContraction(std::vector<std::shared_ptr<Tensor> >&, std::shared_ptr<Tensor>);
    BinaryContraction() {};
    ~BinaryContraction();

    /// returns if this is just an operator (no contraction)
    const bool one() const {return tensor_.size() == 1;};

    /// operation count for this contraction
    const PCost op()  const {return cost_.first; };
    /// memory cost for this contraction
    const PCost mem() const {return cost_.second; };

    /// A pair of tensor for this binary contraction
    const std::vector<std::shared_ptr<Tensor> > tensor() const {return tensor_; };
    /// returns tensors
    std::shared_ptr<Tensor> first() { return tensor_.at(0); };
    std::shared_ptr<Tensor> second() { return tensor_.at(1); };
    /// set the second tensor
    void set_second(std::shared_ptr<Tensor> i) { tensor_.at(1) = i; };

    /// pointer to the target tensor
    std::shared_ptr<Tensor> target_tensor() {return target_tensor_; };
    /// set target tensor
    void set_target_tensor(std::shared_ptr<Tensor> t) { target_tensor_ = t; };

    /// for printing out
    const std::string show() const;

    /// returns if two BC can be factorized or not.
    const bool factorizable(BinaryContraction&);

    //////////////////////////////////////////////////////////////////////////////////////////
    /// for implementation
    void set_outerloop();
    void set_innerloop();
    /// peform contraction runtime
    void perform();
    /// Returns loop_indices_
    const std::shared_ptr<SmartIndexList> loop_indices() const { return loop_indices_; };
};

#endif

