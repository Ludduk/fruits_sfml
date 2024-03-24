#include "Actor.h"

using namespace sf;

Fruit::Fruit(Sprite* sprite, float mass, float max_health, float strength_, Vector2f size) : anim(Animator(sprite)), 
    body(Body(mass, max_health, size)),
    strength(strength_) {}

Body* Fruit::get_body_ptr() { return &body; }

Sprite* Fruit::get_sprite_ptr() { return anim.get_sprite_ptr(); }


Fruit::~Fruit() {}


Object::Object(Sprite* sprite, float mass, float max_health, Vector2f size) : anim(Animator(sprite)), body(Body(mass, max_health, size)) {}

Object::~Object() {}
