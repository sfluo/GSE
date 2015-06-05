
#include "gtest/gtest.h"
#include "missionary.h"
#include "node.h"
#include "engine.h"

class MissionaryTest : public ::testing::Test {
protected:
	void SetUp() {}
	void TearDown() {}
};

TEST_F(MissionaryTest, GenerateSuccssors)
{
	Engine e;
	Stat	st;

	Missionary mc(3, 3, 2); 

	Problem *prob = (Problem *) &mc;

	std::shared_ptr<Node> root(new Node());
	root->setState(prob->getInitState());
	root->setAction(prob->getNullAction());
	root->setParent(std::shared_ptr<Node>());

	std::cout << "This is to test generating successors: " << std::endl; 
	std::cout << "Current State: " << std::endl;
	root->getState()->print();
	std::cout << std::endl;

	std::cout << "Successor States: " << std::endl;
	
	std::shared_ptr<Node> succNode;
	do {
		succNode = e.genSuccessor(prob, root);
		if (succNode)
		{
			succNode->getAction()->print();
			std::cout << "\t";
			succNode->getState()->print();
			std::cout << std::endl;
		}
	} while (succNode);
}

