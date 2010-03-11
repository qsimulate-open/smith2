//
// Author:: Toru Shiozaki
// Date  :: Jan 2010
//
#include <cassert>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <list>
#include <src/binarycontraction.h>
#include <src/my_algorithm.h>
#include <src/macro.h>

using namespace std;
using namespace boost;

// setting up (vector<vector<Tile> >)outerloop_ that is a protected member.
void BinaryContraction::set_outerloop() {

  shared_ptr<SmartIndexList> permutable = target_tensor_->indexinfo()->permutables();
  outerloop_ = loop_indices_->create_loop(*permutable);

}


void BinaryContraction::set_innerloop() {

///////////////////////////////////
//cout << this->show() << endl;
///////////////////////////////////
  if (!one()) {
    innerloop1_ = prod_indices_->create_loop(*prod_indices_);
    innerloop2_ = prod_indices2_->create_loop(*prod_indices2_);
    assert(innerloop1_.size() == innerloop2_.size());
  }

  // for debug
#define LOCAL_DEBUG
#ifdef LOCAL_DEBUG
  perform();
#endif
}


// this is the code that actually contracts tensors
void BinaryContraction::perform() {

  // "depth" of the first tensor.
  const int firstd = tensor_.front()->depth(); 

  // in case of two tensors contracted to give the target tensor
  if (!one()) {

    // calculating the memory size for the target tensor.
    const size_t target_size = target_tensor_->memory_size_needed();
    DataType* target = new DataType[target_size]; 
    DataType* target_tmp = new DataType[target_size]; 

    cout << this->show() << endl;

    // we can parallelize loop in here.
    for (vector<vector<Block> >::const_iterator viter = outerloop_.begin(); viter != outerloop_.end(); ++viter) {

      // setting up restore tables (partial)
      vector<Block> target_first, target_second;
      for (vector<Block>::const_iterator iter = viter->begin(); iter != viter->end(); ++iter) {
        if (firstd == iter->my_tensor())
          target_first.push_back(*iter);
        else
          target_second.push_back(*iter);
      }

      // contractions will come here....
      for (vector<vector<Block> >::const_iterator iiter1 = innerloop1_.begin(), iiter2 = innerloop2_.begin();
                                                  iiter1 != innerloop1_.end(); ++iiter1, ++iiter2) {

        // completing restore tables
        vector<Block> first(target_first.begin(), target_first.end()); 
        first.insert(first.end(), iiter1->begin(), iiter1->end());
        vector<Block> second(target_second.begin(), target_second.end()); 
        second.insert(second.end(), iiter2->begin(), iiter2->end());

        // TODO sindex is that of target tensor; therfore, we cannot sort easily for restore.

        // debugging output for Block::target_tensor_
        if (viter == ++(outerloop_.begin()) && iiter1 == ++(innerloop1_.begin())) {
          cout << "before sort: ";
          for (vector<Block>::const_iterator iter = first.begin(); iter != first.end(); ++iter) 
            cout << iter->show();
          cout << endl;
          sort(first.begin(), first.end());
          cout << "after  sort: ";
          for (vector<Block>::const_iterator iter = first.begin(); iter != first.end(); ++iter) 
            cout << iter->show();
          cout << endl << endl;
#if 0
          for (vector<Block>::const_iterator iter = second.begin(); iter != second.end(); ++iter) 
            cout << iter->show();
          cout << endl;
#endif
        }

      }

      // for debugging
#if 0
      if (viter == outerloop_.begin()) {
        for (vector<vector<Block> >::const_iterator iiter = innerloop_.begin(); iiter != innerloop_.end(); ++iiter) {
//        for (vector<Block>::const_iterator tmp = iiter->begin(); tmp != iiter->end(); ++tmp) cout << tmp->show() << " "; 
          cout << endl;
        }
      }
#endif

      // creating a map to reduced tensor.
      vector<Block> sorted(viter->begin(), viter->end());
      const pair<bool, vector<int> > opair = ts::sort(sorted.begin(), sorted.end());
      const bool parity = opair.first;
      const vector<int> ordering = opair.second;

      // factor + permutation sign from the sort below 
      DataType factor = tensor_.at(1)->factor() * (parity ? -1.0 : 1.0);
 
      // blocksizes 
      vector<size_t> store_block_sizes;
      size_t store_size = 1lu; 
      for (vector<Block>::const_iterator biter = viter->begin(); biter != viter->end(); ++biter) {
        const size_t current = biter->size();
        store_block_sizes.push_back(current); 
        store_size *= current;
      }

      // TODO output of contraction is not *viter; thus we need to change what follows.
      // first, sort indices of the target tensor
      if (*viter != sorted)
        ts::sort_multidimensional_array(target_tmp, target, store_block_sizes, ordering);
      else
        ::memcpy(target, target_tmp, store_size * sizeof(DataType));

      // then, scale.
      ts::scale<DataType>(target, store_size, factor);
    } 

    delete[] target;
    delete[] target_tmp;

  // in case one tensor will be sorted and copied to the target tensor.
  } else {

  }
}

