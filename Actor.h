#include "Body.h"
#include "Animator.h"

using namespace sf;

class Fruit
{
    Body body;
    Animator anim;
    float strength;
public:
    
    Fruit(Sprite* sprite, float mass = 10.f, float max_health = 100.f, float strength = 25.f, Vector2f size = Vector2f(50.f, 50.f));

    Body* get_body_ptr();

    Sprite* get_sprite_ptr();
    ~Fruit();

};

class Object
{
    Body body;
    Animator anim;
public:

    Object(Sprite* sprite, float mass = 10.f, float max_health = 100.f, Vector2f size = Vector2f(50.f, 50.f));

    ~Object();
};
