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

  if (!one()) {
    innerloop1_ = prod_indices_->create_loop(*prod_indices_);
    innerloop2_ = prod_indices2_->create_loop(*prod_indices2_);
    assert(innerloop1_.size() == innerloop2_.size());
  }

  // for debug
//#define LOCAL_DEBUG
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

    const size_t work1_size = tensor_[0]->memory_size_needed();
    DataType* work1 = new DataType[work1_size]; 
    DataType* work1_tmp = new DataType[work1_size]; 

    const size_t work2_size = tensor_[1]->memory_size_needed();
    DataType* work2 = new DataType[work2_size]; 
    DataType* work2_tmp = new DataType[work2_size]; 

    // debugging output
    cout << this->show() << endl;

    // we can parallelize loop in here.
    for (vector<vector<Block> >::const_iterator viter = outerloop_.begin(); viter != outerloop_.end(); ++viter) {

      // setting up restore tables (partial)
      vector<Block> target_first, target_second;
      for (vector<Block>::const_iterator iter = viter->begin(); iter != viter->end(); ++iter) {
        if (firstd == iter->my_tensor()) target_first.push_back(*iter);
        else target_second.push_back(*iter);
      }

      // store size
      long store_size = 1L; 
      for (vector<Block>::const_iterator biter = viter->begin(); biter != viter->end(); ++biter)
        store_size *= biter->size();

      // filling out zero to target memory, which assumes implicit conversion from double to DataType
      fill(target_tmp, target_tmp+store_size, 0.0); 

      // contractions will come here....
      for (vector<vector<Block> >::const_iterator iiter1 = innerloop1_.begin(), iiter2 = innerloop2_.begin();
                                                  iiter1 != innerloop1_.end(); ++iiter1, ++iiter2) {

        // restore input tiles 
        // :::::::::: here ::::::::::

        // completing restore tables; the contraction indices will come to innermost indices
        vector<Block> first(target_first); 
        first.insert(first.end(), iiter1->begin(), iiter1->end());
        vector<Block> second(target_second); 
        second.insert(second.end(), iiter2->begin(), iiter2->end());

        // switching to sindex for restore
        for (vector<Block>::iterator iter = first.begin(); iter != first.end(); ++iter) iter->swap_sindex();
        for (vector<Block>::iterator iter = second.begin(); iter != second.end(); ++iter) iter->swap_sindex();

        // obtain mapping
        const pair<bool, vector<int> > fpair = ts::sort(first.begin(), first.end(), true);
        const pair<bool, vector<int> > spair = ts::sort(second.begin(), second.end(), true);
        const bool restore_parity = (fpair.first)^(spair.first);

        // gathering block size information...
        vector<size_t> restore_block_sizes1, restore_block_sizes2;
        for (vector<Block>::const_iterator biter = first.begin(); biter != first.end(); ++biter)
          restore_block_sizes1.push_back(biter->size()); 
        for (vector<Block>::const_iterator biter = second.begin(); biter != second.end(); ++biter)
          restore_block_sizes2.push_back(biter->size()); 

        // sort data
        ts::sort_multidimensional_array<DataType>(work1_tmp, work1, restore_block_sizes1, fpair.second);
        ts::sort_multidimensional_array<DataType>(work2_tmp, work2, restore_block_sizes2, spair.second);

        // evaluating dgemm parameters
        long contract_size = 1L; 
        for (vector<Block>::const_iterator biter = iiter1->begin(); biter != iiter1->end(); ++biter)
          contract_size *= biter->size();
        long target1_size = 1L; 
        for (vector<Block>::const_iterator biter = target_first.begin(); biter != target_first.end(); ++biter)
          target1_size *= biter->size();
        const long target2_size = target_size / target1_size;

        // dgemm or zgemm, depending on DataType;
        ts::gemm<DataType>(target1_size, target2_size, contract_size, work1, work2, target_tmp, (restore_parity ? -1.0 : 1.0));

      }

      // ***********************************************************************************
      // NOTE that T(1,2,3,4) has a data structure for(1){ for(2){ for(3){ for(4){  } } } }
      // ***********************************************************************************
      // data has a structure as follows:
      vector<Block> sorted(target_second);
      sorted.insert(sorted.end(), target_first.begin(), target_first.end());

      // block info for sorting
      vector<size_t> store_block_sizes;
      for (vector<Block>::const_iterator biter = sorted.begin(); biter != sorted.end(); ++biter)
        store_block_sizes.push_back(biter->size()); 

      // creating a map
      const pair<bool, vector<int> > opair = ts::sort(sorted.begin(), sorted.end());

      // calculating the factor + permutation sign
      DataType factor = tensor_.at(1)->factor() * (opair.first ? -1.0 : 1.0);
 
      // sort indices of the target tensor
      ts::sort_multidimensional_array<DataType>(target_tmp, target, store_block_sizes, opair.second);
      ts::scale<DataType>(target, store_size, factor);

      // store a target tile, finally!
      // :::::::::: here ::::::::::
    } 

    delete[] target;
    delete[] target_tmp;
    delete[] work1;
    delete[] work1_tmp;
    delete[] work2;
    delete[] work2_tmp;

  // in case one tensor will be sorted and copied to the target tensor.
  } else {

  }
}

