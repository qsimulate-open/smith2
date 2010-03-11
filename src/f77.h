//
// Author : Toru Shiozaki
// Date   : February 2010
//

#ifndef __src_f77_h
#define __src_f77_h

extern "C" {
  void dscal_(const long*, const double*, double* const, const long*);
  void zscal_(const long*, const std::complex<double>*, std::complex<double>* const, const long*);
};

#endif

