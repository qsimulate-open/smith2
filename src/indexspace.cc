//
// Author : Toru Shiozaki
// Date   : Jan 2010
//

#include <src/indexspace.h>
#include <iomanip>
#include <sstream>

using namespace std;

string IndexSpace::show() {
  stringstream ss; 
  ss <<   "  ======= block info =======" << endl;
  ss <<   "   tag   #orb   spin   symm" << endl;
  for (auto iter = blocks_.begin(); iter != blocks_.end(); ++iter) {
    ss << "  " << setw(4) << iter->tag()
               << setw(7) << iter->fence() - iter->start()
               << setw(7) << iter->spin()
               << setw(7) << iter->symm() << endl; 
  }
  ss <<   "  ==========================" << endl;
  
  return ss.str();
}

size_t IndexSpace::max_block_size() const {
  size_t out = 0lu;
  for (auto iter = blocks_.begin(); iter != blocks_.end(); ++iter)
    out = max(iter->size(), out);
  assert(out != 0lu);
  return out;
}

