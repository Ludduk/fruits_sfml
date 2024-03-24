#pragma once
#include <string>
#include <vector>
#include <list>
//#include "General.h"
#include <SFML/Graphics.hpp>
#include <iostream>
class Animator
{
public:
	struct Animation
	{
		std::string name;
		std::string texture_name;
		std::vector<sf::IntRect> frames;
		sf::Time duration;
		sf::Texture texture;
		bool looping;

		Animation(std::string const& name_, std::string const& texture_name_, sf::Time const& duration_, bool looping_) : 
			name(name_), texture_name(texture_name_),
			duration(duration_), looping(looping_) 
		{
			if (!texture.loadFromFile(texture_name_))
			{
				std::cout << "texture fail\n";
			}
		}

		void add_frames(sf::Vector2i const& start, sf::Vector2i const& frame_size, unsigned int frames_count, unsigned int rankes)
		{
			sf::Vector2i current = start;

			for (unsigned int i = 0; i < rankes; i++)
			{
				for (unsigned int j = 0; j < frames_count; j++)
				{
					frames.push_back(sf::IntRect(current.x, current.y, frame_size.x, frame_size.y));
					current.x += frame_size.x;
				}
				current.y += frame_size.y;
				current.x = start.x;
			}
		}
	};
	explicit Animator(sf::Sprite* sprite_);

	Animation& create_animation(std::string const& name,
		std::string const& texture_name, sf::Time const& duration,
		bool loop = false);

	void Update(sf::Time const& dt);

	bool switch_animation(std::string const& name);

	std::string get_current_anim_name() const;

	void restart();

	bool get_end_anim() const;

    sf::Sprite* get_sprite_ptr();

    ~Animator();

private:
    Animation* find_animation(std::string const& name);
    void switch_animation(Animation* animation);
    sf::Sprite* sprite;
    sf::Time current_time;
    std::list<Animation> animations;
    Animation* current_animation;
    bool end_anim = false;
};
