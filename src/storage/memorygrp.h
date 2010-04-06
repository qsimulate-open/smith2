//
// Author : Toru Shiozai
// Date   : March 2010
//

#ifndef __src_storage_memorygrp_h
#define __src_storage_memorygrp_h

#include <mpi.h>
#include <map>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <boost/shared_ptr.hpp>

class MemoryGrp {
  protected:
    long datasize_;

  public:
    MemoryGrp(int, char**, long);
    ~MemoryGrp();

    int n() const { return MPI::COMM_WORLD.Get_rank(); }; 
    int nodes() const { return MPI::COMM_WORLD.Get_size(); }; 
    void wait() const { MPI::COMM_WORLD.Barrier(); };

    void regist(const size_t size) {
      datasize_ -= size;
      if (datasize_ < 0) throw std::runtime_error("Needs more memory!");
    };
    void deregist(const size_t size) {
      datasize_ += size;
    };

};

class BlockedArray {
  protected:
    int me_;
    boost::shared_ptr<MemoryGrp> mem_;
    std::map<size_t, int> nodemap_; // tag, node

    char* data_;
    size_t localsize_;
    size_t localoffset_;

  public:
    BlockedArray(boost::shared_ptr<MemoryGrp> mem, const size_t tot, const std::map<size_t, int>& m)
      : mem_(mem), nodemap_(m), me_(mem->n()) {

      int first = 0;
      // TODO this part is improvable with some modification 
      for (std::map<size_t, int>::const_iterator miter = nodemap_.begin(); miter != nodemap_.end(); ++miter) {
        if (miter->second < me_) continue;
        if (first == 0) {
          localoffset_ = miter->second;
          first++;
        }
        if (miter->second > me_) {
          first++;
          localsize_ = miter->first - localoffset_; 
          break;
        }
      }
      // for the last node
      if (first == 1) localsize_ = tot - localoffset_;
      mem_->regist(localsize_);
      data_ = new char[localsize_];
    };

    ~BlockedArray() {
      mem_->deregist(localsize_);
      delete[] data_;
    }

    // returns the node that contains the block with a specific tag
    std::pair<size_t, int> whereis(const size_t tag) {
      const std::map<size_t, int>::const_iterator iter = nodemap_.find(tag);
      if (iter == nodemap_.end()) throw std::runtime_error("tag is not valid"); 

      return std::make_pair(iter->first - localoffset_, iter->second);
    };


};

#endif

