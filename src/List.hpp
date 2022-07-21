#ifndef LIST_H
#define LIST_H
#include "Card.hpp"


class List {
private:
    std::vector<std::unique_ptr<Card>> list;
    int lastindex;
public:
    List();
    ~List();

    void    AddCard         (std::unique_ptr<Card>);
    void    FlipCard        (const int);
    int     LastAddedIndex  ()const;
    void    ClearLastIndex  ();

    void    GetCard         (Card&, const int);

    int     MinAttack       (bool)const;
    int     MaxAttack       (bool)const;
    int     MinDefence      (bool)const;
    int     MaxDefence      (bool)const;
    bool    OnlyDefence     (void)const;
    bool    OnlyEffectCards ()const;
    int     FirstAvailable  ()const;
    int     NumberOfDefence ()const;
    void    ActivateCard    (const unsigned int *);

    const unsigned int*  GetCost         (unsigned int) const;
    void        DisplayContent  (bool)const;
    void        TerminalDisplay ()const;

    std::unique_ptr<Card> RemoveCard(const int);
    std::vector<std::pair<int, int>> AttackList() const;
    friend std::ostream& operator<<(std::ostream&, const List&);
    size_t size() const;
};

#endif 
