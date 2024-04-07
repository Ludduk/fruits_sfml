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
    IntRect area;
    const float mass;
    const float max_health;

    Body(float mass_ = 10.f, float max_health_ = 100.f, IntRect area_ = IntRect(0, 0, 50, 50));
    
    

    ~Body();
};
