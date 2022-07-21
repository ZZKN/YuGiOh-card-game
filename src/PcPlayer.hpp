#ifndef PCPLAYER_H
#define PCPLAYER_H
#include "Player.hpp"

class PcPlayer : public Player {
private:
    Deck deck;
    Deck extradeck;
    List hand;
    List monsterZone;
    int elements[ELEMENTS_COUNT] = { 0 };  // {Earth, Fire, Water}
    std::string  computerNames[3] = { "Anna Bortion", "Freida Slaves", "Anne Fetamine" };
public:
    PcPlayer();

    void NamePrompt();
    bool DrawAndSet();
    bool Attack(int&, int&, const List*);

    void HandCheck();
    void DisplayZone(bool)const;
    void DisplayHand()const;
    bool Load(const char*);
    size_t SizeOfDeck()const;
    size_t SizeOfExtraDeck()const;
    size_t SizeOfZone()const;
    void InitDraw();
    bool ReloadPlayer(std::fstream&);
    void GetZoneCard(Card&, unsigned int);
    bool RemoveCard(unsigned int);
    bool Alive()const;
    void DiscardCard();
    void HandChecknChuck();
    const List* GetZone() const;
    void SavePrint(std::ofstream& os);
};

#endif 
