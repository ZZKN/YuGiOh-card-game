#ifndef DECK_H
#define DECK_H
#include "Card.hpp"

class Deck {
private:
    std::deque<std::unique_ptr<Card>> deck;
public:
    Deck();
    ~Deck();

    bool LoadCards          (std::ifstream&);
    bool IdentifyCard       (std::stringstream&);
    void AddCard            (std::unique_ptr<Card>);
    void shuffle            ();
    
    std::size_t             size        ()const;
    std::unique_ptr<Card>   DrawCard    ();
    friend std::ostream&    operator<<  (std::ostream&,const Deck&);
};

#endif