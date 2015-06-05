
#include <iostream>
#include "bfs.h"

BFS::~BFS()
{
	while (!m_leaves.empty())
	{
		m_leaves.pop();
	}
}

NodePtr BFS::remove()
{
	if (m_leaves.empty())
		return NodePtr();
	NodePtr n = m_leaves.front();
	m_leaves.pop();
	return n;
}

void BFS::insert(NodePtr n)
{
	if (isVisited(n))
		return;
	putToVisitedList(n);
	m_leaves.push(n);
}

