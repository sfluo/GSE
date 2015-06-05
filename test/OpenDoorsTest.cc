
#include "gtest/gtest.h"
#include "opendoors.h"
#include "engine.h"
#include "node.h"

class OpenDoorsTest : public ::testing::Test {
protected:
	OpenDoorsTest() {}
	virtual ~OpenDoorsTest() {}

	virtual void SetUp() 
	{
		/* row, col, north, east, south, west 
		* NESW: 1 - wall, 0 - empty
		*/
		int cmap[] = {
			0, 2, 1, 0, 0, 1,
			0, 3, 1, 1, 0, 0, 
			1, 0, 1, 0, 0, 1,
			1, 1, 1, 0, 1, 0,
			1, 2, 0, 0, 0, 0,
			1, 3, 0, 0, 0, 0,
			1, 4, 1, 0, 1, 0,
			1, 5, 1, 1, 0, 0,
			2, 0, 0, 1, 0, 1,
			2, 2, 0, 1, 0, 1,
			2, 3, 0, 1, 0, 1,
			2, 5, 0, 1, 0, 1,
			3, 0, 0, 0, 0, 1,
			3, 1, 1, 0, 1, 0,
			3, 2, 0, 0, 1, 0, 
			3, 3, 0, 0, 1, 0,
			3, 4, 1, 0, 0, 0, 
			3, 5, 0, 1, 1, 0,
			4, 0, 0, 1, 1, 1,
			4, 4, 0, 1, 1, 1,
		};

		m_map = Map(cmap, 5, 6);	
	
		m_map.placeDoorAt(1, 0, Direction::North, Door(Door::SY_Single, Door::OT_Left));
		m_map.placeDoorAt(1, 2, Direction::East, Door(Door::SY_Single, Door::OT_Left));
		m_map.placeDoorAt(1, 3, Direction::East, Door(Door::SY_Single, Door::OT_Left));
		m_map.placeDoorAt(2, 5, Direction::North, Door(Door::SY_Double, Door::OT_Nil));
		m_map.placeDoorAt(3, 0, Direction::South, Door(Door::SY_Single, Door::OT_Right));
		m_map.placeDoorAt(3, 3, Direction::West, Door(Door::SY_Double, Door::OT_Nil));
		m_map.placeDoorAt(3, 4, Direction::South, Door(Door::SY_Single, Door::OT_Right));
	
		m_map.placeRobotAt(4, 0);
		m_map.placeExitAt(4, 4);
	}

	virtual void TearDown() {}

	Map	m_map;
	
};

TEST_F(OpenDoorsTest, Direction)
{
	Direction n = Direction::North;
	Direction e = Direction::East;
	Direction s = Direction::South;
	Direction w = Direction::West;

	ASSERT_EQ(DirToIndex(n), 0);
	ASSERT_EQ(DirToIndex(e), 1);
	ASSERT_EQ(DirToIndex(s), 2);
	ASSERT_EQ(DirToIndex(w), 3);

	ASSERT_EQ(Opposite(s), Direction::North);
	ASSERT_EQ(Opposite(n), Direction::South);
	ASSERT_EQ(Opposite(w), Direction::East);
	ASSERT_EQ(Opposite(e), Direction::West);

	ASSERT_EQ(DirToRHSIndex(n), 1);
	ASSERT_EQ(DirToLHSIndex(n), 3);

	ASSERT_EQ(DirToRHSIndex(e), 2);
	ASSERT_EQ(DirToLHSIndex(e), 0);

	ASSERT_EQ(DirToRHSIndex(s), 3);
	ASSERT_EQ(DirToLHSIndex(s), 1);

	ASSERT_EQ(DirToRHSIndex(w), 0);
	ASSERT_EQ(DirToLHSIndex(w), 2);
}

TEST_F(OpenDoorsTest, HitWall)
{
	ActionPtr hit = ActionPtr(new Step(Direction::South)); // South is a Wall

	OpenDoors od;
	StatePtr next = od.nextState(m_map, *hit);
	ASSERT_EQ(next, StatePtr());
}

TEST_F(OpenDoorsTest, HitDoor)
{
	ActionPtr hit = ActionPtr(new Step(Direction::North)); // North is Door

	OpenDoors od;
	StatePtr next = od.nextState(m_map, *hit);
	//ASSERT_EQ(next,) 
}

TEST_F(OpenDoorsTest, LoadMap)
{
	try {
		Map m = Map::loadMap("./odmap3.txt");
		m.print();
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;	
	}
}

TEST_F(OpenDoorsTest, GenerateSuccessors)
{
	Engine e;

	OpenDoors od(Map::loadMap("./odmap3.txt"));

	Problem *prob = (Problem *) &od;

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

