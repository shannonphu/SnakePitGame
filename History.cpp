#include "History.h"
#include "globals.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
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
