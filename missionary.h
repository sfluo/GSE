/* 
 * Missionaries & Cannibals: a classic transporting problem
 *
 * Game Description:
 * 	There are N missionaries and N Cannibals in one bank and there is only one boat
 *	The boat can transport M (<N) people (missionary or cannibal) each trip to the other bank
 *  	- Whenever there are more cannibals than missionaries in any of bank or in the boat, 
 *		the cannibals will eat the missionaries, and the game is over, you LOSE!	
 *		- However, if all missionaries (and cannibals) are safely transported to the opposite bank
 *		without being eaten, then you WIN!
 *		- Plus, an empty boat is NOT allowed to move between banks
 *		e.g.
 *		initial state: xxxooo|_		|
 *		goal state:			 |	   _|xxxooo
 *
 * (for shorter naming, we use human for missionary, and beast for cannibals) 
 *
 * Author: Shoufu Luo
 * Date: Apr. 1, 2015
 *
 */

#ifndef __MISSIONARY_H__
#define __MISSIONARY_H__

#include <iostream>
#include <vector>

#include "problem.h"

/*
 * Each action is a move of the boat with some cannibals and some missionary
 * the direction of move is always to the opposite bank
 */
class BoatAction : public Action {
	friend class Missionary;
public:

	/* Constructor */
	BoatAction(int m, int c) : Action(1), m_human(m), m_beast(c) {} // cost is 1

	/* Destructor */
	~BoatAction() {}

	virtual void print() const
	{
		if (m_human == 0 && m_beast == 0)
		{
			std::cout << "-";
		} else {
			std::cout << "(trans " << m_human << " Missionaries, " 
				<< m_beast << " Cannibals)";
		}
	}

	bool operator==(const Action& s) const
	{
		const BoatAction&	ss = static_cast<const BoatAction&>(s);
		return (m_human == ss.m_human && m_beast == ss.m_beast);
	}

private:
	int	m_human; // number of human (missionaries) to transport
	int	m_beast; // number of beast (cannibals) to transport
};

/*
 * The state of this game is represented by 
 * 	the numbers of missionaries and cannibals on both banks 
 *  and the side of the boat is standing
 */
class BankState : public State {
	friend class Missionary;

public:
	
	/* Constructor */
	BankState(int morig=3, int corig=3, int mdest=0, int cdest=0, bool boat=true) :
		m_human_orig(morig),
		m_beast_orig(corig),
		m_human_dest(mdest), 	
		m_beast_dest(cdest),
		m_boat_orig(boat) {}

	/* Destructor */
	~BankState() {} 

	/* State Compare (equal test)  */
	virtual bool operator<(const State& s)  const
	{
		const BankState& ms = static_cast<const BankState&>(s);

		if (m_human_orig > ms.m_human_orig)
			return false;
		if (m_human_orig < ms.m_human_orig)
			return true;
		if (m_beast_orig > ms.m_beast_orig)
			return false;
		if (m_beast_orig < ms.m_beast_orig)
			return true;
		if (m_boat_orig && !ms.m_boat_orig)
			return false;
		if (!m_boat_orig && ms.m_boat_orig)
			return true;
		return false; // equal
	}
	virtual bool operator==(const State& s)  const
	{
		const BankState& ms = static_cast<const BankState&>(s);

		return (m_human_orig == ms.m_human_orig &&
		 		 m_human_dest == ms.m_human_dest &&
		 		 m_beast_orig == ms.m_beast_orig &&
		 		 m_beast_dest == ms.m_beast_dest &&
				 m_boat_orig == ms.m_boat_orig); 
	}

	virtual void print() const
	{
		int total = m_human_orig + m_human_dest + m_beast_dest + m_beast_orig; 

		std::cout << "[";
		inprint(total, m_beast_orig, m_human_orig);
		if (m_boat_orig)
			std::cout << "|_    |";
		else
			std::cout << "|    -|";
		inprint(total, m_beast_dest, m_human_dest);
		std::cout << "]";
		//std::cout << std::endl;
	}

private:
	void inprint(int t, int x, int o) const
	{
		for (int i=0; i < x; i++)
			std::cout << "x";
		for (int i=0; i < o; i++)
			std::cout << "o";
		for (int i=0; i < t-x-o; i++)
			std::cout << " ";
	}

	int m_human_orig; 	
	int m_beast_orig; 	
	int m_human_dest; 	
	int m_beast_dest; 	
	bool m_boat_orig; /* where is the boat, on the orginal bank or on the destination bank */
};

class Missionary : public Problem {

public:
	Missionary(int m=3, int c=3, int b=2) : 
		Problem("Missionaries and Cannibals", StatePtr()),
		m_human(m), m_beast(c), m_boat_cap(b)
	{
		for (int i = 0; i <= m_boat_cap; i++) //missionary
		{
			for (int j = 0; j <= m_boat_cap - i; j++) //beast
			{ 
				/* Actions:
				 * 	rule 1: empty boat is not allowed to move (i==0, j==0)
				 * 	rule 2: i!= 0, i < j: not safe in boat 
				 */
				if ((i == 0 && j == 0) || (i != 0 && i < j))
					continue;
				m_actions.push_back(ActionPtr(new BoatAction(i, j)));
			}
		}
		m_initState = StatePtr(new BankState(m, c, 0, 0, true));
	}

	~Missionary()
	{}

	void setInitState(const int s[], int c, bool b);

	/* interfaces inherited from the abstract problem */
	virtual int		distanceToGoal(const State& st) const;
	virtual bool	isGoalState(const State& st) const;
	virtual StatePtr nextState(const State& st, const Action& a) const;
	virtual ActionPtr	getNullAction() const;

private:
	/* three paramenters of this game */
	int	m_human;
	int	m_beast;
	int m_boat_cap;
}; 
#endif
