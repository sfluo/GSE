/*
 * A utility class for direction given the fact that many game problem
 * has action move up/down/right/left, we use North/South/East/West in general
 *			N
 *		W	x	E
 *			S
 *
 * Author: Shoufu Luo
 * Created on Apr. 1, 2015
 */
#ifndef __DIRECTION_H__
#define __DIRECTION_H__

enum class Direction : uint32_t {
#define Opposite(d)	static_cast<Direction>((static_cast<uint32_t>(d) + 2) % 4)
#define I2Direction(idx)	static_cast<Direction>((idx) % 4)
// Order cannot be changed
	North = 0,
	East,
	South,
	West,
	Nil, 
};

static uint32_t operator-(Direction d1, Direction d2)
{
	return static_cast<uint32_t>(d1) - static_cast<uint32_t>(d2);
}

static std::ostream& operator<<(std::ostream& os, Direction d)
{
	switch(d) {
	case Direction::Nil: os << "[X]"; break;	
	case Direction::North: os << "[N]"; break;	
	case Direction::East: os << "[E]"; break;	
	case Direction::South: os << "[S]"; break;	
	case Direction::West: os << "[W]"; break;	
	default: os.setstate(std::ios_base::failbit);
	}
	return os;
}

#endif
