/*
 * Sudoku 9x9
 *
 * Author: Shoufu Luo
 * Date: Apr. 1, 2015
 */
#ifndef __SUDOKU_H__
#define __SUDOKU_H__

#include <iostream>
#include <cmath>
#include "problem.h"

class Move : public Action
{
	friend class Sudoku;
public:

	Move(int row, int col, int sym) : Action(1), m_row(row), m_col(col), m_sym(sym) {}
	~Move() {}

	virtual void print() const 
	{
		if (m_sym == 0)
		{
			std::cout << "N/A";
			return;
		}
		std::cout << "Place " << m_sym << " at (" << m_row << ", " << m_col << ")";
	}

	virtual bool operator==(const Action& s) const
	{
		const Move&	ss = static_cast<const Move&>(s);
		return (m_row == ss.m_row && m_col == ss.m_col && m_sym == ss.m_sym);
	}
private:
	int	m_row;
	int	m_col;
	int	m_sym;
};

#define BOXID(i, j, s)	(((i) / (s)) * (s) + (j) / (s))

class Board : public State 
{
	friend class Sudoku;
public:
	Board(unsigned int sz) : State()
	{
		m_size = sz;	
		m_board = new int*[m_size];
		m_stat = new int*[m_size * 3];

		for (int i = 0; i < m_size * 3; i++)
		{
			m_stat[i] = new int[m_size];
			for (int j = 0; j < m_size; j++)
			{
				m_stat[i][j] = 0;
			}
		}
		for (int i = 0; i < m_size; i++)
		{
			m_board[i] = new int[m_size];
			for (int j = 0; j < m_size; j++)
				m_board[i][j] = 0;
		}
	}

	Board(const Board& b)
	{
		m_size = b.m_size;
		m_board = new int*[m_size];
		m_stat = new int*[m_size * 3];

		for (int i = 0; i < m_size * 3; i++)
		{
			m_stat[i] = new int[m_size];
			for (int j = 0; j < m_size; j++)
			{
				m_stat[i][j] = b.m_stat[i][j];
			}
		}
		for (int i = 0; i < m_size; i++)
		{
			m_board[i] = new int[m_size];
			for (int j = 0; j < m_size; j++)
				m_board[i][j] = b.m_board[i][j];
		}

	}

	Board(const int a[], unsigned int c) : State()
	{
		if (c == 0)
			return;

		m_size = c;
		m_board = new int*[m_size];
		m_stat = new int*[m_size * 3];

		for (int i = 0; i < m_size * 3; i++)
		{
			m_stat[i] = new int[m_size];
			for (int j = 0; j < m_size; j++)
			{
				m_stat[i][j] = 0;
			}
		}
		for (int i = 0; i < m_size; i++)
		{
			m_board[i] = new int[m_size];
			for (int j = 0; j < m_size; j++)
			{
				int k = i * m_size + j;
				if (a[k] <= 0 || a[k] > m_size)
				{
					m_board[i][j] = 0;
					continue;
				}

				int s = a[k];
				int boxsz = sqrt(m_size);
				if (m_stat[i][s-1] != 0 || // row stat - whether a number is taken in a row 
					m_stat[m_size +j][s-1] != 0 || // col stat: whether a number is taken in a col
					m_stat[m_size * 2 + BOXID(i, j, boxsz)][s-1] != 0) // box stat: whether a number is take in a box
				{
					m_board[i][j] = 0;
					continue;
				}
				m_stat[i][s-1] = 1;
				m_stat[m_size + j][s-1] = 1;
				m_stat[m_size * 2 + BOXID(i, j, boxsz)][s-1] = 1;
				m_board[i][j] = s; 
			}
		}
	}
	~Board() 
	{ 
		if (m_board)
		{
			for (int i = 0; i < m_size; i++)
				delete[] m_board[i];
			delete[] m_board;
		}
		if (m_stat)
		{
			for (int i = 0; i < m_size * 3; i++)
				delete[] m_stat[i];
			delete[] m_stat;
		}
	}

	virtual void print() const
	{
		for (int i = 0; i < m_size; i++)
		{
			for (int j = 0; j < m_size; j++)
			{
				if (m_board[i][j] == 0)
					std::cout << "_";
				else
					std::cout << m_board[i][j];
				std::cout << " ";
			}
			std::cout <<  std::endl;
		}
	}

	virtual bool operator<(const State& s) const 
	{
		const Board& b = static_cast<const Board&>(s);

		for (int i = 0; i < m_size; i++)
		{
			for (int j = 0; j < m_size; j++)
			{
				if (m_board[i][j] > b.m_board[i][j])
					return false;
				if (m_board[i][j] < b.m_board[i][j])
					return true;
			}
		}
		return false;
	}

	virtual bool operator==(const State& s) const 
	{
		const Board& b = static_cast<const Board&>(s);

		for (int i = 0; i < m_size; i++)
		{
			for (int j = 0; j < m_size; j++)
			{
				if (m_board[i][j] != b.m_board[i][j])
					return false;
			}
		}
		return true;
	}

private:
	int m_size;
	int** m_board; // how about extending to more than 9x9, instead 16x16
	int** m_stat;
};

class Sudoku : public Problem
{
public:
	Sudoku() : Problem("Sudoku", StatePtr()) {}
	Sudoku(int a[], int c) : Problem("Sudoku", StatePtr(new Board(a, c))) {}
	~Sudoku() {}

	void setInitialBoard(const int a[], int c);

	StatePtr	nextState(const State& state, const Action& action) const;
	ActionPtr	getNullAction() const;
	int		distanceToGoal(const State& state) const;
	bool	isGoalState(const State& state) const;

	// the actions really depend on the board state
	// we filter actions according to current state
	ActionPtr	actionIterBegin(const State& state) const;
	ActionPtr	actionIterNext(const State& state, const Action& action) const;
};

#endif
