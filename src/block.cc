//
// Author : Toru Shiozaki
// Date   : Jan 2010
//

#include <src/block.h>
#include <sstream>

using namespace std;
using namespace boost;

const string Block::show() const {
  stringstream ss;
  ss << tag_ << "_" << sindex_ << "(" << target_tensor_ << "," << my_tensor_ << ") " ;
  return ss.str();
}
 
