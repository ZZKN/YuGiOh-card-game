#include "List.hpp"

List::List() { lastindex = -1; }

List::~List() { }

int		List::LastAddedIndex	()const { return lastindex; }
void	List::ClearLastIndex	()		{ lastindex = -1; }

// adds a card to the end of the list
void List::AddCard(std::unique_ptr<Card> card) {								
	list.emplace_back(std::move(card));
	lastindex = static_cast<int>(list.size());
	
}

// get the cost of activation from card at index
const unsigned int* List::GetCost(unsigned int ind) const{
	return list[ind]->GetCost();
}

// function removes the pointer to a card at given index and returns a reference to the card, correct index expected
std::unique_ptr<Card> List::RemoveCard(const int index) {							
	std::unique_ptr<Card> c = std::move(list[index]);
	std::vector<std::unique_ptr<Card>>::iterator it = list.begin() + index;
	list.erase(it);
	return (std::move(c));
}

// returns reference to card at index without removing it from the list
void List::GetCard(Card& card, const int  index)  {										
	card = *list[index];
}

// display card in list to terminal
void List::TerminalDisplay() const {
	int index = 1;

	for (size_t i = 0; i != list.size(); i++) {
		std::cout << index << ". " << *list[i] << "\n";
		index++;

	}

}

// number of cards in defence mode in the list
int List::NumberOfDefence() const {
	int count = 0;
	for (size_t i = 0; i != list.size(); i++) {
		if (list[i]->GetMode() == CardMode::Defence) {
			count++;
		}
	}
	return count;
}

// return vector(index, ATK) of cards in attack mode 
std::vector<std::pair<int, int>> List::AttackList()const {
	
	std::vector<std::pair<int, int>> attackCards;


	for (size_t i = 0; i != list.size(); i++){
		if (list[i]->GetMode() == CardMode::Attack) {
			attackCards.emplace_back (static_cast<int>(i), list[i]->GetATK() );
		}
	}
		
	return attackCards;
}

// activate the last card added; state of players elements as parameter
void List::ActivateCard(const unsigned int * a) {
	if (lastindex < 1) {
		std::cout << "Error: No card to activate.\n";

	}
	else if ( list[lastindex-1]->GetType() == MonsterType::Normal) {
		std::cout << "Error: Normal card cannot be activated.\n";

	}
	else  {
		list[lastindex-1]->Activate(a);
	}
}

// change the mode of card at index
void List::FlipCard(const int  index) {

	list[index]->ChangeMode();
}

// return if list contains only cards in defence mode
bool List::OnlyDefence() const {

	for (size_t i = 0; i < list.size(); i++) {
		if (list[i]->GetMode() == CardMode::Attack) {
			return false;
		}

	}
	return true;
}

// print content of list into the terminal; showAll is true if it's public display 
void List::DisplayContent(bool showAll) const {

	unsigned int ind = 1;
	for (size_t i = 0; i < list.size(); i++) {
		std::cout << ind++ << ". ";

		if (!showAll) {
			std::cout << *list[i] << "\n";
		}
		else {
			if (list[i]->GetMode() == CardMode::Defence) {
				std::cout << " DEF \n";
			}
			else if (list[i]->GetMode() == CardMode::Attack) {

				std::cout << *list[i] << "\n";
			}
		}


	}

}

// print into file
std::ostream& operator<<(std::ostream& os, const List& list) {
	for (size_t i = 0; i < list.size(); i++) {
		list.list[i]->CardSave(os);
	}
	return os;
}

// return number of cards in the list
size_t List::size() const {
	return list.size();
}

// return true if all cards in the list are effect cards
bool List::OnlyEffectCards()const {
	for (size_t i = 0; i < list.size(); i++) {
		if (list[i]->GetType() == MonsterType::Normal) {
			return false;
		}
	}
	return true;
}

// return card index with the lowest attack value
int List::MinAttack(bool forAttack) const {
	int min = 4000;
	int ind = -1;
	for (size_t i = 0; i < list.size(); i++) {
		if ((list[i]->GetATK() <= min) && (list[i]->GetType() == MonsterType::Normal)) {
			if (!forAttack || (forAttack && list[i]->GetMode() == CardMode::Attack)) {
				min = list[i]->GetATK();
				ind = static_cast<int>(i);
			}
		}
	}

	return ind;

}
// return card index with max attack value
int List::MaxAttack(bool forAttack) const {
	int max = 0;
	int ind = -1;
	for (size_t i = 0; i < list.size(); i++) {
		if ((list[i]->GetATK() >= max) && (list[i]->GetType() == MonsterType::Normal)) {
			if(!forAttack || (forAttack && list[i]->GetMode() == CardMode::Attack)) {
				max = list[i]->GetATK();
				ind = static_cast<int>(i);

			}
		}
	}

	return ind;

}
// return card index with the lowest defence value
int List::MinDefence(bool forAttack)const {
	int min = 4000;
	int ind = -1;
	for (size_t i = 0; i < list.size(); i++) {
		if ((list[i]->GetDEF() <= min)  && (list[i]->GetType() == MonsterType::Normal)) {
			if (!forAttack || (forAttack && list[i]->GetMode() == CardMode::Attack)) {
				min = list[i]->GetDEF();
				ind = static_cast<int>(i);
			}
		}
	}

	return ind;

}
// return card index with max defence value
int List::MaxDefence(bool forAttack)const {
	int max = 0;
	int ind = -1;
	for (size_t i = 0; i < list.size(); i++) {
		if ((list[i]->GetDEF() >= max) && (list[i]->GetType() == MonsterType::Normal)) {

			if (!forAttack || (forAttack && list[i]->GetMode() == CardMode::Attack)) {
				max = list[i]->GetDEF();
				ind = static_cast<int>(i);
			}
		}
	}

	return ind;

}
// return index first card in the list that is in attack mode
int  List::FirstAvailable()const {

	int ind = 0;
	while (ind < static_cast<int>(list.size()) - 1) {
		if (list[ind]->GetMode() == CardMode::Attack) {
			return ind;
		}
	}
	return -1;

}
