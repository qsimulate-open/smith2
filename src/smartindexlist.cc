//
// Author : Toru Shiozaki
// Date   : Jan 2010
//

#include <iomanip>
#include <src/tensor.h>
#include <string>
#include <stdexcept>
#include <sstream>

using namespace std;


void SmartIndexList::sort() {
  si().sort();
  for (auto iter = si().begin(); iter != si().end(); ++iter) iter->sort();
}

