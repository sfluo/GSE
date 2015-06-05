/* 
 * Search Engine (Tree Search) with Statisitcs
 *
 * Author: Shoufu Luo
 * Date: Apr. 1, 2015
 *
 */
#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <set>
#include "problem.h"
#include "strategy.h"

/* Statistics: used by Engine */
class Stat {

public:
	/* constructor */
	Stat() : elapsed_time(0), num_of_nodes_visited(0), 
		max_num_candidates(0), depth_of_solution(0), max_depth_searched(0)
	{ }

	~Stat() {}; /* destructor */

	unsigned long	elapsed_time; /* elapsed time in seconds */
	unsigned long	num_of_nodes_visited;
	unsigned long 	max_num_candidates;
	unsigned long	depth_of_solution;
	unsigned long 	max_depth_searched; 	

	std::shared_ptr<Node>	terminal; /* Goal Node */

	void print() 
	{
		std::cout << "[Statistics] " << std::endl;		
		std::cout << "\t Elapsed time: " << elapsed_time << std::endl;
		std::cout << "\t Number of Nodes visited: " << num_of_nodes_visited << std::endl;
		std::cout << "\t Maximum size of candidate set: " << max_num_candidates << std::endl;
		std::cout << "\t Depth of solution: " << depth_of_solution << std::endl;
		std::cout << "\t Maximum depth searched: " << max_depth_searched << std::endl;
	}
};

class Engine {

//private:
	//bool isVisited(StatePtr state); /* duplication detection */

public:
	Engine() {}
	~Engine() {}

	/* 
	 * for a specific problem @p, given a candidate node
	 * return NULL if no more succesosr, otherwise return a new successor node
	 */
	std::shared_ptr<Node> genSuccessor(const Problem* pb, std::shared_ptr<Node> candidate);

	/* Search solution for the problem @p using staregy @strategy
	 * record the statistics to @st
	 *
	 * @strategy should be supported algorithm name (look up README), e.g. BFS/DFS
	 */
	void search(const Problem* p, std::string const& strategy, Stat& st);	

	/* Print a path from the given node back to the root of search tree */
	void print_path(NodePtr node);

private:

	StatePtr	m_iteratingActionOnState; /* iterating actions (for generating succesor states) */

	ActionPtr	m_iteratingAction; /* which action (abstract) is being iterated */
};
#endif
