#ifndef PLAYER_H
#define PLAYER_H
#include "OverHead.hpp"
#include "Deck.hpp"
#include "List.hpp"

class Player {
private:
    Deck graveyard;
    unsigned int lifepoints;
    std::string name;
    unsigned int elements[ELEMENTS_COUNT] = { 0 };  // {Earth, Fire, Water}
public:

    Player();
    virtual ~Player();

    virtual bool DrawAndSet         ();
    virtual void FlipCard           (bool);
    virtual bool Attack             (int&, int&, const List*);

    virtual bool PlacementModePrompt()const;
    virtual void InitDraw           ();
    virtual void DiscardCard        ();
    virtual bool ReloadPlayer       (std::fstream&);
    virtual void HandCheck          ();

    virtual const   List* GetZone   () const;
    virtual void    DisplayHand     ()const;
    virtual void    DisplayZone     (bool)const;
    virtual bool    Load            (const char*);
    virtual size_t  SizeOfDeck      ()const;
    virtual size_t  SizeOfExtraDeck ()const;
    virtual size_t  SizeOfZone      ()const;

    virtual bool RemoveCard         (unsigned int);
    virtual void GetZoneCard        (Card&, unsigned int);
    virtual bool Alive              ()const;
    virtual void SavePrint          (std::ofstream& os);

    bool        DecElement          (unsigned int, unsigned int);
    bool        CheckElements       ()const;
    void        AddElement          (unsigned int ind);
    unsigned int GetElement          (unsigned int ind)const;
    void        SubtractCost        (const unsigned int* a);
    bool        CheckElemLimit      ()const;
    void        PayForRareCard      ();
    const unsigned int*  GetAllElem ();
    bool        CheckCost           (int, const List&);
    void        Display             (const List&)const;
    std::string GetName             ()const;
    int         GetLifepoints       ()const;
    bool        LoadDeck            (Deck&, Deck&, const char*);
    void        AddHealth           (unsigned int);
    void        TakeDamage          (unsigned int);
    void        ChangeName          (std::string);
    bool        HandSizeCheck       (List&);
    bool        Reload              (std::fstream&, Deck& deck);
    void        ReloadZone          (int, List&, List&, Deck&);
    void        ReloadHand          (int, List& , Deck&);
    void        FirstDraw           (List&, Deck&);
    bool        DrawToHand          (List&, Deck&);
    bool        MoveToZone          (unsigned int, List&, List&);
    bool        MoveToGraveyard     (unsigned int, List&);
    void        FilePrint           (std::ofstream& os, const List&, const List&, const Deck&, const Deck&);
    friend std::ostream&    operator<< (std::ofstream& os, Player& player);
};

#endif
