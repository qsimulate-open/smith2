//
// Author:: Toru Shiozaki
// Date  :: Feb 2009
//
#ifndef _src_indexspace_h
#define _src_indexspace_h

#include <vector>
#include <src/block.h>
#include <boost/shared_ptr.hpp>
#include <string>

class IndexSpace {
  protected:
    std::vector<Block> blocks_; 

  public:
    // constructor can be called with blocks_
    IndexSpace(const std::vector<Block>& inp) : blocks_(inp) {}; 
    // or use the default constructor and initialize later!
    IndexSpace() {};
    ~IndexSpace() {};

    // returns protected members
    const std::vector<Block> blocks() const { return blocks_; };

    // returns maximum block size
    size_t max_block_size() const;

    std::string show();
};

#endif

