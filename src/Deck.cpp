#include "Deck.hpp"

Deck::Deck() { }

Deck::~Deck() {};

//load cards from stream in argument; return false if the number of cards is out of bounds or if it fails to identify a card
bool Deck::LoadCards(std::ifstream& cfile) {
    
    std::string line;
    std::vector<std::string> tokens;

    while (std::getline(cfile, line) && deck.size() <= MAX_DECK_SIZE) {

        std::stringstream ss(line);
        if (!line.empty() && !IdentifyCard(ss)) {
            return false;
        }

    }

    if (deck.size() < MIN_DECK_SIZE) {
        return false;
    }

    return true;
}

// identify card from stream in argument that has all card attributes returns false if it fails to identify a card
bool Deck::IdentifyCard(std::stringstream& stream) {

    const char delimiter = '/';
    std::vector<std::string> tokens;
    std::string word;

    while (std::getline(stream, word, delimiter)) {
        tokens.push_back(word);
    }

    if (tokens.size() < 5) {
        return false;
    }

    if (stoi(tokens[0]) == static_cast<int>(MonsterType::Normal) ) {
        auto c = std::make_unique<Card>(tokens[1], stoi(tokens[2]), stoi(tokens[3]), stoi(tokens[4]));
        if (tokens.size() > 5 && tokens[5] == "1") {
            c->ChangeMode();
        }
        deck.push_back(std::move(c));
    }
    else if (stoi(tokens[0]) == static_cast<int>(MonsterType::Effect)) {
        if (tokens.size() < 9) {
            return false;
        }
        if (stoi(tokens[1]) == static_cast<int>(Effect::Orc)) {
            auto orc = std::make_unique <Orc>(stoi(tokens[2]), stoi(tokens[3]), stoi(tokens[4]), stoi(tokens[5]), stoi(tokens[6]), stoi(tokens[7]), stoi(tokens[8]));
            if (tokens.size() > 9 && tokens[9] == "1") {
                orc->ChangeMode();
            }
            deck.push_back(std::move(orc));
        }
        else if (stoi(tokens[1]) == static_cast<int>(Effect::Aqua)) {
            
            auto aq = std::make_unique<Aqua>(stoi(tokens[2]), stoi(tokens[3]), stoi(tokens[4]), stoi(tokens[5]), stoi(tokens[6]), stoi(tokens[7]), stoi(tokens[8]));
            if (tokens.size() > 9 && tokens[9] == "1") {
                aq->ChangeMode();
            }
            deck.push_back(std::move(aq));
        }
        else if (stoi(tokens[1]) == static_cast<int>(Effect::Abaki)) {

            auto ab = std::make_unique<Abaki>(stoi(tokens[2]), stoi(tokens[3]), stoi(tokens[4]), stoi(tokens[5]), stoi(tokens[6]), stoi(tokens[7]), stoi(tokens[8]));
            if (tokens.size() > 9 && tokens[9] == "1") {
                ab->ChangeMode();
            }
            deck.push_back(std::move(ab));
        }
        else if (stoi(tokens[1]) == static_cast<int>(Effect::Thunder)) {

            auto th = std::make_unique<Thunder>(stoi(tokens[2]), stoi(tokens[3]), stoi(tokens[4]), stoi(tokens[5]), stoi(tokens[6]), stoi(tokens[7]), stoi(tokens[8]));
            if (tokens.size() > 9 && tokens[9] == "1") {
                th->ChangeMode();
            }
            deck.push_back(std::move(th));
        }
        else { return false; }
        

    }
    else if (stoi(tokens[0]) == static_cast<int>(MonsterType::Rare)) {
        auto c = std::make_unique<EffectCard>(tokens[1], stoi(tokens[2]), stoi(tokens[3]), stoi(tokens[4]), stoi(tokens[5]), stoi(tokens[6]));
        if (tokens.size() > 7 && tokens[7] == "1") {
            c->ChangeMode();
        }
        deck.push_back(std::move(c));
    }
    else { return false; }
    return true;
}

// add card to deque
void  Deck::AddCard(std::unique_ptr<Card> card) {
    deck.push_back(std::move(card));
}

// pop a card from deque
std::unique_ptr<Card> Deck::DrawCard() {
    std::unique_ptr<Card> c = std::move(deck.front());
    deck.pop_front();
    return std::move(c);
}

// random engine to shuffle the cards in deque
void Deck::shuffle() {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(deck.begin(), deck.end(), std::default_random_engine((unsigned int)seed));
}

// prints all cards in deque; used to print to file
std::ostream& operator<<(std::ostream& os, const Deck& deck) {
    for (auto &card : deck.deck)
    {
        card->CardSave(os);

    }
    return os;
}

// return number of cards in deque
size_t Deck::size()const { return deck.size(); }

