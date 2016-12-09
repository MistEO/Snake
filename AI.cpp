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
	//ͷ������_open
	_open.push_back(_snake.head());
	_open.front().G() = 0;

	while (!_open.empty()) {

		//����_open����ֵ��С����Ϊ��ǰ��ʼ�㣬������_close
		_open.sort();
		auto temp_start = _open.front();
		_open.pop_front();
		_close.push_back(temp_start);

		//����Χ��
		std::vector<AStarPoint> surround = _surround_points(temp_start);
		for (auto & p : surround) {

			//��_close�д��ڸõ㣬������
			if (list_exist_point(_close, p)) {
				continue;
			}

			//��_open�д��ڸõ㣬�����µ�G������GС�ڵ�ǰG������¸õ�ĸ��ڵ㡣
			//�������ڣ�����¸��ڵ㲢����_open
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

		//��_open�д����յ㣨��ƻ����,�����ҵ�·��������ѭ��
		if (list_exist_point(_open, _board.apple())) {
			break;
		}
	}	//end while

	//δ�ҵ�·��ʱ����������С�����������
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
		&& _board.get(_snake.head() + Down + Down) >= Blank) {	//��ײ���һ������·��
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
	else {	//�������
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

Point AI::_determine_dict(Point next_point)
{
	int x = next_point.first - _snake.head().first;
	int y = next_point.second - _snake.head().second;
	return Point(x, y);
}