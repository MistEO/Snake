#include "Snake.h"
#include <ctime>
#include <iostream>
#include <Windows.h>


Snake::Snake(Board & b) : _board(b)
{
	//初始化蛇，位于面板中心
	_body.push(Point(BoardSize / 2, BoardSize / 2));
	_board.get(_body.back()) = Body;

	_display();
}

Snake::~Snake()
{
}

bool Snake::move(Point dict)
{
	if (dict == Zero) {
		return false;
	}

	//若两次操作间隔小于Interval，则拒绝操作
	static clock_t last_time = clock() - Interval;
	clock_t now_time = clock();
	if ((now_time - last_time) < Interval) {
		return false;
	}
	last_time = clock();

	//判断即将到达的位置的情况
	Point next_head(_body.back().first + dict.first,
		_body.back().second + dict.second);
	if (_board.get(next_head) == Body
		|| _board.get(next_head) == Border) {
		_board.game_over();
	}

	//判断是否吃到苹果
	bool grow_flag = false;
	if (_board.get(next_head) == Apple) {
		_board.set_apple();
		grow_flag = true;
	}
	_body.push(next_head);
	_board.get(_body.back()) = Body;

	//未吃到苹果则更新尾巴，吃到苹果则不更新尾巴
	Point _tail(0, 0);
	if (!grow_flag) {
		_tail = _body.front();
		_board.get(_tail) = Blank;
		_body.pop();
	}
	_display(_tail);
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

void Snake::_display(const Point & last_tail)
{
	gotoxy(_body.back().second * 2, _body.back().first);
	std::cout << "■";

	//不更新尾巴时，则蛇变长
	if (last_tail != Point(0, 0)) {
		gotoxy(last_tail.second * 2, last_tail.first);
		std::cout << " ";
	}
}
