#include "Board.h"
#include "Snake.h"
#include "AI.h"
#include <conio.h>

void play();
void ai_play();

int main()
{
	//play();
	ai_play();
	return 0;
}


void ai_play()
{
	Board b;
	Snake s(b);
	while (true) {
		AI a(b, s);
		a.find_path();

		while (a.get_dict() != Zero) {
			s.move(a.get_dict());
		}
	}
}

void play()
{
	Board b;
	Snake s(b);
	char op;
	Point dict = Zero;
	while (true) {
		if (_kbhit()) {
			op = _getch();
			switch (op)
			{
			case 'w':
				if (dict == Down) {
					break;
				}
				dict = Up;
				break;
			case 's':
				if (dict == Up) {
					break;
				}
				dict = Down;
				break;
			case 'a':
				if (dict == Right) {
					break;
				}
				dict = Left;
				break;
			case 'd':
				if (dict == Left) {
					break;
				}
				dict = Right;
				break;
			default:
				break;
			}
		}
		s.move(dict);
	}
}