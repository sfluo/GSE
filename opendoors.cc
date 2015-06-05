/*
 * Open Doors Implementation
 *
 * Author: Shoufu Luo
 * Date: Apr. 1, 2015
 *
 */
#include "opendoors.h"

/* Only call once by a constructor */
void OpenDoors::initActionList()
{
	m_actions.push_back(ActionPtr(new Step(Direction::North)));
	m_actions.push_back(ActionPtr(new Step(Direction::South)));
	m_actions.push_back(ActionPtr(new Step(Direction::West)));
	m_actions.push_back(ActionPtr(new Step(Direction::East)));
}

void OpenDoors::setInitialMap(const Map& m)
{
	 m_initState = StatePtr(new Map(m));
}

StatePtr OpenDoors::nextState(const State& state, const Action& action) const
{
	const Map& m = static_cast<const Map&>(state);
	const Step& step = static_cast<const Step&>(action);

 	// we are going to construct a new state by changing status of doors
	std::shared_ptr<Map> newm(new Map(m));

	// 1. step out from current cell
	Cell& origcell = newm->m_grid[newm->m_robot_loc];
	
	bool debug = true;

	// 1.0 if facing a wall, bump back
	if (debug) std::cout << "robot at (" << newm->m_robot_loc << ") heading to " 
		<< step.m_direction << ". ";
	if (origcell.m_edges[DirToIndex(step.m_direction)].isWall())
	{
		if (debug) std::cout << "Wall (" << step.m_direction << ")" << std::endl;
		return StatePtr(); 
	}
	//if (debug) std::cout << "handling doors" << std::endl;
	
	// 1.1 if adjacent edge is a single door and it is open to this edge (facing out)
	Door& adjRHS = origcell.m_edges[DirToRHSIndex(step.m_direction)].getDoor();
	if (adjRHS.isSingleDoor() && adjRHS.isLeftOpen())
	{
		// inside the cell, an opened door from an adjRHS edge blocks the way
		// otherwise the move causes that (closed) door open (the way to open a door)
		if (adjRHS.isOpen())
		{
			if (debug) std::cout << "Door (" << step.m_direction << ")" << std::endl;
			return StatePtr();
		}
		// a leftOpen door on RHS (closed) -> open this door
		if (debug) std::cout << "Open a door at (" << newm->m_robot_loc << ")." << std::endl;
		adjRHS.openit();
		if (adjRHS.isOpen())
		{
			if (debug) std::cout << " Yes, I am open " << std::endl;
		}
	}

	// remeber: facing out LHS
	Door& adjLHS = origcell.m_edges[DirToLHSIndex(step.m_direction)].getDoor();
	if (adjLHS.isSingleDoor() && !adjLHS.isLeftOpen())
	{
		if (adjLHS.isOpen())
		{
			if (debug) std::cout << "Door (" << step.m_direction << ")" << std::endl;
			return StatePtr();
		}
		adjLHS.openit();
	}
	//if (debug) std::cout << "closing doors" << std::endl;
	
	// 1.2 an opened door of the facing edge should be closed
	Door& facing = origcell.m_edges[DirToIndex(step.m_direction)].getDoor();
	if (facing.isNormalDoor())
	{
		if (!facing.isOpen())
		{
			if (debug) std::cout << "Door (" << step.m_direction << ")" << std::endl;
			return StatePtr();
		}
		facing.closeit();
	}

	//if (debug) std::cout << "switching" << std::endl;
	// 1.3 switch
	if (origcell.hasSwitch() && origcell.m_facility)
	{
		std::shared_ptr<Switch> sw = std::dynamic_pointer_cast<Switch> (origcell.m_facility);	
		if (sw->isDirectingTo(step.m_direction))
		{
			if (debug) std::cout << "leaving" << std::endl;
			newm->swtch(sw->getSwtchNum(), !sw->isEnabled());
			sw->switchTo(!sw->isEnabled());
			if (debug) std::cout << "New State: xEnabled?" << sw->isEnabled() << std::endl;
		}
	}

	// 2. step into the target cell
	// 2.0 Since Wall, if any, should be shared by outgoing cell and ingoing cell
	// for consistency, we don't need to check the Wall again (we check in step 1.0)

	int target_idx = newm->m_robot_loc; 
	switch(step.m_direction) {
	case Direction::North:
		if (target_idx < newm->m_cols) return StatePtr(); // NORTH bound, no more NORTH move
		target_idx -= newm->m_cols; 
		break;
	case Direction::East:
		if (target_idx % newm->m_cols == newm->m_cols - 1) return StatePtr(); // EAST bound, no more EAST move
		target_idx += 1;
		break;
	case Direction::South:
		if (target_idx / newm->m_cols == newm->m_rows - 1) return StatePtr(); // SOUTH bound, no more SOUTH move
		target_idx += newm->m_cols;
		break;
	case Direction::West:
		if (target_idx % newm->m_cols == 0) return StatePtr(); // WEST bound, no more WEST move
		target_idx -= 1;
		break;
	default:
		return StatePtr();
	}
	if (debug) std::cout << "Targeting Loc: " << target_idx << std::endl;

	Cell& targetcell = newm->m_grid[target_idx];

	//if (debug) std::cout << "closing" << std::endl;
	// 2.1 facing a closed door (you can open it from outside when you enter)
	// OppositeIndex: when step out the origin from a north edge, step into target cell from its south edge
	Direction td = Opposite(step.m_direction);
	Door& facingIn = targetcell.m_edges[DirToIndex(td)].getDoor();
	if (facingIn.isNormalDoor() && !facingIn.isOpen())
	{
		facingIn.openit();
	}

	// 2.2 if adjacent edge is a single door and it is open to this edge
	// from outside the cell, it can close the door opened to this edge from an adjacent edge
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

	//if (debug) std::cout << "switch-->" << std::endl;
	/* step into a switch cell */
	if (targetcell.hasSwitch() && targetcell.m_facility)
	{
		std::shared_ptr<Switch> sw = std::dynamic_pointer_cast<Switch> (targetcell.m_facility);	
		//if (debug) std::cout << step.m_direction << "entering same? " << sw->isDirectingTo(td) << std::endl;
		if (sw->isDirectingTo(td)) /* in the direction of swtich head */
		{
			//if (debug) std::cout << "Entering. Enabled?" << sw->isEnabled() << std::endl;
			newm->swtch(sw->getSwtchNum(), !sw->isEnabled()); // trigger the switch
			sw->switchTo(!sw->isEnabled());
			//if (debug) std::cout << "New State: Enabled?" << sw->isEnabled() << std::endl;
		}
	}
	
	// 2.4 A Hole, if enabled, would force the robot back to the previous cell
	/* FIXME: if a hole cell has doors installed, the state might be changed as entering this cell
	* then,instead of a NIL (unchanged state, i.e. nonapplicable), we should return the new state
	* But, Let's assume the hole cell cannot have doors (for now)
	* i.e, entering the hole cell won't change the state and can't make progress either
	*/
	if (targetcell.hasHole() && targetcell.isFacilityEnabled())
	{
		if (debug) std::cout << "Hit Hole" << std::endl;
		return StatePtr();		
	}
	//if (debug) std::cout << "Move" << std::endl;

	// finalize the move
	newm->m_robot_loc = target_idx;
	if (debug) 
	{
		std::cout << "-> New Position: (" << step.m_direction << ") ";
		std::cout << "Robot at " << newm->m_robot_loc << std::endl;
	}

	return newm;
}

ActionPtr OpenDoors::getNullAction() const
{
	return ActionPtr(new Step(Direction::Nil));
}

int OpenDoors::distanceToGoal(const State& state) const
{
	const Map& m = static_cast<const Map& >(state); 
	int distance = abs(m.m_robot_loc - m.m_exit_loc);
	return distance % m.m_cols + distance / m.m_cols; // mahanttan distance
}

bool OpenDoors::isGoalState(const State& s) const
{
	const Map& m = static_cast<const Map& >(s); 
	return (m.m_robot_loc == m.m_exit_loc);
}

std::ostream& operator<<(std::ostream& os, const Door& d)
{
	if (!d.isNormalDoor()) {
		os << " ";
	} else if (d.isSingleDoor()) {
		if (!d.isOpen())
			os << "-";
		else if (d.isLeftOpen())
			os << "/";
		else
			os << "\\";
	} else {
		if (!d.isOpen())
			os << "=";
		else
			os << "\'";
	}
	return os;
}

/* print Edge */
std::ostream& operator<<(std::ostream& os, const Edge& e)
{ 
	os << ((e.isWall()) ? "1" : "0") << e.m_door;
	return os;
}

/* print Cell */
std::ostream& operator<<(std::ostream& os, const Cell& c)
{
	for (int i = 0; i < 4; i++)
	{
		if (c.isUsed())
			os << c.m_edges[i];
		else 
			os << " " << " ";
	}
	return os;
};

