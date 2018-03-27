//Erik Koenitzer
#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include "windows.h"
#include <vector>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>

#include "Man.hpp"
#include "Map.hpp"

using namespace std;

char initGame (Map& m, Map& og, vector<Man>& players, int& toWin)
{
  string userIn;
  string mapName;
  int num;
  char whichMap;

  system("CLS");
  cout << "Pick a map: " << endl << "(B)asic: Plain and uninteresting!" << endl;
  cout << "(M)ini: Up close and personal!" << endl << "(V)oid: No hard blocks here!" << endl;
  cout << "(T)wisty: Tricky to navigate!" << endl << endl << "Your choice: ";
  getline(cin, userIn);
  userIn += " "; //So it won't crash if no input

  switch (userIn.at(0))
  {
  case 'm':
  case 'M':
    mapName = "Mini";
    m.setMap(mapName);
    og = m;
    m.setPowerUpFrequency(41);
    m.fillWithBlocks(100);
    whichMap = 'M';
    break;
  case 'v':
  case 'V':
    mapName = "Void";
    m.setMap(mapName);
    og = m;
    m.fillWithBlocks(80);
    whichMap = 'V';
    break;
  case 't':
  case 'T':
    mapName = "Twisty";
    m.setMap(mapName);
    og = m;
    m.setPowerUpFrequency(16);
    m.fillWithBlocks(85);
    whichMap = 'T';
    break;
  default:
    mapName = "Basic";
    m.setMap(mapName);
    og = m;
    m.fillWithBlocks(85);
    whichMap = 'B';
  }

  cout << endl << "How many rounds to win (1-9)? ";
  getline(cin, userIn);
  stringstream(userIn) >> toWin;
  (toWin < 1) && (toWin= 1);
  (toWin > 9) && (toWin = 9);

  cout << endl << "How many Players (2-5)? ";
  getline(cin, userIn);
  stringstream(userIn) >> num;
  (num <= 1) && (num = 2);
  (num > 5) && (num = 5);

  for (int i = 1; i < num+1; i++)
  {
    cout << endl << "Is Player " << i << " (H)uman or (C)PU? ";
    getline(cin, userIn);
    userIn += " "; //So it won't crash if no input
    players.push_back(Man(i, toupper(userIn.at(0))=='C', m));
  }

  if (num == 3)
  {
    players.at(2).placeInCenter(m);
  }

  cout << endl << "Press Enter to start!";
  getline(cin, userIn);
  system("CLS");
  return whichMap;
}

void update (vector<Man>& players, Map& map)
{
  for (Man& m : players)
  {
    m.tickBombs(map);
    m.manageSick();
  }
}

void outputStatus (Map& m, vector<Man>& players, vector<int>& wins)
{
  for (int i = 0; i < players.size(); i++)
  {
    cout << endl << "Player " << i+1 << " has " << wins.at(i) << " win(s), and is feeling ";
    if (players.at(i).isAlive(m))
    switch (players.at(i).getStatus())
    {
    case 'C':
      cout << "All Clogged Up";
      break;
    case 'D':
      cout << "Rather...Loose";
      break;
    case 'R':
      cout << "Very Flustered";
      break;
    case 'F':
      cout << "Way too Strong";
      break;
    default:
      cout << "Pretty Perfect";
      break;
    }
    else
    {
      cout << "Completely Deceased";
    }
  }
}

bool isRoundWon (vector<Man>& players, Map& map)
{
  int numAlive = 0;
  for (Man& m : players)
  {
    m.isAlive(map) && numAlive++;
  }
  return numAlive < 2;
}

void resetMap (vector<Man>& players, Map& m, char c)
{
  switch (c)
  {
  case 'M':
    m.fillWithBlocks(100);
    break;
  case 'V':
    m.fillWithBlocks(80);
    break;
  case 'T':
    m.fillWithBlocks(85);
    break;
  default:
    m.fillWithBlocks(85);
  }

  for (Man& p : players)
  {
    p.respawn(m);
  }

  if (players.size() == 3)
  {
    players.at(2).placeInCenter(m);
  }
}

int whoWonGame (vector<int>& scores, int toWin)
{
  int winner = 0;
  for (int i = 0; i < scores.size() && winner == 0; i++)
  {
    (scores.at(i) >= toWin) && (winner = i+1);
  }
  return winner;
}

void outputPostgameStats (vector<Man>& m)
{
  int hiBombs = 0;
  int hiSteps = 0;
  int hiTiles = 0;
  int hiBlocks = 0;

  for (int i = 1; i < m.size(); i++)
  {
    (m.at(hiBombs).getBombsPlaced() < m.at(i).getBombsPlaced()) && (hiBombs = i);
    (m.at(hiSteps).getSteps() < m.at(i).getSteps()) && (hiSteps = i);
    (m.at(hiTiles).getTiles() < m.at(i).getTiles()) && (hiTiles = i);
    (m.at(hiBlocks).getBlocks() < m.at(i).getBlocks()) && (hiBlocks = i);
  }

  cout << endl << "********* Awards *********" << endl;
  cout << "Loosey Goosey: Player " << hiBombs+1 << endl;
  cout << "Lil' Jitterbug: Player " << hiSteps+1 << endl;
  cout << "Angsty Arsonist: Player " << hiTiles+1 << endl;
  cout << "Block Bully: Player " << hiBlocks+1 << endl;
}

int main()
{
  srand(time(0));
  string userIn = "Standard";
  srand(time(0));
  int speed = 40;
  cout << fixed << setprecision(1);

  do
  {
    Map m;
    Map ogMap = m;
    vector<Man> players;
    int toWin;
    int bigWinner = 0;
    char usedMap = initGame(m, ogMap, players, toWin);

    vector<int> wins;
    for (Man& p : players)
    {
      wins.push_back(0);
    }

    do
    {
      double timer = 60.0;
      do
      {
        this_thread::sleep_for(chrono::milliseconds(speed));
        cout << m;
        outputStatus(m, players, wins);

        //***Player 1***
        if (GetAsyncKeyState('S') && players.at(0).isAlive(m) && !players.at(0).isAI())
        {
          if (players.at(0).getStatus() != 'R')
          {
            players.at(0).move(0, 1, m, players);
          }
          else
          {
            players.at(0).move(0, -1, m, players);
          }
        }
        if (GetAsyncKeyState('W') && players.at(0).isAlive(m) && !players.at(0).isAI())
        {
          if (players.at(0).getStatus() != 'R')
          {
            players.at(0).move(0, -1, m, players);
          }
          else
          {
            players.at(0).move(0, 1, m, players);
          }
        }
        if (GetAsyncKeyState('A') && players.at(0).isAlive(m) && !players.at(0).isAI())
        {
          if (players.at(0).getStatus() != 'R')
          {
            players.at(0).move(-1, 0, m, players);
          }
          else
          {
            players.at(0).move(1, 0, m, players);
          }
        }
        if (GetAsyncKeyState('D') && players.at(0).isAlive(m) && !players.at(0).isAI())
        {
          if (players.at(0).getStatus() != 'R')
          {
            players.at(0).move(1, 0, m, players);
          }
          else
          {
            players.at(0).move(-1, 0, m, players);
          }
        }
        if ((GetAsyncKeyState('E') || players.at(0).getStatus() == 'D') && (players.at(0).isAlive(m) && !players.at(0).isAI() && players.at(0).getStatus() != 'C'))
        {
          players.at(0).placeBomb(m);
        }

        //***Player 2***
        if (GetAsyncKeyState(VK_DOWN) && players.size()>1 && players.at(1).isAlive(m) && !players.at(1).isAI())
        {
          if (players.at(1).getStatus() != 'R')
          {
            players.at(1).move(0, 1, m, players);
          }
          else
          {
            players.at(1).move(0, -1, m, players);
          }
        }
        if (GetAsyncKeyState(VK_UP) && players.size()>1 && players.at(1).isAlive(m) && !players.at(1).isAI())
        {
          if (players.at(1).getStatus() != 'R')
          {
            players.at(1).move(0, -1, m, players);
          }
          else
          {
            players.at(1).move(0, 1, m, players);
          }
        }
        if (GetAsyncKeyState(VK_LEFT) && players.size()>1 && players.at(1).isAlive(m) && !players.at(1).isAI())
        {
          if (players.at(1).getStatus() != 'R')
          {
            players.at(1).move(-1, 0, m, players);
          }
          else
          {
            players.at(1).move(1, 0, m, players);
          }
        }
        if (GetAsyncKeyState(VK_RIGHT) && players.size()>1 && players.at(1).isAlive(m) && !players.at(1).isAI())
        {
          if (players.at(1).getStatus() != 'R')
          {
            players.at(1).move(1, 0, m, players);
          }
          else
          {
            players.at(1).move(-1, 0, m, players);
          }
        }
        if (players.size()>1 && (GetAsyncKeyState(VK_SPACE) || GetAsyncKeyState(VK_CONTROL) || players.at(1).getStatus() == 'D') && (players.at(1).isAlive(m) && !players.at(1).isAI() && players.at(1).getStatus() != 'C'))
        {
          players.at(1).placeBomb(m);
        }

        //***Player 3***
        if ((GetAsyncKeyState(VK_NUMPAD5) || GetAsyncKeyState(VK_NUMPAD2)) && players.size()>2 && players.at(2).isAlive(m) && !players.at(2).isAI())
        {
          if (players.at(2).getStatus() != 'R')
          {
            players.at(2).move(0, 1, m, players);
          }
          else
          {
            players.at(2).move(0, -1, m, players);
          }
        }
        if (GetAsyncKeyState(VK_NUMPAD8) && players.size()>2 && players.at(2).isAlive(m) && !players.at(2).isAI())
        {
          if (players.at(2).getStatus() != 'R')
          {
            players.at(2).move(0, -1, m, players);
          }
          else
          {
            players.at(2).move(0, 1, m, players);
          }
        }
        if (GetAsyncKeyState(VK_NUMPAD4) && players.size()>2 && players.at(2).isAlive(m) && !players.at(2).isAI())
        {
          if (players.at(2).getStatus() != 'R')
          {
            players.at(2).move(-1, 0, m, players);
          }
          else
          {
            players.at(2).move(1, 0, m, players);
          }
        }
        if (GetAsyncKeyState(VK_NUMPAD6) && players.size()>2 && players.at(2).isAlive(m) && !players.at(2).isAI())
        {
          if (players.at(2).getStatus() != 'R')
          {
            players.at(2).move(1, 0, m, players);
          }
          else
          {
            players.at(2).move(-1, 0, m, players);
          }
        }
        if (players.size()>2 && (GetAsyncKeyState(VK_NUMPAD0) || GetAsyncKeyState(VK_NUMPAD7)  || players.at(2).getStatus() == 'D') && (players.at(2).isAlive(m) && !players.at(2).isAI() && players.at(2).getStatus() != 'C'))
        {
          players.at(2).placeBomb(m);
        }

        update(players, m);
        timer -= speed/1000.0;
        if (timer < 0)
        {
          m.suddenDeathFill();
          cout << endl << "Hurry Up!  Sudden Death!";
        }
        else
        {
          cout << endl << "Time left: " << timer;
        }
      } while (!(isRoundWon(players, m)));

      cout << m;

      int winner = -1;
      for (int i = 0; i < players.size() && winner < 0; i++)
      {
        (players.at(i).isAlive(m)) && (winner = i);
      }

      wins.at(winner)++;
      outputStatus(m, players, wins);

      if (winner >= 0)
      {
        cout << endl << endl << "Player " << winner+1 << " won this round!";
      }
      else
      {
        cout << endl << endl << "It's a tie!";
      }

      cout << endl << endl << "Press Enter to continue...";
      FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
      getline(cin,userIn);

      m = ogMap;
      resetMap(players, m, usedMap);
      system("CLS");
      bigWinner = whoWonGame(wins, toWin);
    } while (bigWinner < 1);

    cout << "This game's winner is..." << endl << endl;
    cout << "***********";
    cout << endl << "Player " << bigWinner << "!!!" << endl;
    cout << "***********" << endl;
    outputPostgameStats(players);

    cout << endl << "Play again? (Y/N):  ";
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    getline(cin,userIn);
    userIn += " "; //So it won't crash if no input
  } while (toupper(userIn.at(0)) != 'N');

	return 0;
}
