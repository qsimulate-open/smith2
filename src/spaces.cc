//
// Author : Toru Shiozaki
// Date   : Jan 2010
//

#include <src/spaces.h>
#include <src/block.h>

using namespace std;

Spaces::Spaces(const long num_occ_blocks, const long num_vir_blocks,
               const vector<long>& num_occ_orb, const vector<long>& num_vir_orb,
               const vector<long>& sym_occ, const vector<long>& sym_vir) {

  int tag = 0;
  int offset = 0;
  // occupied blocks
  vector<Block> occ_blocks;
  for (auto iter = num_occ_orb.begin(), siter = sym_occ.begin();    
        iter != num_occ_orb.end(); ++iter, ++siter, ++tag) {
    Block current(tag, offset, offset+*iter, 0, *siter); // tag, orb_start, orb_end, spin, symm
    occ_blocks.push_back(current);
    offset += *iter;
  }
  for (auto iter = num_occ_orb.begin(), siter = sym_occ.begin();
        iter != num_occ_orb.end(); ++iter, ++siter, ++tag) {
    Block current(tag, offset, offset+*iter, 1, *siter);
    occ_blocks.push_back(current);
    offset += *iter;
  }
  // virtual blocks
  vector<Block> vir_blocks;
  for (auto iter = num_vir_orb.begin(), siter = sym_vir.begin();
        iter != num_vir_orb.end(); ++iter, ++siter, ++tag) {
    Block current(tag, offset, offset+*iter, 0, *siter);
    vir_blocks.push_back(current);
    offset += *iter;
  }
  for (auto iter = num_vir_orb.begin(), siter = sym_vir.begin();
        iter != num_vir_orb.end(); ++iter, ++siter, ++tag) {
    Block current(tag, offset, offset+*iter, 1, *siter);
    vir_blocks.push_back(current);
    offset += *iter;
  }
  // then define general blocks
  vector<Block> gen_blocks(occ_blocks);
  gen_blocks.insert(gen_blocks.end(), vir_blocks.begin(), vir_blocks.end());

  IndexSpace occ_space(occ_blocks);
  IndexSpace vir_space(vir_blocks);
  IndexSpace gen_space(gen_blocks);

  occ_space_ = occ_space;
  vir_space_ = vir_space;
  gen_space_ = gen_space;
}

Spaces::~Spaces() {

}


