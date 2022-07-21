#include "Card.hpp"

Card::Card()  {}
Card::Card(const std::string n, int a, int d, int elem) : atk(a), def(d), name(n), element(static_cast<Elements>(elem)) {  }
Card::~Card() {}

bool		Card::Activate			(const unsigned int*)						{ return false; }		// virtual method for derived classes
const unsigned int*	Card::GetCost			()const								{ return nullptr; }		// vitural method for derived classes
int			Card::GetATK			()const								{ return atk; }			// return the attack value of the card
int			Card::GetDEF			()const								{ return def; }			// return the defence value of the card
Elements	Card::GetElem			()const								{ return element; }		// each card represents one of the elements
MonsterType Card::GetType			()const								{ return type; }		// return type of monster card
std::string Card::GetName			()const								{ return name; }		// return name of the card (not unique)
CardMode	Card::GetMode			()const								{ return mode; }		// return mode of the class (attack or defence)
void		Card::SetType			(MonsterType mon)					{ type = mon; }			// set the type of monster card


void Card::CardSave(std::ostream& os) {
	os << static_cast<int>(type) << "/" << name << "/" << atk << "/" << def << "/" << static_cast<int>(element) << "/" << static_cast<int>(mode) << "\n";

}

// changes between defence mode and attack mode
void Card::ChangeMode() {								
	if (mode == CardMode::Defence) {
		mode = CardMode::Attack;
	}
	else {
		mode = CardMode::Defence;
	}
}

//print out card attributes for display in terminal
std::ostream& Card::print(std::ostream& os) {
	os.ios_base::setf(std::ios_base::left);								// all strings appear on the left by default
	os << std::setw(38) << name;
	os << std::setw(4) << atk;
	os << " " << std::setw(4) << def;
	os << " " << std::setw(6) << TypeToString(GetType());
	os << " " << std::setw(5) << ModeToString(GetMode());
	os << " " << std::setw(7) << ElemToString(GetElem());
	return os;
}

std::ostream& operator<<(std::ostream& os, Card& card) {
	
	return card.print(os);
}

// derived classes can add attack and defence values (can also be negative depends on the given class)
void Card::Boost(int atkbonus, int defbonus) {
	atk += atkbonus;
	def -= defbonus;
}

EffectCard::EffectCard(const std::string& name, int atk, int def, int earth, int fire, int water) : Card(name, atk, def,0) {
	SetType(MonsterType::Rare);
	cost[0] = earth;
	cost[1] = fire;
	cost[2] = water;
}

// virutal method for derived classes
bool EffectCard::Activate(const unsigned int*) {

	return false;
}

// print to file
void EffectCard::CardSave(std::ostream& os) {
	os << static_cast<int>(GetType()) << "/" << GetName() << "/" << GetATK() << "/" << GetDEF() << "/";

	for (unsigned int i = 0; i < ELEMENTS_COUNT; i++) {
		os << cost[i] << "/";

	}
		
	os << static_cast<int>(GetMode()) << "\n";

}

// print to file for rare cards (derived classes from EffectCard)
std::ostream& EffectCard::printEffect(std::ostream& os , int inc, int dec, Effect effect) {
	os << static_cast<int>(GetType()) << "/" << static_cast<int>(effect) << "/" << GetATK() << "/" << GetDEF() << "/";

	for (unsigned int i = 0; i < ELEMENTS_COUNT; i++) {
		os << cost[i] << "/";

	}

	os << inc << "/" << dec << "/" << static_cast<int>(GetMode()) <<"\n";
	return os;
}


// return array of int that represent how many of each element the player needs to activate the card
const unsigned int* EffectCard::GetCost() const {
	return cost;
}

// print Effect cards for terminal
std::ostream& EffectCard::print(std::ostream& os) {
	os.ios_base::setf(std::ios_base::left);								// all strings appear on the left by default
	os << std::setw(38) << GetName();
	os << std::setw(4) << GetATK();
	os << " " << std::setw(4) << GetDEF();
	os << " " << std::setw(6) << TypeToString(GetType());
	os << " " << std::setw(5) << ModeToString(GetMode());
	os << " " << std::setw(7) << ElemToString(GetElem());
	os << " " << cost[0] << "/" << cost[1] << "/" << cost[2] << std::setw(7) << " ";
	printDesc(os);
	return os;
}

// virtual method for derived classes
std::ostream& EffectCard::printDesc(std::ostream& os) {
	
	return os;
}

Orc::Orc(int atk, int def, int earth, int fire, int water, int inc, int dec) : EffectCard("Orc", atk, def, earth, fire, water) {
	SetType(MonsterType::Effect);
	effect = Effect::Orc;
	valueInc = inc;
	valueDec = dec;
}

// boost ATK value and DEF based on the number of elements of the player which is given as parameter
bool Orc::Activate(const unsigned int* a) {
	int signedval = (int) a[static_cast<int>(Elements::Earth)];
	Boost(a[static_cast<int>(Elements::Earth)] * valueInc, -signedval * valueDec);
	return true;
}

// print description of boost for HumanPlayer in terminal
std::ostream& Orc::printDesc(std::ostream& os) {
	os << "For every earth element ";
	if (valueInc > 0) {
		os << "add " << valueInc << " to ATK value";
	}
	if (valueDec > 0) {

		os << " but decrease DEF value by " << valueDec;
	}
	return os;
}

// print to file
void Orc::CardSave(std::ostream& os) {
	printEffect(os, valueInc, valueDec, effect);
}

Aqua::Aqua(int atk, int def, int earth, int fire, int water, int inc, int dec) : EffectCard("Aqua", atk, def, earth, fire, water) {
	SetType(MonsterType::Effect);
	effect = Effect::Aqua;
	valueInc = inc;
	valueDec = dec;
}

// boost ATK value and DEF based on the number of elements of the player which is given as parameter
bool Aqua::Activate(const unsigned int* a) {
	int signedval = (int)a[static_cast<int>(Elements::Fire)];
	Boost(a[static_cast<int>(Elements::Water)] * valueInc, -signedval * valueDec);
	return true;
}

// print description of boost for HumanPlayer in terminal
std::ostream& Aqua::printDesc(std::ostream& os) {
	os << "For every water element ";
	if (valueInc > 0) {
		os << "add " << valueInc << " to ATK value";
	}
	if (valueDec > 0) {

		os << " but for every fire element decrease DEF value by " << valueDec;
	}
	return os;
}

// print to file
void Aqua::CardSave(std::ostream& os) {
	printEffect(os, valueInc, valueDec, effect);
}

Abaki::Abaki(int atk, int def, int earth, int fire, int water, int inc, int dec) : EffectCard("Abaki", atk, def, earth, fire, water) {
	SetType(MonsterType::Effect);
	effect = Effect::Abaki;
	valueInc = inc;
	valueDec = dec;
}

// boost ATK value and DEF based on the number of elements of the player which is given as parameter
bool Abaki::Activate(const unsigned int* a) {
	int signedval = (int)a[static_cast<int>(Elements::Water)];
	Boost(-signedval * valueDec, a[static_cast<int>(Elements::Fire)] * valueInc);
	return true;
}

// print description of boost for HumanPlayer in terminal
std::ostream& Abaki::printDesc(std::ostream& os) {
	os << "For every fire element ";
	if (valueInc > 0) {
		os << "add " << valueInc << " to DEF value";
	}
	if (valueDec > 0) {

		os << " but for every water element decrease ATK value by " << valueDec;
	}
	return os;
}

// print to file
void Abaki::CardSave(std::ostream& os) {
	printEffect(os, valueInc, valueDec, effect);
}

Thunder::Thunder(int atk, int def, int earth, int fire, int water, int inc, int dec) : EffectCard("Thunder", atk, def, earth, fire, water) {
	SetType(MonsterType::Effect);
	effect = Effect::Thunder;
	valueInc = inc;
	valueDec = dec;
}

// boost ATK value and DEF based on the number of elements of the player which is given as parameter
bool Thunder::Activate(const unsigned int* a) {
	int signedval = (int)a[static_cast<int>(Elements::Earth)];
	Boost(a[static_cast<int>(Elements::Earth)] * valueInc, -signedval * valueDec);
	return true;
}

// print description of boost for HumanPlayer in terminal
std::ostream& Thunder::printDesc(std::ostream& os) {
	os << "For every earth element ";
	if (valueInc > 0) {
		os << "add " << valueInc << " to ATK value";
	}  
	if (valueDec > 0) {

		os << " but decrease DEF value by " << valueDec;
	}
	return os;
}

// print to file
void Thunder::CardSave(std::ostream& os) {
	printEffect(os, valueInc, valueDec, effect);
}
