#ifndef GAME_H

#define GAME_H

class Pit;
class History;

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
//	History* m_history;
};

#endif