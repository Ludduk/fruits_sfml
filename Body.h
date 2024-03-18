#include "Object.h"

enum TYPES
{
	OBJECT,
	FRUIT
};

template<class T>
class Body
{
	T t;

public:

	bool changed = false;

Body(TYPES type, float mass, float max_health, float strength)
{
	if (type == TYPES::OBJECT)
	{
        t = T(mass, max_health);
	}
	else if (type == TYPES::FRUIT)
	{
	    t = T(mass, max_health, strength);
    }
	else
	{
		print_info("wrong type");
	}
}

	
bool get_state(int number)
{
	if (number >= 0 and number < t.get_states_count())
		return t.get_state(number);
	print_info("wrong number");
	return false;
}

const bool* get_all_state()
{
	return t.get_all_states();
}

int get_states_count() 
{
	return t.get_states_count();
}

void set_state(int number, bool val) 
{
	if (number >= 0 and number < t.get_states_count())
	{
		t.set_state(number, val);
		changed = true;
	}
	else
		print_info("wrong number of state");
}

void set_full_state(bool* states, int size) 
{
	if (size != t.get_states_count())
	{
		print_info("wrong size of states buf");
	}
	else
	{
		for (int i = 0; i < size; i++)
			set_state(i, states[i]);
	}
}

bool is_alive()
{
	return t.is_alive();
}

void move(int dx, int dy)
{
	t.x += dx;
	t.y += dy;
}

void set_coord(int new_x, int new_y)
{
	t.x = new_x;
	t.xt.y = new_y;
}
~Body() {}
};
