/* 
 * Sliding Puzzle (e.g. 8-puzzle)
 *
 * Game Description: 
 * 	Given 9 blocks in one board, there are 8 of them are filled initially random
 *		with number from 1 through 8, while one block is left empty, 
 * 		the goal is to rearrange those numbers in the right order by moving them
 *		e.g. a initial state might be
 *			3 2 6
 * 			7 6 8
 *			1 _	4 
 *  	the goal, however, is always
 *			1 2 3
 *			4 5 6 
 *			7 8 _
 *
 * There are variants of this game,for instance, 9 pecies of pictures instead of numbers
 *
 * Author: Shoufu Luo
 * Date: Apr. 1, 2015
 *
 */

#ifndef __SLIDING_H__
#define __SLIDING_H__

#include <iostream>
#include <vector>

#include "problem.h"
#include "direction.h"

/*
 * Action in sliding puzzle is how to move the blank (instead of numbers)
 * four actions:  North, South, East, West
 *			N
 *		W	_	E
 *			S
 */
class SlidingAction : public Action {
	friend class Sliding;

public:

	SlidingAction(Direction m, int cost) : Action(cost), m_move(m) {}
	~SlidingAction() {}

	virtual void print() const
	{
		std::cout << m_move;
	}

	virtual bool operator==(const Action& s) const
	{
		const SlidingAction&	ss = static_cast<const SlidingAction&>(s);
		return (m_move == ss.m_move);
	}

private:

	Direction	m_move;
};

/*
 * Each State in sliding puzzle is represented by an array of integer (1, 2, ...)
 * the integers represent the locations where they supposely stand in the goal state
 */
class SlidingState : public State {
	friend class Sliding;

public:
	/* Sliding State: e.g. a[] ={5, 4, 3, 2, 1, 8, 7, 6, 0}
	 * @c: the length of the array
	 * @cols: the number of columns
	 */
	SlidingState(const int a[], int rows, int cols) : m_cols(cols)
	{
		/* Flexible Blank:
		   2 3 6	 1 2 3
		   8 9 0  -> 4 _ 6
		   4 1 7	 7 8 9

		 * FIXME: what if the lenght of a[] is not rows * cols 
		 * throw exception!!!
		 */
		m_blank = 0;
		m_board.resize(rows * cols);
		for (int i=0; i < rows * cols; i++)
		{
			m_blank = m_blank xor (i + 1) xor a[i];
			m_board[i] = a[i];
			if (a[i] == 0)
			{
				m_blank_pos = i;
			}
		}
		m_board[m_blank_pos] = m_blank;
	}
	SlidingState(int r, int c) : m_cols(c)
	{
		if (r > 0 && c > 0)
			m_board.resize(r * c);
	}

	~SlidingState() {}

	virtual bool operator<(const State& s) const 
	{
		const SlidingState &ss = dynamic_cast<const SlidingState&>(s);
		for (int i=0; i < m_board.size(); i++)
		{
			if (m_board[i] > ss.m_board[i])
				return false;
			if (m_board[i] < ss.m_board[i])
				return true;
		}
		return false;
	}

	virtual bool operator==(const State& s)  const
	{
		try {
			const SlidingState &ss = dynamic_cast<const SlidingState&>(s);
			return (m_board == ss.m_board);
		} catch (...) {
		}
		return false;
	}

	void print() const
	{
		//std::cout << "[" << std::endl;
		for (int i=0; i < m_board.size(); i++)
		{
			if (i % m_cols == 0)
				std::cout << "  ";
			if (m_board[i] == m_blank)
				std::cout << "-" << " ";
			else
				std::cout << m_board[i] << " ";

			if ((i+1) % m_cols == 0)
				std::cout << std::endl;
		}
		//std::cout << "]";
	}

private:
	/* we use one-dimension array to represent 2-dimension board 
	 * e.g. 3x3 8-puzzle game, we use m_board[9] 
	 * the goal state: m_board[9] = {1, 2, 3, 4, 5, 6, 7, 8, (9)} where (9) is hidden
	 */
	std::vector<int>	m_board;		
	int	m_blank_pos; 	/*	The postion where the blank locates */
	int m_cols;			/*  the number of columns of each row */

	int	m_blank; 		/*  what is missing, e.g., the number 9 in 8-puzzle */
};

class Sliding : public Problem {

public:
	Sliding(const int a[], int r, int c): 
		Problem("Sliding Puzzle", StatePtr(new SlidingState(a, r, c))), m_rows(r), m_cols(c)
	{
		m_actions.push_back(ActionPtr(new SlidingAction(Direction::North, 1)));
		m_actions.push_back(ActionPtr(new SlidingAction(Direction::East, 1)));
		m_actions.push_back(ActionPtr(new SlidingAction(Direction::South, 1)));
		m_actions.push_back(ActionPtr(new SlidingAction(Direction::West, 1)));
	}

	~Sliding() {}

	void setInitState(const int a[], int c);

	/* interfaces inhirent from abstract problem */
	int		distanceToGoal(const State& st) const;
	bool	isGoalState(const State& st) const;
	StatePtr nextState(const State& st, const Action& a) const;
	ActionPtr	getNullAction() const;

private:
	const int	m_rows; /* number of blocks in one row of the puzzle */
	const int	m_cols; /* number of blocks in one columen of the puzzle */ 
}; 
#endif
