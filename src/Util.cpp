#include <AkaNetCore/Util.h>


bool int2::operator==(const int2& v) const
{
	return (x == v.x && y == v.y);
}
COORD int2::to_coord()
{
	return COORD(x, y);
}