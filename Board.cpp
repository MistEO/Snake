#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "Board.h"

Board::Board()
	: e(time(0)), u(1, BoardSize - 2)
{
	//��ʼ��Border
	for (auto & i : _table) {
		i.assign(Blank);
		i.front() = Border;
		i.back() = Border;
	}
	_table.front().assign(Border);
	_table.back().assign(Border);

	set_apple();
	_display_border();
}

void Board::_display_border()
{
	for (int i = 0; i != _table.size(); ++i) {
		for (int j = 0; j != _table.size(); ++j) {
			if (_table[i][j] == Border) {
				gotoxy(j * 2, i);
				std::cout << "#";
			}
		}
	}
}


Board::~Board()
{
}


const Point & Board::set_apple()
{
	do {
		_apple = Point(u(e), u(e));
	} while (get(_apple) != Blank);

	get(_apple) = Apple;
	_display_apple();
	return _apple;
}

void Board::_display_apple()
{
	gotoxy(_apple.second * 2, _apple.first);
	std::cout << "��";
}

void Board::game_over()
{
	gotoxy(0, BoardSize);
	std::cout << "Game Over! Press Space to Exit";
	while (_getch() != 32)
		;
	exit(0);
}

int & Board::get(const Point & coo)
{
	return _table[coo.first][coo.second];
}

const Point & Board::apple() const
{
	return _apple;
}

//��VC��ʹ��gotoxy  
void gotoxy(int x, int y)
{
	CONSOLE_SCREEN_BUFFER_INFO         csbiInfo;
	HANDLE     hConsoleOut;

	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);

	csbiInfo.dwCursorPosition.X = x;
	csbiInfo.dwCursorPosition.Y = y;
	SetConsoleCursorPosition(hConsoleOut, csbiInfo.dwCursorPosition);
}