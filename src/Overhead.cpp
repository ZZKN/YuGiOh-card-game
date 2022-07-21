
#include "Overhead.hpp"

void clrscr() { system("cls"); }						//used to clear the screen
void flushCin() { std::cin.seekg(std::ios_base::end); }	//used to clear Cin

void EnterKey() {
	flushCin();
	std::cout << "Press The ENTER Key To Continue.";
	getchar();
	clrscr();
}


std::string ModeToString(CardMode mode) {
	std::string modeStr[2] = { "ATK", "DEF" };
	return modeStr[static_cast<int>(mode)];
}

std::string TypeToString(MonsterType type) {
	std::string typeStr[3] = { "normal", "effect", "spell" };
	return typeStr[static_cast<int>(type)];
}


std::string ElemToString(Elements elem) {
	std::string typeStr[4] = { "none", "earth", "fire", "water" };
	return typeStr[static_cast<int>(elem)];
}
