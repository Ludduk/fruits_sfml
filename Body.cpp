#include "Body.h"

Body::Body(float mass_, float max_health_, IntRect area_) : mass(mass_), max_health(max_health_), area(area_)
{
    coord = Vector2f(0.f, 0.f);
    vel = coord;
    acc = coord;
    health = max_health;
}

Body::~Body() {}
