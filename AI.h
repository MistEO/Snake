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
	int find_path(bool is_scout = false, Point start = Zero, Point end = Zero, std::list<Point> snake = std::list<Point>());
	Point get_dict();
	Point wander();

	bool scout_move();
private:
	std::vector<AStarPoint> _surround_points(AStarPoint center, std::list<Point> snake, Point target);
	int _calcG(AStarPoint start, AStarPoint point);
	int _calcH(AStarPoint end, AStarPoint point);
	int _export_path(const AStarPoint & res_point);
	Point _determine_dict(Point next_point);

	Board & _board;
	Snake & _snake;
	std::list<AStarPoint> _open;	//待计算的点
	std::list<AStarPoint> _close;	//已计算的点
	std::list<Point> _path;
	std::list<Point> _scout_snake;
};

bool list_exist_point(const std::list<AStarPoint> & li, const AStarPoint & p);
