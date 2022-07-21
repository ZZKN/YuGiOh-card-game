#include "Player.hpp"

Player::Player() {
    lifepoints = LIFE_POINTS;
}
Player::~Player() {}

void		Player::DisplayZone		(bool) const				{ }						// virtual method to display content of monster zone
void		Player::InitDraw		(void)						{ }						// virtual method to initiate first draw of the game
bool		Player::DrawAndSet		(void)						{ return false; }		// virtual method to draw a card and set a card
void		Player::FlipCard		(bool)						{ }						// virtual method to change mode of a card
bool		Player::Attack			(int&, int&, const List*)	{ return false; }		// virtual method to choose cards into an attack
bool		Player::Alive			(void)const					{ return false; }		// virtual method to see if the player is breathing
bool		Player::Load			(const char*)				{ return false; }		// virtual method to load cards
size_t		Player::SizeOfDeck		(void) const				{ return 0; }			// virtual method to get number of cards left in deck
size_t		Player::SizeOfExtraDeck	(void) const				{ return 0; }			// virtual method to get number of cards left in extradeck			
size_t		Player::SizeOfZone		(void) const				{ return 0; }			// virtual method to get number of cards in the zone
const List* Player::GetZone			(void) const				{ return nullptr; }		// virtual method to get zone
void		Player::DiscardCard		(void)						{ }						// virtual method to throw away a card
void		Player::HandCheck		(void)						{ }						// virtual method to check number of cards on hand
void		Player::DisplayHand		(void) const				{ }						// virtual method to show content of hand
bool		Player::ReloadPlayer	(std::fstream&)				{ return false; }		// virtual method to reload player attributes from file
bool		Player::RemoveCard		(unsigned int)				{ return false; }		// virtual method to remove card at index
void		Player::GetZoneCard		(Card&, unsigned int)		{ }						// virtual method to copy card at index
int			Player::GetLifepoints	(void) const				{ return lifepoints; }  // return lifepoints
std::string	Player::GetName			(void) const				{ return name; }		// return name
void		Player::SavePrint		(std::ofstream& os)			{ }						// virtual method to save player into file

std::ostream&	operator<<			(std::ofstream& os, Player& player)	{ return os; }

// load cards into deck and extra deck from file
bool Player::LoadDeck( Deck & deck, Deck& extradeck, const char* filepath) {

	std::ifstream cfile(filepath);

	std::string delimiter = "/";
	std::string line;
	unsigned int length = 0;
	unsigned int index = 1;

	if (!cfile.is_open()) {
		return false;
	}
	std::getline(cfile, line);
	line.erase(0, line.find(delimiter) + delimiter.length());
	length = stoi(line);


	if (length < MIN_DECK_SIZE) {
		return false;
	}

	while (std::getline(cfile, line) && index <= length && index <= MAX_DECK_SIZE) {

		std::stringstream ss(line);
		if (!line.empty() && !deck.IdentifyCard(ss)) {
			return false;
		}
		index++;
	}

	unsigned int max = MAX_DECK_SIZE;
	while (length > max) {
		//ignore line
		max++;

	}

	std::string token = line.substr(0, line.find(delimiter));
	line.erase(0, line.find(delimiter) + delimiter.length());
	length = stoi(line);
	index = 0;


	if ( (token.compare("extra") != 0) || (length < MIN_EXTRADECK_SIZE ) || (length > MAX_EXTRADECK_SIZE)  ){
		return false;
	}

	while (std::getline(cfile, line) && index <= length ) {

		std::stringstream ss(line);
		if (!line.empty() && !extradeck.IdentifyCard(ss)) {
			return false;
		}
		index++;
	}

	extradeck.shuffle();
	deck.shuffle();
    return true;
}

// reload saved game
bool Player::Reload(std::fstream& savedgames, Deck& deck) {
	std::string line;
	std::string delimiter = "/";

	getline(savedgames, line);

	std::string token = line.substr(0, line.find(delimiter));

	line.erase(0, line.find(delimiter) + delimiter.length());
	token = line.substr(0, line.find(delimiter));
	name = token;

	line.erase(0, line.find(delimiter) + delimiter.length());

	lifepoints = std::stoi(line);

	getline(savedgames, line);
	for (unsigned int i = 0; i < ELEMENTS_COUNT; i++) {

		token = line.substr(0, line.find(delimiter));
		line.erase(0, line.find(delimiter) + delimiter.length());
		elements[i] = std::stoi(token);
	}


	return true;
}

// reload monster cards from deck into monsterzone
void Player::ReloadZone(int size, List& zone, List& hand, Deck& deck) {
	for (int i = 0; i < size; i++) {
		DrawToHand(hand, deck);
		MoveToZone(0,zone, hand);
	}

}

// reload cards from deck to hand
void Player::ReloadHand(int size, List& hand, Deck& deck) {
	for (int i = 0; i < size; i++) {
		DrawToHand(hand, deck);
	}

}

// change name of the player
void Player::ChangeName(std::string n) {
	name = n;
}

// check if card at index on hand costs more than current state of elements of the player
bool Player::CheckCost(int index, const List& hand) {
	const unsigned int* a = hand.GetCost(index);
	if (a == nullptr) {
		std::cout << "Error: Normal cards do not have costs.\n";
		return false;
	}

	unsigned int size = sizeof(elements) / sizeof(*elements);
	for (unsigned int i = 0; i < size; i++) {
		if (a[i] > elements[i]) {
			return false;
		}
	}
	SubtractCost(a);
	return true;
}

// subtract the cost in parameter from elements
void Player::SubtractCost(const unsigned int* a) {

	int size = sizeof(elements) / sizeof(*elements);
	for (int i = 0; i < size; i++) {
		elements[i] = elements[i] - a[i];
	}

}

// get array of number of elements
const unsigned int* Player::GetAllElem() {
	return elements;
}

// subtract from elements the const of rare cards
void  Player::PayForRareCard() {
	elements[PAY_ELEMENT] = elements[PAY_ELEMENT] - PAY_ELEMENT_LIMIT;
}

// check if player has enough of elements to pay for a rare card
bool Player::CheckElemLimit()  const {
	return elements[PAY_ELEMENT] < PAY_ELEMENT_LIMIT;
}

// check if the player reach the goal for elements
bool Player::CheckElements()const {
	int size = sizeof(elements) / sizeof(*elements);
	for (int i = 0; i < size; i++) {
		if (elements[i] < ELEMENTS_LIMIT) {
			return false;
		}
	}
	return true;
}

// get number of elements by index
unsigned int Player::GetElement(unsigned int ind)const {
	if (ind >= 0 && ind < ELEMENTS_COUNT) {
		return elements[ind];
	}
	else { return -1; }
}

// add 1 to element at index
void Player::AddElement(unsigned int ind) {
	if (ind >= 0) {
		elements[ind]++;
	}
}

// subtrack the emount from element at index
bool Player::DecElement(unsigned int ind, unsigned int amount) {
	if (ind >= 0 && ind < ELEMENTS_COUNT && elements[ind] >= amount) {
		elements[ind] = elements[ind] - amount;
		return true;
	}
	return false;
}

// print hand to terminal
void Player::Display(const List& hand) const {
	std::cout << "Hand:\n";
	hand.TerminalDisplay();
}

// prompt player to choose between attack and defence mode when placing a card
bool Player::PlacementModePrompt()const {
	int mode = 0; 
	std::cout << "Place card in defence or attack mode? Select 1 for attack and 2 for defence.\n";
	std::cin >> mode;

	while (mode == 0) {
		std::cin >> mode;
		while (!std::cin.good()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input! Select an option: ";
			std::cin >> mode;

		}
		
		if (mode != 1 && mode != 2) {
			mode = 0;
			std::cout << "Please enter a valid option.: ";
		}

	}
	return mode - 1;
}

// subtrack from lifepoints
void  Player::TakeDamage(unsigned int d) {
    if (d >= lifepoints) {
        lifepoints = 0;
    } else lifepoints -= d;
}

// add to lifepoints
void Player::AddHealth(unsigned int h) {
    lifepoints += h;
}

// check number of cards on hand
bool Player::HandSizeCheck(List& hand) {
	if (hand.size() == HAND_SIZE) {
		return true;
	}

	return false;
}

// draw a card from deck to hand
bool Player::DrawToHand(List& hand, Deck& deck) {							// removes the top card from deck and adds it to the hand
    if (deck.size() > 0) {

        hand.AddCard(std::move(deck.DrawCard()));
        return true;                        
    }

    return false;															// hand is full 
}

// first draw of the game
void Player::FirstDraw(List& hand, Deck& deck) {							// game is initialized by drawing full hand
    for (unsigned int i = 0; i < FIRST_DRAW; i++) {
        DrawToHand(hand, deck);
    }
}

// move card at index from list at third argument hand to list in second argument zone 
bool Player::MoveToZone(unsigned int index, List& MonsterZone, List& hand) { 
    if (MonsterZone.size() <= MONSTER_ZONE_SIZE) {

        MonsterZone.AddCard(std::move(hand.RemoveCard(index)));
		
        return true;
    }
    return false; 
}

// move card at index from zone to graveyard
bool Player::MoveToGraveyard(unsigned int index,  List& MonsterZone) {
    graveyard.AddCard(std::move(MonsterZone.RemoveCard(index)));
    return true; 
}

// prtin player into file 
void Player::FilePrint(std::ofstream& os, const List& monsterZone, const List& hand, const Deck& deck, const Deck& extradeck) {
	os << "Player/" << name << "/" << lifepoints << "\n";

	for (unsigned int i = 0; i < ELEMENTS_COUNT; i++) {

		os << elements[i];
		if (i < ELEMENTS_COUNT - 1) {
			os << "/";
		}
	}
	os << "\n";
	os << "zone/" << monsterZone.size() << "\n";
	os << monsterZone;

	os << "hand/" << hand.size() << "\n";
	os << hand;

	os << "deck/" << deck.size() << "\n";
	os << deck;

	os << "extradeck/" << extradeck.size() << "\n";
	os << extradeck;
}
