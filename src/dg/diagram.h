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
    // operators
    std::list<Operator> operators_;
    // tensors
    std::list<Tensor> tensors_;

    void update_contracted_indices(const std::list<std::pair<int, int> >, const std::vector<std::string>);
    void assign_dagger();

  public:
    Diagram(const std::list<Operator>&, const std::list<Tensor>&);
    Diagram(const Diagram& o) : operators_(o.operators()), tensors_(o.tensors()) {};
    Diagram(std::string);
    ~Diagram();

    // returns private members
    const std::list<Operator> operators() const { return operators_; };
    const std::list<Tensor> tensors() const { return tensors_; };

    // printing functions that return strings
    const std::string show() const;
    const std::string show_only_tensors() const;

    // count the number of creation operators 
    const int count_creation_indices() const;
    // make a list of contracted diagrams
    const std::list<Diagram> contract_one() const;

    // returns if this diagram is connected under the give rule.
    const bool connected(const std::pair<std::string, std::list<std::string> > rule) const; 

};

}

#endif

