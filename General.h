#pragma once
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
const float PI = 3.141593f;
const float eps = 0.01f;
const float SQRT2 = 1.414f;
const float SQRTSQRT2 = 1.1892f;

const std::string APPLE_ANIMATIONS_PATH = "images/apple_animation_v1_4_1.png";
const std::string TEMPLATES_PATH = "templates.txt";
void print_info(std::string msg); //процедура вывода системной инфы для программиста, может быть переделана

void print_info(float count);

void print_info(int count);
