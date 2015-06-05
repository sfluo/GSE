/* 
 * Missionaries & Cannibals implementation
 *
 * Author: Shoufu Luo
 * Date: Apr. 1, 2015
 *
 */
#include "missionary.h"

void Missionary::setInitState(const int s[], int c, bool b)
{
	if (c != 4) return;
	m_initState = StatePtr(new BankState(s[0], s[1], s[2], s[3], b));
}

/* 
 * Goal Test:
 *	all human (missionary) and beast (cannibals) are on the destination bank 
 */
bool Missionary::isGoalState(const State& st) const
{
	const BankState& mst = static_cast<const BankState&>(st);

	return (mst.m_human_dest == m_human &&
			mst.m_beast_dest == m_beast);
}

int Missionary::distanceToGoal(const State& st) const
{
	const BankState& mst = static_cast<const BankState&>(st);
	return mst.m_human_orig + mst.m_beast_orig + ((mst.m_boat_orig) ? 0 : 1);
}

/*
 * Given a state and an action, perform the action to the state 
 * Output: 
 * 	- a shared pointer to the new state : if the action is applicable to the given state
 *  - StatePtr() : otherwise
 */
StatePtr Missionary::nextState(const State& st, const Action& a) const
{
	const BankState &bst = static_cast<const BankState&>(st);
	const BoatAction &bact = static_cast<const BoatAction&>(a);
	std::shared_ptr<BankState>	newbst = std::shared_ptr<BankState>();

	/* Invalid Action: Empty boat is not allowed to move. (Null/Dummy Action) */
	if (bact.m_human == 0 && bact.m_beast == 0)
		return newbst;
	/* Invalid Action: too many */
	if (bact.m_human + bact.m_beast > m_boat_cap)
		return newbst;
	/* Invalid Action: Not safe (on boat) */
	if (bact.m_human > 0 && bact.m_human < bact.m_beast)
		return newbst;

	/* boat move: orig -> dest */
	if (bst.m_boat_orig)
	{
		/* not enough */
		if (bst.m_human_orig < bact.m_human || bst.m_beast_orig < bact.m_beast)
			return newbst;
		/* orig not safe */
		if (bst.m_human_orig - bact.m_human > 0 &&
			bst.m_beast_orig - bact.m_beast > bst.m_human_orig - bact.m_human) 
			return newbst;
		/* dest not safe */
		if (bst.m_human_dest + bact.m_human > 0 &&
			bst.m_beast_dest + bact.m_beast > bst.m_human_dest + bact.m_human) 
			return newbst;
		newbst = std::shared_ptr<BankState>(new BankState(bst));
		newbst->m_beast_orig -= bact.m_beast;
		newbst->m_human_orig -= bact.m_human;
		newbst->m_beast_dest += bact.m_beast;
		newbst->m_human_dest += bact.m_human;
		newbst->m_boat_orig = false;	
	 } else { /* boat move: dest -> orig */
		/* not enough */
		if (bst.m_human_dest < bact.m_human || bst.m_beast_dest < bact.m_beast)
			return newbst;
		/* dest not safe */
		if (bst.m_human_dest - bact.m_human > 0 &&
			bst.m_beast_dest - bact.m_beast > bst.m_human_dest - bact.m_human) 
			return newbst;
		/* orig not safe */
		if (bst.m_human_orig + bact.m_human > 0 &&
			bst.m_beast_orig + bact.m_beast > bst.m_human_orig + bact.m_human) 
			return newbst;
		newbst = std::shared_ptr<BankState>(new BankState(bst));
		newbst->m_beast_orig += bact.m_beast;
		newbst->m_human_orig += bact.m_human;
		newbst->m_beast_dest -= bact.m_beast;
		newbst->m_human_dest -= bact.m_human;
		newbst->m_boat_orig = true;	
	 }
	 return newbst;
}

/* dummy action */
ActionPtr Missionary::getNullAction() const
{
	return std::shared_ptr<BoatAction>(new BoatAction(0, 0));
}
