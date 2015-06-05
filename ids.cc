
#include "ids.h"

NodePtr IDS::remove()
{
	if (m_leaves.empty())
		return NodePtr();

	NodePtr n = m_leaves.top();

	if (n->getDepth() >= 0) // normal node
	{
		m_leaves.pop();
		return n;
	}

	/* OTHERWISE (n is the DummyRootNode):
	 * 		we reach the dummy root node (next iteration to restart DFS)
	 * 		reconstruct DFS if there is supposed next level (extensible)
	 */
	if (m_extensible) {
		NodePtr root(new Node(*n));		
		root->setDepth(0);
		m_extensible = false;

		m_cutoff = m_updated_cutoff; 
		m_updated_cutoff = std::numeric_limits<long>::max();
		cleanVisitedList();

		std::cout << " Restart DFS by cutoff=" << m_cutoff << std::endl;

		return root;  // back to the root
	} 

	/* Or, no next level, then iteration is done */
	m_leaves.pop();
	m_initialized = false;
	return NodePtr();
}

void IDS::insert(NodePtr n)
{ 
	/* 
  	 * Assume that the first node being inserted is the root node, 
	 * push the root node to the stack TWICE
	 * so that we can restart DFS and minic the iteration 
  	 */
	if (!m_initialized)
	{
		NodePtr dummyRoot(new Node(*n)); // copy the root
		dummyRoot->setDepth(-1); // make it special: iteration-restart
		m_leaves.push(dummyRoot);

		m_cutoff = initCutoff(n); 
		m_initialized = true;
	}

	// cutoff and update also the cutoff 
	if (m_cutoff > 0 && cutoff(n)) 
	{
		//std::cout << " cutoff" << std::endl;
		m_extensible = true; // we cut off some branch, which means the search is extensible
		return;
	}

	// No duplicated node 
	if (isVisited(n))
		return;
	putToVisitedList(n);

	m_leaves.push(n);
}

