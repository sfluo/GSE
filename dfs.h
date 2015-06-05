/* 
 * Depth First Search (DFS): a standardy way of depth first search using stack
 * 	- DFS-cutoff (CDFS)
 *
 * Author: Shoufu Luo
 * Date: Apr. 1, 2015
 */
#ifndef __DFS_H__
#define __DFS_H__

#include <stack>
#include "strategy.h"

class DFS : public Strategy {

protected:
	DFS(std::string name, long c) : Strategy(name), m_cutoff(40) {}

public:
	DFS() : Strategy(std::string("DFS")) { m_cutoff = 0; }
	virtual ~DFS();

	virtual NodePtr remove();
	virtual void insert(NodePtr u);

	virtual bool	cutoff(NodePtr c) { return false; }
	virtual long	qsize() { return m_leaves.size(); }

protected:
	std::stack<NodePtr > m_leaves; /* canddiates in our queue */
	long	m_cutoff; // when m_cutoff is 0, it is disabled
};

/*
 * DFS-cutoff
 */
class CDFS : public DFS {
public:
	CDFS(int c) : DFS("DFS-Cutoff", c) {}
	~CDFS() {}

	/* override */
	bool cutoff(NodePtr n) 
	{
		return (n->getDepth() > m_cutoff);
	}
};


#endif
