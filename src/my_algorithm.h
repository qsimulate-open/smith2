//
// Author : Toru Shiozaki
// Date   : February
//

#ifndef __src_my_algorithm_h
#define __src_my_algorithm_h

#include <map>
#include <complex>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <src/macro.h>
#include <src/f77.h>

namespace ts {

  // Scale function.
  template<class T>
  void scale(T* const data, const size_t size, const T& factor) {
    for (size_t i = 0; i != size; ++i) data[i] *= factor;
  };

  template<>
  void scale(double* const data, const size_t size, const double& factor) {
    const long unit = 1L;
    const long ssize = size;
    dscal_(&ssize, &factor, data, &unit);
  }; 

  template<>
  void scale(std::complex<double>* const data, const size_t size, const std::complex<double>& factor) {
    const long unit = 1L;
    const long ssize = size;
    zscal_(&ssize, &factor, data, &unit);
  }; 

  template<class T>
  void gemm(const long, const long, const long, const T*, const T*, T*, T) {
    std::stringstream ss;
    ss << "This type of gemm is not supported. File: " << __FILE__ << " Line: " << __LINE__;
    throw std::logic_error(ss.str());
  };

  template<>
  void gemm(const long t1, const long t2, const long c, const double* w1, const double* w2, double* target, double coeff) {
    const long unit = 1L;
    const double one = 1.0;
    dgemm_("T", "N", &t1, &t2, &c, &one, w1, &c, w2, &c, &coeff, target, &t1);
  };

  // Sort function.
  // T should be an iterator or a pointer...
  // (-1)^parity is gonna be a permutation factor.
  // This is just a simple bubble sort; therfore, can be used for std::list also.
  template<class T>
  std::pair<bool, std::vector<int> > sort(const T& start, const T& end, const bool restore = false) {
    
    const size_t num_elements = end - start;
    assert(num_elements > 0);

    // setting up ordering vector
    bool parity = false; 
    std::vector<int> ordering(num_elements);
    {
      int tmp = 0; 
      for (auto iter = ordering.begin(); iter != ordering.end(); ++iter, ++tmp) *iter = tmp; 
    }

    // defining the start iterators for vector<int> ordering
    const auto ostart = ordering.begin(); 
    
    int innerloop_end = num_elements;
    for (T target = start; target != end-1; ++target, --innerloop_end) { 
      for (size_t i = 1; i != innerloop_end; ++i) { 
        if (*(start+i) < *(start+i-1)) {
          std::swap(*(start+i), *(start+i-1));
          std::swap(*(ostart+i), *(ostart+i-1));
          parity = !parity;
        }
      }
    }

    // ordering means that we will sort the tensor as 
    // (0, 1, 2, 3) => (ordering[0], ordering[1], ordering[2], ordering[3]) such as (1, 0, 2, 3).

    // if we want to...
    // ordering means that we will sort the tensor as 
    // 0->ordering[0], 1->ordering[1], 2->ordering[2], 3->ordering[3] and so on.
    // setup std::map for inverting this map
    if (restore) {
      std::map<int, int> util;
      int i = 0;
      for (auto iter = ordering.begin(); iter != ordering.end(); ++iter, ++i)
        util[*iter] = i; 

      // util is already sorted, so extract an info
      auto oiter = ordering.begin();
      for (auto miter = util.begin(); miter != util.end(); ++miter, ++oiter) {
        *oiter = miter->second;
      }
    }

    return make_pair(parity, ordering);
  };


  // Tensor sort function; variable dimension.
  // DateType signifies either double or complex<double>.
  template<class T>
  void sort_multidimensional_array
                (const T* const inp,
                 T* out,
                 const std::vector<size_t>& blocksizes,
                 const std::vector<int>& ordering)
 {
    assert(blocksizes.size() == ordering.size());

    const T* current = inp; 

    // TODO should be better to devise a generic algorithm... Blame me.
    if (blocksizes.size() == 2) {
      const size_t b0 = blocksizes[0];
      const size_t b1 = blocksizes[1];
      for (size_t i1 = 0; i1 != b1; ++i1) {
        size_t i10 = i1;
        for (size_t i0 = 0; i0 != b0; ++i0, ++current, i10 += b1)
          out[i10] = *current; 
      }
    } else if (blocksizes.size() == 4) {
      const size_t b0 = blocksizes[0];
      const size_t b1 = blocksizes[1];
      const size_t b2 = blocksizes[2];
      const size_t b3 = blocksizes[3];
      const int o0 = ordering[0];
      const int o1 = ordering[1];
      const int o2 = ordering[2];
      const int o3 = ordering[3];
      const size_t nb0 = blocksizes[o0];
      const size_t nb1 = blocksizes[o1];
      const size_t nb2 = blocksizes[o2];
      const size_t nb3 = blocksizes[o3];
      size_t i[4];
      for (i[3] = 0; i[3] != b3; ++i[3]) {
        for (i[2] = 0; i[2] != b2; ++i[2]) {
          for (i[1] = 0; i[1] != b1; ++i[1]) {
            for (i[0] = 0; i[0] != b0; ++i[0], ++current) {
              const size_t ni0 = i[o0];
              const size_t ni1 = i[o1];
              const size_t ni2 = i[o2];
              const size_t ni3 = i[o3];
              const size_t iall = ni0 + nb0 * (ni1 + nb1 * (ni2 + nb2 * ni3)); 
              out[iall] = *current;
            }
          }
        }
      }
    }
  };

}

#endif

