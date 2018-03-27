//Erik Koenitzer
#ifndef BOMB_HPP
#define BOMB_HPP

#include "Map.hpp"

class Bomb
{
public:
  Bomb() {fuse = CLEARED; tilesScorched = 0; blocksDestroyed = 0;}
  bool place(int xLoc, int yLoc, int fire, Map& m);
  void tick(Map& m);
  void explode (Map& m);

  friend class Man;

private:
  void clearExplosion(Map& m);
  static const int MAXFUSE = 23;
  static const int CLEARED = -6;
  static const char EXPLOSION = '*';
  int fuse;
  int fireUp;
  int x;
  int y;
  int tilesScorched; //THESE GET TALLIED CORRECTLY, ERROR IS ELSEWHERE
  int blocksDestroyed;
};

#endif