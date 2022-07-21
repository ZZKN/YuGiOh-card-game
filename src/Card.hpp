#ifndef CARD_H
#define CARD_H
#include "OverHead.hpp"

class Card {
private:
    MonsterType     type;
    CardMode        mode;
    Elements        element;
    std::string     name;
    int             atk;
    int             def;
public:
    Card();
    Card(const std::string, int, int, int);
    virtual ~Card();


    virtual bool            Activate    (const unsigned int*);
    virtual const unsigned int*      GetCost     () const;
    virtual void            CardSave    (std::ostream&);
    virtual std::ostream&   print       (std::ostream&);

    MonsterType         GetType     ()const;
    CardMode            GetMode     ()const;
    std::string         GetName     ()const;
    int                 GetATK      ()const;
    int                 GetDEF      ()const;
    Elements            GetElem     ()const;
    void                SetType     (MonsterType);
    void                ChangeMode();

    friend std::ostream& operator<<(std::ostream&, Card&);
protected:
    void Boost(int, int);
};

class EffectCard : public Card {
private:
    unsigned int cost[3];
public:
    EffectCard(const std::string&, int, int, int, int, int);

    const unsigned int* GetCost() const;
    virtual bool        Activate(const unsigned int*);
    virtual void        CardSave(std::ostream&);
    virtual std::ostream& print(std::ostream& );
    virtual std::ostream& printDesc(std::ostream&);
    std::ostream& printEffect(std::ostream&, int, int, Effect);
};


class Orc : public EffectCard {
private:
    int valueInc;
    int valueDec;
    Effect effect;
public:
    Orc(int, int, int, int, int, int, int);
    bool Activate(const unsigned int*);
    std::ostream& printDesc(std::ostream&);
    void CardSave(std::ostream&);
};

class Aqua : public EffectCard {
private:
    int valueInc;
    int valueDec;
    Effect effect;
public:
    Aqua(int, int, int, int, int, int, int);
    bool Activate(const unsigned  int*);
    std::ostream& printDesc(std::ostream&);
    void CardSave(std::ostream&);
};

class Abaki : public EffectCard {
private:
    int valueInc;
    int valueDec;
    Effect effect;
public:
    Abaki(int, int, int, int, int, int, int);
    bool Activate(const unsigned int*);
    std::ostream& printDesc(std::ostream&);
    void CardSave(std::ostream&);

};


class Thunder : public EffectCard {
private:
    int valueInc;
    int valueDec;
    Effect effect;
public:
    Thunder(int, int, int, int, int, int, int);
    bool Activate(const unsigned int*);
    std::ostream& printDesc(std::ostream&);
    void CardSave(std::ostream&);
};

#endif 
