//Erik Koenitzer
#include "map.hpp"

#include "windows.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>


BOOL gotoxy(const WORD x, const WORD y)
{
	COORD xy;
	xy.X = x;
	xy.Y = y;
	return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
}

void Map::setMap(std::string& s)
{
  s += ".txt";
  std::ifstream fin(s);
  if (fin)
  {
    while (getline(fin, s))
		{
			map.push_back(s);
		}
    fin.close();
  }
}

void Map::fillWithBlocks(int density)
{
  for (int i = 1; i < map.size()-1; i++)
  {
    for (int j = 1; j < map[i].size(); j++)
    {
      if (map[i][j] == '@')
      {
        map[i][j] = ' ';
      }
      else if (map[i][j] == ' ')
      {
        int randum = rand() % 100;
        (randum < density) && (map[i][j] = BLOCK);
      }
    }
  }
}

void Map::suddenDeathFill()
{
  bool placedOne = false;
  for (int y = 1; y < map.size() && !placedOne; y++)
  {
    for (int x = 1; x < map.at(y).size() && !placedOne; x++)
    {
        (map[y][x] != HARDBLOCK) && (map[y][x] = HARDBLOCK) && (placedOne = true);
    }
  }
}

bool Map::placePower (int y, int x) //Always false
{
  int randum = rand() % 100;
  randum < powerUpDistr ? map[y][x] = 'B' :
  randum < powerUpDistr*2 ? map[y][x] = 'F' :
  randum == powerUpDistr*2 ? map[y][x] = 'J' :
  randum < powerUpDistr*2+3 ? map[y][x] = 'X':
  map[y][x] = ' ';
  return false;
}
  
std::ostream& operator << (std::ostream& out, const Map& m)
{
  gotoxy(0, 0);
  for (int i = 0; i < m.map.size(); i++)
  {
    out << m.map.at(i) << std::endl;
  }
  return out;
}