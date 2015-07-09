#ifndef PLAYER_H

#define PLAYER_H

class Pit;
class History;

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

#endif