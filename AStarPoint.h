#pragma once
#include "Board.h"

#include <memory>

class AStarPoint
{
public:
	AStarPoint()
		:x(-1), y(-1) {}
	AStarPoint(Point p);
	~AStarPoint();
	std::shared_ptr<AStarPoint> & parent();
	int & F();
	int & G();
	int & H();

	const int x;
	const int y;

	const bool operator<(AStarPoint & rhs);
	operator Point();
	//const bool operator==(AStarPoint & rhs);
private:
	int _f = 0;	//���ۺķ�, F=G+H
	int _g = 0;	//������ƶ��ķ�
	int _h = 0;	//���յ�Ԥ�ƺķ�
	std::shared_ptr<AStarPoint> _parent_point = nullptr;
};