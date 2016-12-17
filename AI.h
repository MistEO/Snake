#pragma once

#include "Board.h"
#include "Snake.h"
#include "AStarPoint.h"
#include <list>
#include <vector>

class AI
{
public:
	AI(Board & b, Snake & s);
	~AI();
	bool find_path(
		bool is_scout = false,
		Point start = Zero, Point end = Zero,
		std::list<Point> snake = std::list<Point>());
	bool in_advanced();
	Point get_dict();
	Point wander();

	bool scout_move();
private:
	std::vector<Point> _surround_points(const Point & center, const std::list<Point> & snake, Point target = Point(-1,-1));
	int _calcG(AStarPoint & point);
	int _calcH(const AStarPoint & end, const AStarPoint & point) const;
	size_t _export_path(const AStarPoint & res_point);
	Point _determine_dict(const Point & next_point);

	Board & _board;
	Snake & _snake;
	std::list<Point> _path;
};

bool list_exist_point(const std::list<AStarPoint> & li, const AStarPoint & p);
