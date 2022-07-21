#ifndef MENU_H
#define MENU_H


#include "Overhead.hpp"
#include "Game.hpp"

class Menu {
private:
	std::vector<std::string>	Games;
	const std::string			delimiter = "/";
	const char *				SavedGamesFilePath = "../examples/savedgames.txt";
	const char*					SavedTitlesFilePath = "../examples/savedgamestitles.txt";
	const char*					tmpFilePath = "../examples/tmp.txt";

public:
	inline Menu() {}
	inline ~Menu() {}

	int		PrintOptions		()const;
	void	NewGame				(bool);
	void	ReloadGame			();
	void	DeleteGame			(int);
	void	DeleteGameTitle		(int);
	void	LoadListFile		();
	void	SaveGame			(Game&);
	int		LoadGameFile		(std::fstream& ,bool&, int&);
	int		ChooseGamePrompt	()const;
	std::string TitlePrompt		()const;
};

#endif 