//
// Author:: Toru Shiozaki
// Date  :: Apr 2009
//
#ifndef _ig_inputgenerator_h
#define _ig_inputgenerator_h

#include <string>
#include <list>

namespace IG {

class InputGenerator {
  protected:
    std::string filename_;

  public:
    InputGenerator(std::string);
    ~InputGenerator();

    // returns private members
    const std::string filename() const {return filename_;};

    // each entry in the returned list is a "DG::Diagram" object without any contraction 
    const std::list<std::string> generate();

};

}

#endif
