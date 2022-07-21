
#include "Game.hpp"

Game::Game	() { player.reserve(2); round = 0; }
Game::~Game	() {}

// initialize game between two human players; returns false if player class fails to load deck of cards 
bool Game::Initialize() {								// return false if decks failed to load
	HumanPlayer* p1 = new HumanPlayer();
	HumanPlayer* p2 = new HumanPlayer();

	if (p1->Load(CardsFilePath) && p2->Load(CardsFilePath)) {
		player.emplace_back(p1);
		player.emplace_back(p2);

		return true;
	}

	return false;
}

/*  initialize game between human and computer; 
returns false if player class fails to load deck of cards;
it is given that the computer player is always second in the vector of players
*/
bool Game::InitializeSolo() {
	HumanPlayer* p1 = new HumanPlayer();
	PcPlayer* p2 = new PcPlayer();

	if (p1->Load(CardsFilePath) && p2->Load(CardsFilePath)) {
		player.emplace_back(p1);
		player.emplace_back(p2);
		return true;
	}

	return false;
}

// initialize game between two human players from file stream; returns false if it fails to load players from file
bool Game::ReloadInitialize(std::fstream& fs) {	
	Player* p1 = new HumanPlayer();
	Player* p2 = new HumanPlayer();

	if (p1->ReloadPlayer(fs) && p2->ReloadPlayer(fs)) {
		player.emplace_back(p1);
		player.emplace_back(p2);
	}
	return true;
}

// initialize game between human and computer player from file stream; returns false if it fails to load players from file
bool Game::ReloadInitializeSolo(std::fstream& fs) {
	Player* p1 = new HumanPlayer();
	PcPlayer* p2 = new PcPlayer();

	if (p1->ReloadPlayer(fs) && p2->ReloadPlayer(fs)) {
		player.emplace_back(p1);
		player.emplace_back(p2);
	}
	return true;
}

// prompts human player ( the first one if its human vs human) to choose a coin side to decide who goes first at the begining of the game
void Game::CoinFlip() {
	int pick = 0;

	std::cout << "To determine who goes first, let's toss a coin!\n";
	std::cout << player[0]->GetName() << ", pick a side, 1 for Heads or 2 for Tails: ";
	while (pick != 1 && pick != 2) {
		std::cin >> pick;
		while (!std::cin.good()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input! Select an option: ";
			std::cin >> pick;

		}

		if (pick != 1 && pick != 2) {
			pick = 0;
			std::cout << "Invalid input! Really? How hard can it be to pick one out of two numbers?\nSelect an option: ";

		}
	}

	int coin = rand() % 2 + 1;
	if (coin == pick) {
		currentplayer = 0;
	}
	else currentplayer = 1;

	std::cout << "The coin landed on " << ((coin == 1) ? "Heads" : "Tails") << ", " << player[currentplayer]->GetName() << " will go first.\n";
	//player[currentplayer] write out if human
	//std::cout << "We're all good to go! The following step will reveal your cards on hand, " << player[currentplayer]->GetName() << ", so prepare accordingly.\nReady?\n";


}

// start the game by coin flip and initial draw of FIRST_DRAW cards by each player
void Game::Start() {

	CoinFlip();
	EnterKey();

	player[0]->InitDraw();
	player[1]->InitDraw();
	GamePlay();
}

// restarts game loaded from file 
void Game::ReStart(int curplayer) {
	currentplayer = curplayer;

	std::cout << "We're all good to go! Get ready, " << player[currentplayer]->GetName() << "!\n";
	
	EnterKey();
	GamePlay();
}

// rounds of players 
void Game::GamePlay() {

	int winner = 0;
	while (winner == 0 && !quit) {

		DisplayBoard(true);														// display the currents state of the board visible to both players
		std::cout << player[currentplayer]->GetName() << "'s turn.\n";			// announce the current player
		EnterKey();

		DisplayBoard(Solo() && currentplayer);									// display the board with every card visible form the current player

		player[currentplayer]->HandCheck();										// current player will draw a card in the next step, check if the number of card on hand hasnt reached the limit
		clrscr();
		DisplayBoard(Solo() && currentplayer);									// re-load the board in the same sense again in case of some changes in the previous step

		if (!player[currentplayer]->DrawAndSet()) {								// current player draws a card, if they do not have any cards in the deck left the opponent wins
			winner = (currentplayer + 1) % 2;
		}

		EnterKey();
		clrscr();

		Battle();																// begin battle phase

		winner = CheckWinner();													// current has finished check if any changes made someone a winner
		if (winner != 0) {
			break;
		}
		round++;
		if (winner == 0 && round%2 == 0) {										// players have the chance to save and exit game only if a full round has been played ( both players took the same number of turns)
			if (SavePrompt()) {
				quit = true;
			}
		}

		SwapPlayers();															// change the current players

	}
	if (quit) {																	// leave if players decided to save and exit
		return;
	}

	if (winner == 3) {															// game ended with both players losing/winning 

		std::cout << " It' a tie!\n";
	}
	else if (winner == 1) {														// first player won
		std::cout << player[0]->GetName() << " wins!!\n";
		std::cout << "  Congratulations!\n";
	}
	else if (winner == 2) {														// second player won if solo game then this would be the computer player
		std::cout << player[1]->GetName() << " wins!!\n";
		if (Solo()) {
			std::cout << "Better luck next time!\n";

		}
	}


	EnterKey();
}

void Game::Battle() {
	int p1 = 0;
	int p2 = 0;


	DisplayBoard(Solo() && currentplayer);										// display the board for the current player to see all their cards
	
	if (!player[currentplayer]->Attack(p1, p2, player[(currentplayer + 1) % 2]->GetZone() )) {		// player is prompt to pick a card to attack with (p1) and which of the opponent's they want to attack (p2) 
																									// size parameter is given as a way to secure the player chooses a card within the bounds of opponents monster zone
		EnterKey();																					// prompt returns false if player chose not to attack (or couldnt)
		return;
	}

	int difference = 0;
	EnterKey();

	Card attacker;
	player[currentplayer]->GetZoneCard(attacker, p1 - 1);							// get the player's card from the monster zone (board|
	Card attacked;
	player[(currentplayer + 1) % 2]->GetZoneCard(attacked, p2 - 1);					// get the opponent's card from the boards=

	std::cout << player[currentplayer]->GetName() << " activated card \n";			// print in terminal choices of the player
	PrintHeader();
	std::cout << "---" << attacker << "\n and attacks card\n";
	PrintHeader();
	std::cout << "---" << attacked << "\n";

	// if the card that is attacked is in defence the resulting points are calculated:
	if (attacked.GetMode() == CardMode::Defence) {
		difference = attacker.GetATK() - attacked.GetDEF();
		if (difference < 0) {										// opponent won 
			player[currentplayer]->TakeDamage(abs(difference));
			std::cout << player[(currentplayer + 1) % 2]->GetName() << " wins this round and " << player[currentplayer]->GetName() << " takes damage " << abs(difference) << " lifepoints.\n";
			std::cout << "No cards are removed because a defence monster was attacked.\n";
			player[(currentplayer + 1) % 2]->AddElement(static_cast<unsigned int>(attacked.GetElem()));
			std::cout << player[(currentplayer + 1) % 2]->GetName() << " gains " << ElemToString(attacked.GetElem()) << " element.\n";
		}
		else if (difference > 0) {									// player won
			player[(currentplayer + 1) % 2]->TakeDamage(difference);
			std::cout << player[currentplayer]->GetName() << " wins this round but "  << player[(currentplayer + 1) % 2]->GetName() << " takes no damage since a defence monster was attacked.\n";
			player[(currentplayer + 1) % 2]->RemoveCard(p2 - 1);
			std::cout << player[(currentplayer + 1) % 2]->GetName() << "'s card is destroyed.\n";
			player[currentplayer]->AddElement(static_cast<unsigned int>(attacker.GetElem()));
			std::cout << player[currentplayer]->GetName() << " gains " << ElemToString(attacker.GetElem()) << " element.\n";
		} else {														// same values on both cards
			player[currentplayer]->RemoveCard(p1 - 1);
			player[(currentplayer + 1) % 2]->RemoveCard(p2 - 1);
			std::cout << "It's a tie! Both cards are destroyed and none of the players take any damage.\n";
		}

	}
	else {// if the card that is attacked is in attack the resulting points are calculated:
		difference = attacker.GetATK() - attacked.GetATK();
		if (difference < 0) {										// opponent won
			player[currentplayer]->TakeDamage(abs(difference));
			player[currentplayer]->RemoveCard(p1-1);
			std::cout << player[(currentplayer + 1) % 2]->GetName() << " wins this round and " << player[currentplayer]->GetName() << " takes damage " << abs(difference) << " lifepoints.\n";
			std::cout << player[currentplayer]->GetName() << "'s card is destroyed.\n";
			player[(currentplayer + 1) % 2]->AddElement(static_cast<unsigned int>(attacked.GetElem()));
			std::cout << player[(currentplayer + 1) % 2]->GetName() << " gains " << ElemToString(attacked.GetElem()) << " element.\n";
		}
		else if (difference > 0) {									// player won
			player[(currentplayer + 1) % 2]->TakeDamage(difference);
			player[(currentplayer + 1) % 2]->RemoveCard(p2-1);
			std::cout << player[currentplayer]->GetName() << " wins this round and " << player[(currentplayer + 1) % 2]->GetName() << " takes damage " << abs(difference) << " lifepoints.\n";
			std::cout << player[(currentplayer + 1) % 2]->GetName() << "'s card is destroyed.\n";
			player[currentplayer]->AddElement(static_cast<unsigned int>(attacker.GetElem()));
			std::cout << player[currentplayer]->GetName() << " gains " << ElemToString(attacker.GetElem()) << " element.\n";
		}
		else {														// same value on both cards
			player[currentplayer]->RemoveCard(p1-1);
			player[(currentplayer + 1) % 2]->RemoveCard(p2-1);
			std::cout << "It's a tie! Both cards are destroyed and none of the players take any damage.\n";
		}

	
	}
	
	EnterKey();
	DisplayBoard(true);									// display the results on the board 
	EnterKey();
}

// change the current player
void Game::SwapPlayers() {
	currentplayer = (currentplayer + 1) % 2;
}

// check if any of the players won/lost
int Game::CheckWinner() const {
	int winner = 0;
	if ( (player[0]->GetLifepoints() == 0)&& (player[1]->GetLifepoints() == 0)) {
		winner = 3;
	}
	else if (player[0]->GetLifepoints() == 0 || player[0]->CheckElements()) {
		winner = 2;
	}
	else if (player[1]->GetLifepoints() == 0 || player[1]->CheckElements()) {
		winner = 1;
	}

	return winner;
}

//prints top label information for the hand
void Game::PrintHeader()const {															

	std::cout.ios_base::setf(std::ios_base::left, std::ios_base::adjustfield);
	std::cout << "#  " << std::setw(38) << "Monster Name";
	std::cout << std::setw(4) << "ATK";
	std::cout << " " << std::setw(4) << "DEF";
	std::cout << " " << std::setw(6) << "Type" ;
	std::cout << " " << std::setw(5) << "Mode";
	std::cout << " " << std::setw(5) << "Element";
	std::cout << " " << std::setw(5) << "Cost(E/F/W)";
	std::cout << " " << "Description\n";

}

// print out board of game, parameter forall controls when are privated cards shown
void Game::DisplayBoard(bool forall)const {
	// first part is always for the enemy therefore private cards are never shown
	std::cout << "-----------------------------------------------------------------------------------------------\n";
	std::cout  << player[(currentplayer + 1) % 2]->GetName() << " Side\n";	// show the name of the opponent
	// and show all their public attributes that is: life points, number of cards in deck and extradeck, number of elements 
	std::cout << "LifePoints: " << player[(currentplayer + 1) % 2]->GetLifepoints() << " | Deck: " << player[(currentplayer + 1) % 2]->SizeOfDeck() << " cards" << " | ExtraDeck: " << player[(currentplayer + 1) % 2]->SizeOfExtraDeck() << " cards ";
	std::cout << " | Elements: earth: " << player[(currentplayer + 1) % 2]->GetElement(0) << " fire: " << player[(currentplayer + 1) % 2]->GetElement(1) << " water: " << player[(currentplayer + 1) % 2]->GetElement(2) << "\n";
	PrintHeader();
	player[(currentplayer + 1) % 2]->DisplayZone(true); 
	
	std::cout << "-----------------------------------------------------------------------------------------------\n";
	// second half is for the current player privater cards are not shown if current player is the computer, reveals the same attributes as on the opponent's side
	std::cout << player[currentplayer]->GetName() << " Side\n";
	
	std::cout << "LifePoints: " << player[currentplayer]->GetLifepoints() << " | Deck: " << player[currentplayer]->SizeOfDeck() <<" cards" << " | ExtraDeck: " << player[currentplayer]->SizeOfExtraDeck() << " cards ";
	std::cout << " | Elements: earth: " << player[currentplayer]->GetElement(0) << " fire: " << player[currentplayer]->GetElement(1) << " water: " << player[currentplayer]->GetElement(2) << "\n";
	PrintHeader();
	player[currentplayer]->DisplayZone(forall);

	std::cout << "-----------------------------------------------------------------------------------------------\n";
	
}

// returns true if both players are human, only used to properly write down saved game
bool Game::Solo()const {
	return player[0]->Alive() && player[0]->Alive();
}

// return true if players chose to save the game
bool Game::SaveAndEnd() const {
	return quit;
}

// used to properly write down the state of the game in a file
int Game::GetCurrentPlayer()const {
	return currentplayer;
}

// change players
void Game::SavePlayers(std::ofstream& os) {

	player[0]->SavePrint(os);
	player[1]->SavePrint(os);
}

// asks human player if they would like to save and exit the game
bool Game::SavePrompt() const {

	bool result = false;
	std::cout << "SAVE and EXIT? (y/n) ";
	char c = getchar();
	flushCin();

	while (c != 'y' && c != 'n') {

		std::cout << "Listen, this could not be a more simple question, y or n ? ";
		c = getchar();
		flushCin();

	}
	if ( c == 'y') {
		result = true;
	}

	flushCin();
	clrscr();
	return result;
}