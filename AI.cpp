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


int AI::find_path(bool is_scout, Point start, Point end, std::list<Point> snake)
{
	if (start == end
		&& start != Zero) {
		return 1;
	}
	if (!is_scout) {
		start = _snake.head();
		end = _board.apple();
		snake = _snake.body();
	}
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
		std::vector<AStarPoint> surround = _surround_points(temp_start, snake, end);
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
				p.H() = _calcH(end, p);
				p.F();
				_open.push_back(p);

			}
			//��_open�д����յ�,�����ҵ�·��������ѭ��
			if (list_exist_point(_open, end)) {
				complete_flag = true;
				break;
			}
		}	//end for
	}	//end while

	//δ�ҵ�·��ʱ����������С�����������
	if (_open.empty()) {
		return 0;
	}
	if (is_scout) {
		return 1;
	}
	return _export_path(_open.back());
}

std::vector<AStarPoint> AI::_surround_points(AStarPoint center, std::list<Point> snake, Point target)
{
	std::vector<AStarPoint> sur;
	std::vector<Point> temp(4);

	temp[0] = center + Left;
	temp[1] = center + Up;
	temp[2] = center + Right;
	temp[3] = center + Down;

	for (auto p : temp) {
		if (p == target 
			|| (std::find(snake.begin(), snake.end(), p) == snake.end()	//�õ㲻Ϊ����
			&& _board.get(p) != Border))								//�Ҳ�Ϊ�߽�
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
		return Down;
	}
}


bool AI::scout_move()
{
	_scout_snake = _snake.body();
	for (auto p : _path) {
		_scout_snake.push_back(p);
		_scout_snake.pop_front();
	}
	if (find_path(true, _scout_snake.back(), _scout_snake.front(), _scout_snake)) {
		return true;
	}
	return false;
}