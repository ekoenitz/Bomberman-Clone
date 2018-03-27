//Erik Koenitzer
#ifndef MAN_HPP
#define MAN_HPP

#include "Map.hpp"
#include "Bomb.hpp"
#include <vector>

class Man
{
public:
  Man(int i, bool ai, Map& m);
  void move(int xDir, int yDir, Map& m, std::vector<Man>& players);
  void placeBomb(Map& m);
  void tickBombs(Map& m);
  void manageSick();
  void respawn(Map& m);
  void placeInCenter (Map& m) {m.map[y][x] = ' '; x = m.map.at(m.map.size()/2).size()/2; y = m.map.size()/2; m.map[y][x] = mapChar;} //For games with only 3 players

  bool deathScatter (Map& m); //Always returns false, bool purely for short-circuit And-ing
  bool isAlive(Map& m) {return alive = (alive && ((m.map[y][x] != Bomb::EXPLOSION && m.map[y][x] != Map::HARDBLOCK) || deathScatter(m)));}
  bool isAI() {return isAnAI;}

  char getStatus() {return status;}
  int getBombsPlaced() {return bombsPlaced;}
  int getSteps() {return stepsTaken;}
  int getTiles() {return tilesScorched;}
  int getBlocks() {return blocksDestroyed;}

private:
  static const char* conditions;
  bool alive;
  bool canJump;
  bool isAnAI;
  int x;
  int y;
  int fireUp;
  int nextBomb;
  int sickTimer;
  char mapChar;
  char status; //Good = ' '

  int bombsPlaced;
  int stepsTaken;
  int tilesScorched;
  int blocksDestroyed;
  double timeAlive;
  std::vector<Bomb> bombs;
};

#endif