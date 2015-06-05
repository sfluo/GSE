/*
 * Strategy Base Implementation
 *
 * Author: Shoufu Luo
 * Date: Apr. 1, 2015
 */
#include <iostream>
#include "strategy.h"
#include "dfs.h"
#include "bfs.h"
#include "astar.h"
#include "ids.h"
#include "greedy.h"

#ifndef __SELF_REGISTRATION__

static StrategyItem<DFS>	DfsAlg;
static StrategyItem<BFS>	BfsAlg;
static StrategyItem<Astar>	AstarAlg;
static StrategyItem<IDS>	IDSAlg;
static StrategyItem<IDAstar>	IDAstarAlg;
static StrategyItem<Greedy>	GreedyAlg;

/* Name must be in UPPER case */
std::map<const std::string, StrategyBase*> StrategyBase::m_strategies = 
{
	{"DFS", &DfsAlg},
	{"BFS", &BfsAlg},
	{"IDS", &IDSAlg},
	{"GRDY", &GreedyAlg},
	{"A*", &AstarAlg},
	{"IDA*", &IDAstarAlg} 
};

#endif

bool Strategy::isVisited(NodePtr n)
{
	if (!n || !n->getState())
		return false;

#if 0
	std::vector<NodePtr>::iterator m_nit;
	for (m_nit = m_visited_nodes.begin(); m_nit != m_visited_nodes.end(); m_nit++)
	{
		NodePtr node = *m_nit;
		if (node->isSameState(n))
		{
			return true; // found -> already visited state 
		}
	}
#endif
	
	std::set<NodePtr, nodePtrCompare>::iterator it = m_visited_nodes.find(n);
	if (it != m_visited_nodes.end())
	{
		//std::cout << " duplication found " << std::endl;
		//n->getState()->print();
		//(*it)->getState()->print();
		return true;
	}
	return false;
}

void Strategy::putToVisitedList(NodePtr n)
{
	//m_visited_nodes.push_back(n);
	m_visited_nodes.insert(n);
}

void Strategy::cleanVisitedList()
{
	m_visited_nodes.clear();
}


