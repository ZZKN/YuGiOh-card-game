#ifndef OVERHEAD_H
#define OVERHEAD_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string.h>
#include <stdlib.h>     
#include <time.h>      
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <random> 
#include <chrono>
#include <deque>
#include <map>
#include <unordered_map>
#include <list>
#include <limits> 
#include <iterator>

static const unsigned int MAX_DECK_SIZE = 40;
static const unsigned int MIN_DECK_SIZE = 20;
static const unsigned int MAX_EXTRADECK_SIZE = 15;
static const unsigned int MIN_EXTRADECK_SIZE = 5;
static const unsigned int MONSTER_ZONE_SIZE = 5;
static const unsigned int FIRST_DRAW = 5;
static const unsigned int HAND_SIZE = 7;
static const unsigned int LIFE_POINTS = 2000;
static const unsigned int ELEMENTS_COUNT = 3;
static const unsigned int ELEMENTS_LIMIT = 1;
static const unsigned int PAY_ELEMENT = 0;
static const unsigned int PAY_ELEMENT_LIMIT = 1;


enum class Options { OnePlayer = 0, TwoPlayers = 1, LoadGame = 2, Exit = 3 };

enum class MonsterType { Normal = 0, Effect = 1, Rare = 2 };

enum class CardMode { Attack = 0, Defence = 1 };

enum class Effect { Orc = 0, Aqua = 1, Abaki = 2, Thunder = 3};

enum class Elements { Earth = 0, Fire = 1, Water = 2 };

void EnterKey();
void clrscr();
void flushCin();

std::string ModeToString(CardMode);
std::string ElemToString(Elements);
std::string TypeToString(MonsterType);
#endif