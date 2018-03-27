//Erik Koenitzer
#include "Man.hpp"

#include "Map.hpp"
#include "Bomb.hpp"
#include <cstdlib>

char conds [4] = {'C', 'D', 'F', 'R'};
const char* Man::conditions = conds;

Man::Man(int i, bool ai, Map& m)
{
  mapChar = i + '0';
  status = ' ';
  alive = true;
  canJump = false;
  isAnAI = ai;
  fireUp = 2;
  nextBomb = 0;
  sickTimer = 0;
  bombsPlaced = 0;
  stepsTaken = 0;
  timeAlive = 0.0;
  tilesScorched = 0;
  blocksDestroyed = 0;
  bombs.push_back(Bomb());

  switch (i)
  {
  case 1:
    x = 1;
    y = 1;
    break;
  case 2:
    x = m.map.back().size() - 2;
    y = m.map.size() - 2;
    break;
  case 3:
    x = m.map[1].size() - 2;
    y = 1;
    break;
  case 4:
    x = 1;
    y = m.map.size() - 2;
    break;
  case 5:
    x = m.map.at(m.map.size()/2).size()/2;
    y = m.map.size()/2;
  }
  m.map[y][x] = mapChar;
}

void Man::move(int xDir, int yDir, Map& m, std::vector<Man>& players)
{
  if (alive)
  {
    int randum = rand() % 4; //Belongs to case 'X', compiler gets pissy when it's in there
    switch (m.map[y+yDir][x+xDir])
    {
    case ' ':
      (m.map[y][x] == mapChar) && (m.map[y][x] = ' ');
      x += xDir;
      y += yDir;
      m.map[y][x] = mapChar;
      stepsTaken++;
      break;
    case Bomb::EXPLOSION:
      (m.map[y][x] == mapChar) && (m.map[y][x] = ' ');
      alive = false;
      stepsTaken++;
      break;
    case 'B':
      (m.map[y][x] == mapChar) && (m.map[y][x] = ' ');
      x += xDir;
      y += yDir;
      m.map[y][x] = mapChar;
      bombs.push_back(Bomb());
      (nextBomb == bombs.size()-2) && (nextBomb++);
      stepsTaken++;
      break;
    case 'F':
      (m.map[y][x] == mapChar) && (m.map[y][x] = ' ');
      x += xDir;
      y += yDir;
      m.map[y][x] = mapChar;
      fireUp++;
      stepsTaken++;
      break;
    case 'J':
      (m.map[y][x] == mapChar) && (m.map[y][x] = ' ');
      x += xDir;
      y += yDir;
      m.map[y][x] = mapChar;
      canJump = true;
      stepsTaken++;
      break;
    case 'X':
      (m.map[y][x] == mapChar) && (m.map[y][x] = ' ');
      x += xDir;
      y += yDir;
      m.map[y][x] = mapChar;
      status = conditions[randum];
      sickTimer = 200;
      stepsTaken++;
      break;
    case 'o':
    case 'O':
      if (canJump)
      {
        move(xDir+xDir, yDir+yDir, m, players);
        stepsTaken++;
      }
      break;
    case '1':
      if (players.at(0).status == ' ')
      {
        players.at(0).status = status;
        players.at(0).sickTimer = sickTimer;
      }
      else if (status == ' ')
      {
        status = players.at(0).status;
        sickTimer = players.at(0).sickTimer;
      }
      break;
    case '2':
      if (players.size()>1)
      {
        if (players.at(1).status == ' ')
        {
          players.at(1).status = status;
          players.at(1).sickTimer = sickTimer;
        }
        else if (status == ' ')
        {
          status = players.at(1).status;
          sickTimer = players.at(1).sickTimer;
        }
      }
      break;
    case '3':
      if (players.size()>2)
      {
        if (players.at(2).status == ' ')
        {
          players.at(2).status = status;
          players.at(2).sickTimer = sickTimer;
        }
        else if (status == ' ')
        {
          status = players.at(2).status;
          sickTimer = players.at(2).sickTimer;
        }
      }
      break;
    case '4':
      if (players.size()>3)
      {
        if (players.at(3).status == ' ')
        {
          players.at(3).status = status;
          players.at(3).sickTimer = sickTimer;
        }
        else if (status == ' ')
        {
          status = players.at(3).status;
          sickTimer = players.at(3).sickTimer;
        }
      }
      break;
    case '5':
      if (players.size()>4)
      {
        if (players.at(4).status == ' ')
        {
          players.at(4).status = status;
          players.at(4).sickTimer = sickTimer;
        }
        else if (status == ' ')
        {
          status = players.at(4).status;
          sickTimer = players.at(4).sickTimer;
        }
      }
      break;
    }
  }
}

void Man::placeBomb (Map& m)
{
  if (bombs.at(nextBomb).fuse <= Bomb::CLEARED)
  {
    bombs.at(nextBomb).place(x, y, status != 'F' ? fireUp : 12, m) && nextBomb++;
    (nextBomb >= bombs.size()) && (nextBomb = 0);
    bombsPlaced++;
  }
}

void Man::tickBombs (Map& m)
{
  for (Bomb& b : bombs)
  {
    if (b.fuse > Bomb::CLEARED)
    {
      b.tick(m);
    }
  }
}

void Man::manageSick()
{
  (sickTimer > 0) && (sickTimer--);
  (sickTimer <= 0) && (status = ' ');
}

void Man::respawn(Map& m)
{
  status = ' ';
  alive = true;
  canJump = false;
  fireUp = 2;
  nextBomb = 0;
  sickTimer = 0;
  timeAlive = 0.0;
  for (const Bomb& b : bombs) {tilesScorched += b.tilesScorched;}
  for (const Bomb& b : bombs) {blocksDestroyed += b.blocksDestroyed;}
  
  bombs.clear();
  bombs.push_back(Bomb());

  switch (mapChar)
  {
  case '1':
    x = 1;
    y = 1;
    break;
  case '2':
    x = m.map.back().size() - 2;
    y = m.map.size() - 2;
    break;
  case '3':
    x = m.map[1].size() - 2;
    y = 1;
    break;
  case '4':
    x = 1;
    y = m.map.size() - 2;
    break;
  case '5':
    x = m.map.at(m.map.size()/2).size()/2;
    y = m.map.size()/2;
  }
  m.map[y][x] = mapChar;
}

bool Man::deathScatter (Map& m)
{
  bool placed = true;
  int randY;
  int randX;

  for (int i = 0; i < bombs.size()-1; i+=placed)
  {
    randY = 1 + rand() % (m.map.size() - 1);
    randX = 1 + rand() % (m.map[randY].size() - 1);
    placed = ((m.map[randY][randX] == ' ') && (m.map[randY][randX] = 'B'));
  }

  for (int i = 0; i < fireUp-2; i+=placed)
  {
    randY = 1 + rand() % (m.map.size() - 1);
    randX = 1 + rand() % (m.map[randY].size() - 1);
    placed = ((m.map[randY][randX] == ' ') && (m.map[randY][randX] = 'F'));
  }

  placed = false;

  while (canJump && !placed)
  {
    randY = 1 + rand() % (m.map.size() - 1);
    randX = 1 + rand() % (m.map[randY].size() - 1);
    placed = ((m.map[randY][randX] == ' ') && (m.map[randY][randX] = 'J'));
  }

  placed = false;

  while (status != ' ' && !placed)
  {
    randY = 1 + rand() % (m.map.size() - 1);
    randX = 1 + rand() % (m.map[randY].size() - 1);
    placed = ((m.map[randY][randX] == ' ') && (m.map[randY][randX] = 'X'));
  }

  return false;
}
