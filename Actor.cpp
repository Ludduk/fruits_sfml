#include "Actor.h"

using namespace sf;
using namespace std;
Fruit::Fruit(Sprite* sprite, float mass, float max_health, float strength_, Vector2f size) : anim(Animator(sprite)), 
    body(Body(mass, max_health, size)),
    strength(strength_) {}

States* Fruit::get_states_ptr() { return &rel; }

Body* Fruit::get_body_ptr() { return &body; }

Sprite* Fruit::get_sprite_ptr() { return anim.get_sprite_ptr(); }

Animator* Fruit::get_anim_ptr() { return &anim; }

Fruit::~Fruit() {}


Object::Object(Sprite* sprite, float mass, float max_health, Vector2f size) : anim(Animator(sprite)), body(Body(mass, max_health, size)) {}

Object::~Object() {}

States::States() {}

void States::create_state(string name, int priority)
{
    change = true;
    states[name] = false;
    priorities[name] = priority;
}

bool States::get_state(string name)
{
    return states[name];
}

bool States::set_state(string name, bool val)
{
    change = true;
    if (states.count(name) == 0)
        return false;
    states[name] = val;
    return true;
}

void States::reset_all()
{
    change = true;
    for (auto& el : states)
        el.second = false;
}

pair<string, bool> States::get_most_priority_state()
{
    string max_str = "";
    int max_pr = -1;
    if (priorities.size() < 1)
        return make_pair("", false);

    for (auto el : priorities)
        if (el.second > max_pr or max_pr == -1)
        {
            max_str = el.first;
            max_pr = el.second;
        }
    return make_pair(max_str, states[max_str]);
}
