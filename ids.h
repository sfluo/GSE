/*
 * DFS family: iterative deepening search strategies:
 *	- Iterative Deepening DFS (IDS)
 *	- Iteratie Deepening A* (IDA*)
 *
 * Author: Shoufu Luo
 * Date: Apr. 1, 2015
 */

#ifndef __IDS_H__
#define __IDS_H__

#include <iostream>
#include "dfs.h"

/* 
 * Iterative Deepening Search (IDS): 
 *		recursively increase cut-off when performing DFS until a solution is found
 *
 *
 */
class IDS : public DFS {

protected:
	IDS(std::string name, long c) : DFS(name, c),
		m_updated_cutoff(std::numeric_limits<long>::max()), 
		m_initialized(false), m_extensible(false) {}

public:
	IDS() : DFS(std::string("Iterative Deepening DFS"), 1),
		m_updated_cutoff(std::numeric_limits<long>::max()), 
		m_initialized(false), m_extensible(false) { }
	virtual ~IDS() {}
	
	virtual NodePtr remove();
	virtual void insert(NodePtr u);

	virtual long initCutoff(NodePtr n)
	{
		return 1;
	}

	virtual bool cutoff(NodePtr n) 
	{
		if (n->getDepth() > m_cutoff)
		{
			m_updated_cutoff = std::min(n->getDepth(), m_updated_cutoff);
			return true;
		}
		return false;
	}
protected:
	long		m_updated_cutoff;

private:
	bool	m_initialized;
	bool	m_extensible;
};

/*
 * Iterative Deepening A* (IDA*) 
 *	- a similar implementation of IDS but heuristics are used for the cutoff instead of depth
 *
 * cutoff: the smallest f-cost of any node that exceeded the cutoff on the previous iteration
 */
class IDAstar : public IDS {
public:
	IDAstar() : IDS(std::string("Iterative Deepening A*"), 1) {} 
	~IDAstar() {}

	long initCutoff(NodePtr r)
	{
		return r->getCost() + r->getDistanceToGoal();
	}

	bool cutoff(NodePtr n) 
	{
		/* f = h + g 
		 * h: cost to this node
		 * g: estimate cost to the goal 
		 */
		long f_cost = n->getCost() + n->getDistanceToGoal();
		if (f_cost > m_cutoff) // cut
		{
			m_updated_cutoff = std::min(m_updated_cutoff, f_cost);
			return true;
		}
		return false;
	}
};

#endif
