#include "Body.h"

Body::Body(float mass_, float max_health_, Vector2f size_) : mass(mass_), max_health(max_health_), size(size_)
{
    coord = Vector2f(0.f, 0.f);
    vel = coord;
    acc = coord;
    health = max_health;
}

Body::~Body() {}
