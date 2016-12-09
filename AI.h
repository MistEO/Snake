#pragma once

#include "Board.h"
#include "Snake.h"
#include "AStarPoint.h"
#include <list>
#include <stack>

class AI
{
public:
	AI(Board & b, Snake & s);
	~AI();
	int find_path();
	Point get_dict();
	Point wander();
private:
	std::vector<AStarPoint> _surround_points(AStarPoint center);
	int _calcG(AStarPoint start, AStarPoint point);
	int _calcH(AStarPoint end, AStarPoint point);
	int _export_path(const AStarPoint & res_point);
	Point _determine_dict(Point next_point);

	Board & _board;
	Snake & _snake;
	std::list<AStarPoint> _open;	//待计算的点
	std::list<AStarPoint> _close;	//已计算的点
	std::list<Point> _path;
	std::queue<Point> _virtual_snake;
};

bool list_exist_point(const std::list<AStarPoint> & li, const AStarPoint & p);
