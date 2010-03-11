//
// Author : Toru Shiozaki
// Date   : February 2010
//

#ifndef __src_f77_h
#define __src_f77_h

extern "C" {
  void dscal_(const long*, const double*, double* const, const long*);
  void zscal_(const long*, const std::complex<double>*, std::complex<double>* const, const long*);
  void dgemm_(const char*, const char*, const long*, const long*, const long*, const double*, const double*, const long*,
              const double*, const long*, const double*, double*, const long*);
};

#endif

