
#include <ctime>
#include "gtest/gtest.h"
#include "sudoku.h"
#include "node.h"
#include "engine.h"

class SudokuTest : public ::testing::Test {
protected:
	void SetUp () {
		int initState[] = {
			0,0,0,1,2,0,0,0,0,
			0,0,0,0,0,0,8,0,5,
			0,8,0,0,0,0,0,2,7,
			0,0,9,0,3,2,0,0,0,
			2,0,3,5,1,0,0,8,0,
			0,0,0,0,0,0,9,0,0,
			0,0,0,9,0,0,6,0,3,
			0,1,7,0,0,5,0,0,0,
			0,0,0,0,8,7,1,0,0
		};

		//m_board(initState, 9);
	}
	void TearDown() {}

	Board	m_board;
};

TEST(Sudoku, GenerateSuccessors)
{
	/*int initState[] = {
		0,0,0,1,2,0,0,0,0,
		0,0,0,0,0,0,8,0,5,
		0,8,0,0,0,0,0,2,7,
		0,0,9,0,3,2,0,0,0,
		2,0,3,5,1,0,0,8,0,
		0,0,0,0,0,0,9,0,0,
		0,0,0,9,0,0,6,0,3,
		0,1,7,0,0,5,0,0,0,
		0,0,0,0,8,7,1,0,0
	};

	Board b(initState, 9);
	*/
	int initState[] = {
		0, 3, 0, 1,
		0, 1, 0, 4,
		3, 4, 1, 2,
		0, 0, 4, 0
	};

	Sudoku	sk(initState, 4);

	Engine e;
	Stat	st;

	Problem *prob = (Problem *) &sk;

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
			std::cout << std::endl;
			succNode->getState()->print();
			std::cout << std::endl;
		}
	} while (succNode);
}

TEST(Sudoku, GoalTest)
{
	int fullcase[] = {
		5,7,4,1,2,8,3,6,9,
		6,3,2,7,9,4,8,1,5,
		9,8,1,6,5,3,4,2,7,
		7,6,9,8,3,2,5,4,1,
		2,4,3,5,1,9,7,8,6,
		1,5,8,4,7,6,9,3,2,
		8,2,5,9,4,1,6,7,3,
		4,1,7,3,6,5,2,9,8,
		3,9,6,2,8,7,1,5,4
	};
			
	Board b(fullcase, 9);

	Sudoku	sk;
			
	ASSERT_EQ(sk.isGoalState(b), true);
			
	/* Test Case 1: if flip any of the number, Not Solution anymore 
	 */
	int seed = time(NULL);
	for (int i = 0; i < 81; i++)
	{
		srand(seed);
		seed = rand();
		int temp = fullcase[i];
		while (temp == fullcase[i])
		{
			fullcase[i] = (temp + rand()) % 9; 		
		}
		Board b1(fullcase, 9);
		ASSERT_EQ(sk.isGoalState(b1), false);
		fullcase[i] = temp;
	}

	/* Test Case 1: if flip any of two numbers, Not Solution anymore 
	*/
	for (int i = 0; i < 1000; i++)
	{
		srand(seed);
		seed = rand();
		int j = rand() % 81;
		int k = rand() % 81;

		if (j != k && fullcase[j] != fullcase[k])
		{
			int temp = fullcase[j];
			fullcase[j] = fullcase[k];
			fullcase[k] = temp;
			Board b2(fullcase, 9);
			ASSERT_EQ(sk.isGoalState(b2), false);
			fullcase[k] = fullcase[j];
			fullcase[j] = temp;
		}
	}
}

TEST(Sudoku, NextState)
{
	int initState[] = {
		0,0,0,1,2,0,0,0,0,
		0,0,0,0,0,0,8,0,5,
		0,8,0,0,0,0,0,2,7,
		0,0,9,0,3,2,0,0,0,
		2,0,3,5,1,0,0,8,0,
		0,0,0,0,0,0,9,0,0,
		0,0,0,9,0,0,6,0,3,
		0,1,7,0,0,5,0,0,0,
		0,0,0,0,8,7,1,0,0
	};

	Board b(initState, 9);
	Sudoku	sk(initState, 9);;
		
	Move m1(0, 1, 2);
	StatePtr next1 = sk.nextState(b, m1); // no effect
	ASSERT_EQ(next1, StatePtr());

	Move m2(3, 1, 3);
	StatePtr next2 = sk.nextState(b, m2);
	ASSERT_EQ(next2, StatePtr());
	Move m3(3, 1, 5);
	StatePtr next3 = sk.nextState(b, m3);
	initState[28] = 4; //incorrect
	Board b2(initState, 9);
	ASSERT_NE(b2, *next3);
	initState[28] = 5; //correct
	Board b3(initState, 9);
	//StatePtr b3(new Board(initState, 9));
	ASSERT_EQ(b3, *next3);
	initState[28] = 0; //restore

#if 0
	Engine e;
	Problem *pb = (Problem *) &sk;

	NodePtr root(new Node());
	root->setState(pb->getInitState());
	root->setAction(pb->getNullAction());
	root->setParent(NodePtr());

	NodePtr succNode;
	while ((succNode = e.genSuccessor(pb, root)))
	{
		succNode->getAction()->print();
		std::cout << std::endl;
		succNode->getState()->print();
		std::cout << std::endl;
	}
#endif
}

TEST(Sudoku, General)
{
	int final[] = {
		5,7,4,1,2,8,3,6,9,
		6,3,2,7,9,4,8,1,5,
		9,8,1,6,5,3,4,2,7,
		7,6,9,8,3,2,5,4,1,
		2,4,3,5,1,9,7,8,6,
		1,5,8,4,7,6,9,3,2,
		8,2,5,9,4,1,6,7,3,
		4,1,7,3,6,5,2,9,8,
		3,9,6,2,8,7,1,5,4
	};
	
	Sudoku	sk;

	final[0] = 0; // 5
	final[8] = 0;  // 9
	final[12] = 0;  // 7

	Board b(final, 9);

	Move m1(0, 0, 5);
	StatePtr n1 = sk.nextState(b, m1);
	ASSERT_NE(n1, StatePtr());

	//n1->print();

	Move m2(0, 8, 9);
	StatePtr n2 = sk.nextState(*n1, m2);
	
	//std::cout << std::endl;
	//n2->print();

	Move m3(1, 3, 7);
	StatePtr n3 = sk.nextState(*n2, m3);

	//std::cout << std::endl;
	//n3->print();

	ASSERT_EQ(sk.isGoalState(*n3), true);
}
