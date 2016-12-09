#include "AI.h"
#include <algorithm>
#include <cmath>

AI::AI(Board & b, Snake & s)
	: _board(b), _snake(s)
{
}


AI::~AI()
{
}


int AI::find_path()
{
	//头部加入_open
	_open.push_back(_snake.head());
	_open.front().G() = 0;

	while (!_open.empty()) {

		//排序_open，将值最小点设为当前起始点，并加入_close
		_open.sort();
		auto temp_start = _open.front();
		_open.pop_front();
		_close.push_back(temp_start);

		//求周围点
		std::vector<AStarPoint> surround = _surround_points(temp_start);
		for (auto & p : surround) {

			//若_close中存在该点，则跳过
			if (list_exist_point(_close, p)) {
				continue;
			}

			//若_open中存在该点，计算新的G。若新G小于当前G，则更新该点的父节点。
			//若不存在，则更新父节点并加入_open
			if (list_exist_point(_open, p)) {
				int G = _calcG(temp_start, p);
				if (G < p.G()) {
					p.parent() = std::make_shared<AStarPoint>(temp_start);
					p.G() = G;
					p.F();
				}
			}
			else {
				p.parent() = std::make_shared<AStarPoint>(temp_start);
				p.G() = _calcG(temp_start, p);
				p.H() = _calcH(_board.apple(), p);
				p.F();
				_open.push_back(p);

			}
		}	//end for

		//若_open中存在终点（即苹果）,则已找到路径，结束循环
		if (list_exist_point(_open, _board.apple())) {
			break;
		}
	}	//end while

	//未找到路径时情况，开发中。。。。。。
	if (_open.empty()) {
		return 0;
	}
	return _export_path(_open.back());
}

Point AI::get_dict()
{
	if (_path.front() == _snake.head()) {
		_path.pop_front();
	}
	if (_path.empty()) {
		return Point(0, 0);
	}
	Point dict = _determine_dict(_path.front());
	return dict;
}

Point AI::wander()
{
	//static Point left_right_flag = Left;
	if (_board.get(_snake.head() + Up) >= Blank) {
		return Up;
	}
	else if (_board.get(_snake.head() + Down) >= Blank
		&& _board.get(_snake.head() + Down + Down) >= Blank) {	//最底部留一条逃生路线
		return Down;
	}
	else if (_board.get(_snake.head() + Left) >= Blank) {
		//left_right_flag = Left;
		return Left;
	}
	else if (_board.get(_snake.head() + Right) >= Blank) {
		//left_right_flag = Right;
		return Right;
	}
	else {	//错误情况
		return Zero;
	}
}

std::vector<AStarPoint> AI::_surround_points(AStarPoint center)
{
	std::vector<AStarPoint> sur;
	Point temp;

	temp = Point(center.x - 1, center.y);
	if (_board.get(temp) >= Blank
		&& center.x != 0) {
		sur.push_back(temp);
	}

	temp = Point(center.x, center.y - 1);
	if (_board.get(temp) >= Blank
		&& center.y != 0) {
		sur.push_back(temp);
	}

	temp = Point(center.x + 1, center.y);
	if (_board.get(temp) >= Blank
		&& center.x != BoardSize - 1) {
		sur.push_back(temp);
	}

	temp = Point(center.x, center.y + 1);
	if (_board.get(temp) >= Blank
		&& center.y != BoardSize - 1) {
		sur.push_back(temp);
	}

	return sur;
}

int AI::_calcG(AStarPoint start, AStarPoint point)
{
	//int G = abs(point.x - start.x) + abs(point.y - start.y);
	int parentG = ((point.parent() == nullptr) ? 0 : point.parent()->G());
	//return G + parentG;
	return parentG + 1;
}

int AI::_calcH(AStarPoint end, AStarPoint point)
{
	int H = abs(point.x - end.x) + abs(point.y - end.y);
	return H;
}

int AI::_export_path(const AStarPoint & res_point)
{
	//从结果点不断寻找父节点，所有父节点集合即为路径
	std::shared_ptr<AStarPoint> temp = std::make_shared<AStarPoint>(res_point);
	while (temp->parent() != nullptr) {
		_path.push_front(*temp);
		temp = temp->parent();
	}
	return _path.size();
}

bool list_exist_point(const std::list<AStarPoint>& li, const AStarPoint & p)
{
	for (const auto & i : li) {
		if (i.x == p.x && i.y == p.y) {
			return true;
		}
	}
	return false;
}

Point AI::_determine_dict(Point next_point)
{
	int x = next_point.first - _snake.head().first;
	int y = next_point.second - _snake.head().second;
	return Point(x, y);
}