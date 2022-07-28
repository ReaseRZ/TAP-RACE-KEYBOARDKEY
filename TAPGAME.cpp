#include <iostream>
#include <conio.h>
#include <windows.h>
#include <dos.h>
#include <time.h>
#include <string>
#include <thread>
#include <functional>

#define START_INDEX 0
#define END_TEXT_CONSOLE gotoxy(0,50)
#define CLRSCRN system("cls")
#define RANDOM(MIN,MAX) MIN + (rand() % static_cast<int>(MAX - MIN + 1))

unsigned LENAREA = 150;
const unsigned MINCHARACTER = 97;
const unsigned MAXCHARACTER = 122;

int PlayerButton = 97;

HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

enum class Difficult
{
	EASY,
	NORMAL,
	HARD,
	NONE
};

Difficult CurrentDifficult;

struct Position
{
	int X;
	int Y;
};

class ObjectCharacter
{
private:
	Position CharacterPosition;
	const char& CharacterPlayer;
	const std::string& NameObject;
public:
	ObjectCharacter(const char& Character, const std::string& Name)
		:CharacterPlayer(Character),
		NameObject(Name)
	{
		CharacterPosition.X = 0;
		CharacterPosition.Y = 0;
	}

	~ObjectCharacter() = default;

	void set_position(const int& CoordX, const int& CoordY) 
	{
		CharacterPosition.X = CoordX;
		CharacterPosition.Y = CoordY;
	}

	void set_position(const Position& position)
	{
		CharacterPosition = position;
	}

	const char get_character()const
	{
		return CharacterPlayer;
	}

	std::string get_name()const
	{
		return NameObject;
	}

	Position get_position()const
	{
		return CharacterPosition;
	}
};

void gotoxy(const int& x,const int& y)
{
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(Console, CursorPosition);
}

void set_cursor_info(bool visible, DWORD size)
{
	if (size == 0)
		size = 20;
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(Console, &lpCursor);
}

void draw_map()
{
	const unsigned UPPERLAYERCOORD = 0;
	const unsigned LOWERLAYERCOORD = 10;
	const std::string STARTTEXT = "START";
	const std::string FINISHTEXT = "FINISH";

	//Draw Start Text
	for (size_t i = START_INDEX; i < STARTTEXT.length(); i++)
	{
		gotoxy(1, i + 3); std::cout << STARTTEXT[i];
	}

	for (size_t i = START_INDEX; i < FINISHTEXT.length(); i++)
	{
		gotoxy(LENAREA-1, i + 2); std::cout << FINISHTEXT[i];
	}

	//Upper Layer
	for (size_t i = START_INDEX; i < LENAREA; i++)
	{
		gotoxy(i, UPPERLAYERCOORD); std::cout << "=";
	}
	//Lower Layer
	for (size_t i = START_INDEX; i < LENAREA; i++)
	{
		gotoxy(i, LOWERLAYERCOORD); std::cout << "=";
	}
}

template<class T>
T InputValue(const std::string& text)
{
	T var;
	std::cout << text; std::cin >> var;
	return var;
}

void pick_difficult()
{
	const std::string Description = "Choose a difficult : \n1. Easy\n2. Normal\n3. Hard\n";
	std::cout << Description;
	switch (InputValue<int>(">>>"))
	{
	case 1:
		CurrentDifficult = Difficult::EASY;
		break;
	case 2:
		CurrentDifficult = Difficult::NORMAL;
		break;
	case 3:
		CurrentDifficult = Difficult::HARD;
		break;
	default:
		std::cout << "No Choice\n";
		break;
	}
}

void setting()
{
	const std::string MenuList = "1. Set Length Area\n2. Set Difficult\n";
	std::cout << MenuList;
	switch (InputValue<int>(">>>"))
	{
	case 1:
		LENAREA = InputValue<int>("Enter new area length value : ");
		break;
	case 2:
		std::cout << "Not Available For Now !\n";
		break;
	}
}

void draw_object(const ObjectCharacter& ptr)
{
	gotoxy(ptr.get_position().X, ptr.get_position().Y); std::cout << " ";
	gotoxy(ptr.get_position().X, ptr.get_position().Y); std::cout << ptr.get_character();
}

int event_key()
{
	return _getch();
}

void print_button_random_seed(const std::string& text, const int& CoordX, const int& CoordY, const int& button)
{
	gotoxy(CoordX, CoordY); std::cout << text << static_cast<char>(button);
}

void update_position_object(ObjectCharacter& ptr,int& CodeKeyboard)
{
	Position changer = ptr.get_position();
	if (_kbhit())
	{
		int character = event_key();
		if (character == CodeKeyboard)
		{
			changer.X++;
			CodeKeyboard = RANDOM(MINCHARACTER, MAXCHARACTER);
		}
	}
	ptr.set_position(changer);
}

void update_position_object_computer(std::shared_ptr<ObjectCharacter>& ptr,const bool& gameplay)
{
	while (gameplay)
	{
		Position CurrentPosition = ptr->get_position();
		switch (CurrentDifficult)
		{
		case Difficult::EASY:
			Sleep(RANDOM(250, 500));
			break;
		case Difficult::NORMAL:
			Sleep(RANDOM(150, 250));
			break;
		case Difficult::HARD:
			Sleep(RANDOM(50, 100));
			break;
		default:
			break;
		}
		ptr->set_position(CurrentPosition);
	}
}

void check_winner(const ObjectCharacter& ptr, bool& gameplay)
{
	if (ptr.get_position().X == LENAREA-1)
	{
		gotoxy(0, 15); std::cout << "Complete !!!";
		gameplay = false;
	}
}

void game()
{
	bool gameplay = true;
	clock_t time = clock();
	std::shared_ptr<ObjectCharacter>pPlayer = std::make_shared<ObjectCharacter>('P', "Player");
	pPlayer->set_position(1, 5);
	while (gameplay)
	{
		print_button_random_seed("Pressed Keyboard : ", 0, 13, PlayerButton);
		update_position_object(*pPlayer, PlayerButton);
		draw_object(*pPlayer);
		draw_map();
		check_winner(*pPlayer, gameplay);
		Sleep(25);
		CLRSCRN;
	}
	time = clock() - time;
	gotoxy(0, 12); std::cout << "You completed in in " << static_cast<float>(time) / CLOCKS_PER_SEC << " sec(s)\n";
	system("pause");
	pPlayer.reset();
}

void mainmenu()
{
	const std::string opening = "TAP RACE CONSOLE GAME\n";
	const std::string MenuList =
		"1. Blitz\n2. Setting\n3. EXIT\n";
	std::cout << opening << MenuList;
	switch (InputValue<int>("Input A Value : "))
	{
	case 1:
		game();
		break;
	case 2:
		setting();
		break;
	case 3:
		exit(0);
	default:
		std::cout << "Not Available Feature !!!\n";
		break;
	}
}

void run_game()
{
	while (true)
	{
		mainmenu();
	}
}

int main()
{
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	srand(static_cast<unsigned>(time(NULL)));
	run_game();
	END_TEXT_CONSOLE;
}
