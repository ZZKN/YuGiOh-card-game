
#include "Game.hpp"
#include "Card.hpp"
#include "menu.hpp"


int main() {
    Menu menu;
    int selection = -1;

    while (selection != static_cast<int>(Options::Exit)) {
        selection = menu.PrintOptions();
        switch (selection-1) {
        case static_cast<int>(Options::OnePlayer) :
            menu.NewGame(true);
            break;
        case static_cast<int>(Options::TwoPlayers):
            menu.NewGame(false);
            break;
        case static_cast<int>(Options::LoadGame):
            menu.ReloadGame();
            break;
            case static_cast<int>(Options::Exit) :
                selection = static_cast<int>(Options::Exit);
            break;
        default:
            selection = -1;
            std::cout << "Please select a valid option.\n";
            break;
        }
    }

    std::cout << "Goodbye!!\n";
    return 0;
}
