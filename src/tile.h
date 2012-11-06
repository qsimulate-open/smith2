//
// SMITH2 - Automated Code Generation Program for General Order CC Programs.
// Filename: tile.h
// Copyright (C) 2010 Toru Shiozaki
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

#ifndef _src_tile_h
#define _src_tile_h

namespace smith2 {

class Tile {
  protected:
    // tag of this tile
    long tag_;
    // start and fence (molecular orbitals) 
    long start_;
    long fence_;

    // spin of this tile; 0 alpha, 1 beta
    int spin_;
    // symmetry of this tile;
    int symm_;

  public:
    // Tile set up will be done in the interface to integrals
    Tile(const long tag__, const long start__, const long fence__, const int spin__, const int sym__)
      : tag_(tag__), start_(start__), fence_(fence__), spin_(spin__), symm_(sym__) {};
    // Default constructor for debugging
    Tile() {};
    ~Tile() {};

    // Some funcitons to retrieve private members.
    long tag() const { return tag_; };
    long start() const { return start_; };
    long fence() const { return fence_; };
    int spin() const { return spin_; };
    int symm() const { return symm_; };

    // for sorting tiles
    bool operator>(const Tile& other) { return tag_ > other.tag_; };
    bool operator>=(const Tile& other) { return tag_ >= other.tag_; };
    bool operator<(const Tile& other) { return tag_ < other.tag_; };
    bool operator<=(const Tile& other) { return tag_ <= other.tag_; };
    bool operator==(const Tile& other) { return tag_ == other.tag_; };
};

}

#endif

