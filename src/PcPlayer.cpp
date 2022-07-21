
#include "PcPlayer.hpp"

PcPlayer::PcPlayer(): Player() { }

size_t	PcPlayer::SizeOfDeck		()const { return deck.size(); }				// returns number of cards left in the deck
size_t	PcPlayer::SizeOfExtraDeck	()const { return extradeck.size(); }		// returns number of cards left in the extradeck
size_t	PcPlayer::SizeOfZone		()const { return monsterZone.size(); }		// returns number of cards in the monster zone
bool	PcPlayer::Alive				()const { return false; }					// not alive
void PcPlayer::InitDraw				()		{ FirstDraw(hand, deck); }			// draw at the beginig of a new game

// chooses a name and introduces themselves
void PcPlayer::NamePrompt() {
	int ind = rand() % 3;
	ChangeName(computerNames[ind]);
	std::cout << "Your opponent is " << GetName() << ".\n";
}

// throw random card to the graveyard
void PcPlayer::DiscardCard() {
	unsigned int ind = rand() % hand.size();
	MoveToGraveyard(ind, hand);
}

// print zone into the terminal keeping private cards hidden
void PcPlayer::DisplayZone(bool forAll)const {
	monsterZone.DisplayContent(forAll);
}

// print hand 
void PcPlayer::DisplayHand()const {
	Display(hand);
}

// call base class method to load deck from file
bool PcPlayer::Load(const char* filepath) {
	if (!LoadDeck(deck, extradeck, filepath)) {
		return false;
	}
	NamePrompt();
	return true;
}

// find card at index in monster zone and copy its attributes into the parameter
void PcPlayer::GetZoneCard(Card& card, unsigned int ind)  {
	monsterZone.GetCard(card,ind);
}

// if limit of card for hand is reached, random cards is throw into graveyard
void PcPlayer::HandCheck() {
	if (HandSizeCheck(hand)) {
		DiscardCard();
	}

}

// draw a card and set one on the board 
bool PcPlayer::DrawAndSet() {
	int selection = -1;
	int mode = 0;
	size_t handSize = 0;

	//Display(hand);


	if (!DrawToHand(hand, deck)) {							// deck is empty
		monsterZone.ClearLastIndex();
		return false;
	}

	if (monsterZone.size() == MONSTER_ZONE_SIZE) {			// monster zone is full
		monsterZone.ClearLastIndex();
		return true;
	}


	if (hand.OnlyEffectCards()) {							// zone contains only effect cards
		DiscardCard();

		monsterZone.ClearLastIndex(); 
		return true;

	} else if (monsterZone.NumberOfDefence() < monsterZone.size() / 2) {		// keep half of the zone filled with defence cards
		selection = hand.MaxDefence(false); 
		if (selection >= 0) {													
			hand.FlipCard(selection);
		}
		else  {																	// if hand fails to find max defence, try min attack and flip it to defence mode
			selection = hand.MinAttack(false);									
			if (selection >= 0) {
				hand.FlipCard(selection);
			}
		}

	} 

	if (selection < 0) {														// no selection has yet been made
		if ((selection = hand.MaxAttack(false)) < 0) {							// try to find max attack in hand (keep in attack mode)
			selection = hand.MinDefence(false);									// if even that fails find min defence and keep in attak mode 
		}
	}
	


	if (selection < 0) {														// if four attempt at finding a card failed just skip this step
		monsterZone.ClearLastIndex();
		return true;
	}



	MoveToZone(selection, monsterZone, hand);
	//MoveToZone(selection - 1);
	
	return true;
}																	

// re-load deck from a file
// method pushes every card into deck and then draw form the deck into particular zones according to the size given in the file
bool PcPlayer::ReloadPlayer(std::fstream& savedgames) {

	this->Reload(savedgames, deck);
	std::string line;
	int linenumber = 1;
	std::string delimiter = "/";

	getline(savedgames, line);
	line.erase(0, line.find(delimiter) + delimiter.length());
	int zoneSize = std::stoi(line);

	for (int i = 0; i < zoneSize; i++) {

		getline(savedgames, line);
		std::stringstream ss(line);
		deck.IdentifyCard(ss);
	}

	getline(savedgames, line);
	line.erase(0, line.find(delimiter) + delimiter.length());
	int handSize = std::stoi(line);

	for (int i = 0; i < handSize; i++) {

		getline(savedgames, line);
		std::stringstream ss(line);
		deck.IdentifyCard(ss);
	}

	getline(savedgames, line);
	line.erase(0, line.find(delimiter) + delimiter.length());
	int deckSize = std::stoi(line);

	for (int i = 0; i < deckSize; i++) {

		getline(savedgames, line);
		std::stringstream ss(line);
		deck.IdentifyCard(ss);
	}



	ReloadZone(zoneSize, monsterZone, hand, deck);
	ReloadHand(handSize, hand, deck);
	return true;
}


// pick a card to attack and which card to attack
bool PcPlayer::Attack(int& p1, int& p2, const List* opponentZone) {
	size_t limit = opponentZone->size();
	int selection = -1;



	if ( limit == 0 ){																		// there are no monster to attack 
		return false;
	} 
	if (monsterZone.OnlyDefence() && (monsterZone.NumberOfDefence() > MONSTER_ZONE_SIZE  / 2)) {		// more than half of the zone is taken up by cards in defence mode
		monsterZone.FlipCard(monsterZone.MaxAttack(false));												// then flip the one with highest attack value					
	}

	std::vector<std::pair<int, int>> zone = opponentZone->AttackList();									// get list of all attack monsters from the opponent
	
	int min = 0;
	int opCardInd = -1;


	selection = monsterZone.MaxAttack(true);															// find an attack card with the highest value
	if (selection == monsterZone.LastAddedIndex() || selection <0) {
		selection = monsterZone.MinDefence(true);
	}

	if ( selection < 0) {
		selection = monsterZone.FirstAvailable();
	}

	if (zone.size() == 0) {																				// if opponent has not attack monsters, attack random defence monster
		p2 = rand() % (limit - 1);
		p1 = selection;
		return true;
	}

	Card tmp;
	monsterZone.GetCard(tmp, selection);

	for (size_t i = 0; i < zone.size(); i++) {															// attack a sure win
		if (zone[i].second >= min && zone[i].second < tmp.GetATK()) {
			opCardInd = zone[i].first;
			min = zone[i].second;
		}
	}

	if (opCardInd == -1) {
		return false;
	}

	p1 = selection;
	p2 = opCardInd;


	return true;

}

// card at index in monster zone is move to graveyard
bool PcPlayer::RemoveCard(unsigned int ind) {
	return MoveToGraveyard(ind, monsterZone);
}

// return monster zone 
const List* PcPlayer::GetZone() const {
	return &(monsterZone);
}

// call base class method to print player attributes to file
void PcPlayer::SavePrint(std::ofstream& os) {
	FilePrint(os, monsterZone, hand, deck, extradeck);

}
