#include "Snake.h"
#include <ctime>
#include <iostream>
#include <Windows.h>
#include <assert.h>


Snake::Snake(Board & b) : _board(b)
{
	//��ʼ���ߣ�λ���������
	_body.push_back(Point(BoardSize / 2, BoardSize / 2));
	_board.get(_body.back()) = Body;

	_display();
}

Snake::~Snake()
{
}

bool Snake::move(Point dict)
{
	//���Ƿ�����
	assert(dict == Left
		|| dict == Right
		|| dict == Up
		|| dict == Down);

	//�����β������С��Interval����ܾ�����
	static clock_t last_time = clock() - Interval;
	clock_t now_time = clock();
	if ((now_time - last_time) < Interval) {
		return false;
	}
	last_time = clock();

	//�жϼ��������λ�õ����
	Point next_head(_body.back().first + dict.first,
		_body.back().second + dict.second);

	//�ж��Ƿ�Ե�ƻ��
	bool grow_flag = false;
	if (_board.get(next_head) == Apple) {
		_board.set_apple();
		grow_flag = true;
	}

	//δ�Ե�ƻ�������β�ͣ��Ե�ƻ���򲻸���β��
	Point last_tail(0, 0);
	if (!grow_flag) {
		last_tail = _body.front();
		_board.get(last_tail) = Blank;
		_body.pop_front();
	}
	if ((_board.get(next_head) == Body)
		|| _board.get(next_head) == Border) {
		_board.game_over();
	}

	//����ͷ��
	_body.push_back(next_head);
	_board.get(_body.back()) = Body;

	_display(last_tail);

	return true;
}

const Point & Snake::head() const
{
	return _body.back();
}

const Point & Snake::tail() const
{
	return _body.front();
}

const std::list<Point>& Snake::body() const
{
	return _body;
}

void Snake::_display(const Point & last_tail)
{
	//������β��ʱ�����߱䳤
	if (last_tail != Point(0, 0)) {
		gotoxy(last_tail.second * 2, last_tail.first);
		std::cout << " ";
	}

	static Point last_head = _body.back();

	//���ͷ��
	gotoxy(_body.back().second * 2, _body.back().first);
	std::cout << "��";

	//�������
	if (_body.size() > 1) {
		gotoxy(last_head.second * 2, last_head.first);
		std::cout << "��";
	}
	last_head = _body.back();
}
