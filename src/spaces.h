//
// Author : Toru Shiozaki
// Date   : Jan 2010
//

#ifndef __src_spaces_h
#define __src_spaces_h

#include <vector>
#include <src/indexspace.h>

class Spaces {
  protected:
    IndexSpace occ_space_;
    IndexSpace vir_space_;
    IndexSpace gen_space_;

  public:
    Spaces(const long, const long, const std::vector<long>&, const std::vector<long>&,
                                   const std::vector<long>&, const std::vector<long>&);
    ~Spaces();

    const IndexSpace occ_space() const { return occ_space_; };
    const IndexSpace vir_space() const { return vir_space_; };
    const IndexSpace gen_space() const { return gen_space_; };

};

#endif

