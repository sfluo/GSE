
#include <iostream>
#include "dfs.h"

DFS::~DFS()
{
	while (!m_leaves.empty())
	{
		m_leaves.pop();
	}
}

NodePtr DFS::remove()
{
	if (m_leaves.empty())
		return NodePtr();
	NodePtr n = m_leaves.top();
	m_leaves.pop();
	return n;
}

void DFS::insert(NodePtr n)
{ 
	if (m_cutoff > 0 && cutoff(n)) // cutoff this branch
		return;
	if (isVisited(n))
		return;
	putToVisitedList(n);
	m_leaves.push(n);
}

