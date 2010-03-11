//
// Author : Toru Shiozaki
// Date   : Jan 2010
//

#include <iomanip>
#include <src/tensor.h>
#include <string>
#include <stdexcept>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

vector<vector<Block> > SmartIndexList::create_loop(const SmartIndexList& perm) {
  // "this" has a list of SmartIndex in si_
  vector<vector<Block> > out;

  int counter = 0;
  int current_perm = 0;
  list<SmartIndex>::const_iterator piter = perm.begin();
  for (list<SmartIndex>::const_iterator iter = si_.begin(); iter != si_.end(); ++iter) {
    const int thissize = iter->indices().size();
    if (current_perm == 0) {
      current_perm = piter->indices().size(); 
      ++piter;
      current_perm -= thissize;
      if (current_perm < 0) {
        const string filename = __FILE__;
        const string error_message = "Complicated permutation has not been considered yet. See file: "
                                      + filename + " line: " + lexical_cast<string>(__LINE__); 
        throw logic_error(error_message);
      }
    } else {
      current_perm -= thissize;
    }

    if (si_.begin() == iter) {
      out = iter->blocklist();
    } else {
      vector<vector<Block> > next;
      vector<vector<Block> > current = iter->blocklist();
      for (vector<vector<Block> >::iterator viter = current.begin(); viter != current.end(); ++viter) {
        // Updating Block::sindex_
        for (vector<Block>::iterator xiter = viter->begin(); xiter != viter->end(); ++xiter)
          xiter->update_sindex(counter);
        // Making direct products!  
        for (vector<vector<Block> >::const_iterator viter2 = out.begin(); viter2 != out.end(); ++viter2) {
          vector<Block> tmp = *viter2;
          tmp.insert(tmp.end(), viter->begin(), viter->end());
          next.push_back(tmp);
        }
      }
      out = next;
    }
    if (current_perm == 0) ++counter;
  }

//#define DEBUG_PRINT
#ifdef DEBUG_PRINT
  cout << "  ==== loop print out ====" << endl;
  for (vector<vector<Block> >::const_iterator iter = out.begin(); iter != out.end(); ++iter) {
    cout << "  ";
    for (vector<Block>::const_iterator iter2 = iter->begin(); iter2 != iter->end(); ++iter2) {
      cout << setw(4) << iter2->show() << " "; 
    }
    cout << endl;
  }
#endif

  return out;
}

const list<Index> SmartIndexList::indexlist() const {
  list<Index> out;
  for (list<SmartIndex>::const_iterator iter = si_.begin(); iter != si_.end(); ++iter) {
    const list<Index> tmp = iter->indices();
    out.insert(out.end(), tmp.begin(), tmp.end()); 
  }
  return out;
} 


void SmartIndexList::sort() {
  si_.sort();
  for (std::list<SmartIndex>::iterator iter = si_.begin(); iter != si_.end(); ++iter) iter->sort();
}

