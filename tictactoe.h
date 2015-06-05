
#include "problem.h"

class Move : public Action {
public:
	Move() : Action(1) {}
	~Move() {}

	virtual void print() const
	{
		std::cout << "Take " << m_where << std::endl;
	}
	virtual void fancy_print() const
	{
		print();
	}
private:
	int	m_where;
};

class OXOState : public State {
public:
	OXOState(int c) 
	{ 
		m_board.resize(c);
	}
	~OXOState() {};	

	virtual void print() const = 0;
	virtual void fancy_print() const
	{
		print();
	}

	virtual bool operator==(const State& s) const = 0;

private:
	typedef enum {
		Eempty = 0,
		XPlayer,
		OPlayer,
	} Player;
	std::vector<int>	m_board;
	Player	m_xo;		
};


