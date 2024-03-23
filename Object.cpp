#include "Object.h"

int Object::states_count()
{
    return OBJECT_STATES::DESTROYED + 1;
}

Object::Object(float mass_, float max_health) : MASS(mass_), MAX_HEALTH(max_health)
{
    states = new bool[states_count()];
	health = MAX_HEALTH;
	x = 0;
	y = 0;
	for (int i = 0; i < states_count(); i++)
		states[i] = false;
}

Object::Object(const Object& obj) : MASS(obj.MASS), MAX_HEALTH(obj.MAX_HEALTH)
{
    states = new bool[states_count()];
    health = obj.health;
    x = obj.x;
    y = obj.y;
    
	for (int i = 0; i < states_count(); i++)
        states[i] = false;
}

void Object::hit(int damage)
{
    if (damage > 0)
        health -= damage;
}

void Object::set_state(int number, bool val)
{
	if (number >= 0 and number < states_count())
		states[number] = val;
	else
		print_info("wrong state number");
}

bool Object::get_state(int number)
{
	if (number >= 0 and number < states_count())
		return states[number];
	else
		print_info("wrong state number");
	return false;
}

const bool* Object::get_all_states()
{
	return states;
}

float Object::get_health()
{
    return health;
}

bool Object::is_alive()
{
	return health > 0;
}

Object::~Object()
{
	delete[] states;
}

int Fruit::states_count()
{
    return FRUIT_STATES::DEAD + 1;
}


void Fruit::set_state(int number, bool val)
{
	if (number >= 0 and number < states_count())
		states[number] = val;
	else
		print_info("wrong state number");
}

bool Fruit::get_state(int number)
{
	if (number >= 0 and number < states_count())
		return states[number];
	else
		print_info("wrong state number");
	return false;
}

Fruit::Fruit(float mass_, float max_health_, float strength_)
{
    states = new bool[states_count()];
    MASS = mass_;
    MAX_HEALTH = max_health_;
    STRENGTH = strength_;
	health = max_health_;
    
    for (int i = 0; i < states_count(); i++)
		states[i] = false;
}


Fruit::Fruit(const Fruit& ft)
{
    states = new bool[states_count()];
    MASS = ft.MASS;
    MAX_HEALTH = ft.MAX_HEALTH;
    STRENGTH = ft.STRENGTH;
    health = ft.health;
    x = ft.x;
    y = ft.y;
  	
    for (int i = 0; i < states_count(); i++)
        states[i] = false;
}

Fruit::~Fruit()
{
	delete[] states;
}
