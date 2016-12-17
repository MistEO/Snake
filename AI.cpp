#include "AI.h"
#include <cmath>
#include <algorithm>
#include <functional>

AI::AI(Board & b, Snake & s)
	: _board(b), _snake(s)
{
}


AI::~AI()
{
}

bool AI::calc_path()
{
	std::list<AStarPoint> open;
	_find_path(_snake.head(), _board.apple(), _snake.body(), open);
	if (open.empty()) {
		return false;
	}
	_export_path(open.back());
	return true;
}
bool AI::_find_path(Point start, Point end, std::list<Point> snake, std::list<AStarPoint> & _open)
{
	if (start == end
		&& start != Zero) {
		return true;
	}

	//std::list<AStarPoint> _open;	//待计算的点
	std::list<AStarPoint> _close;	//已计算的点

	//头部加入_open
	_open.push_back(start);
	_open.front().G() = 0;

	bool complete_flag = false;
	while (!_open.empty() && !complete_flag) {

		//排序_open，将值最小点设为当前起始点，并加入_close
		_open.sort();
		auto temp_start = _open.front();
		_open.pop_front();
		_close.push_back(temp_start);

		//求周围点
		std::vector<Point> surround = _surround_points(temp_start, snake, end);
		for (auto & a : surround) {
			AStarPoint asp(a);
			//若_close中存在该点，则跳过
			if (_list_exist_point(_close, asp)) {
				continue;
			}

			//若_open中存在该点，计算新的G。若新G小于当前G，则更新该点的父节点。
			//若不存在，则更新父节点并加入_open
			if (_list_exist_point(_open, asp)) {
				int G = _calcG(asp);
				if (G < asp.G()) {
					asp.parent() = std::make_shared<AStarPoint>(temp_start);
					asp.G() = G;
				}
			}
			else {
				asp.parent() = std::make_shared<AStarPoint>(temp_start);
				asp.G() = _calcG(asp);
				asp.H() = _calcH(end, asp);
				_open.push_back(asp);

			}
			//若_open中存在终点,则已找到路径，结束循环
			if (_list_exist_point(_open, end)) {
				complete_flag = true;
				break;
			}
		}	//end for
	}	//end while

	if (_open.empty()) {
		return false;
	}
	return true;
}

std::vector<Point> AI::_surround_points(const Point & center, const std::list<Point> & snake, Point target)
{
	std::vector<Point> sur;
	std::vector<Point> temp(4);

	temp[0] = center + Left;
	temp[1] = center + Up;
	temp[2] = center + Right;
	temp[3] = center + Down;

	for (auto p : temp) {
		if (!Board::is_border(p)		//该点不为边界
			&& (p == target ||			//该点为目标点或该点不为蛇身
				std::find(snake.begin(), snake.end(), p) == snake.end()))
			sur.push_back(p);
	}

	return sur;
}

int AI::_calcG(AStarPoint & point)
{
	//int G = abs(point.x - start.x) + abs(point.y - start.y);
	int parentG = ((point.parent() == nullptr) ? 0 : point.parent()->G());
	//return G + parentG;
	return parentG + 1;
}

int AI::_calcH(const AStarPoint & lhs, const AStarPoint & rhs)
{
	int H = abs(lhs.x - rhs.x) + abs(lhs.y - rhs.y);
	return H;
}

size_t AI::_export_path(const AStarPoint & res_point)
{
	_path.clear();
	//从结果点不断寻找父节点，所有父节点集合即为路径
	std::shared_ptr<AStarPoint> temp = std::make_shared<AStarPoint>(res_point);
	while (temp->parent() != nullptr) {
		_path.push_front(*temp);
		temp = temp->parent();
	}
	return _path.size();
}

bool AI::_list_exist_point(const std::list<AStarPoint>& li, const AStarPoint & p)
{
	for (const auto & i : li) {
		if (i.x == p.x && i.y == p.y) {
			return true;
		}
	}
	return false;
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

Point AI::_determine_dict(const Point & next_point)
{
	int x = next_point.first - _snake.head().first;
	int y = next_point.second - _snake.head().second;

	return Point(x, y);
}

//无脑漫步模式
//Point AI::wander()
//{
//	//static Point left_right_flag = Left;
//	if (_board.get(_snake.head() + Up) >= Blank) {
//		return Up;
//	}
//	else if (_board.get(_snake.head() + Down) >= Blank
//		&& _board.get(_snake.head() + Down + Down) >= Blank) {	//最底部留一条逃生路线
//		return Down;
//	}
//	else if (_board.get(_snake.head() + Left) >= Blank) {
//		//left_right_flag = Left;
//		return Left;
//	}
//	else if (_board.get(_snake.head() + Right) >= Blank) {
//		//left_right_flag = Right;
//		return Right;
//	}
//	else {	//错误情况
//		return Down;
//	}
//}

Point AI::wander()
{
	//寻找蛇头周围可以吃到尾巴的点
	std::vector<Point> surround = _surround_points(_snake.head(), _snake.body(), _snake.tail());
	std::vector<Point> current_point;
	for (auto p : surround) {
		if (_find_path(p, _snake.tail(), _snake.body())) {	//此处O(n)非常高，待优化
			current_point.push_back(p);
		}
	}

	//此情况为：除了蛇头本身，附近没有点可以吃到尾巴
	if (current_point.empty()) {
		//return _determine_dict(_snake.tail());
		std::list<AStarPoint> open;
		_find_path(_snake.head(), _snake.tail(), _snake.body(), open);
		_export_path(open.back());
		return get_dict();
	}

	//寻找可以吃到尾巴的点中，距离尾巴最远、苹果最近的点
	const Point apple = _board.apple();
	const Point tail = _snake.tail();
	auto calcH = [](const Point & lhs, const Point & rhs)->int {
		int H = abs(lhs.first - rhs.first) + abs(lhs.second - rhs.second);
		return H;
	};
	Point p = *std::max_element(current_point.begin(), current_point.end(),
		[apple, tail, calcH](const Point & lhs, const Point & rhs) ->bool {
		if (calcH(tail, lhs) == calcH(tail, rhs)) {
			return calcH(apple, lhs) > calcH(apple, rhs);
		}
		return calcH(tail, lhs) < calcH(tail, rhs);

	});
	return _determine_dict(p);
}

bool AI::scout_move()
{
	//探路蛇吃苹果
	std::list<Point> _scout_snake = _snake.body();
	Point new_tail;
	for (auto p : _path) {
		_scout_snake.push_back(p);
		new_tail = _scout_snake.front();
		_scout_snake.pop_front();
	}
	//吃苹果后增长的尾巴
	_scout_snake.push_front(new_tail);

	//若探路蛇吃完苹果后，可以吃到自己的尾巴，则路线可行
	if (_find_path(_scout_snake.back(), _scout_snake.front(), _scout_snake)) {
		return true;
	}
	//system("PAUSE");
	return false;
}

//进入后期，待开发中。。。
bool AI::in_advanced()
{
	if ((BoardSize - 2)*(BoardSize - 2 - 2) < _snake.body().size()) {
		return true;
	}
	return false;
}