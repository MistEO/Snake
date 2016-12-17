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
	bool calc_path();
	bool in_advanced();
	Point get_dict();
	Point wander();

	bool scout_move();
private:
	static bool _find_path(
		Point start,
		Point end,
		std::list<Point> snake,
		std::list<AStarPoint> & _open = std::list<AStarPoint>());
	static std::vector<Point> _surround_points(const Point & center, const std::list<Point> & snake, Point target = Point(-1,-1));
	static int _calcG(AStarPoint & point);
	static int _calcH(const AStarPoint & end, const AStarPoint & point);
	static bool _list_exist_point(const std::list<AStarPoint> & li, const AStarPoint & p);
	size_t _export_path(const AStarPoint & res_point);
	Point _determine_dict(const Point & next_point);

	Board & _board;
	Snake & _snake;
	std::list<Point> _path;
};