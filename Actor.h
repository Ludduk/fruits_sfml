#include <SFML/Graphics.hpp>
#include "Body.h"
#include "Animator.h"
#include <list>
#include <map>

template <class T>
class Actor //класс, с которым будет работать движок, в нём персонаж объединяется с анимациями
{
	sf::Sprite& sprite;
	Body<T> body;
	Animator anim;
	std::list<Animator::Animation> animations;
	std::map<int, std::string> relations;

public:
	
Actor(sf::Sprite& sprite_, TYPES type_, float mass, float max_health, float strength) : sprite(sprite_), anim(Animator(sprite_)), body(Body<T>(type_, mass, max_health, strength)) {}

void add_animation(std::string name, std::string path, sf::Time duration, bool looping, sf::Vector2i const& start,
        sf::Vector2i const& size, unsigned int frames, unsigned int lines)
{
	auto& animation = anim.create_animation(name, path, duration, looping);
	animation.add_frames(start, size, frames, lines);
	animations.push_back(animation);
}
void create_relations(std::string name, Object::OBJECT_STATES state)
{
	relations.insert(std::pair<int, std::string>(state, name));
}

void create_relations(std::string name, Fruit::FRUIT_STATES state)
{
	relations.insert(std::pair<int, std::string>(state, name));
}

Body<T>* get_obj_ptr()
{
	return &body;
}

void Update(sf::Time dt)
{
	if (body.changed)
	{
		body.changed = false;
		int i = body.get_states_count() - 1;
		while (i >= 0)
		{
			if (body.get_state(i))
			{
				anim.switch_animation(relations[i]);
				i = -1; //выходим из цикла
			}
			i--;
		}
	}
	anim.Update(dt);
}

~Actor() {}
};
