/*
 * Engine Implementation
 *
 * Author: Shoufu Luo
 * Date: Apr. 1, 2015
 */

#include <iostream>
#include <ctime>

#include "engine.h"

#if 0
/* 
 * Check wether @state is already visited 
 * by iterating all states visited and stored in @m_visited_nodes
 * (Not efficient - to be improved)
 */
bool Engine::isVisited(StatePtr state)
{ 
	/* Invalid state */
	if (!state)
		return false;

	// FIXME: we should use a better data structure to speed up this, e.g. set
	std::vector<NodePtr>::iterator m_nit;
	for (m_nit = m_visited_nodes.begin(); m_nit != m_visited_nodes.end(); m_nit++)
	{
		NodePtr node = *m_nit;
		StatePtr old_state = node->getState();
		if (old_state && *old_state == *state) 
		{
			return true; // found -> already visited state 
		}
	}
	return false;
}
#endif

/* Iterate actions to generate successor nodes (states) of a candidate node 
 * if no more successors, return null, otherwise a new node
 */
NodePtr
Engine::genSuccessor(const Problem* prob, NodePtr cand)
{
	StatePtr state = cand->getState(); // get requesting state

	/* Restart the iteration, if from a different state,*/ 
	if (m_iteratingActionOnState != state)
	{
		m_iteratingActionOnState = state;
		m_iteratingAction = prob->actionIterBegin(*state);	
	} else {
		m_iteratingAction = prob->actionIterNext(*state, *m_iteratingAction);
	}

	/* Our next state (TBD) */
	StatePtr next_state = StatePtr(); 

	while (m_iteratingAction)
	{
		// Note: NULL if the action is not applicable 
		next_state = prob->nextState(*state, *m_iteratingAction); 
		if (next_state) // && !isVisited(next_state)) /* new state -> duplication detection */
		{
			// YES! next state found --> new node
			NodePtr succNode(new Node());
			succNode->setState(next_state);
			succNode->setAction(m_iteratingAction);
			succNode->setDepth(cand->getDepth() + 1);
			succNode->setDistanceToGoal(prob->distanceToGoal(*next_state)); 
			succNode->setCost(cand->getCost() + m_iteratingAction->getCost());
			succNode->setParent(cand);

			return succNode;
		}
		// iteratre next action if not a new state or not applicable action
		m_iteratingAction = prob->actionIterNext(*state, *m_iteratingAction);
	} 

	/* No more succesor */

	return NodePtr(); 
}

/*
 * Print a path from a given node back to the root, e.g. a solution (goal node)
 *
 * Note: the path printed is in a reverse order, e.g.
 *		from the goal state to init state when printing a solution
 */
void Engine::print_path(NodePtr n)
{
	NodePtr p = n;
	
	std::cout << "Solution Path: \n";
	while (p)
	{
		std::cout << "Depth: " << p->getDepth() << ", Action: ";
		if (p->getAction()) // a node may not have a action, for example, the root
		{
			p->getAction()->print();
			std::cout << "-->" << std::endl;
		}
		if (p->getState())
			p->getState()->print();
		p = p->getParent();
		std::cout << std::endl;
	} 
}

/* 
 * Search one solution for the problem @prob
 * 
 * @sname: strategy name, e.g. BFS/DFS 
 * @st: statistics, which includes the solution if any
 */
void Engine::search(const Problem *prob, std::string const& sname, Stat& st)
{
	/* Welcome Message */
	std::cout << "Search solutions for [" << prob->Name 
		<< "] using (" << sname  << ")." << std::endl;
	if (!prob->getInitState()) // not initialized
	{
		std::cout << "Error: invalid init state. " << std::endl;
		return;
	}

	/* Find the strategy handler, e.g. BFS */
	Strategy *alg = StrategyBase::find(sname);
	if (alg == NULL)
	{
		std::cout << "Oops! [" << sname << "] not supported yet!" << std::endl;
		return;
	}

	/* Starting ... */ 
	std::cout << "Engine (" << sname << ") ready." << std::endl;
	std::cout << "The Initial State:" << std::endl;
	prob->getInitState()->print();
	std::cout << std::endl;

	/* create root of the Tree from the initial state */
	NodePtr root(new Node());
	root->setDistanceToGoal(prob->distanceToGoal(*prob->getInitState())); 
	root->setState(prob->getInitState());
	root->setAction(prob->getNullAction()); // no action
	root->setParent(NodePtr()); // no parent
	
 	/* insert the root */
	alg->insert(root);

	/* initialize stats */
	long candidates_in_queue = 1;
	const clock_t begin_time = clock(); // record the starting time

 	/* explore candidates */
	NodePtr candidate;
	while ((candidate = alg->remove()))
	{
		/* update stats */
		st.num_of_nodes_visited++;
		candidates_in_queue--;
	 	if (candidate->getDepth() > st.max_depth_searched)
			st.max_depth_searched = candidate->getDepth();

#if 1 // for debug use 
		//std::cout << "----------------------" << std::endl;
		//candidate->getAction()->print();
		//std::cout << " Depth=" << candidate->getDepth() 
		//		<< " Cost=" << candidate->getCost()
		//		<< " Distance=" << candidate->getDistanceToGoal() << std::endl;
		candidate->getState()->print();
		candidate->getAction()->print();
		std::cout << std::endl;
#endif

		/* Test the candidate node */
		if (prob->isGoalState(*(candidate->getState())))
		{
			/* Wow!! Solution found!!!  */
			st.depth_of_solution = candidate->getDepth();
			st.terminal = candidate;
			break; // Continue to search for more solutions ??? 
		} 

		/* Explore its successors if candidate is not a goal state */
		NodePtr succNode = genSuccessor(prob, candidate);
		while (succNode)
		{
			alg->insert(succNode);
			succNode = genSuccessor(prob, candidate); // next successor
		}
		
		/* This is a special fix for Iterative Deepening family
		 * for they'd return to the root node to generate successors
		 * after the root has been explored (depth = 1 iteration) */
		m_iteratingActionOnState = StatePtr();

		/* Update: the maxinum candidates in queue */
		if (alg->qsize() > st.max_num_candidates)
		{
			st.max_num_candidates = alg->qsize();
		}
	}

	/* Calulate the elapsed time */
	st.elapsed_time = (clock() - begin_time) / (CLOCKS_PER_SEC / 1000); /* stop the timer */

	delete alg; // clean
}

