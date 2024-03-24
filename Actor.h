#include "Body.h"
#include "Animator.h"
#include <vector>
#include <map>

using namespace sf;
using namespace std;

class States
{   
    map<string, bool> states;
    map<string, int> priorities;
public:
    bool change = false;
    States();

    void create_state(string name, int priority);

    bool get_state(string name);

    bool set_state(string name, bool val);

    void reset_all();

    pair<string, bool> get_most_priority_state();

};

class Fruit
{
    Body body;
    Animator anim;
    float strength;
    States rel;
public:
    
    Fruit(Sprite* sprite, float mass = 10.f, float max_health = 100.f, float strength = 25.f, Vector2f size = Vector2f(50.f, 50.f));

    States* get_states_ptr();

    Body* get_body_ptr();

    Sprite* get_sprite_ptr();

    Animator* get_anim_ptr();

    ~Fruit();

};

class Object
{
    Body body;
    Animator anim;
    States rel;
public:

    Object(Sprite* sprite, float mass = 10.f, float max_health = 100.f, Vector2f size = Vector2f(50.f, 50.f));

    ~Object();
};


