//
// Author : Toru Shiozaki
// Date   : March 2010
//

#include <src/storage/memorygrp.h>

MemoryGrp::MemoryGrp(int a, char** b, long s) : datasize_(s) {
  MPI::Init(a, b);
};

MemoryGrp::~MemoryGrp() {
  MPI::Finalize();
};

