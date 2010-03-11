//
// Author:: Toru Shiozaki
// Date  :: Jan 2010
//
#ifndef _src_tile_h
#define _src_tile_h

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

#endif

