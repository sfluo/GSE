/*
 * Testing and Examples
 *
 * 	- Sliding Puzzle
 *  - Missionaries & Cannibals	 
 * 	- Sudoku
 * 	- Open Doors
 * 	- Generate Successor
 *
 * Author: Shoufu Luo
 * Date: April 1, 2015
 */
#include <iostream>

#include "engine.h"
#include "sliding.h"
#include "missionary.h"
#include "opendoors.h"
#include "sudoku.h"
#include "direction.h"

/*
 * Testing open doors
 */
void testOpenDoors(const char *alg, const char *notused)
{
	/* 
	 * Map: row, col, North, East, South, West 
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

	Map m(cmap, 5, 6);	
	
	m.placeDoorAt(1, 0, Direction::North, Door(Door::SY_Single, Door::OT_Right));
	m.placeDoorAt(1, 2, Direction::East, Door(Door::SY_Single, Door::OT_Right));
	m.placeDoorAt(1, 3, Direction::East, Door(Door::SY_Single, Door::OT_Right));
	m.placeDoorAt(2, 5, Direction::North, Door(Door::SY_Double, Door::OT_Nil));
	m.placeDoorAt(3, 0, Direction::South, Door(Door::SY_Single, Door::OT_Left));
	m.placeDoorAt(3, 3, Direction::West, Door(Door::SY_Double, Door::OT_Nil));
	m.placeDoorAt(3, 4, Direction::South, Door(Door::SY_Single, Door::OT_Left));

	m.placeRobotAt(4, 0);
	m.placeExitAt(4, 4);

	//m.placeSwitchAt(1, 2, Direction::South, 1);
	//m.placeSwitchAt(3, 4, Direction::East, 1);
	//m.placeHoleAt(4, 5, 1);

	Map m1 = Map::loadMap("./odmap3.txt");
	m1.print();
	OpenDoors od;	
	od.setInitialMap(m1);

	Engine e;
	Stat	st;

	Problem *p = (Problem *) &od;

	e.search(p, alg, st);

	if (st.depth_of_solution != 0)
	{
		std::cout << "[ " << p->Name << "] Solution Found: " << std::endl;
		e.print_path(st.terminal);
	}
	st.print();
}

/*
 * testing sliding puzzle
 */
void testSliding(const char *alg, const char* notused)
{
	Engine e;
	Stat	st;

#if 1
	int initState[] = {
//		4, 2, 3,
//		8, 1, 0,
//		5, 7, 6,

		8, 1, 6, 
		4, 2, 7, 
		5, 0, 3,

//		4, 7, 8,
//		3, 1, 0,
//		5, 2, 6,
	};
	Sliding sp(initState, 3, 3);
#else
	int initState[] = {
		3, 4,
		1, 2
	};
	Sliding sp(initState, 2, 2);
#endif

	Problem *p = (Problem *) &sp;
	e.search(p, alg, st);
	if (st.depth_of_solution != 0)
	{
		std::cout << "[ " << p->Name << "] Solution Found: " << std::endl;
		e.print_path(st.terminal);
	}
	st.print();
}

/* 
 * Test Missionaries and Cannibals
 */
void testMissionary(const char *alg, const char *notused)
{
	Engine e;
	Stat	st;

	Missionary mprob(3, 3, 2); 
	
	Problem *p = (Problem *) &mprob;
	e.search(p, alg, st);
	if (st.depth_of_solution != 0)
	{
		std::cout << "[ " << p->Name << "] Solution Found: " << std::endl;
		e.print_path(st.terminal);
	}
	st.print();
}

void testSudoku(const char *alg,const char* notused)
{
#if 0
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
	int initState[] = {
		0,7,4,1,2,8,3,6,0,
		6,3,2,7,9,4,8,1,5,
		0,8,1,6,5,3,4,2,7,
		7,0,9,8,3,2,5,4,1,
		2,4,3,5,1,9,7,8,6,
		1,5,8,0,7,6,9,3,2,
		8,2,5,9,4,0,6,7,3,
		4,1,7,3,6,5,2,9,8,
		3,9,6,2,8,7,1,5,4
	};
	int initState[] = {
		1,0,0,0,0,0,0,0,0,
		0,3,2,4,0,0,1,0,8,
		7,0,0,3,6,0,0,0,9,
		0,0,8,0,1,3,7,5,0,
		6,0,9,0,7,0,3,0,2,
		0,7,5,6,4,0,8,0,0,
		9,0,0,0,5,4,0,0,3,
		2,0,7,0,0,9,6,1,0,
		0,0,0,0,0,0,0,0,4
	};
	Sudoku	sk(initState, 9);
	int initState[] = {
		4, 0, 0, 2,
		0, 3, 0, 0,
		0, 0, 0, 0,
		1, 0, 0, 0
	};
#else
	int initState[] = {
		0, 3, 0, 1,
		0, 1, 0, 4,
		3, 4, 1, 2,
		0, 0, 4, 0
	};
#endif

	Sudoku	sk(initState, 4);

	Engine e;
	Stat	st;

#ifdef FORWARD_CHECKING
	std::cout << "FORWARD_CHECKING Defined" << std::endl;
#endif

	Problem *p = (Problem *) &sk;
	e.search(p, alg, st);
	if (st.depth_of_solution != 0)
	{
		std::cout << "[ " << p->Name << "] Solution Found: " << std::endl;
		e.print_path(st.terminal);
	}
	st.print();
}

void testGenerateSuccessor()
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
	while ((succNode = e.genSuccessor(pb, root)))
	{
	if (succNode == root)
			continue;
		succNode->getState()->print();
		std::cout << std::endl;
	}
}

void testMissionaryInGroup(const char* fname, const char* alg)
{
	std::string		line;	
	std::ifstream	infile(fname);

	// load configuration from file 'fname'
	// 4 4 2
	while (std::getline(infile, line))
	{
		std::stringstream iss(line); // get one line
		iss >> std::ws; // remove white space
		if (iss.eof()) 
			continue;
#if 0
			// get the preceeding character
			char type;
			tmp_iss >> type;
			if (type == '#') // comment
				continue;
#endif
			
		int missionaries = 0, cannibals = 0, bcapacity = 0;
		try {
			iss >> missionaries >> cannibals >> bcapacity;

			if (missionaries < 0 || missionaries > 9999 ||
				cannibals < 0 || cannibals > 9999 ||
				bcapacity >= cannibals || bcapacity >= missionaries) // nonsense data
			{
				std::cout << " Error: Nonsense configuration" << std::endl; 
				continue;
			}
			if (missionaries != cannibals)
			{
				std::cout << " Error: Mismatched numbers of missionaries and cannibals" << std::endl;
				continue;
			}

			Engine e;
			Stat st;
			Missionary mprob(missionaries, cannibals, bcapacity); 
				
			Problem *p = (Problem *) &mprob;
			e.search(p, alg, st);
			st.print();

		} catch (...)
		{
			continue;
		}
	}
}

void testSudokuInGroup(const char* fname, const char* alg)
{
	std::string		line;	
	std::string ofname = std::string("results.sudoku.") + std::string(alg) + "." + fname;
	std::ofstream	outfile(ofname, std::ofstream::out);

	try {
		std::ifstream	infile(fname);

		// load configuration from file 'fname'
		// 4 4 2 0 4 5 3 10 11 9 8 6 12 7 14 15 1 13
		while (std::getline(infile, line))
		{
			std::stringstream iss(line); // get one line
			iss >> std::ws; // remove white space
			if (iss.eof()) 
				continue;
#if 0
			// get the preceeding character
			char type;
			tmp_iss >> type;
			if (type == '#') // comment
				continue;
#endif
			
			int* state = NULL;
			int rows = 0, cols = 0;
			try {
				iss >> rows >> cols;

				if (rows < 0 || rows > 999 || cols < 0 || cols > 999)
					continue;

				//std::cout << "rows: " <<  rows << " cols: " << cols << std::endl;
				state = new int[rows * cols];
				for (int i = 0; i < rows * cols; i++)
				{
					iss >> state[i];
				}
				
			} catch (...)
			{
				//bad configuration
				if (state != NULL)
				{
					delete state;
					state = NULL;
				}
				continue;
			}

			if (rows == 0 || cols == 0)
				continue;

			Engine e;
			Stat st;
			assert(rows == cols);
			Sudoku sk(state, rows); 

			Problem *p = (Problem *) &sk;
			e.search(p, alg, st);
			st.print();
			outfile << rows << ' ' << cols << ' ' << 
					st.elapsed_time << ' ' <<
					st.num_of_nodes_visited << ' ' <<
					st.max_num_candidates << ' ' <<
					st.depth_of_solution <<  ' '<<
					st.max_depth_searched << std::endl;;

			delete state;
		}
		outfile.close();
	} catch (...)
	{
		//testing failed, ignore 	
		outfile.close();
	}
}

void testOpenDoorsInGroup(const char* fname, const char* alg)
{
	Map m1 = Map::loadMap(fname);

	OpenDoors od;	
	od.setInitialMap(m1);

	Engine e;
	Stat	st;

	Problem *p = (Problem *) &od;

	e.search(p, alg, st);

	st.print();
}


void testSlidingInGroup(const char* fname, const char* alg)
{
	std::string		line;	
	std::string ofname = std::string("results.sliding.") + std::string(alg) + std::string(".repo");
	std::ofstream	outfile(ofname, std::ofstream::out);

	try {
		std::ifstream	infile(fname);

		// load configuration from file 'fname'
		// 4 4 2 0 4 5 3 10 11 9 8 6 12 7 14 15 1 13
		while (std::getline(infile, line))
		{
			std::stringstream iss(line); // get one line
			iss >> std::ws; // remove white space
			if (iss.eof()) 
				continue;
#if 0
			// get the preceeding character
			char type;
			tmp_iss >> type;
			if (type == '#') // comment
				continue;
#endif
			
			int* state = NULL;
			int rows = 0, cols = 0;
			try {
				iss >> rows >> cols;

				if (rows < 0 || rows > 999 || cols < 0 || cols > 999)
					continue;

				//std::cout << "rows: " <<  rows << " cols: " << cols << std::endl;
				state = new int[rows * cols];
				for (int i = 0; i < rows * cols; i++)
				{
					iss >> state[i];
				}
				
			} catch (...)
			{
				//bad configuration
				if (state != NULL)
				{
					delete state;
					state = NULL;
				}
				continue;
			}

			if (rows == 0 || cols == 0)
				continue;

			Engine e;
			Stat st;
			Sliding sp(state, rows, cols);

			Problem *p = (Problem *) &sp;
			e.search(p, alg, st);
			st.print();
			outfile << rows << ' ' << cols << ' ' << 
					st.elapsed_time << ' ' <<
					st.num_of_nodes_visited << ' ' <<
					st.max_num_candidates << ' ' <<
					st.depth_of_solution <<  ' '<<
					st.max_depth_searched << std::endl;;

			delete state;
		}
		outfile.close();
	} catch (...)
	{
		//testing failed, ignore 	
		outfile.close();
	}
}

#include <ctime>

int main(int argc, char ** argv)
{
	//testGenerateSuccessor();
	//testMissionary();
	//testSliding();
	//testOpenDoors();

	typedef struct {
		const char *name;
		void	(* func)(const char *, const char *);
	} _testfunc; 

	std::cout << " CLOCKS_PER_SEC: " << CLOCKS_PER_SEC << std::endl;
#define STAGE2

#ifdef STAGE1
	_testfunc tfuncs[] = {
		{"Missionary", testMissionary},
		{"Sliding", testSliding},
		{"Opendoors", testOpenDoors},
		{"Sudoku", testSudoku},
	};
	
	for (int k = 1; k + 1 < argc; k += 2)
	{
		std::cout << "\n------------------------------\n" ;
		std::cout << "Test to solve " << argv[k] << " using [" << argv[k+1] << "]..." << std::endl;
		int i = 0;
		for (;i < sizeof(tfuncs) / sizeof(_testfunc); i++)
		{
			if (strcasecmp(argv[k], tfuncs[i].name) == 0)
			{
				tfuncs[i].func(argv[k+1], NULL);
				break;
			}
		}
		if (i == sizeof(tfuncs) / sizeof(_testfunc))
			std::cout << "Oops! No problem named [" <<  argv[k] << "]." << std::endl;
	}
#elif defined(STAGE2)
	
	if (argc < 4)
	{
		std::cout << " Fatal Error: too few parameters. " << std::endl;
		return 0;
	}
	std::cout << "Test " << argv[1] << " using [" << argv[2] << "] from " << argv[3] << std::endl;

	_testfunc tfuncs[] = {
		{"Missionary", testMissionaryInGroup},
		{"Sliding", testSlidingInGroup},
		{"Opendoors", testOpenDoorsInGroup},
		{"Sudoku", testSudokuInGroup},
	};
	
	int i = 0;
	for (;i < sizeof(tfuncs) / sizeof(_testfunc); i++)
	{
		if (strcasecmp(argv[1], tfuncs[i].name) == 0)
		{
			tfuncs[i].func(argv[3], argv[2]);
			break;
		}
	}
	if (i == sizeof(tfuncs) / sizeof(_testfunc))
		std::cout << "Oops! No problem named [" <<  argv[1] << "]." << std::endl;

#endif

	return 0;
}
