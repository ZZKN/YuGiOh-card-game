
#include "HumanPlayer.hpp"

HumanPlayer::HumanPlayer() : Player() { }
HumanPlayer::~HumanPlayer() {}


size_t	HumanPlayer::SizeOfZone			() const { return monsterZone.size(); }		// returns number of cards in the monster zone
size_t	HumanPlayer::SizeOfDeck			() const { return deck.size(); }			// returns number of cards left in the deck
size_t	HumanPlayer::SizeOfExtraDeck	() const { return extradeck.size(); }		// or extra deck
void	HumanPlayer::InitDraw			()		 { FirstDraw(hand, deck); }			// draw at the beginig of a new game 
bool	HumanPlayer::Alive				() const { return true; }					// check if player lives

// asks player for a name
void HumanPlayer::NamePrompt() {
	std::string name;
    std::cout << "Player enter your name:";
	std::cin >> name;
    flushCin();
	ChangeName(name);
}

// call base class method to load deck from file
bool HumanPlayer::Load(const char* filepath) {
	if (!LoadDeck(deck, extradeck, filepath)) {
		return false;
	}
	NamePrompt();
	return true;
}

// re-load deck from a file
// method pushes every card into deck and then draw form the deck into particular zones according to the size given in the file
bool HumanPlayer::ReloadPlayer(std::fstream& savedgames) {

	Reload(savedgames, deck);									//call base class method to re-load the attributes not needed in this derived class
	std::string line;
	std::string token;
	int linenumber = 1;
	std::string delimiter = "/";

																
	getline(savedgames, line);
	line.erase(0, line.find(delimiter) + delimiter.length());	// first in the file is monster zone
	int zoneSize = std::stoi(line);								// get the number of cards in the zone

	for (int i = 0; i < zoneSize; i++) {

		getline(savedgames, line);
		std::stringstream ss(line);
		deck.IdentifyCard(ss);									// push into the deck (deque)
	}

	getline(savedgames, line);
	line.erase(0, line.find(delimiter) + delimiter.length());	// second in the file is the hand
	int handSize = std::stoi(line);								// get the number of cards in the hand

	for (int i = 0; i < handSize; i++) {

		getline(savedgames, line);
		std::stringstream ss(line);
		deck.IdentifyCard(ss);									// push into the deck (deque)
	}

	getline(savedgames, line);
	line.erase(0, line.find(delimiter) + delimiter.length());	// third is the deck itself
	int deckSize = std::stoi(line);								// get the number of cards left in the deck

	for (int i = 0; i < deckSize; i++) {

		getline(savedgames, line);
		std::stringstream ss(line);
		deck.IdentifyCard(ss);									// push into the deck (deque)
	}

	getline(savedgames, line);
	line.erase(0, line.find(delimiter) + delimiter.length());	// last one is the extra deck
	deckSize = std::stoi(line);

	for (int i = 0; i < deckSize; i++) {

		getline(savedgames, line);
		std::stringstream ss(line);
		extradeck.IdentifyCard(ss);
	}


	this->ReloadZone(zoneSize, monsterZone, hand, deck);		// draw to monster zone according to the size given
	this->ReloadHand(handSize, hand, deck);						// draw to hand
	return true;
}

// card at index in monster zone is move to graveyard
bool HumanPlayer::RemoveCard(unsigned int ind) {
	return MoveToGraveyard(ind, monsterZone);
}

// prompt player to choose from which deck to draw
// player must draw a card at the begining of each turn
// if they choose the extra deck pay with elements
int HumanPlayer::DeckPrompt()const {
	int selection = 0;


	std::cout << "Draw from:\n";
	std::cout << "   1. Main Deck\n";
	std::cout << "   2. Extra Deck\n"; 
	std::cout << "\nPlease select an option: ";
	
	while (selection != 1 && selection != 2) {
		std::cin >> selection;
		flushCin();
		while (!std::cin.good())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input! Select an option: ";
			std::cin >> selection;
			flushCin();

		}
		if ( selection != 1 && selection != 2 ) {
			selection = 0;
			std::cout << "Invalid input! Select an option: ";

		} else if ( (selection == 2) && CheckElemLimit()) {
			selection = 0;
			std::cout << "Minimum "<< PAY_ELEMENT_LIMIT << " of "<< ElemToString ( (Elements)(PAY_ELEMENT)) <<" elements. Please enter a valid option.";

		}

	}


	return selection;
}

// random card from hand is move to graveyard
void HumanPlayer::DiscardCard() {
	unsigned int ind = rand() % hand.size();
	MoveToGraveyard(ind, hand);
}

// draw from chosen deck
bool HumanPlayer::Draw() {

	int option = DeckPrompt();

	if (option == 1) {
		if (!DrawToHand(hand, deck)) {
			std::cout << "Deck is empty.";																			// main deck is empty
			return false;
		}
	}
	else {
		if (!DrawToHand(hand, extradeck)) {
			std::cout << "ExtraDeck is empty.";																			// extra deck is empty
			return false;
		}

		PayForRareCard();
	}

	return true;

}

// if limit of card for hand is reached, random cards is throw into graveyard
void HumanPlayer::HandCheck() {
	if (HandSizeCheck(hand)) {
		std::cout << "You have reached the limit for number of cards in hand. Before the next draw one of the cards will be randomly discarded.";
		DiscardCard();
	}

}

// hand has only effect cards the player must pay for a card or thow one into graveyard
void HumanPlayer::PayOrThrow() {
	int selection = -1;
	int choice = -1;
	std::cout << "Choose one (by index) to discard or to purchase. If you do not have elements to pay for the card it will be discarded.\n";
	
	std::cout << "1. Discard\n";
	std::cout << "2. Purchase\n";
	std::cout << "Please select an option: ";
	std::cin >> choice;

	while (choice != 1 && choice != 2) {
		std::cin >> choice;
		while (!std::cin.good())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "We don't have all day. Enter a valid option: ";
			std::cin >> choice;

		}
		if (choice != 1 && choice != 2) {
			choice = 0;
			std::cout << "We don't have all day. Enter a valid option: ";

		}

	}
	std::cout << "Enter index: \n";

	while (selection < 0) {
		std::cin >> selection;
		while (!std::cin.good())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input! Select an option: ";
			std::cin >> selection;

		}
		if (selection < 1 || selection > (int)hand.size()) {
			selection = -1;
			std::cout << "Enter a valid option: ";
		}

	}

	if (choice == 1) {
		MoveToGraveyard(selection-1, hand);
		return;
	}

	if (CheckCost(selection - 1, hand)) {
		if (MoveToZone(selection - 1, monsterZone, hand)) {													// move selected card from hand to the board (zone)
			//monsterZone.ActivateCard(GetAllElem());
			return;
		}
	}
	else {
		MoveToGraveyard(selection - 1, hand);
	}

}

// draw a card and set one on the board 
bool HumanPlayer::DrawAndSet() {
	int selection = -1;
	int mode = 0;
	size_t handSize = 0;
	bool special = false;
	
	if (!Draw()) {											// deck is empty
		return false;
	}

	DisplayHand();

	if (hand.OnlyEffectCards()) {
		PayOrThrow();
	}

	handSize = hand.size();

	if (monsterZone.size() == MONSTER_ZONE_SIZE) {																	// monster zone is full
		std::cout << "You cannot place any cards in this turn because the Monster Zone is full.\n";				
		monsterZone.ClearLastIndex();
		FlipCard(false);
		return true;
	}

	std::cout << "Select a card you would like to place on the board by index. (1 - " << handSize << "). Select " << handSize + 1 << " to skip this step.\n";
	
	while (selection < 0) {
		std::cin >> selection;
		while (!std::cin.good()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input! Select an option: ";
			std::cin >> selection;

		}
		if (selection < 1 || selection > handSize + 1) {										// selected index is out of range
			selection = -1;
			std::cout << "Please enter a valid index: ";
		}
		else if (selection != handSize + 1) {
			Card tmp;
			hand.GetCard(tmp, selection - 1);
			special = (tmp.GetType() != MonsterType::Normal);
			if (special && !CheckCost(selection - 1, hand)) {
				selection = -1;
				std::cout << "Not enough elements.\n";
			}
		}

	}

	if (selection == handSize + 1) {											// skip placing a card
		
		if (monsterZone.size() > 1) {
			monsterZone.ClearLastIndex();
			FlipCard(true);

		}
		return true;
	}

	if (PlacementModePrompt()) {												// prompt to change mode
		hand.FlipCard(selection-1);
	}
	
	if (MoveToZone(selection - 1, monsterZone, hand) && special) {				// move selected card from hand to the board (zone)
		//monsterZone.ActivateCard(GetAllElem());
	}

	
	if (monsterZone.size() > 1) {
		FlipCard(true);
	}
	return true;
}

// find card at index in monster zone and copy its attributes into the parameter
void HumanPlayer::GetZoneCard(Card& card,unsigned int ind)  {
	monsterZone.GetCard(card, ind);
}

// show content of monter zone; forAll is set to true if the display public
void HumanPlayer::DisplayZone(bool forAll) const {
	monsterZone.DisplayContent(forAll);
}

// show cards on hand
void HumanPlayer::DisplayHand() const {
	Display(hand);
}

// change mode of the card, CardAdded is true if a card was placed into zone this round
void HumanPlayer::FlipCard(bool CardAdded) {
	int selection = -1;

	if (monsterZone.size() == 0) {
		return;
	}

	std::cout << "Select a card from the board to change into defence/attack position. Select 0 to continue.\n";
	
	while (selection < 0) {
		std::cin >> selection;
		while (!std::cin.good()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input! Select an option: ";
			std::cin >> selection;

		}
		if (selection < 0 || selection > monsterZone.size()) {
			selection = -1;
			std::cout << "Please enter a valid index.\n";
		}
		else if (selection == 0) {
			break;
		}
		else if (CardAdded && selection == monsterZone.size()) {
			selection = -1;
			std::cout << "Card that was just place cannot be flipped.\n";

		}
		else if (selection >= 0 && selection <= monsterZone.size()) {
			monsterZone.FlipCard(selection);
		}

	}
}

// return monster zone 
const List* HumanPlayer::GetZone() const {
	return &monsterZone;
}

// pick a card to attack and which card to attack
bool HumanPlayer::Attack(int& p1, int& p2, const List* opponentZone) {
	int limit = (int)opponentZone->size();
	if (! BattlePrompt()) {
		return false;
	}

	std::cout << "Select a card for an attack:\n";
	while (p1 == 0) {
		std::cin >> p1;
		while (!std::cin.good()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input! Select an option: ";
			std::cin >> p1;

		}
		if (p1 < 1 || p1 > monsterZone.size()) {													// chosen index out of range
			p1 = 0;
			std::cout << "Please make a valid selection: ";
		}
		else if ( p1 == monsterZone.LastAddedIndex()) {												// chosen card was placed in this round
			std::cout << "\nYou cannot attack and place the same monster during one turn.\n";

			std::cout << "\nWould you like to skip the battle and resume gameplay? (y/n) ";
			char c = getchar();
			
			if (c == 'y') {
				return false;
			}
			else if (c != 'n') {
				std::cout << "\nWell okay, again.\n";
			}
			p1 = 0;
		}
		else {
			Card tmp;
			monsterZone.GetCard(tmp, p1 - 1);
			if (tmp.GetMode() == CardMode::Defence) {												// chosen card is in defence mode
				p1 = 0;
				std::cout << "\nThe card must be in attack position.\n";
			} 
			
		}
	}


	std::cout << "Select opponent's card to attack:\n";
	while (p2 == 0) {
		std::cin >> p2;
		while (!std::cin.good()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input! Select an option: ";
			std::cin >> p2;

		}
		if (p2 < 1 || p2 > limit) {																	// chosen card is out of bounds
			p2 = 0;
			std::cout << "\nPlease make a valid selection: ";
		}
	}
	
	return true;
}

// ask player to attakc
bool HumanPlayer::BattlePrompt() const {
	

	std::cout << "Battle Phase:\n";

	if (SizeOfZone() == 0) {
		std::cout << "There are no mosters to attack at the moment.\n";
		return false;
	} 
	if (monsterZone.OnlyDefence()) {
		std::cout << "You cannot attack the opponent with no monsters in attack position.\n";
		return false;
	}

	
	if (SizeOfZone() == 1 && monsterZone.LastAddedIndex() != -1) {
		std::cout << "You cannot attack and place the same monster during one turn.\n";
		return false;
	}

	int selection = -1;
	char response;
	while (selection < 0) {
		std::cout << "Would you like to attack your opponent?(y/n)\n";
		std::cin >> response;
		if (response == 'n') {
			return false;
		} else if (response == 'y') {
			selection = 0;
		}
		else {
			std::cout << "Invalid Input!\n";
			selection = -1;
		}
	}
	return true;

}

// call base class method to print player attributes to file
void HumanPlayer::SavePrint(std::ofstream& os) {
	FilePrint(os, monsterZone, hand, deck, extradeck);

}

