#pragma once

#include "General.h"

class Object //терпильный объект, который просто получает урон
{
	float health;

	static const int states_count = 2;
	bool* states = new bool[states_count] {false, false}; //состояние, в которых может находиться объект
public:
	float MASS;
	float MAX_HEALTH;
	int x, y;

	enum OBJECT_STATES //перечисление, созданное для удобства использования состояний
	{
		GET_DAMAGE,
		DESTROYED
	};

	Object(float mass_ = 1.f, float max_health_ = 100.f);

    Object(const Object& obj);

	virtual void set_state(int number, bool val);

	virtual bool get_state(int number);

	virtual const bool* get_all_states();

	virtual int get_states_count();

	virtual bool is_alive();

    Object& operator = (const Object& obj)
    {
        if (this == &obj)
            return *this;
        
        MASS = obj.MASS;
        MAX_HEALTH = obj.MAX_HEALTH;
        x = obj.x;
        y = obj.y;
        health = obj.health;
        return *this;
    }

	~Object();
};

class Fruit : public Object //может дать по роже
{
	float health;

	static const int states_count = 9;
	bool* states = new bool[states_count];
public:
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
	float STRENGTH, MASS, MAX_HEALTH;

	Fruit(float mass_ = 1.f, float max_health_ = 100.f, float strength_ = 50.f);

    Fruit(const Fruit& ft);

    Fruit& operator = (const Fruit& ft)
    {
        if (this == &ft)
            return *this;
        
        MASS = ft.MASS;
        MAX_HEALTH = ft.MAX_HEALTH;
        
        x = ft.x;
        y = ft.y;
        health = ft.health;
        STRENGTH  = ft.STRENGTH;
        return *this;
    }
	~Fruit();
};
