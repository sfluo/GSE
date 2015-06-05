
#include "gtest/gtest.h"
#include "sliding.h"
#include "node.h"
#include "engine.h"

class SlidingTest : public ::testing::Test {
protected:
	void SetUp() {
		int initState[] = {
			7, 0, 1,
			8, 5, 2,
			6, 4, 3,
		};

		pb = (Problem *) new Sliding(initState, 3, 3);
	}
	void TearDown() {}

	Problem *pb;
};


TEST_F(SlidingTest, GoalTest)
{
	int initState[] = {
		1, 2, 3,
		4, 5, 6,
		7, 8, 0,
	};
	SlidingState ss(initState, 3, 3);
	Sliding sp(initState, 3, 3);
	ASSERT_EQ(sp.isGoalState(ss), true);

	int state4x4[] = {
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 10, 11, 12,
		13, 14, 15, 0 
	};
	SlidingState ss4x4(state4x4, 4, 4);
	Sliding sp4x4(state4x4, 4, 4);
	ASSERT_EQ(sp4x4.isGoalState(ss4x4), true);
}

TEST_F(SlidingTest, EqualTest)
{
	int initState[] = {
		1, 2, 3,
		4, 5, 6,
		7, 8, 0,
	};
	SlidingState s1(initState, 3, 3);
	SlidingState s2(initState, 3, 3);

	ASSERT_EQ(s1, s2);

	initState[0] = 2;
	initState[1] = 1;
	SlidingState s3(initState, 3, 3);

	ASSERT_NE(s1, s3);
}

TEST_F(SlidingTest, GenerateSuccessors)
{
	Engine e;

	int initState[] = {
		7, 0, 1,
		8, 5, 2,
		6, 4, 3,
	};
	Sliding sp(initState, 3, 3);
	Problem *pb = (Problem *) &sp;

	std::shared_ptr<Node> root(new Node());
	root->setState(pb->getInitState());
	root->setAction(pb->getNullAction());
	root->setParent(std::shared_ptr<Node>());

	std::cout << "This is to test generating successors: " << std::endl; 
	std::cout << "Current State: " << std::endl;
	root->getState()->print();
	std::cout << std::endl;

	std::cout << "Successor States: " << std::endl;
	
	std::shared_ptr<Node> succNode;
	do {
		succNode = e.genSuccessor(pb, root);
		if (succNode)
		{
			ActionPtr ap = succNode->getAction();
			ASSERT_FALSE(!ap);

			StatePtr new_state = succNode->getState();
			if (*ap == SlidingAction(Direction::West, 1))
			{
				int succState[9] = {0, 7, 1, 8, 5, 2, 6, 4, 3};
				ASSERT_TRUE(*new_state == SlidingState(succState, 3, 3));
			} else if (*ap == SlidingAction(Direction::East, 1))
			{
				int succState[9] = {7, 1, 0, 8, 5, 2, 6, 4, 3};
				ASSERT_TRUE(*new_state == SlidingState(succState, 3, 3));
			} else if (*ap == SlidingAction(Direction::South, 1))
			{
				int succState[9] = {7, 5, 1, 8, 0, 2, 6, 4, 3};
				ASSERT_TRUE(*new_state == SlidingState(succState, 3, 3));
			} else {
				ASSERT_TRUE(false);
			}
			succNode->getAction()->print();
			std::cout << std::endl;
			succNode->getState()->print();
			std::cout << std::endl;
		}
	} while (succNode);

#if 0
	int state2[] = {
		0, 7, 1,
		8, 5, 2,
		6, 4, 3,
	};
	StatePtr ss(new SlidingState(state2, 3, 3));
	std::shared_ptr<Node> node2(new Node());
	node2->setState(ss);
	node2->setAction(pb->getNullAction());
	node2->setParent(std::shared_ptr<Node>());

	std::cout << "Current State: " << std::endl;
	node2->getState()->print();
	std::cout << std::endl;

	std::cout << "Successor States: " << std::endl;

	do {
		succNode = e.genSuccessor(pb, node2);
		if (succNode)
		{
			succNode->getAction()->print();
			std::cout << std::endl;
			succNode->getState()->print();
			std::cout << std::endl;
		}
	} while (succNode);
#endif
}

TEST_F(SlidingTest, DistanceToGoal)
{
	int state[] = { 
		4,7,8,
		3,1,6,
		5,2,0	
	};

	SlidingState *s = new SlidingState(state, 3, 3);
	ASSERT_EQ(pb->distanceToGoal(*s), 16);
}
