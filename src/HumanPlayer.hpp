#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H
#include "Player.hpp"
#include "OverHead.hpp"


class HumanPlayer : public Player {
private:
    Deck deck;
    Deck extradeck;
    List hand;
    List monsterZone;

public:
    HumanPlayer();
    ~HumanPlayer();

    void    NamePrompt      ();
    bool    DrawAndSet      ();
    void    FlipCard        (bool);
    bool    Attack          (int&, int&, const List*);
    bool    ReloadPlayer    (std::fstream&);
    bool    Load            (const char*);
    size_t  SizeOfDeck      ()const;
    size_t  SizeOfExtraDeck ()const;
    void    HandCheck       ();
    void    PayOrThrow      ();
    void    DiscardCard     ();
    size_t  SizeOfZone      ()const;
    void    InitDraw        ();
    void    DisplayHand     ()const;
    bool    BattlePrompt    ()const;
    void    DisplayZone     (bool)const;
    bool    RemoveCard      (unsigned int);
    void    GetZoneCard     (Card&, unsigned int);
    bool    Alive           ()const;
    int     DeckPrompt      ()const;
    bool    Draw            ();
    void    SavePrint       (std::ofstream& os);

    const List* GetZone() const;
};

#endif 
