#pragma once

#include "General.h"

class Object 
{
	float health;

	bool* states; 
    public:
    
	float MASS;
	float MAX_HEALTH;
	int x, y;

	enum OBJECT_STATES 
    {
		GET_DAMAGE,
		DESTROYED
	};

    virtual int states_count();

	Object(float mass_ = 1.f, float max_health_ = 100.f);

    Object(const Object& obj);

    virtual void hit(int damage);

	virtual void set_state(int number, bool val);

	virtual bool get_state(int number);

	virtual const bool* get_all_states();

    virtual float get_health();

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

class Fruit : public Object 
{
	float health;

	bool* states;
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
	float STRENGTH;

    void set_state(int number, bool val);

    bool get_state(int number);

    int states_count();

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
    //это заглушка для того, чтобы компилятор не ругался на 
    //конструктор класса Body
    Fruit& operator = (Object obj) { return *this; }

	~Fruit();
};
