/* 
 * The abstract Strategy: all strategies should inherit class Strategy
 *	A strategy is used to prioritize the candidate list for exploring
 *
 * Author: Shoufu Luo
 * Date: Apr. 1, 2015
 */

#ifndef __STRATEGY_H__
#define __STRATEGY_H__

#include <iostream>
#include <string>
#include <set>
#include <map>
#include "node.h"

class Strategy {
public:
	virtual ~Strategy() { /*cleanVisitedList();*/ }

	/* Interface: remove node */
	virtual NodePtr remove() = 0;

	/* Interface: insert node */
	virtual void insert(NodePtr n) = 0;

	/* Interface: the number of candidates in the queue */
	virtual long qsize() = 0;

	/* Strategy Name */
	const std::string name;

protected:
	Strategy(const std::string& n) : name(n) {}

	/* duplication detection */
	bool isVisited(NodePtr n);
	void putToVisitedList(NodePtr n);	
	void cleanVisitedList();

	//std::vector<NodePtr>	m_visited_nodes; /* visited nodes: open list + closed list */
	
	struct nodePtrCompare {
		// less than
		bool operator()(const NodePtr n1, const NodePtr n2) 
		{
			if (!n1 || !n2)
				return false;
			StatePtr s1 = n1->getState();
			StatePtr s2 = n2->getState();
			if (!s1 || !s2)
			{
				//std::cout << "Empty State" << std::endl;
				return false;
			}
			if (*s1 == *s2)
			{
				//std::cout << "Equal State" << std::endl;
				return false;
			}

			return (*s1 < *s2);
		}
	};

	std::set<NodePtr, nodePtrCompare>	m_visited_nodes;

};

/*
 * StrategyBase: a management tool to centrally manage all strategies
 * 	a strategy registers itself by a unique name and an external program could search by name
 *  and create an instance if found
 */
class StrategyBase {
protected:
	StrategyBase() {}

public:
	virtual ~StrategyBase() {}
	
	/*
	 * Search a strategy by name @name
	 * Return a instance pointer if found otherwise a Null pointer.
	 * 
	 * !!!Note: the caller is responsible to free memory 
	 */
	static Strategy* find(const std::string &name)
	{
		try {
			std::string sname = name;
			std::transform(sname.begin(), sname.end(), sname.begin(), ::toupper);

			StrategyBase *sb = m_strategies[sname];
			if (sb != NULL)
			{
				//std::cout << " [" << name << "] found"  << std::endl;
				return static_cast<Strategy *> (sb->create());	
			}
		} catch (...) {
			std::cout << " error [" << name << "]"  << std::endl;
			return NULL;
		}
		return NULL;
	}

	// Show what strategies are available in our list 
	static void listAll() 
	{
		std::map<const std::string, StrategyBase *>::iterator mit;
		for (mit = m_strategies.begin(); mit != m_strategies.end(); mit++)
		{
			std::cout << mit->first << std::endl;
		}
	}

private:
	virtual void* create() = 0;

	/* this data member manages all supported strategies */
	static std::map<const std::string, StrategyBase *> m_strategies; 
};

template <class T>
class StrategyItem: public StrategyBase {
public:
	StrategyItem(): StrategyBase() {}

	virtual ~StrategyItem() {}

private:
	/* create a new instance of strategy T */
	virtual void* create()
	{
		return new T;
	}	
};

#endif
