#ifndef HISTORY_H

#define HISTORY_H
class Player;
class Snake;
class Pit;
#include "globals.h"
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
	int numTimesAtSpot[MAXROWS][MAXCOLS]; //initialized to 0
};

#endif