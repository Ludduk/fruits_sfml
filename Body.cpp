
#include "Body.h"
/*
template<class T>
Body<T>::Body(TYPES type, float mass, float max_health, float strength)
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

template<class T>
bool Body<T>::get_state(int number)
{
	if (number >= 0 and number < t.get_states_count())
		return t.get_state(number);
	print_info("wrong number");
	return false;
}

template<class T>
const bool* Body<T>::get_all_state()
{
	return t.get_all_states();
}

template<class T>
int Body<T>::get_states_count() 
{
	return t.get_states_count();
}

template<class T>
void Body<T>::set_state(int number, bool val) 
{
	if (number >= 0 and number < t.get_states_count())
	{
		t.set_state(number, val);
		changed = true;
	}
	else
		print_info("wrong number of state");
}

template<class T>
void Body<T>::set_full_state(bool* states, int size) 
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

template<class T>
bool Body<T>::is_alive()
{
	return t.is_alive();
}

template<class T>
void Body<T>::move(int dx, int dy)
{
	t.x += dx;
	t.y += dy;
}

template<class T>
void Body<T>::set_coord(int new_x, int new_y)
{
	t.x = new_x;
	t.xt.y = new_y;
}
template<class T>
Body<T>::~Body() {}
*/
