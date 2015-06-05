/*
 * Odmap: all building blocks for Map (State for the Open Doors) 
 * 
 *	[Cell] a map consists of a 2-D array of cells
 * 	[dge] each cell consists of 4 Edges, either empty edge or a wall edge
 * 		- empty edge: the robot can walk through if there is no door on that edge
 *		- wall edge: the robot will bounce back
 *	[Door] each door can be attached to an edge. two types of door:
 * 		- Single door: it can be opened to one neighboring side (either to left or to right) 
 *						and it will block that neighboring edge
 *		- Double door: it can be opened to two sides; it won't block two neighbor edges when opened
 *		when the door is closed, the robot cannot step out from the edge where the door is attached
 *		when a single door is opened, the robot cannot step out from the edge the door is opened to
 *		the robot can trigger the door for outside of the cell on that edge   
 *
 *	Facility: [Siwtch] and [Hole] (see the decription below)
 *		A cell can be installed only one facility, not both
 * 
 * Author: Shoufu Luo
 * Date: April 1, 2015
 *
 */

#ifndef __ODMAP_H__
#define __ODMAP_H__

class Facility;
typedef std::shared_ptr<Facility> FacilityPtr;

class Facility {
protected:
	Facility() {}
public:
	virtual ~Facility() {}

	virtual bool operator==(const Facility& f) const = 0;
	virtual bool operator<(const Facility& f) const = 0;
	virtual bool isEnabled() const = 0;
	virtual bool isCtrlBy(int sw) const = 0;
	virtual void switchTo(bool on) = 0;

	virtual FacilityPtr clone() = 0;
};

/*
 * [Switch] a facility placed in a cell which has two states: on and off
 * 	A switch is used to trigger one or more doors/holes simutanously
 *	e.g.  a switch | T |, 
 			- If the robot enter the cell from below cell, the switch will be triggered
			- the switch will be triggered again only if the robot leaves below
 */ 
class Switch : public Facility {
public:
	// sw must be positive number
	Switch(int sw, const Direction &d) : m_head(d), m_on(false), m_swtch(sw) {}
	~Switch() {}

	/* Testing Function */
	bool isDirectingTo(const Direction& d) { return (m_head == d); }
	bool isEnabled() const { return m_on; }
	bool isCtrlBy(int sw) const { return m_swtch == sw; }

	int	getSwtchNum() const { return m_swtch; }

	/* Switch on/off */
	void switchTo(bool st) { m_on = st; }

	/* Comparison */
	bool operator==(const Facility& f) const
	{
		const Switch& s = static_cast<const Switch &>(f);
		return  (m_head == s.m_head && m_on == s.m_on && m_swtch == s.m_swtch);
	}
	bool operator<(const Facility& f) const
	{
		const Switch& s = static_cast<const Switch &>(f);
		if (m_head < s.m_head)
			return true;
		if (m_head > s.m_head)
			return false;
		if (m_on < s.m_on)
			return true;
		if (m_on > s.m_on)
			return false;
		if (m_swtch < s.m_swtch)
			return true;
		return false;
	}
	FacilityPtr clone() 
	{
		return FacilityPtr(new Switch(*this));	
	}

private:
	Direction	m_head;
	bool		m_on;
	int		m_swtch;
};

/*
 * [Hole] a facility placed in a cell where the robot will be forced back if 
 *		it steps in to this cell when the hole is enabled (controlled by a switch)
 * 		however, nothing will happen (acts as a normal cell) when the hole is not enabled
 */
class Hole : public Facility {
public:
	Hole(int sw) : m_on(false), m_swtch(sw) {}
	~Hole() {}

	bool operator==(const Facility& f) const
	{
		const Hole& s = static_cast<const Hole &>(f);
		return  (m_on == s.m_on && m_swtch == s.m_swtch);
	}

	bool operator<(const Facility& f) const
	{
		const Hole& s = static_cast<const Hole &>(f);
		if (m_on < s.m_on)
			return true;
		if (m_on > s.m_on)
			return false;
		if (m_swtch < s.m_swtch)
			return true;
		return false;
	}

	bool isEnabled() const { return m_on; }
	bool isCtrlBy(int sw) const { return m_swtch == sw; }

	/* switch on/off */
	void switchTo(bool on) { m_on = on; }

	FacilityPtr clone() 
	{
		return FacilityPtr(new Hole(*this));	
	}
	bool	m_on; /* status */
	int		m_swtch; /* associated to Switch */
};

class Door {

	friend std::ostream& operator<<(std::ostream& os, const Door& d);
public:
	typedef enum {
		SY_Empty = 0,
		SY_Single,
		SY_Double,
	} Style;
	typedef enum {
		ST_Closed = 0,
		ST_Open,
	} Status;
	typedef enum {
		OT_Nil =  0,
		OT_Left,
		OT_Right,	
	} OpenTo; 

	Door() : m_style(SY_Empty), m_opento(OT_Nil), m_swtch(0) {} // dummy door
	Door(Style s, OpenTo d) : m_style(s), m_status(ST_Closed), m_opento(d), m_swtch(0) {}
	Door(Style s, OpenTo d, int sw) : m_style(s), m_status(ST_Closed), m_opento(d), m_swtch(sw) {}
	~Door() {}

	bool isNormalDoor() const { return (m_style == SY_Single || m_style == SY_Double); }
	bool isLeftOpen() const {return m_opento == OT_Left; }
	bool isSingleDoor() const { return m_style == SY_Single; }
	bool isOpen() const { return (m_status == ST_Open); }
	bool isCtrlBy(int sw) const { return m_swtch == sw; }

	void closeit() { m_status = ST_Closed; }
	void openit() { m_status = ST_Open; }

	bool operator==(const Door& d) const
	{
		return (m_style == d.m_style && m_status == d.m_status &&
			m_opento == d.m_opento && m_swtch == d.m_swtch);
	}
	bool operator<(const Door& d) const
	{
		if (m_style < d.m_style)
			return true;
		if (m_style > d.m_style)
			return false;
		if (m_opento < d.m_opento)
			return true;
		if (m_opento > d.m_opento)
			return false;
		if (m_status < d.m_status)
			return true;
		if (m_status > d.m_status)
			return false;
		if (m_swtch < d.m_swtch)
			return true;
		return false;
	}

private:
	Style	m_style; /* single or double */
	Status	m_status; /* open or closed */

	/* for single door mode only: open to left/right 
	 * not applicable to double door
	 */
	OpenTo	m_opento;
	int		m_swtch; // 0 - self control, otherwise, by switch
};

class Edge {
	friend std::ostream& operator<<(std::ostream& os, const Edge& c);
public:
	typedef enum  {
		Empty = 0,
		Wall
	} Type;

	Edge() : m_type(Edge::Empty), m_door() {}
	Edge(Type t) : m_type(t), m_door() {}
	Edge(Type t, const Door& d) : m_type(t), m_door(d) {}

	~Edge() {}

	bool isWall() const { return m_type == Wall; }
	bool hasDoor() const { return m_door.isNormalDoor(); }
	Door& getDoor() { return m_door; }

	void attachDoor(const Door& d) { m_door = d; }

	bool operator==(const Edge& e) const
	{
		return (m_type == e.m_type && m_door == e.m_door);
	}
	bool operator<(const Edge& e) const
	{
		if (m_type < e.m_type)
			return true;
		if (m_type > e.m_type)
			return false;
		if (m_door < e.m_door)
			return true;
		return false;
	}

private:
	Type	m_type; /* empty or wall */
	Door	m_door; /* door attached */
};

class Cell {
	friend class Map;
	friend class OpenDoors;
public:
	typedef enum {	
		CT_Normal = 0,
		CT_Switch,
		CT_Hole,
	} Type;

private:	
	Type	m_type;	 // Normal/switch/hole
	bool	m_used;; // cell is default not used, unless specified with edges
	FacilityPtr	m_facility;

#define DirToIndex(c)	((c) - Direction::North)
#define DirToRHSIndex(c)	(((c) - Direction::North + 1) % 4) // right-hand side adjacence, facing out
#define DirToLHSIndex(c)	(((c) - Direction::North - 1) % 4) // left-hand side adjacence, facing out
	Edge	m_edges[4];

	friend std::ostream& operator<<(std::ostream& os, const Cell& c);

public:

	Cell() : m_type(Cell::CT_Normal), m_used(false), m_facility(FacilityPtr())  {}

	Cell(const Cell& c)
	{
		m_type = c.m_type;
		m_used = c.m_used;
		m_edges[0] = c.m_edges[0];	
		m_edges[1] = c.m_edges[1];	
		m_edges[2] = c.m_edges[2];	
		m_edges[3] = c.m_edges[3];	

		if (c.m_facility)
			m_facility = c.m_facility->clone();
	}

	Cell& operator=(const Cell& c)
	{
		if (this == &c) return *this;

		m_type = c.m_type;
		m_used = c.m_used;
		m_edges[0] = c.m_edges[0];	
		m_edges[1] = c.m_edges[1];	
		m_edges[2] = c.m_edges[2];	
		m_edges[3] = c.m_edges[3];	

		if (c.m_facility)
			m_facility = c.m_facility->clone();
		return *this;
	}

	/* clockwise : North, East, South, West */
	Cell(int n, int e, int s, int w) : m_type(Cell::CT_Normal), m_facility(FacilityPtr())
	{
		Edge wall(Edge::Wall);

		m_edges[0] = ((n == 1) ? wall : Edge());
		m_edges[1] = ((e == 1) ? wall : Edge());
		m_edges[2] = ((s == 1) ? wall : Edge());
		m_edges[3] = ((w == 1) ? wall : Edge());
		
		m_used = true;
	}

	~Cell() {}
	void use() { m_used = true; }
	bool isUsed() const { return m_used; }
	bool hasSwitch() const { return m_type == CT_Switch;}
	bool hasHole() const { return m_type == CT_Hole;}

	bool isFacilityCtrlBy(int sw) const { if (m_facility) return m_facility->isCtrlBy(sw); return false;}
	bool isFacilityEnabled() const { if (m_facility) return m_facility->isEnabled(); return false; }
	void opFacility(bool st) const { if (m_facility) m_facility->switchTo(st); }

	bool operator<(const Cell& c) const
	{
		if (m_used && !c.m_used)
			return true;
		if (!m_used && c.m_used)
			return false;
		if (m_type < c.m_type)
			return true;
		if (m_type > c.m_type)
			return false;
		for (int i = 0; i < 4; i++)
		{
			if (m_edges[i] == c.m_edges[i])
				continue;
			if (m_edges[i] < c.m_edges[i])
				return true;
			return false;
		}
		if (!m_facility)
			return false;
		if (!c.m_facility)
			return true;
		if (*m_facility < *c.m_facility)
			return true;
		return false;
	}

	bool operator==(const Cell& c) const
	{
		return (m_used == c.m_used &&
			m_type == c.m_type && 
			m_edges[0] == c.m_edges[0] &&
			m_edges[1] == c.m_edges[1] &&
			m_edges[2] == c.m_edges[2] &&
			m_edges[3] == c.m_edges[3] &&
			((!m_facility && !c.m_facility) ||
			 (m_facility && c.m_facility && *m_facility == *c.m_facility)));
	}

	void installSwitch(const Direction d, int sw)
	{
		m_facility = std::shared_ptr<Switch>(new Switch(sw, d));
		m_type = Cell::CT_Switch; // the cell turns to be a switch cell
	}

	void installHole(int sw)
	{
		m_facility = std::shared_ptr<Hole>(new Hole(sw));
		m_type = Cell::CT_Hole; // the cell turns to be a switch cell
	}

	void installDoor(const Direction d, const Door& dr)
	{
		m_edges[DirToIndex(d)].attachDoor(dr);
	}

};

#endif

