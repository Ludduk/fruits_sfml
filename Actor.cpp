#include "Actor.h"

Actor::Actor(sf::Sprite& sprite_, TYPES type_, float mass, float max_health, float strength) : sprite(sprite_), anim(Animator(sprite_)), body(new Body(type_, mass, max_health, strength)) {}

void Actor::add_animation(std::string name, std::string path, sf::Time duration, bool looping, sf::Vector2i const& start, sf::Vector2i const& size, unsigned int frames, unsigned int lines)
{
	auto& animation = anim.create_animation(name, path, duration, looping);
	animation.add_frames(start, size, frames, lines);
	animations.push_back(animation);
}

void Actor::create_relations(std::string name, Object::OBJECT_STATES state)
{
	relations.insert(std::pair<int, std::string>(state, name));
}

void Actor::create_relations(std::string name, Fruit::FRUIT_STATES state)
{
	relations.insert(std::pair<int, std::string>(state, name));
}

Body* Actor::get_obj_ptr()
{
	return body;
}

void Actor::Update(sf::Time dt)
{
	if (body->changed)
	{
		body->changed = false;
		int i = body->get_states_count() - 1;
		while (i >= 0)
		{
			if (body->get_state(i))
			{
				anim.switch_animation(relations[i]);
				i = -1; //������� �� �����
			}
			i--;
		}
	}
	anim.Update(dt);
}

Actor::~Actor()
{
	delete body;
}