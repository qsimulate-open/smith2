//
// Author:: Toru Shiozaki
// Date  :: Mar 2009
//
#ifndef _dg_dg_h
#define _dg_dg_h

#include <list>
#include <src/dg/diagram.h>

namespace DG {

class DiagramGenerator {
  protected:
    const std::string filename_;
    std::list<std::string> stringinput_; 

    std::list<Diagram> read_input(std::string);
    std::list<Diagram> read_input(std::list<std::string>);

  public:
    DiagramGenerator(std::list<std::string> l) : stringinput_(l) {};
    DiagramGenerator(const std::string filename) : filename_(filename) {};
    ~DiagramGenerator() {};

    const std::string filename() const { return filename_; };

    std::list<std::string> generate();

};

}

#endif
