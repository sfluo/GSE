
#include "gtest/gtest.h"
#include "node.h"
#include "Astar.h"
#include <random>

TEST(Astar, General)
{
	int seed = time(NULL);

	srand(seed);

	int cost[] = {2, 6, 3, 5, 4};
	int esti[] = {8, 5, 3, 9, 12};

	int ordered_cost[] = {3, 2, 6, 5, 4};
	int ordered_esti[] = {3, 8, 5, 9, 12};

	Strategy *ast = StrategyBase::find("A*");
	if (ast == NULL)
	{
		std::cout << "Oops! [A*] not supported yet!" << std::endl;
		return;
	}

	for (int i = 0; i < 5; i++)
	{
		NodePtr node(new Node());
		node->setCost(cost[i]);
		node->setDistanceToGoal(esti[i]);

		std::cout << i << " cost: " << node->getCost() << ", estimate: " << node->getDistanceToGoal() << std::endl;
		ast->insert(node);

		seed = rand();
	}
	std::cout <<  " Initialized. " << std::endl;
	
	for (int i = 0; i < 5; i++)
	{
		NodePtr n = ast->remove();
		ASSERT_NE(n, NodePtr());
		std::cout << i << " cost: " << n->getCost() << ", estimate: " << n->getDistanceToGoal() << std::endl;
		ASSERT_EQ(n->getCost(), ordered_cost[i]);
		ASSERT_EQ(n->getDistanceToGoal(), ordered_esti[i]);
	}

	delete ast;
}
