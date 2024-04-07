#include "Actor.h"

using namespace sf;
using namespace std;
Fruit::Fruit(Sprite* sprite, float mass, float max_health, float strength_, IntRect area, Vector2i hitting_area_) : anim(Animator(sprite)), 
    body(Body(mass, max_health, area)),
    strength(strength_), hitting_area(hitting_area_) {}

States* Fruit::get_states_ptr() { return &rel; }

Body* Fruit::get_body_ptr() { return &body; }

Sprite* Fruit::get_sprite_ptr() { return anim.get_sprite_ptr(); }

Animator* Fruit::get_anim_ptr() { return &anim; }

bool Fruit::operator > (Fruit& frt)
{
    return body.coord.y + body.area.top + body.area.height > frt.body.coord.y + frt.body.area.top + frt.body.area.height;
}

Fruit::~Fruit() {}


Object::Object(Sprite* sprite, float mass, float max_health, IntRect area) : anim(Animator(sprite)), body(Body(mass, max_health, area)) {}

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
    if (states.count(name) == 0)
        return false;
    if (val != states[name])
    {
        states[name] = val;
        change = true;
        return true;
    }
    return false;
}

void States::reset_all()
{
    change = true;
    for (auto& el : states)
        el.second = false;
}

pair<string, bool> States::get_most_priority_active_state()
{
    string max_str = "";
    int max_pr = -1;
    if (priorities.size() < 1)
        return make_pair("", false);

    for (auto el : priorities)
        if ((el.second > max_pr or max_pr == -1) and states[el.first])
        {
            max_str = el.first;
            max_pr = el.second;
        }
    return make_pair(max_str, states[max_str]);
}
