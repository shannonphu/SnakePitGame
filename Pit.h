#ifndef PIT_H

#define PIT_H

class Player;
class Snake;
#include <string>
#include "globals.h"
#include "History.h"

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

#endif