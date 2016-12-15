#pragma once

#include "Board.h"
#include <list>

constexpr Point Left(0, -1),
Right(0, 1),
Up(-1, 0),
Down(1, 0),
Zero(0, 0);

class Snake
{
public:
	Snake(Board & b);
	~Snake();

	bool move(Point dict);

	const Point & head() const;
	const Point & tail() const;
	const std::list<Point> & body() const;
private:
	void _display(const Point & last_tail = Point(0, 0));

	Board & _board;
	std::list<Point> _body;
};

