#ifndef GAME_H
#define GAME_H
#include "OverHead.hpp"
#include "Player.hpp"
#include "HumanPlayer.hpp"
#include "PcPlayer.hpp"
#include "Card.hpp"


class Game {
private:
    std::vector<Player*> player;
    const char* CardsFilePath = "../assets/cards.txt";
    int         currentplayer = 0;
    int         round = 0;
    bool        quit = false;
public:
    Game();
    ~Game();

    void Start          ();
    void ReStart        (int);
    void GamePlay       ();
    void Battle         ();

    bool InitializeSolo ();
    bool Initialize     ();

    bool ReloadInitializeSolo   (std::fstream&);
    bool ReloadInitialize       (std::fstream&);

    void CoinFlip           ();
    void SwapPlayers        ();
    void DisplayBoard       (bool)const;
    void PrintHeader        () const;

    int  CheckWinner        ()const;
    int  GetCurrentPlayer   ()const;
    bool Solo               ()const;

    bool SaveAndEnd         ()const;
    bool SavePrompt         ()const;
    void SavePlayers        (std::ofstream&);
};
#endif 