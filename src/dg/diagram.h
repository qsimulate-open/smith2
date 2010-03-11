//
// Author:: Toru Shiozaki
// Date  :: Mar 2009
//
#ifndef _dg_diagram_h
#define _dg_diagram_h

#include <string>
#include <list>
#include <map>
#include <src/dg/operator.h>
#include <src/dg/tensor.h>

namespace DG {

class Diagram {
  protected:
    std::list<Operator> operators_;
    std::list<Tensor> tensors_;
    std::map<int, int> imap_;

    void update_contracted_indices(const std::list<std::pair<int, int> >, const std::vector<std::string>);
    void assign_dagger();

  public:
    Diagram(const std::list<Operator>&, const std::list<Tensor>&);
    Diagram(const Diagram& o) : operators_(o.operators()), tensors_(o.tensors()), imap_(o.imap()) { };
    Diagram(std::string);
    ~Diagram();

    const std::list<Operator> operators() const { return operators_; };
    const std::list<Tensor> tensors() const { return tensors_; };
    const std::map<int, int> imap() const { return imap_; };

    const std::string show() const;
    const std::string show_only_tensors() const;

    const int count_creation_indices() const;
    const std::list<Diagram> contract_one();

    const bool connected(const std::pair<std::string, std::list<std::string> >) const; 

};

}

#endif

