/*
 * Abstract class for Problem, State and Action
 *	all problems should inherit the class Problem, and implement their own
 *  version of state class and action class 
 *
 * Author: Shoufu Luo
 * Date: April 1, 2015
 */

#ifndef __PROBLEM_H__
#define __PROBLEM_H__

#include <string>
#include <vector>
#include <memory>
#include <exception>

/* abstract action */
class Action {
public:
	virtual ~Action() {}

	/* print the action */
	virtual void print() const = 0;

	/* the cost of this action */
	const int getCost() const { return m_cost;}

	/* INTERNAL use: Action Comparision */
	virtual bool operator==(const Action& s) const = 0;
	bool operator!=(const Action& s) const 
	{
		return !(*this == s);
	}

protected:
	Action(int c) : m_cost(c) {}
	const int	m_cost;
};

/* abstract state */
class State {
public:	
	virtual ~State() {}

	/* print the state */
	virtual void print() const = 0;

	/* State Compare (equal test)  */
	virtual bool operator<(const State& s) const = 0;
	virtual bool operator==(const State& s) const = 0;

	bool operator!=(const State& s) const 
	{
		return !(*this == s);
	}

protected:
	State() {}
};

typedef std::shared_ptr<State>	StatePtr;
typedef std::shared_ptr<Action>	ActionPtr;

/* abstract problem */
class Problem {

protected:
	/* init state and name must be provided to initialize a problem */
	Problem(const std::string& s, StatePtr init) : m_initState(init), Name(s) {}

	StatePtr	m_initState; /* intial state */
	
	/* 
	 * NOTE: if actions are heavily depends on a state, 
	 * 	instead of a general list of all possible actions (using m_actions)
	 *	you should override the following interfaces to iterate actions on a given state
	 *		virtual ActionPtr actionIterBegin(const State& state) const
	 *		virtual ActionPtr actionIterNext(const State& state, const Action& action) const
	 *
	 * (see example suduko.h)
	 */ 
	mutable std::vector<ActionPtr>	m_actions; /* iterable actions */

public:
	virtual ~Problem() {}

	const std::string Name;

	/* Return StatePtr() if the action is not applicable otherwise a new state ptr */
	virtual StatePtr	nextState(const State& state, const Action& action) const = 0;

	/* A dummy action pointer */
	virtual ActionPtr	getNullAction() const = 0;

	/* Distance to the goal from the state given */
	virtual int		distanceToGoal(const State& state) const = 0;

	/* Goal test of the given state */
	virtual bool	isGoalState(const State& state) const = 0;

	/* return the initial state */
	virtual StatePtr getInitState() const final
	{
		return m_initState;
	}

	/* Note: I don't have any konwledge about a state or an action
	* Simply iterate all known possible actions initialized by the derived class,
	* (ignoring what state is and what the prev action is)
	* if you are not happy with that, override me!!
	* so that you could decide actions for any specific state
	*/ 
	virtual ActionPtr  actionIterBegin(const State& state) const
	{
		m_ait = m_actions.begin();
		return (m_ait != m_actions.end()) ? *m_ait : ActionPtr();
	}
	/* 
	 * Continue to iterate on the state @state
	 * @state: the current state
	 * @action: previous action
	 */
	virtual ActionPtr actionIterNext(const State& state, const Action& action) const
	{
		if (m_ait != m_actions.end())
			m_ait++; 
		return (m_ait != m_actions.end()) ? *m_ait : ActionPtr();
	}

private:
	mutable std::vector<ActionPtr>::iterator	m_ait; /* action iterator */
};

#endif
