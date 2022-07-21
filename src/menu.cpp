
#include "menu.hpp"
// print out ootion of action
int Menu::PrintOptions()const {
	int selection = -1;
	std::cout << "Yugioh Main Menu:\n\n";
	std::cout << "   Options:\n";
	std::cout << "      1. One Player\n";
	std::cout << "      2. Two Players\n";
	std::cout << "      3. Load Game\n";
	std::cout << "      4. Exit\n";
	std::cout << "\nPlease select an option:";

	std::cin >> selection;
	flushCin();
	while (!std::cin.good()) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid input! Select an option: ";
		std::cin >> selection;

	}
	return selection;
}

// load new game
void  Menu::NewGame(bool solo) {
	Game game;

	try {
		if ((solo && !game.InitializeSolo()) || (!solo && !game.Initialize())) {
			std::cout << "\nFailed to load cards. Please exit and correct the \"cards.txt\" file.\n";
			return;

		}
		
	}
	catch (std::exception& err) {
		 std::cout << "Faulty input files: " << err.what() << "\nPlease exit and correct the \"cards.txt\" file." << std::endl;
		 return;
	}

	game.Start();
	if (game.SaveAndEnd()) {
		SaveGame(game);
	}
}

// load saved game
void Menu::ReloadGame() {

	std::fstream savedgames;
	bool solo;
	int curPlayer;
	int gameNum;
	
	Game game;
	
	try {

		savedgames.open(SavedGamesFilePath, std::ios::in);
		if (!savedgames.is_open()) {
			std::cout << "Unable to open file.\n";
			return;

		}
		gameNum = LoadGameFile(savedgames, solo, curPlayer);
		if (gameNum == 0) {
			return;
		}

		if ((solo && !game.ReloadInitializeSolo(savedgames)) || (!solo && !game.ReloadInitialize(savedgames))) {
			std::cout << "Failed to re-load selected game.\n";
			return;
		}

	}
	catch (std::exception& err) {
		std::cout << "Failed to re-load selected game: " << err.what() << std::endl;
		return;
	}

	

	savedgames.close();

	DeleteGame(gameNum);
	DeleteGameTitle(gameNum);

	game.ReStart(curPlayer);

	if (game.SaveAndEnd()) {
		SaveGame(game);
	}
}

// load list of names of saved games
void Menu::LoadListFile() {
	std::fstream gametitles;
	std::string line;

	gametitles.open(SavedTitlesFilePath, std::ios::in);

	if (!gametitles.is_open()) {
		std::cout << "Unable to open file.\n";
		return;

	}

	while (getline(gametitles, line)) {
		Games.push_back(line);
	}

	gametitles.close();

}

// ask for a name for the game about to be saved
std::string Menu::TitlePrompt()const {
	std::string title = "";
	std::cout << "Choose a title for your game: ";
	std::cin >> title;
	int selection = -1;
	while (selection  < 0) {
		if (std::find(Games.begin(), Games.end(), title) != Games.end()) {
			std::cout << "Game with this title already exists. Choose again: ";
			std::cin >> title;
			selection = -1;
			flushCin();
		}
		else {
			selection = 0;
		}
	}
	
	
	return title;

}

// save in-progress game
void Menu::SaveGame(Game& game) {
	std::string title;
	std::ofstream savedgames;
	
	if (Games.size() == 0) {
		LoadListFile();

	}
	title = TitlePrompt();
	savedgames.open(SavedGamesFilePath, std::ios::app);

	savedgames << title << "/" << game.Solo() << "/" << game.GetCurrentPlayer() << "\n";

	game.SavePlayers(savedgames);


	savedgames.close();

	std::fstream gametitles;
	gametitles.open(SavedTitlesFilePath, std::ios::app);

	gametitles << title << "\n";
	gametitles.close();

	std::cout << "Game Saved!\n";
	EnterKey();
}

// find the start of the wanted game in the file
int Menu::LoadGameFile(std::fstream& savedgames, bool& SoloPlay, int& StartPlayer) {
	std::string line;
	std::string token; 
	std::string title;
	bool found = false;

	LoadListFile();

	int gameNum = ChooseGamePrompt();

	if (gameNum <= 0) {
		return 0;
	}
	else if (gameNum > 0) {
		title = Games[gameNum - 1];
	}

	while (!found && getline(savedgames, line)) {
		
		token = line.substr(0, line.find(delimiter));
		if (token.compare(title) == 0) {
			found = true;
		}
	}

	if (!found) {
		std::cout << "No such game found.\n";
		EnterKey();
		return 0;
	}

	line.erase(0, line.find(delimiter) + delimiter.length());
	token = line.substr(0, line.find(delimiter));
	SoloPlay = std::stoi(token);

	line.erase(0, line.find(delimiter) + delimiter.length());
	StartPlayer = std::stoi(line);

	return gameNum;
}

// prints all names of saved games and prompts user to select one or leave
int Menu::ChooseGamePrompt()const {
	int selection = -1;
	size_t size = Games.size();
	for (size_t i = 0; i < size; i++) {
		std::cout << i +1 << ". " << Games[i] << "\n";
	}
	if (Games.size() == 0) {
		std::cout << "\nNo games saved.\n";
		EnterKey();
		return 0;
	}
	std::cout << ++size << ". return to menu\n";

	while (selection <= 0) {
		std::cout << "\nPlease select an option: ";
		std::cin >> selection;
		flushCin();

		if (selection < 1 || selection > (int)size) {
			selection = -1;
			std::cout << "Please enter a valid index.\n";

		}
		else if (selection == (int)size) {
			clrscr();
			return 0;
		}

		
	}
	return selection;


}

// delete title of the game from file
void Menu::DeleteGameTitle(int gamenum)  {
	std::string deleteline = Games[gamenum - 1];
	std::string line;
	std::string token;

	std::ifstream savedtitles;
	savedtitles.open(SavedTitlesFilePath);

	std::ofstream temp;
	temp.open(tmpFilePath);
	while (std::getline(savedtitles, line)) {

		if (line.substr(0, line.find(delimiter)).compare(deleteline) == 0 && gamenum == (int)Games.size()) {
			temp << "";
			break;
		}
		else if (line.substr(0, line.find(delimiter)).compare(deleteline) == 0 && gamenum < (int)Games.size()) {
			getline(savedtitles, line);
			
			temp << "";
			temp << line << std::endl;
		}
		else {

			temp << line << std::endl;
		}

	}

	Games.erase(Games.begin() + gamenum - 1);

	
	temp.close();
	savedtitles.close();
	remove(SavedTitlesFilePath);
	int result = rename(tmpFilePath, SavedTitlesFilePath);
	if (result != 0) {
		std::cout << "Failed to save to file.";
	}
		

}

// delete content of the game from file
void Menu::DeleteGame(int gamenum)  {
	

	std::string deleteline = Games[gamenum - 1];
	std::string line;
	std::string token;
	int size = (int)Games.size();
	std::ifstream savedgames;
	savedgames.open(SavedGamesFilePath);

	std::ofstream temp;
	temp.open(tmpFilePath);
	while (std::getline(savedgames, line)) {
		//copy until game title then skip over the game or end if its the last one in the file/vector of titles
		
		if (line.substr(0, line.find(delimiter)).compare(deleteline) == 0 && gamenum == size) {
			temp << "";
			break;
		}
		else if (line.substr(0, line.find(delimiter)).compare(deleteline) == 0 && gamenum < size) {
			getline(savedgames, line);
			while (line.substr(0, line.find(delimiter)).compare(Games[gamenum]) != 0) {
				getline(savedgames, line);

			}
			temp << "";
			temp << line << std::endl;
		}
		else {

			temp << line << std::endl;
		}
		
	}

	
	
	temp.close();
	savedgames.close();
	remove(SavedGamesFilePath); 
	errno_t err;
	_get_errno(&err);
	printf("errno = %d\n", err);
	printf("fyi, ENOENT = %d\n", ENOENT);
	int result = rename(tmpFilePath, SavedGamesFilePath);
	if (result == 0) {
		puts("File successfully renamed");
	}
	else {
		puts("File rename unsuccessful");
	}
	_get_errno(&err);
	printf("errno = %d\n", err);
	printf("fyi, ENOENT = %d\n", ENOENT);

	
}
