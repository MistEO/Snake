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
		Point dict;
		if (a.find_path() 
			&& a.scout_move()) {
			dict = a.get_dict();
			while (dict != Zero) {
				while (!s.move(dict))
					;
				dict = a.get_dict();
			}
		}
		else {
			dict = a.wander();
			while (!s.move(dict))
				;
		}
	}
	while (true) {
		Point dict = a.wander();
		while (!s.move(dict))
			;
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