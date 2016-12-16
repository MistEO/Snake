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
	AI a(b, s);
	while (!a.in_advanced()) {
		if (a.find_path() 
			&& a.scout_move()) {
			while (a.get_dict() != Zero) {
				s.move(a.get_dict());
			}
		}
		else {
			s.move(a.wander());
		}
	}
	while (true) {
		s.move(a.wander());
	}
}

void play()
{
	Board b;
	Snake s(b);
	char op;
	Point dict = Right;
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