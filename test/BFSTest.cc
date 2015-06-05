
//#include <memory>
#include "gtest/gtest.h"
#include "node.h"
#include "bfs.h"

TEST(BFS, Queue)
{
	Strategy *bfs = StrategyBase::find("BFS");

	ASSERT_TRUE(bfs!=NULL);

	ASSERT_EQ(bfs->qsize(), 0);
	for (int i = 0; i < 5; i++)
	{
		NodePtr node(new Node());
		node->setDepth(i);
		bfs->insert(node);
	}
	ASSERT_EQ(bfs->qsize(), 5);

	NodePtr t;
	int i = 0;	
	while ((t = bfs->remove()))
	{
		ASSERT_EQ(t->getDepth(), i++);
	}
	ASSERT_EQ(bfs->qsize(), 0);

	delete bfs;
}
