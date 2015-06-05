/*
 * Open Doors
 *
 * Game Description:
 * 		A robot is searching routine from one postion to an exit in the room
 *		Inside the room, there are may doors that devide the spaces.The robot may can 
 *		trigger a door from the cell where the door is installed, but it may also be
 *		blocked by a door when it tries to step out from a cell if there is a door ahead of it
 *		
 *		Advanced level: may introduce switch/hole facility to make the game harder
 *
 * http://armorgames.com/play/3357/open-doors-2
 * 
 * Author: Shoufu Luo
 * Date: Apr. 1, 2015
 *
 */
#ifndef __OPENDOORS_H__
#define __OPENDOORS_H__

#include <iostream>
#include <ostream>
#include <sstream>
#include <assert.h>
#include <fstream>

#include "problem.h"
#include "direction.h"
#include "odmap.h"

#define STEP_COST	1

/* action: how the robot move left/right/up/down */
class Step : public Action {
	friend class OpenDoors;
public:
	Step(Direction d) : Action(STEP_COST), m_direction(d) {}
	~Step() {}
	
	virtual void print() const
	{
		std::cout << m_direction;
	}

	bool operator==(const Action& s) const
	{
		const Step&	ss = static_cast<const Step&>(s);
		return (m_direction == ss.m_direction);
	}

private:
	Direction	m_direction;
};

/* the state of the game: the map of the room with cells, edges, doors etc.*/
class Map : public State {
	friend class OpenDoors;

private:
	Map(int rows, int cols) : m_rows(rows), m_cols(cols)
	{
		m_grid.resize(m_rows * m_cols);
	}
public:
	Map() : m_rows(0), m_cols(0), m_robot_loc(0), m_exit_loc(0) {}

	/* Constructor
	 * @m[] contains the edge info in a group of six 1 or 0 for all cells  
	 * 	e.g. 4, 5, 1, 0, 0, 1
	 * 	means; the cell at 4th row, 5th column has Wall edge in north, empty edge in East and South
	 *  and wall edge on the west
	 * @rows: number of rows 
	 * @cols: number of cols 
	 *
	 * (No need to include a cell which is in the rectangle but not actually used)
	 */
	Map(int m[], int rows, int cols) : m_rows(rows), m_cols(cols)
	{
		/* TODO: illegal parameter, we should throw exception */
		m_grid.resize(m_rows * m_cols);
		for (int i = 0; i < rows * cols; )
		{
			Cell c = Cell(m[i+2], m[i+3], m[i+4], m[i+5]);
			addCell(c, m[i], m[i+1]);
			i += 6;
		}
	}

	~Map() {}

	/* we can load a map from a map conf file with a pre-defined format 
	 *
	 * line starts with '#': comment
	 * line starts with 'c': a cell with [row] [col] and four edges: 1 - wall, 0 - empty
	 * line starts with 'd': a door with [row] [col] [s/d] [NESW] [R/L] [Swtch]
	 * line starts with 'f': a facility with [row] [col] [S/H] [NESW] [Switch] [status]
	 *
	 * e.g. 
	 * --------------------
	 * # start/end 
	 * r 5 9 4 4 1 8
 	 * # layout
	 * c 0 2 1 0 0 1
	 * ...
	 * c 0 2 1 0 0 1
	 * c 0 3 1 0 1 0
	 * c 0 4 1 0 0 0
 	 * # [door] [row] [col] [s/d] [NESW] [R/L] [Swtch]
	 * #  door   row   col   type direction opento switch
	 * # Direction: N - 1  E - 2  S - 3  W - 4  Not Applied - 0
	 * # opento (inside the cell): r(1) - right  l (2) - left  0 - not applied
	 * # switch: 0 - auto  # - switch number
	 * d 0 6 1 2 1 0
	 * d 2 1 1 2 1 0
	 * d 2 2 1 2 2 0
	 * d 2 4 1 2 2 0
	 * # facilities
	 * # [f] [row] [col] [S/H] [NESW] [Switch No] [status]
	 * # S (1): switch  H: Hole (2)
	 * f 0 2 1 3 1 0
	 * f 3 1 2 0 1 0
	 */
	static Map loadMap(const char *fname)
	{
		Map m;

		std::string		line;	
		std::ifstream	infile(fname);

		bool initialized = false;
		
		//c,0,2,1,0,0,1
		while (std::getline(infile, line))
		{
			std::stringstream iss(line); // get one line
			iss >> std::ws; // remove white space
			if (iss.eof()) 
				continue;

			// get the preceeding character
			char type;
			iss >> type;

			if (type == '#') // comment
				continue;
			if (type == 'r') // robot location (and also exit)
			{
				/* [total rows] [total cols] [Robot Row] [Robot col] [Exit row] [Exit col] */
				int rows, cols, rrow, rcol, erow, ecol;
				iss >> rows >> cols >> rrow >> rcol >> erow >> ecol;
				//std::cout << rows << "," << cols << ". Robot at (" << rrow << ", " << rcol
				//	<< "), Exit at (" << erow << ", " << ecol << ")." << std::endl;

				if (rows > 0 && cols > 0)
				{
					m = Map(rows, cols);
					m.placeRobotAt(rrow, rcol);
					m.placeExitAt(erow, ecol);
					initialized = true;
				}
			} 
			if (!initialized)
				continue;

			// cell with four edges 
			if (type == 'c') 
			{
				int r, c, n, e, s, w;
				iss >> r >> c >> n >> e >> s >> w;
				m.addCell(Cell(n, e, s, w), r, c);
				//std::cout << "Cell at (" << r << ", " << c << ") with " 
				//	<< n << "-" << e  << "-" << s << "-" << w << std::endl;
			} else if (type == 'd') { // door
				int r, c, s, d, o, a;
				iss >> r >> c >> s >> d >> o >> a;
				Door::Style style = (s==1)? Door::SY_Single : Door::SY_Double;
				Door::OpenTo opento = (o == 1) ? Door::OT_Right : Door::OT_Left;
				if (d > 4 || d < 0)
					d = 0;
				//std::cout << "Door at (" << r << ", " << c << ") with " 
				//	<< (s == 1? "single" : "double") << ", on " << d << ", Open to " << o << std::endl;
				m.placeDoorAt(r, c, I2Direction(d-1), Door(style, opento, a));
			} else if (type == 'f') { // facility
				int r, c, s, d, sn, st;
				iss >> r >> c >> s >> d >> sn >> st;
				if (d > 4 || d < 1)
					d = 1;
				if (s == 1)
					m.placeSwitchAt(r, c, I2Direction(d-1), sn);
				else if (s == 2)
					m.placeHoleAt(r, c, sn);
				//std::cout << ((s == 1) ? "Switch" : "Hole") <<  " at (" << r << ", " << c << ") on " 
				//	<< d << ", by " << sn << std::endl;
			}
		}
		return m;
	}

	/* when switch status changed, 
	 * all doors/holes status associated to this switch should be updated 
	 */
	void swtch(int sw, bool on)
	{
		for (int i = 0; i < m_rows; i++)
		{
			for (int j = 0; j < m_cols; j++)
			{
				int cidx = i * m_cols + j;
				Cell& c =  m_grid[cidx];
				if (!c.isUsed() || c.hasSwitch())
					continue;
				if (c.hasHole() && c.isFacilityCtrlBy(sw))
				{
					c.opFacility(on);
					continue;
				}
				for (int k = 0; k < 4; k++)
				{
					Door& d = c.m_edges[k].getDoor();
					if (!d.isCtrlBy(sw))
						continue;
					if (d.isOpen() == on)
						continue;
					if (d.isOpen() && on == false)
					{
						d.closeit();
					} else if (!d.isOpen() && on == true)
					{
						d.openit();
					}
					if (c.m_edges[k].isWall())
						continue;
					int target_idx = cidx;
					switch (I2Direction(k)) {
					case Direction::North: 
						if (i == 0) continue;
						else target_idx -= m_cols;
						break;
					case Direction::East:
						if (j == m_cols - 1) continue;
						else target_idx += 1;
						break;
					case Direction::South:
						if (i == m_rows - 1) continue;
						else target_idx += m_cols;
						break;
					case Direction::West:
						if (j == 0) continue;
						else target_idx -= 1;
						break;
					default: continue;
					}

					Cell& targetcell = m_grid[target_idx];

					// 2.1 facing a closed door (you can open it from outside when you enter)
					Direction td = Opposite(I2Direction(k));
					Door& facingIn = targetcell.m_edges[DirToIndex(td)].getDoor();
					if (facingIn.isNormalDoor() && !facingIn.isOpen())
					{
						facingIn.openit();
					}

					// 2.2 if adjacent edge is a single door and it is open to this edge
					Door& adjRHS2 = targetcell.m_edges[DirToRHSIndex(td)].getDoor();
					if (adjRHS2.isSingleDoor() && adjRHS2.isLeftOpen() && adjRHS2.isOpen())
					{
						adjRHS2.closeit();
					}
					Door& adjLHS2 = targetcell.m_edges[DirToLHSIndex(td)].getDoor();
					if (adjLHS2.isSingleDoor() && !adjLHS2.isLeftOpen() && adjLHS2.isOpen())
					{
						adjLHS2.closeit();
					}
				}
			}
		}
	}

	void addCell(const Cell& c, int row, int col)
	{
		int index = row * m_cols + col;
		if (m_grid.size() < index) //extend the grid
			m_grid.resize((row + 1) * m_cols);
		m_grid[index] = c;
		m_grid[index].use();

		/* TODO: check consistency with adjacent cells: Wall vs. Edge */
	}

	void placeSwitchAt(int row, int col, const Direction& d, int sw)
	{
		assert(row < m_rows && col < m_cols);
		m_grid[row * m_cols + col].installSwitch(d, sw);
	}

	void placeHoleAt(int row, int col, int sw)
	{
		assert(row < m_rows && col < m_cols);
		m_grid[row * m_cols + col].installHole(sw);
	}

	void placeDoorAt(int row, int col, const Direction& d, const Door& dr)
	{
		assert(row < m_rows && col < m_cols);
		m_grid[row * m_cols + col].installDoor(d, dr);
	}

	void placeRobotAt(int row, int col)
	{
		assert(row < m_rows && col < m_cols);
		m_robot_loc = row * m_cols + col;
	}

	void placeExitAt(int row, int col)
	{
		assert(row < m_rows && col < m_cols);
		m_exit_loc = row * m_cols + col;
	}

	virtual void print() const
	{
		for (int i = 0; i < m_rows; i++)
		{
			for (int j = 0; j < m_cols; j++)
			{
				int cur = i * m_cols + j;
				const Cell& c =  m_grid[i * m_cols + j];
				if (cur == m_robot_loc) 
					std::cout << ".";
				else if (cur == m_exit_loc)
					std::cout << "x";
				else if (c.m_type == Cell::CT_Hole)
					if (c.isFacilityEnabled())
						std::cout << "o";
					else
						std::cout << "@";
						
				else if (c.m_type == Cell::CT_Switch)
				{
					if (c.isFacilityEnabled())
						std::cout << "`";
					else
						std::cout << "~";
				} else
					std::cout << " ";
				std::cout << c << " ";
			}
			std::cout << "\n";
		}
		//std::cout << " Robot (.) Exit(x), Single Door(-), Double Door(=)"
		//	<< ", Hole (o), Switch (~), Door Left Opened (/), Right Opened(\\)." 
		//	<< ", Double Door Opened(')" << std::endl;
	}

	virtual bool operator<(const State& s) const
	{
		const Map& ms = static_cast<const Map &>(s);

		if (m_robot_loc > ms.m_robot_loc)
			return false;
		if (m_robot_loc < ms.m_robot_loc)
			return true;

		for (int i = 0; i < m_rows; i++)
		{
			for (int j  = 0; j < m_cols; j++)
			{
				int idx = i * m_cols + j;
				#if 0
				if (m_grid[idx].isUsed() && !ms.m_grid[idx].isUsed())
					return true;
				if (!m_grid[idx].isUsed())
					return false;
				#endif
				if (m_grid[idx] < ms.m_grid[idx])
					return true;
				if (m_grid[idx] == ms.m_grid[idx])
					continue;
				return false;
			}
		}
		return false;
	}

	virtual bool operator==(const State& s) const
	{
		const Map& ms = static_cast<const Map &>(s);

		assert(m_exit_loc == ms.m_exit_loc);
		assert(m_rows == ms.m_rows);
		assert(m_cols == ms.m_cols);

		if (m_robot_loc != ms.m_robot_loc)
		{
			return false;
		}

		for (int i = 0; i < m_rows; i++)
		{
			for (int j  = 0; j < m_cols; j++)
			{
				int idx = i * m_cols + j;
				if (!m_grid[idx].isUsed() && !ms.m_grid[idx].isUsed())
					continue;
				if (!(m_grid[idx] == ms.m_grid[idx]))
					return false;
			}
		}
		return true;
	}

	// for internal use only
	int getRobotLoc() { return m_robot_loc; }

private:
	int m_rows;
	int m_cols;

	int	m_robot_loc;  // where the robot is
	int m_exit_loc;  // where the exit cell

	std::vector<Cell>	m_grid; // 2-dimensional
};

class OpenDoors : public Problem {
public:
	OpenDoors() : Problem("Open Doors 2", StatePtr())
	{
		initActionList();
	}
	OpenDoors(const Map& m) : Problem("Open Doors 2", StatePtr(new Map(m)))
	{
		initActionList();
	}
	~OpenDoors() {}
	
	void setInitialMap(const Map& m);

	/* interfaces inherited from the abstract problem */
	StatePtr	nextState(const State& state, const Action& action) const;
	ActionPtr	getNullAction() const;
	int		distanceToGoal(const State& state) const;
	bool	isGoalState(const State& state) const;
private:
	void initActionList();
};

#endif
