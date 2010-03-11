//
// Author:: Toru Shiozaki
// Date  :: Jan 2010
//
#ifndef _src_block_h
#define _src_block_h

#include <string>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

class Block {
  protected:
    // tag of this block
    long tag_;
    // start and fence (molecular orbitals) 
    long start_;
    long fence_;

    // spin of this tile; 0 alpha, 1 beta
    int spin_;
    // symmetry of this block;
    int symm_;

    // in the restore/store
    int sindex_;

    // target tensor
    int target_tensor_;
    // my tensor
    int my_tensor_;

  public:
    // Block set up will be done in the interface to integrals
    Block(const long tag__, const long start__, const long fence__, const int spin__, const int sym__)
      : tag_(tag__), start_(start__), fence_(fence__), spin_(spin__), symm_(sym__), sindex_(0) {};
    // Default constructor for debugging, doing nothing
    Block() : sindex_(0) {};
    ~Block() {};

    // Some funcitons to retrieve private members.
    long tag() const { return tag_; };
    long start() const { return start_; };
    long fence() const { return fence_; };
    int spin() const { return spin_; };
    int symm() const { return symm_; };

    // returns the number of orbitals contained in this block
    size_t size() const { return fence_ - start_; };

    // setting target_tesnor_ (used in SmartIndex::blocklist() const
    void set_target_tensor(int t) { target_tensor_ = t; };
    // setting my_tesnor_ (used in SmartIndex::blocklist() const
    void set_my_tensor(int t) { my_tensor_ = t; };

    // return target/my tensors
    int target_tensor() const { return target_tensor_; };
    int my_tensor() const { return my_tensor_; };

    // for sorting blocks
    bool operator>(const Block& other)  const { return (sindex_<<8)+tag_ >  (other.sindex_<<8)+other.tag_; };
    bool operator>=(const Block& other) const { return (sindex_<<8)+tag_ >= (other.sindex_<<8)+other.tag_; };
    bool operator<(const Block& other)  const { return (sindex_<<8)+tag_ <  (other.sindex_<<8)+other.tag_; };
    bool operator<=(const Block& other) const { return (sindex_<<8)+tag_ <= (other.sindex_<<8)+other.tag_; };
    bool operator==(const Block& other) const { return (sindex_<<8)+tag_ == (other.sindex_<<8)+other.tag_; };
    bool operator!=(const Block& other) const { return (sindex_<<8)+tag_ != (other.sindex_<<8)+other.tag_; };

    // debugging function
    const std::string show() const;

    // update sindex_
    void update_sindex(const int inp) { sindex_ = inp; };
};

#endif

