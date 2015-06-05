

#include "gtest/gtest.h"
#include "engine.h"
#include "sliding.h"
#include "strategy.h"

class EngineTest : public ::testing::Test 
{
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() 
	{

	}
	
};

TEST(EngineTest, GeneratorSuccessor)
{
	int initState[] = {
		3, 5, 11, 15,
		7, 9, 12, 13,
		10, 8, 14, 0,
		1, 2, 4, 6
	};
	Sliding sp(initState, 4, 4);

	Problem *pb = (Problem *) &sp;
	std::shared_ptr<Node> candidate = std::shared_ptr<Node>(new Node());
	candidate->setState(pb->getInitState());
	candidate->setAction(pb->getNullAction());

	int counter = 0;

	Engine e;

	std::shared_ptr<Node> succNode;
	while ((succNode = e.genSuccessor(pb, candidate)))
	{
		counter++;		
	}
	ASSERT_EQ(counter, 3) << " Generator 3 successor from ";
}

TEST(EngineTest, General)
{
	StrategyBase::listAll();
}
