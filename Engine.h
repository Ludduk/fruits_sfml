#pragma once
#include "Actor.h"
#include <list>

class Engine
{
	int frame_rate;
	std::list<Actor> actors; //��� ������� �� �������
	
public:
	Engine(int frame_rate_ = 60) : frame_rate(frame_rate_) {}

	void Update(); //�������� �� ���������� ���������� �� ��������

	void add_object(Fruit fruit);
};