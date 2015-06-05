
#include <iostream>
#include <assert.h>

#include "sliding.h"

bool Sliding::isGoalState(const State& s) const
{
	const SlidingState &ss = (const SlidingState &) s;

	assert(ss.m_board.size() == m_rows * m_cols); 
	for (int i = 0; i < ss.m_board.size() - 1; i++) // the last one is left empty (0)
	{
		if (ss.m_board[i] != i + 1)
			return false;
	}
	return true;
}

std::shared_ptr<State> Sliding::nextState(const State& st, const Action& a) const
{	
	const SlidingState &ss = (const SlidingState &) st;
	const SlidingAction &sa = (const SlidingAction &) a;
	std::shared_ptr<SlidingState> ns = std::shared_ptr<SlidingState>();

	switch (sa.m_move) {
	case Direction::West:
		if (ss.m_blank_pos % m_cols == 0)
			break;
		//std::cout << "Move Left" << std::endl; 
		ns = std::make_shared<SlidingState>(ss);
		ns->m_board[ns->m_blank_pos] = ns->m_board[ns->m_blank_pos - 1];
		ns->m_board[ns->m_blank_pos - 1] = ns->m_blank;
		ns->m_blank_pos -= 1;
		break;
	case Direction::East:
		if (ss.m_blank_pos % m_cols == m_cols - 1)
			break;
		//std::cout << "Move Right" << std::endl; 
		ns = std::make_shared<SlidingState>(ss);
		ns->m_board[ns->m_blank_pos] = ns->m_board[ns->m_blank_pos + 1];
		ns->m_board[ns->m_blank_pos + 1] = ns->m_blank;
		ns->m_blank_pos += 1;
		break;
	case Direction::North:
		if (ss.m_blank_pos / m_rows == 0)
			break;
		//std::cout << "Move Up" << std::endl; 
		ns = std::make_shared<SlidingState>(ss);
		ns->m_board[ns->m_blank_pos] = ns->m_board[ns->m_blank_pos - m_cols]; 
		ns->m_board[ns->m_blank_pos - m_cols] = ns->m_blank;
		ns->m_blank_pos -= m_cols;
		break;
	case Direction::South:
		if (ss.m_blank_pos / m_rows == m_rows - 1)
			break;
		//std::cout << "Move Down" << std::endl; 
		ns = std::make_shared<SlidingState>(ss);
		ns->m_board[ns->m_blank_pos] = ns->m_board[ns->m_blank_pos + m_cols];
		ns->m_board[ns->m_blank_pos + m_cols] = ns->m_blank;
		ns->m_blank_pos += m_cols;
		break;
	default:
		break;
	}
	
	return ns;
}

void Sliding::setInitState(const int a[], int c)
{
	assert(c == m_rows * m_cols);
	if (c != m_rows * m_cols)
		throw "Invalid init state.";

	m_initState = StatePtr(new SlidingState(a, c, m_cols));
}

int	Sliding::distanceToGoal(const State& state) const
{
	const SlidingState& ss = (const SlidingState &) state;
	int error = 0;
#if 0 // Heuristics 1
	for (int i = 0; i < ss.m_board.size(); i++)
	{
		if (ss.m_board[i] != i + 1)
			error += 1;
	}
#else // Heuristics 2
	for (int i = 0; i < ss.m_board.size(); i++)
	{
		// manhattan distance
		error += abs((ss.m_board[i] - 1) % ss.m_cols - i % ss.m_cols) +
			abs((ss.m_board[i]-1) / ss.m_cols - i / ss.m_cols);
	}
#endif
	return  error;
}

ActionPtr	Sliding::getNullAction() const
{
	return ActionPtr(new SlidingAction(Direction::Nil, 0));
}
