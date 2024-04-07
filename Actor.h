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

    pair<string, bool> get_most_priority_active_state();

};

class Fruit
{
    Body body;
    Animator anim;
    States rel;
    
public:

    float strength;
    Vector2i hitting_area;
    Fruit(Sprite* sprite, float mass = 10.f, float max_health = 100.f, float strength_ = 25.f, IntRect area = IntRect(0, 0, 50, 50),
            Vector2i hitting_area_ = Vector2i(10, 10));

    States* get_states_ptr();

    Body* get_body_ptr();

    Sprite* get_sprite_ptr();

    Animator* get_anim_ptr();

    bool operator > (Fruit& frt);

    ~Fruit();

};

class Object
{
    Body body;
    Animator anim;
    States rel;
public:

    Object(Sprite* sprite, float mass = 10.f, float max_health = 100.f, IntRect area = IntRect(0, 0, 50, 50));

    ~Object();
};


