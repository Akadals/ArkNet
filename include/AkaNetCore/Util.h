#pragma once

typedef struct int2 INT2;
struct int2
{
	int x, y;

	int2(int x, int y) : x(x), y(y) {};
	bool operator==(const int2& v) const;
	COORD to_coord();
};