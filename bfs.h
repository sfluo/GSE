/* 
 * Breadth First Search  (BFS):  a standard way of breadth first search using queue
 *
 * Author: Shoufu Luo
 * Date: Apr. 1, 2015
 */
#ifndef __BFS_H__
#define __BFS_H__

#include <queue>
#include "strategy.h"

/* BFS is a strategy (NodePtr: refer to node.h) */
class BFS : public Strategy {
public:
	BFS() : Strategy(std::string("BFS")) {}
	virtual ~BFS();

	/* Interfaces */
	virtual NodePtr remove();
	virtual void insert(NodePtr np);

	/* interface: the number of items in the queue */
	virtual long	qsize() { return m_leaves.size(); }

private:
	std::queue<NodePtr> m_leaves;
};

#endif
