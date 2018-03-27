//Erik Koenitzer
#include "Bomb.hpp"

#include "Map.hpp"
#include <vector>

bool Bomb::place(int xLoc, int yLoc, int fire, Map& m)
{
  if (m.map[yLoc][xLoc] != 'O' && m.map[yLoc][xLoc] != 'o')
  {
    x = xLoc;
    y = yLoc; 
    fireUp = fire; 
    fuse = MAXFUSE; 
    m.map[y][x] = 'O';
    return true;
  }
  else
  {
    return false;
  }
}

void Bomb::tick(Map& m)
{
  fuse--;
  if (fuse >= 0)
  {
    if (fuse == 0 || m.map[y][x] == EXPLOSION)
    {
      fuse = -1;
      explode(m);
    }
    else
    {
      fuse % 6 > 2 ? m.map[y][x] = 'O' : m.map[y][x] = 'o';
    }
  }
  else if (fuse == CLEARED)
  {
    clearExplosion(m);
  }
}

void Bomb::explode(Map& m)
{
  m.map[y][x] = EXPLOSION;
  std::vector<bool> dirs; // = {true, true, true, true};
  dirs.push_back(true);
  dirs.push_back(true);
  dirs.push_back(true);
  dirs.push_back(true);
  //0 = left, 1 = right, 2 = up, 3 = down

  for (int i = 1; i < fireUp+1; i++)
  {
    //left
    (dirs.at(0) && (m.map[y][x-i] == Map::HARDBLOCK)) && (dirs.at(0) = false);
    (dirs.at(0) && m.map[y][x-i] == Map::BLOCK && blocksDestroyed++) && (dirs.at(0) = m.placePower(y, x-i));
    (dirs.at(0)) && ((m.map[y][x-i] = EXPLOSION) && tilesScorched++);
    //right
    (dirs.at(1) && (m.map[y][x+i] == Map::HARDBLOCK)) && (dirs.at(1) = false);
    (dirs.at(1) && m.map[y][x+i] == Map::BLOCK && blocksDestroyed++) && (dirs.at(1) = m.placePower(y, x+i));
    (dirs.at(1)) && ((m.map[y][x+i] = EXPLOSION) && tilesScorched++);
    //up
    (dirs.at(2) && (m.map[y-i][x] == Map::HARDBLOCK)) && (dirs.at(2) = false);
    (dirs.at(2) && m.map[y-i][x] == Map::BLOCK && blocksDestroyed++) && (dirs.at(2) = m.placePower(y-i, x));
    (dirs.at(2)) && ((m.map[y-i][x] = EXPLOSION) && tilesScorched++);
    //down
    (dirs.at(3) && (m.map[y+i][x] == Map::HARDBLOCK)) && (dirs.at(3) = false);
    (dirs.at(3) && m.map[y+i][x] == Map::BLOCK && blocksDestroyed++) && (dirs.at(3) = m.placePower(y+i, x));
    (dirs.at(3)) && ((m.map[y+i][x] = EXPLOSION) && tilesScorched++);
  }
}

void Bomb::clearExplosion(Map& m)
{
  m.map[y][x] = ' ';
  std::vector<bool> dirs; // = {true, true, true, true};
  dirs.push_back(true);
  dirs.push_back(true);
  dirs.push_back(true);
  dirs.push_back(true);
  //0 = left, 1 = right, 2 = up, 3 = down

  for (int i = 1; i < fireUp+1; i++)
  {
    //left
    (dirs.at(0) && (m.map[y][x-i] != EXPLOSION && m.map[y][x-i] != ' ')) && (dirs.at(0) = false);
    (dirs.at(0)) && (m.map[y][x-i] = ' ');
    //right
    (dirs.at(1) && (m.map[y][x+i] != EXPLOSION && m.map[y][x+i] != ' ')) && (dirs.at(1) = false);
    (dirs.at(1)) && (m.map[y][x+i] = ' ');
    //up
    (dirs.at(2) && (m.map[y-i][x] != EXPLOSION && m.map[y-i][x] != ' ')) && (dirs.at(2) = false);
    (dirs.at(2)) && (m.map[y-i][x] = ' ');
    //down
    (dirs.at(3) && (m.map[y+i][x] != EXPLOSION && m.map[y+i][x] != ' ')) && (dirs.at(3) = false);
    (dirs.at(3)) && (m.map[y+i][x] = ' ');
  }
}