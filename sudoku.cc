/*
 * Sudoku 9x9 Implementation
 *
 * Author: Shoufu Luo
 * Date: Apr. 1, 2015
 */
#include <queue>
#include <cmath>
#include "sudoku.h"
#include "assert.h"

/* set inital state of this game */
void Sudoku::setInitialBoard(const int a[], int c)
{
	m_initState = StatePtr(new Board(a, c));
}

StatePtr Sudoku::nextState(const State& state, const Action& action) const
{
	const Move& m = static_cast<const Move&>(action);
	const Board& b = static_cast<const Board&>(state);
	
	if (m.m_sym == 0) // dummy action: do nothing
		return StatePtr();

	std::shared_ptr<Board> nb(new Board(b));
	int boxsz = sqrt(nb->m_size);

	try {
		if (nb->m_board[m.m_row][m.m_col] != 0)
		{
			return StatePtr(); // occupied, failed
		}

		// check row
		for (int j = 0; j < nb->m_size; j++)
		{
			if (nb->m_board[m.m_row][j] == m.m_sym)
				return StatePtr();
		}
		// check column
		for (int i = 0; i < nb->m_size; i++)
		{
			if (nb->m_board[i][m.m_col] == m.m_sym)
				return StatePtr();
		}

		// check box
		int r_base = m.m_row / boxsz;
		int c_base = m.m_col / boxsz;

		for (int i = r_base * boxsz; i < r_base * boxsz + boxsz; i++)
		{
			for (int j = c_base * boxsz; j < c_base * boxsz + boxsz; j++) 
			{
				if (nb->m_board[i][j] == m.m_sym)
					return StatePtr();
			}
		}

		// Not conflicted	
		nb->m_stat[m.m_row][m.m_sym - 1] = 1;	
		nb->m_stat[nb->m_size + m.m_col][m.m_sym - 1] = 1;	
		nb->m_stat[nb->m_size * 2 + BOXID(m.m_row, m.m_col, boxsz)][m.m_sym - 1] = 1;	
		nb->m_board[m.m_row][m.m_col] = m.m_sym;	
		
	} catch (const std::exception e) { // FIXME: OutOfIndex
		std::cout << e.what() << std::endl;
		return StatePtr();
	}

/* Shall we check whether the new state could possibly lead to a solution ?? 
 * forward-checking (?): check left legal values of empty cells in row, col and box 
 * (if ONLY one choice, fill it automatically. do checking recursively)
 */
//#ifndef FORWARD_CHECKING_ONESTEP
//#define FORWARD_CHECKING_ONESTEP
//#define FORWARD_CHECKING_REC
//#endif

#ifdef	FORWARD_CHECKING
#define	FORWARD_CHECKING_REC 
	std::queue<std::pair<int, int> > shotq;
	shotq.push(std::pair<int, int>(m.m_row, m.m_col));
	while (!shotq.empty())
	{
		std::pair<int, int> shot = shotq.front();
		shotq.pop();

		int row = shot.first;
		int col = shot.second;

		for (int j = 0; j < nb->m_size; j++)
		{
			if (nb->m_board[row][j] == 0) // empty
			{
				int counter = 0;
				int choice = 0; // last choice
				for (int k = 0; k < nb->m_size; k++)
				{
					if (nb->m_stat[row][k] == 0 && 
						nb->m_stat[nb->m_size+j][k] == 0 && 
						nb->m_stat[nb->m_size * 2+BOXID(row, j, boxsz)][k] == 0 )
					{
						counter++;
						choice = k;
					}
				}
				if (counter == 0) // no choice
					return StatePtr();
#ifdef	FORWARD_CHECKING_REC 
				if (counter == 1) // constraint propogation
				{
					int k = choice;
					std::cout << "Row: only " << k+1 << " at (" << row << ", "<< j << ")" << std::endl;
					nb->m_stat[row][k] = 1;	
					nb->m_stat[nb->m_size + j][k] = 1;	
					nb->m_stat[nb->m_size * 2 + BOXID(row, j, boxsz)][k] = 1;	
					nb->m_board[row][j] = k + 1;
					shotq.push(std::pair<int, int>(row, j));
				}
#endif
			}
		}

		// check empty cell in the col
		for (int i = 0; i < nb->m_size; i++)
		{
			if (nb->m_board[i][col] == 0) // empty
			{
				int counter = 0;
				int choice = 0;
				for (int k = 0; k < nb->m_size; k++)
				{
					if (nb->m_stat[i][k] == 0 && 
						nb->m_stat[nb->m_size+col][k] == 0 && 
						nb->m_stat[nb->m_size * 2 +BOXID(i, col, boxsz)][k] == 0 )
					{
						counter++;
						choice = k;
					}
				}
				if (counter == 0) // no choice
					return StatePtr();
#ifdef	FORWARD_CHECKING_REC
				if (counter == 1)
				{
					int k = choice;
					//std::cout << "Col: only " << k+1 << " at (" << i << ", "<< col << std::endl;
					nb->m_stat[i][k] = 1;	
					nb->m_stat[nb->m_size + col][k] = 1;	
					nb->m_stat[nb->m_size * 2 + BOXID(i, col, boxsz)][k] = 1;	
					nb->m_board[i][col] = k + 1;
					shotq.push(std::pair<int, int>(i, col));
				}
#endif
			}
		}

		// check empty cell in the box
		int r_base = row / boxsz;
		int c_base = col / boxsz;
		for (int i = r_base * boxsz; i < r_base * boxsz + boxsz; i++)
		{
			for (int j = c_base * boxsz; j < c_base * boxsz + boxsz; j++) 
			{
				if (nb->m_board[i][j] == 0) // empty
				{
					int counter = 0;
					int choice = 0;
					for (int k = 0; k < nb->m_size; k++)
					{
						if (nb->m_stat[i][k] == 0 && 
							nb->m_stat[nb->m_size+j][k] == 0 && 
							nb->m_stat[nb->m_size*2+BOXID(i,j, boxsz)][k] == 0 )
						{
							counter++;
							choice = k;
						}
					}
					if (counter == 0) // no choice
						return StatePtr();

#ifdef	FORWARD_CHECKING_REC
					if (counter == 1)
					{
						int k = choice;
						//std::cout << "Box: only " << k + 1<< " at (" << i << ", "<< j << std::endl;
						nb->m_stat[i][k] = 1;	
						nb->m_stat[nb->m_size + j][k] = 1;	
						nb->m_stat[nb->m_size * 2 + BOXID(i, j, boxsz)][k] = 1;	
						nb->m_board[i][j] = k + 1;
						shotq.push(std::pair<int, int>(i, j));
					}
#endif
				}
			}
		}
	}
#endif

	return nb;
}

ActionPtr	Sudoku::getNullAction() const
{
	return ActionPtr(new Move(0,0,0));
}

int	Sudoku::distanceToGoal(const State& state) const 
{
	return 999;
}

/*
 * goal test: each number should be taken for each row, column and box
 */
bool Sudoku::isGoalState(const State& state) const
{
	const Board& bs = static_cast<const Board&>(state);

	for (int i = 0; i < bs.m_size * 3; i++)
	{
		for (int j = 0; j < bs.m_size; j++)
		{
			if (bs.m_stat[i][j] != 1)
			{
				return false;
			}
		}
	}
	return true;
}

/* find empty cell and iterate all possible legal values (action) for that cell
 * by checking the conflictio in the row, column and box where the cell belongs to
 * Output: a smart pointer to the action, which is applicable, otherwise a empty pointer
 */
ActionPtr Sudoku::actionIterBegin(const State& state) const
{
	const Board& b = static_cast<const Board&>(state);

	int row = 0;
	int col = 0;
	int boxsz = sqrt(b.m_size);
	int ncell = pow(b.m_size, 2); // the number of cells in the board

	int i = 0;
	for (; i < ncell; i++)
	{
		row = i / b.m_size; 
		col = i % b.m_size;
		if (b.m_board[row][col] == 0) // frist empty cell
			break;
	}
	if (i == ncell)
		return ActionPtr();
	
	int k = 0;
	for (; k < b.m_size; k++)
	{
		if (b.m_stat[row][k] == 0 && b.m_stat[b.m_size + col][k] == 0 &&
			b.m_stat[b.m_size * 2 + BOXID(row, col, boxsz)][k] == 0)
			break;
	}
	if (k == b.m_size) // no feasible action
		return ActionPtr();

	return ActionPtr(new Move(row, col, k+1));  
}

ActionPtr Sudoku::actionIterNext(const State& state, const Action& action) const
{
	const Board& b = static_cast<const Board&>(state);
	const Move& m = static_cast<const Move&>(action);

	int boxsz = sqrt(b.m_size);
	int ncell = pow(b.m_size, 2);

	if (m.m_sym < b.m_size) // still have more option, e.g. m.m_sym = 7, we have 8 or 9 in 9x9 
	{
		// next symbol, but index for m_sym is m_sym - 1
		// k is the index (1 less than m_sym)
		for (int k = m.m_sym; k < b.m_size; k++)
		{
			if (b.m_stat[m.m_row][k] == 0 && b.m_stat[b.m_size + m.m_col][k] == 0 &&
				b.m_stat[b.m_size * 2 + BOXID(m.m_row, m.m_col, boxsz)][k] == 0)
			{
				return ActionPtr(new Move(m.m_row, m.m_col, k+1));  
			}
		}
	} 

	// move to next cell 
	// 1. end of the state - no more 
	if (m.m_row == b.m_size && m.m_col == b.m_size) 
		return ActionPtr();

	int idx = m.m_row * b.m_size + m.m_col + 1; // next
	int row = 0;
	int col = 0;
	int i = idx;
	for (; i < ncell; i++)
	{
		row = i / b.m_size;
		col = i % b.m_size;
		if (b.m_board[row][col] == 0)
			break;
	}
	if (i == ncell) // no empty cell
		return ActionPtr();
	
	// 2. move to the new cell and return the first action 
	int k = 0;
	for (; k < b.m_size; k++)
	{
		if (b.m_stat[row][k] == 0 && b.m_stat[b.m_size + col][k] == 0 &&
			b.m_stat[b.m_size * 2 + BOXID(row, col, boxsz)][k] == 0)
			break;
	}
	if (k == b.m_size) // no feasible action
		return ActionPtr();

	return ActionPtr(new Move(row, col, k+1));  
}
