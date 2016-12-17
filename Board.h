#pragma once

#include <array>
#include <utility>
#include <random>
#include <ctime>

using Point = std::pair<int, int>;

const Point operator+(const Point & lhs, const Point & rhs);

constexpr int Blank = 0,
Apple = 1,
Body = -1,
Border = -2;

constexpr int BoardSize = 20;
constexpr int Interval = 30;

class Board
{
public:
	Board();
	~Board();

	int & get(const Point & coo);
	const Point & apple() const;
	const Point & set_apple();
	void game_over(bool win = false);
private:
	std::array<std::array<int, BoardSize>, BoardSize> _table;
	void _display_border();
	void _display_apple();
	Point _apple;
	std::default_random_engine e;
	std::uniform_int_distribution<int> u;
};

void gotoxy(int x, int y);