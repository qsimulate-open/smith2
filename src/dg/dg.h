//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: dg.h
// Copyright (C) 2009 Toru Shiozaki
//
// Author: Toru Shiozaki <shiozaki@northwestern.edu>
// Maintainer: Shiozaki group
//
// This file is part of the SMITH2 package (to be renamed).
//
// The SMITH2 package is free software; you can redistribute it and\/or modify
// it under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// The SMITH2 package is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with the SMITH2 package; see COPYING.  If not, write to
// the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
//

#ifndef _dg_dg_h
#define _dg_dg_h

#include <list>
#include <src/dg/diagram.h>

namespace smith2 {
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
}

#endif
