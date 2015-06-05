/*
 * Recursive Best-First Search (RBFS)
 *
 * A kind of recursive Depth-first search, instead of going down the current path, 
 * 	it uses the f_limit variable to keep track of the f-vlaue of the best alternative 
 *	from the ancestor of the current node. If the current node exceeds this limit, 
 * 	the recurion unwinds back to the alternative path.
 *
 * Reference: AIMA page 99.
 *
 * Author: Shoufu Luo
 * Date: Apr. 21, 2015
 */

#ifndef __RBFS_H__
#define __RBFS_H__

#include <stack>
#include "strategy.h"
#include "astar.h"

class RBFSOrder {
public:
	/* less then, i.e. x < y (x put after y) */
	bool operator()(const NodePtr x, const NodePtr y) const
	{
	//	return x->f > y->f;	
		return x->getCost() + x->getDistanceToGoal() > y->getCost() + y->getDistanceToGoal();
	}
};

typedef std::priority_queue<NodePtr, std::vector<NodePtr>, RBFSOrder>	PriorityQueue; 
typedef std::shared_ptr<PriorityQueue>	PriorityQueuePtr; 

class RBFSBlock {

public:
	RBFSBlock() : queueptr(PriorityQueuePtr()), alternative(0), pending(false) {}
	~RBFSBlock() {}

	PriorityQueuePtr	queueptr;		

	/* alternative may inherit from ancestor (parent) or its sibling */
	long	alternative;

	bool	pending; /* is this block pending to put to stack */
};

class RBFS : public Strategy {

public:
	RBFS() : Strategy("RBFS"), m_current_blk(), m_qsize(0) {}
	~RBFS() {}

	NodePtr remove();
	void insert(NodePtr n);
	long qsize();

private:
	/* 
	 * We use each prioirty_queue for each layer of the tree, ordered by A*
	 * use stack to achieve DFS fashion explore
	 */
	std::stack<RBFSBlock> m_tree;
	
	/* Temporally store the queue for one "layer" (successors of one node)*/
	RBFSBlock	m_current_blk;	

	/* The ancestor associated to the current queue (all nodes should be its successors) */
	NodePtr	m_ancestor;

	long m_qsize;
};

 #endif

