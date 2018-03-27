//Erik Koenitzer
#ifndef map_hpp
#define map_hpp

#include <iostream>
#include <string>
#include <vector>

class Map
{
public:
  Map() {powerUpDistr = 12;}
  void setMap(std::string& s);
  void setPowerUpFrequency(int x) {powerUpDistr = x;}
  void fillWithBlocks(int density);
  void suddenDeathFill();
  bool placePower (int y, int x); //Always returns false, bool purely for short-circuit And-ing

  friend std::ostream& operator << (std::ostream& out, const Map& m);
  friend class Man;
  friend class Bomb;

private:
  static const char HARDBLOCK = '#';
  static const char BLOCK = 254;
  int powerUpDistr; //Chance = this*2 + 3
  std::vector<std::string> map;
};

#endif