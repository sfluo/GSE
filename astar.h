
#ifndef __ASTAR_H__
#define __ASTAR_H__

#include <queue>
#include <vector>
#include "strategy.h"

template<class T>
class Ordering : public Strategy {

protected:
	Ordering(std::string name) : Strategy(name) {}

public:
	virtual ~Ordering() {}

	virtual NodePtr remove()
	{
		if (!m_nodeptrs.empty())
		{
			NodePtr next = m_nodeptrs.top();
			m_nodeptrs.pop();
			return next;
		}
		return NodePtr();
	}

	virtual void insert(NodePtr n)
	{
		if (isVisited(n))
			return;
		putToVisitedList(n);
		m_nodeptrs.push(n);
	}
	virtual long	qsize() { return m_nodeptrs.size(); }

protected:
	std::priority_queue<NodePtr, std::vector<NodePtr>, T>	m_nodeptrs;
};

class AstarOrder {
public:
	/* less then, i.e. x < y (x put after y) */
	bool operator()(const NodePtr x, const NodePtr y) const
	{
		return x->getCost() + x->getDistanceToGoal() > y->getCost() + y->getDistanceToGoal();
	}
};

class Astar : public Ordering<AstarOrder> {
public:
	Astar() : Ordering("A*") {}
	~Astar() {}
};

class GreedyOrder {
public:
	/* less then, i.e. x < y (x put after y) */
	bool operator()(const NodePtr x, const NodePtr y) const
	{
		return x->getDistanceToGoal() > y->getDistanceToGoal();
	}
};

class Greedy : public Ordering<GreedyOrder> {
public:
	Greedy() : Ordering("Greedy") {}
	~Greedy() {}
};

#endif
