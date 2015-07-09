// SHANNON PHU
// SnakePitGame C++ PROJECT
// Game Description:
//		You are the player (represented by '@' symbol) who is stuck in a pit of snakes
//		(represented by 'S' or a number signifying how many snakes are at that spot)! 
//		You must try to kill the randomly moving snakes by jumping over them when they 
//		are next to you, the player. You navigate the playing field by pressing 'u'(up), 
//		'd'(down), 'l'(left), or 'r'(right) to move the player around. You can simply 
//		press enter to stand in place and not move. To see how many snakes you have killed 
//		in what locations press 'h' for history. Happy gaming!

#include <iostream>
#include <string>
using namespace std;

/////////////////////////////////////////////////
// GLOBAL VARIABLES
/////////////////////////////////////////////////
const int MAXROWS = 20;             // max number of rows in the pit
const int MAXCOLS = 40;             // max number of columns in the pit
const int MAXSNAKES = 180;          // max number of snakes allowed

const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS DECLARATIONS

///////////////////////////////////////////////////
// AUXILIARY FUNCTION DECLARATION
//	- decodeDirection: translates user input into player's direction
//	- directionToDeltas: returns change in position depending on direction going
//	- clear screen: clears the screen to prepare for next screen display
///////////////////////////////////////////////////
int decodeDirection(char dir);
bool directionToDeltas(int dir, int& rowDelta, int& colDelta);
void clearScreen();

///////////////////////////////////////////////////
// GAME HISTORY DECLARATION
//	- record: records where player has killed snakes and how many at that location
//	- display: displays the screen showing history of snake kills
///////////////////////////////////////////////////
class Pit;
class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
private:
	Pit* m_pit;
	int m_rowsHistory;
	int m_colsHistory;
	int numTimesAtSpot[MAXROWS][MAXCOLS];
};

//////////////////////////////////////////////////
// GAME PIT DECLARATION
//	- location of playing ground for all game actors
//	- displays all game actors at their stored location each player's turn
//	- manages the creation and destruction of snakes and player
//	- manages the random movement of all the snakes on the screen
//////////////////////////////////////////////////
class Player;
class Snake;
class Pit
{
public:
	// Constructor/destructor
	Pit(int nRows, int nCols);
	~Pit();

	// Accessors
	int     rows() const;
	int     cols() const;
	Player* player() const;
	History& history();
	int     snakeCount() const;
	int     numberOfSnakesAt(int r, int c) const;
	void    display(std::string msg) const;

	// Mutators
	bool   addSnake(int r, int c);
	bool   addPlayer(int r, int c);
	bool   destroyOneSnake(int r, int c);
	bool   moveSnakes();

private:
	int     m_rows;
	int     m_cols;
	Player* m_player;
	Snake*  m_snakes[MAXSNAKES];
	int     m_nSnakes;
	History m_history;
};

/////////////////////////////////////////////////
// PLAYER DECLARATION
//	- deals with the user input and appropriately changes stored location in data to "move"
//	- recognizes when player has been attacked by a snake and appropriately "dies"
//	- knows which Pit it is in and what row/column it is in
/////////////////////////////////////////////////
class Player
{
public:
	// Constructor
	Player(Pit *pp, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	int  age() const;
	bool isDead() const;

	// Mutators
	void   stand();
	void   move(int dir);
	void   setDead();

private:
	Pit*  m_pit;
	History* m_history;
	int   m_row;
	int   m_col;
	int   m_age;
	bool  m_dead;
};

/////////////////////////////////////////////////
// SNAKE DECLARATION
//	- deals with random movement of that particular instance of snake
//	- knows which Pit it is in and what row/column it is in
/////////////////////////////////////////////////
class Snake
{
public:
	// Constructor
	Snake(Pit* pp, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;

	// Mutators
	void move();

private:
	Pit* m_pit;
	int  m_row;
	int  m_col;
};

//////////////////////////////////////////////////
// GAME DECLARATION
//	- manages the playing of the game by creating a Pit and adding snakes and a player in
//	- accepts user input to run the game and player's movements
//////////////////////////////////////////////////
class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nSnakes);
	~Game();

	// Mutators
	void play();

private:
	Pit* m_pit;
};

// END CLASS DECLARATIONS
///////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS DEFINITITONS

///////////////////////////////////////////////////
// AUXILIARY FUNCTION DEFINITION
///////////////////////////////////////////////////
int decodeDirection(char dir)
{
	switch (dir)
	{
	case 'u':  return UP;
	case 'd':  return DOWN;
	case 'l':  return LEFT;
	case 'r':  return RIGHT;
	}
	return -1;  // bad argument passed in!
}

bool directionToDeltas(int dir, int& rowDelta, int& colDelta)
{
	switch (dir)
	{
	case UP:     rowDelta = -1; colDelta = 0; break;
	case DOWN:   rowDelta = 1; colDelta = 0; break;
	case LEFT:   rowDelta = 0; colDelta = -1; break;
	case RIGHT:  rowDelta = 0; colDelta = 1; break;
	default:     return false;
	}
	return true;
}

#ifdef _MSC_VER  //  Microsoft Visual C++

#include <windows.h>

void clearScreen()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD upperLeft = { 0, 0 };
	DWORD dwCharsWritten;
	FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
		&dwCharsWritten);
	SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <cstring>

void clearScreen()  // will just write a newline in an Xcode output window
{
	static const char* term = getenv("TERM");
	if (term == nullptr || strcmp(term, "dumb") == 0)
		cout << endl;
	else
	{
		static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
		cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
	}
}

#endif

///////////////////////////////////////////////////
// GAME HISTORY DEFINITION
///////////////////////////////////////////////////
History::History(int nRows, int nCols)
{
	// Initialize each position's counter of snakes killed
	// at that spot to be 0.
	m_rowsHistory = nRows;
	m_colsHistory = nCols;
	for (int r = 0; r < nRows; r++)
	{
		for (int c = 0; c < nCols; c++)
			numTimesAtSpot[r][c] = 0;
	}
}

bool History::record(int r, int c)
{
	if (r > m_rowsHistory || r < 1 || c > m_colsHistory || c < 1)
		return false;
	(numTimesAtSpot[r][c])++;
	return true;
}

void History::display() const
{
	char historyGrid[MAXROWS][MAXCOLS];
	int r, c;

	// For each spot in the history grid, replace the '.' with appropriate
	// letter representing how many times a snake was killed at that row and column
	for (r = 1; r <= m_rowsHistory; r++)
		for (c = 1; c <= m_colsHistory; c++)
		{
		if (numTimesAtSpot[r][c] == 0)
			historyGrid[r][c] = '.';
		else if (numTimesAtSpot[r][c] > 0 && numTimesAtSpot[r][c] < 26)
			historyGrid[r][c] = 'A' + (numTimesAtSpot[r][c] - 1);
		else
			historyGrid[r][c] = 'Z';
		}

	clearScreen();

	// Draw the grid
	for (r = 1; r <= m_rowsHistory; r++)
	{
		for (c = 1; c <= m_colsHistory; c++)
			cout << historyGrid[r][c];
		cout << endl;
	}
	cout << endl;
}

//////////////////////////////////////////////////
// GAME PIT DEFINITION
//////////////////////////////////////////////////
Pit::Pit(int nRows, int nCols)
	: m_history(nRows, nCols)
{
	if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
	{
		cout << "***** Pit created with invalid size " << nRows << " by "
			<< nCols << "!" << endl;
		exit(1);
	}
	m_rows = nRows;
	m_cols = nCols;
	m_player = nullptr;
	m_nSnakes = 0;
}

Pit::~Pit()
{
	for (int k = 0; k < m_nSnakes; k++)
		delete m_snakes[k];
	delete m_player;
}

int Pit::rows() const
{
	return m_rows;
}

int Pit::cols() const
{
	return m_cols;
}

Player* Pit::player() const
{
	return m_player;
}

int Pit::snakeCount() const
{
	return m_nSnakes;
}

History& Pit::history()
{
	return m_history;
}

int Pit::numberOfSnakesAt(int r, int c) const
{
	int count = 0;
	for (int k = 0; k < m_nSnakes; k++)
	{
		const Snake* sp = m_snakes[k];
		if (sp->row() == r  &&  sp->col() == c)
			count++;
	}
	return count;
}

void Pit::display(string msg) const
{
	// Position (row,col) in the pit coordinate system is represented in
	// the array element grid[row-1][col-1]
	char grid[MAXROWS][MAXCOLS];
	int r, c;

	// Fill the grid with dots
	for (r = 0; r < rows(); r++)
		for (c = 0; c < cols(); c++)
			grid[r][c] = '.';

	// Indicate each snake's position
	for (int k = 0; k < m_nSnakes; k++)
	{
		const Snake* sp = m_snakes[k];
		char& gridChar = grid[sp->row() - 1][sp->col() - 1];
		switch (gridChar)
		{
		case '.':  gridChar = 'S'; break;
		case 'S':  gridChar = '2'; break;
		case '9':  break;
		default:   gridChar++; break;  // '2' through '8'
		}
	}

	// Indicate player's position
	if (m_player != nullptr)
	{
		char& gridChar = grid[m_player->row() - 1][m_player->col() - 1];
		if (m_player->isDead())
			gridChar = '*';
		else
			gridChar = '@';
	}

	// Draw the grid
	clearScreen();
	for (r = 0; r < rows(); r++)
	{
		for (c = 0; c < cols(); c++)
			cout << grid[r][c];
		cout << endl;
	}
	cout << endl;

	// Write message, snake, and player info
	cout << endl;
	if (msg != "")
		cout << msg << endl;
	cout << "There are " << snakeCount() << " snakes remaining." << endl;
	if (m_player == nullptr)
		cout << "There is no player." << endl;
	else
	{
		if (m_player->age() > 0)
			cout << "The player has lasted " << m_player->age() << " steps." << endl;
		if (m_player->isDead())
			cout << "The player is dead." << endl;
	}
}

bool Pit::addSnake(int r, int c)
{
	// Dynamically allocate a new Snake and add it to the pit
	if (m_nSnakes == MAXSNAKES)
		return false;
	m_snakes[m_nSnakes] = new Snake(this, r, c);
	m_nSnakes++;
	return true;
}

bool Pit::addPlayer(int r, int c)
{
	// Don't add a player if one already exists
	if (m_player != nullptr)
		return false;

	// Dynamically allocate a new Player and add it to the pit
	m_player = new Player(this, r, c);
	return true;
}

bool Pit::destroyOneSnake(int r, int c)
{
	// If snake is at the passed in row and column, destroy that snake
	for (int k = 0; k < m_nSnakes; k++)
	{
		if (m_snakes[k]->row() == r  &&  m_snakes[k]->col() == c)
		{
			delete m_snakes[k];
			m_snakes[k] = m_snakes[m_nSnakes - 1];
			m_nSnakes--;
			return true;
		}
	}
	return false;
}

bool Pit::moveSnakes()
{
	for (int k = 0; k < m_nSnakes; k++)
	{
		Snake* sp = m_snakes[k];
		sp->move();
		if (sp->row() == m_player->row() && sp->col() == m_player->col())
			m_player->setDead();
	}

	// return true if the player is still alive, false otherwise
	return !m_player->isDead();
}

/////////////////////////////////////////////////
// PLAYER DEFINITION
/////////////////////////////////////////////////
Player::Player(Pit* pp, int r, int c)
{
	if (pp == nullptr)
	{
		cout << "***** The player must be in some Pit!" << endl;
		exit(1);
	}
	if (r < 1 || r > pp->rows() || c < 1 || c > pp->cols())
	{
		cout << "**** Player created with invalid coordinates (" << r
			<< "," << c << ")!" << endl;
		exit(1);
	}
	m_pit = pp;
	m_row = r;
	m_col = c;
	m_age = 0;
	m_dead = false;
	m_history = &m_pit->history();
}

int Player::row() const
{
	return m_row;
}

int Player::col() const
{
	return m_col;
}

int Player::age() const
{
	return m_age;
}

void Player::stand()
{
	m_age++;
}

void Player::move(int dir)
{
	m_age++;
	int maxCanMove = 0;  // maximum distance player can move in direction dir
	switch (dir)
	{
	case UP:     maxCanMove = m_row - 1;             break;
	case DOWN:   maxCanMove = m_pit->rows() - m_row; break;
	case LEFT:   maxCanMove = m_col - 1;             break;
	case RIGHT:  maxCanMove = m_pit->cols() - m_col; break;
	}
	if (maxCanMove == 0)  // against wall
		return;
	int rowDelta;
	int colDelta;
	if (!directionToDeltas(dir, rowDelta, colDelta))
		return;

	// No adjacent snake in direction of movement
	if (m_pit->numberOfSnakesAt(m_row + rowDelta, m_col + colDelta) == 0)
	{
		m_row += rowDelta;
		m_col += colDelta;
		return;
	}

	// Adjacent snake in direction of movement, so jump
	if (maxCanMove >= 2)  // need a place to land
	{
		m_pit->destroyOneSnake(m_row + rowDelta, m_col + colDelta);
		m_row += 2 * rowDelta;
		m_col += 2 * colDelta;
		if (m_pit->numberOfSnakesAt(m_row, m_col) > 0)  // landed on a snake
			setDead();
		else
			m_history->record(m_row, m_col);
	}
}

bool Player::isDead() const
{
	return m_dead;
}

void Player::setDead()
{
	m_dead = true;
}

/////////////////////////////////////////////////
// SNAKE DEFINITION
/////////////////////////////////////////////////
Snake::Snake(Pit* pp, int r, int c)
{
	if (pp == nullptr)
	{
		cout << "***** A snake must be in some Pit!" << endl;
		exit(1);
	}
	if (r < 1 || r > pp->rows() || c < 1 || c > pp->cols())
	{
		cout << "***** Snake created with invalid coordinates (" << r << ","
			<< c << ")!" << endl;
		exit(1);
	}
	m_pit = pp;
	m_row = r;
	m_col = c;
}

int Snake::row() const
{
	return m_row;
}

int Snake::col() const
{
	return m_col;
}

void Snake::move()
{
	// Attempt to move in a random direction but if can't move, don't move.
	switch (rand() % 4)
	{
	case UP:     if (m_row > 1)             m_row--; break;
	case DOWN:   if (m_row < m_pit->rows()) m_row++; break;
	case LEFT:   if (m_col > 1)             m_col--; break;
	case RIGHT:  if (m_col < m_pit->cols()) m_col++; break;
	}
}

//////////////////////////////////////////////////
// GAME DEFINITION
//////////////////////////////////////////////////
Game::Game(int rows, int cols, int nSnakes)
{
	if (nSnakes < 0)
	{
		cout << "***** Cannot create Game with negative number of snakes!" << endl;
		exit(1);
	}
	if (nSnakes > MAXSNAKES)
	{
		cout << "***** Cannot create Game with " << nSnakes
			<< " snakes; only " << MAXSNAKES << " are allowed!" << endl;
		exit(1);
	}
	if (rows == 1 && cols == 1 && nSnakes > 0)
	{
		cout << "***** Cannot create Game with nowhere to place the snakes!" << endl;
		exit(1);
	}

	// Create pit
	m_pit = new Pit(rows, cols);

	// Add player
	int rPlayer = 1 + rand() % rows;
	int cPlayer = 1 + rand() % cols;
	m_pit->addPlayer(rPlayer, cPlayer);

	// Populate with snakes
	while (nSnakes > 0)
	{
		int r = 1 + rand() % rows;
		int c = 1 + rand() % cols;
		// Don't put a snake where the player is
		if (r == rPlayer  &&  c == cPlayer)
			continue;
		m_pit->addSnake(r, c);
		nSnakes--;
	}
	//m_history = &m_pit->history();
}

Game::~Game()
{
	delete m_pit;
}

void Game::play()
{
	Player* p = m_pit->player();
	if (p == nullptr)
	{
		m_pit->display("");
		return;
	}
	string msg = "";
	while (!m_pit->player()->isDead() && m_pit->snakeCount() > 0)
	{
		m_pit->display(msg);
		msg = "";
		cout << endl;
		cout << "Move (u/d/l/r//h/q): ";
		string action;
		getline(cin, action);
		if (action.size() == 0)
			p->stand();
		else
		{
			switch (action[0])
			{
			default:   // if bad move nothing should move
				cout << '\a' << endl;  
				continue;
			case 'q':
				return;
			case 'u':
			case 'd':
			case 'l':
			case 'r':
				p->move(decodeDirection(action[0]));
				break;
			case 'h':
				m_pit->history().display();
				cout << "Press enter to continue.";
				cin.ignore(10000, '\n');
				break;
			}
		}
		m_pit->moveSnakes();
	}
	m_pit->display(msg);
}

// END CLASS DEFINITIONS
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include <ctime>
#include <cstdlib>
int main()
{
	// Introduction
	cout << "You are the player (represented by '@' symbol) who is stuck in a pit of snakes (represented by 'S' or a number signifying how many snakes are at that spot)! You must try to kill the randomly moving snakes by jumping over them when they are next to you, the player. You navigate the playing field by pressing 'u'(up), 'd'(down), 'l'(left), or 'r'(right) to move the player around. You can simply press enter to stand in place and not move. To see how many snakes you have killed in what locations press 'h' for history. Happy gaming!" << endl;
	cout << endl;
	cout << "Press enter to continue or 'q' to quit now or at any point in the game..." << endl;

	string userPlaying;
	getline(cin, userPlaying);
	if (userPlaying == "q")
		exit(0);

	// User wants to begin a game.

	// Initialize the random number generator.
	srand(static_cast<unsigned int>(time(0)));

	// Create a game with desired height, width, and number of snakes in pit.
	Game g(12, 18, 28);

	// Play the game
	g.play();
}