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


int AI::find_path(bool is_scout, Point start, Point end, std::list<Point> snake)
{
	if (start == end
		&& start != Zero) {
		return 1;
	}
	start == Zero ? start = _snake.head() : Zero;
	end == Zero ? end = _board.apple() : Zero;
	snake.empty() ? snake = _snake.body() : std::list<Point>();

	_open.clear();
	_close.clear();

	//ͷ������_open
	_open.push_back(start);
	_open.front().G() = 0;

	bool complete_flag = false;
	while (!_open.empty() && !complete_flag) {

		//����_open����ֵ��С����Ϊ��ǰ��ʼ�㣬������_close
		_open.sort();
		auto temp_start = _open.front();
		_open.pop_front();
		_close.push_back(temp_start);

		//����Χ��
		std::vector<Point> surround = _surround_points(temp_start, snake, end);
		for (auto & a : surround) {
			AStarPoint asp(a);
			//��_close�д��ڸõ㣬������
			if (list_exist_point(_close, asp)) {
				continue;
			}

			//��_open�д��ڸõ㣬�����µ�G������GС�ڵ�ǰG������¸õ�ĸ��ڵ㡣
			//�������ڣ�����¸��ڵ㲢����_open
			if (list_exist_point(_open, asp)) {
				int G = _calcG(temp_start, asp);
				if (G < asp.G()) {
					asp.parent() = std::make_shared<AStarPoint>(temp_start);
					asp.G() = G;
					asp.F();
				}
			}
			else {
				asp.parent() = std::make_shared<AStarPoint>(temp_start);
				asp.G() = _calcG(temp_start, asp);
				asp.H() = _calcH(end, asp);
				asp.F();
				_open.push_back(asp);

			}
			//��_open�д����յ�,�����ҵ�·��������ѭ��
			if (list_exist_point(_open, end)) {
				complete_flag = true;
				break;
			}
		}	//end for
	}	//end while

	if (_open.empty()) {
		return 0;
	}
	if (is_scout) {
		return 1;
	}
	return _export_path(_open.back());
}

std::vector<Point> AI::_surround_points(Point center, std::list<Point> snake, Point target)
{
	std::vector<Point> sur;
	std::vector<Point> temp(4);

	temp[0] = center + Left;
	temp[1] = center + Up;
	temp[2] = center + Right;
	temp[3] = center + Down;

	for (auto p : temp) {
		if (_board.get(p) != Border		//�õ㲻Ϊ�߽�
			&& (p == target ||			//�õ�ΪĿ����õ㲻Ϊ����
				std::find(snake.begin(), snake.end(), p) == snake.end()))
			sur.push_back(p);
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

size_t AI::_export_path(const AStarPoint & res_point)
{
	_path.clear();
	//�ӽ���㲻��Ѱ�Ҹ��ڵ㣬���и��ڵ㼯�ϼ�Ϊ·��
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

Point AI::_determine_dict(Point next_point)
{
	int x = next_point.first - _snake.head().first;
	int y = next_point.second - _snake.head().second;

	return Point(x, y);
}

//��������ģʽ
//Point AI::wander()
//{
//	//static Point left_right_flag = Left;
//	if (_board.get(_snake.head() + Up) >= Blank) {
//		return Up;
//	}
//	else if (_board.get(_snake.head() + Down) >= Blank
//		&& _board.get(_snake.head() + Down + Down) >= Blank) {	//��ײ���һ������·��
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
//	else {	//�������
//		return Down;
//	}
//}

Point AI::wander()
{
	//Ѱ����ͷ��Χ���ԳԵ�β�͵ĵ�
	std::vector<Point> surround = _surround_points(_snake.head(), _snake.body(), _snake.tail());
	std::vector<Point> current_point;
	for (auto p : surround) {
		if (find_path(true, p, _snake.tail())) {
			current_point.push_back(p);
		}
	}

	//�����Ϊ��������ͷ��������û�е���ԳԵ�β��
	if (current_point.empty()) {
		//return _determine_dict(_snake.tail());
		find_path(false, _snake.head(), _snake.tail());
		return get_dict();
	}

	//Ѱ�ҿ��ԳԵ�β�͵ĵ��У�����ƻ����Զ�ĵ�
	const Point apple = _board.apple();
	auto calcH = [](const Point & lhs, const Point & rhs)->int {
		int H = abs(lhs.first - rhs.first) + abs(lhs.second - rhs.second);
		return H;
	};
	Point p = *std::max_element(current_point.begin(), current_point.end(),
		[apple, calcH](const Point & lhs, const Point & rhs) ->bool {
		return calcH(apple, lhs) < calcH(apple, rhs);
	});
	return _determine_dict(p);
}

bool AI::scout_move()
{
	//̽·�߳�ƻ��
	_scout_snake = _snake.body();
	Point new_tail;
	for (auto p : _path) {
		_scout_snake.push_back(p);
		new_tail = _scout_snake.front();
		_scout_snake.pop_front();
	}
	//��ƻ����������β��
	_scout_snake.push_front(new_tail);

	//��̽·�߳���ƻ���󣬿��ԳԵ��Լ���β�ͣ���·�߿���
	if (find_path(true, _scout_snake.back(), _scout_snake.front(), _scout_snake)) {
		return true;
	}
	//system("PAUSE");
	return false;
}

//������ڣ��������С�����
bool AI::in_advanced()
{
	if (_snake.body().size() > (BoardSize - 2)*(BoardSize - 2)/2) {
		return true;
	}
	return false;
}