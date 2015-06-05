
#include "rbfs.h"

/*
 * Assumption: we assume the upper layer (engine) always gets the next node 
 * by calling this interface, i.e. remove(), and generates successors from the node it got
 * and then, insert those successors back by the interface, insert()
 */
NodePtr RBFS::remove()
{
#if 0
	/* we are done with ancestor X, now the engine wants the best successor X, say Y 
	 * all nodes in m_current_queueptr are X's successors inserted through insert() 
	 * we need to either return Y to engine for further expand, 
	 * 	or return to X's alternative if best.f excceeds the f_limits
	 */
	if (m_current_blk.pending)	
	{
		PriorityQueuePtr	qptr = m_current_blk.queueptr;		
		
		NodePtr best = qptr->top();

		Nodeptr ancestor = best->getParent();
		best->f = best->getCost() + best->getDistanceToGoal();
		if (best->f > f_limits) // Go for ancestor's (X's) alternative (Y)
		{
			ancestor->f = best->f; // update its ancestor's f score
			ancestor_queueptr = m_tree.top();
			ancestor_queueptr->pop();
			ancestor_queueptr->push(ancestor); // put it back so it will be reordered
			
			NodePtr alt = ancestor_queueptr->top();
							
		}
		m_tree.push(std::move(m_current_queueptr));
		// prepare a new priority queue
		m_current_queueptr = PriorityQuquePtr(new PriorityQueue());
	}
#endif
	return NodePtr();
}

/*
 * make sure the f value is correctly set before insert to the priority queue. 
 */
void RBFS::insert(NodePtr n)
{
}

long RBFS::qsize()
{
	return m_qsize;
}
