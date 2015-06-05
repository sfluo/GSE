/*
 * Building block of tree: Node
 * 
 * Author: Shoufu Luo
 * Date: Apr. 1, 2015
 *
 */
#ifndef __NODE_H__
#define __NODE_H__

#include "problem.h"

class Node;

typedef std::shared_ptr<Node>	NodePtr;

class Node {

public:

	Node() : m_depth(0), m_distance(0), m_cost_sofar(0), 
		m_parent(NodePtr()), m_state(StatePtr()), m_action(ActionPtr()) {}

	Node(StatePtr s) : m_depth(0), m_distance(0), 
		m_cost_sofar(0),  m_parent(NodePtr()), m_state(s), m_action(ActionPtr()) {}
	
	~Node() {} 

	/* Setter */
	void setState(StatePtr s) { m_state = s; }
	void setAction(ActionPtr s) { m_action = s; }
	void setDistanceToGoal(int d) { m_distance = d; }
	void setDepth(int d) { m_depth = d; }
	void setCost(int c) { m_cost_sofar = c; }

	void setParent(NodePtr p) { m_parent = p; }

	/* Getter */
	StatePtr getState() { return m_state; }
	ActionPtr getAction() { return m_action; }
	long getCost() { return m_cost_sofar; }
	long getDistanceToGoal() { return m_distance; }
	long getDepth() { return m_depth; }

	NodePtr getParent() { return m_parent; }

	bool isSameState(NodePtr n)
	{
		StatePtr nst = n->m_state;
		return (*nst == *m_state);
	}

private:
	long	m_depth;
	long	m_distance; /* distance to goal state */
	long	m_cost_sofar; 

	NodePtr	m_parent;

	// problem-dependent 
	StatePtr	m_state; /* the associated state */
	ActionPtr	m_action; /* action led to this state */ 
};

#endif
