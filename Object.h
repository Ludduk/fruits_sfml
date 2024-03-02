#pragma once
#include "General.h"

class Object //���������� ������, ������� ������ �������� ����
{
	float health;

	static const int states_count = 2;
	bool* states = new bool[states_count] {false, false}; //���������, � ������� ����� ���������� ������
public:
	const float MASS;
	const float MAX_HEALTH;
	int x, y;

	enum OBJECT_STATES //������������, ��������� ��� �������� ������������� ���������
	{
		GET_DAMAGE,
		DESTROYED
	};

	Object(float mass_, float max_health_);

	virtual void set_state(int number, bool val);

	virtual bool get_state(int number);

	virtual const bool* get_all_states();

	virtual int get_states_count();

	virtual bool is_alive();

	~Object();
};

class Fruit : public Object //����� ���� �� ����
{
	float health;

	static const int states_count = 9;
	bool* states = new bool[states_count];
public:
	//������� �����, ��� ������ �����, ��� ������������ ���������(�� ���� ��� ������������� ����� � ��������, ���� ����� � ����������)
	enum FRUIT_STATES
	{
		MOVE_DOWN,
		MOVE_UP,
		MOVE_LEFT,
		MOVE_RIGHT,
		STAND_LEFT,
		STAND_RIGHT,
		HIT_LEFT,
		HIT_RIGHT,
		DEAD
	};
	const float STRENGTH;

	Fruit(float mass_, float max_health_, float strength_);

	~Fruit();
};