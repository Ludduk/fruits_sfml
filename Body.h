#pragma once

#include <SFML/Graphics.hpp>
#include "General.h"
using namespace sf;

struct Body
{
    float health;
    bool hitted = false;
    Vector2f coord;
    Vector2f vel;
    Vector2f acc;
    Vector2f force;
    Vector2f size;
    const float mass;
    const float max_health;

    Body(float mass_ = 10.f, float max_health_ = 100.f, Vector2f size_ = Vector2f(50.f, 50.f));
    
    

    ~Body();
};
