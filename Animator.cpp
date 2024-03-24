#include "Animator.h"

Animator::Animator(sf::Sprite* sprite_) : current_animation(nullptr), sprite(sprite_) {}

Animator::Animation& Animator::create_animation(std::string const& name_,
	std::string const& texture_name_, sf::Time const& duration_, bool loop_)
{
	animations.emplace_back(name_, texture_name_, duration_, loop_);

	if (current_animation == nullptr) switch_animation(&animations.back());

	return animations.back();
}

void Animator::switch_animation(Animator::Animation* animation)
{
	if (animation != nullptr)
	{
		sprite->setTexture(animation->texture);
	}
	current_animation = animation;
	current_time = sf::Time::Zero;
}

bool Animator::switch_animation(std::string const& name)
{
	auto animation = find_animation(name);
	if (animation != nullptr)
	{
		switch_animation(animation);
		return true;
	}
	return false;
}

Animator::Animation* Animator::find_animation(std::string const& name)
{
	for (auto it = animations.begin(); it != animations.end(); ++it)
	{
		if (it->name == name) return &*it;
	}

	return nullptr;
}

std::string Animator::get_current_anim_name() const
{
	if (current_animation != nullptr) return current_animation->name;
	return "";
}

void Animator::restart()
{
	current_time = sf::Time::Zero;
	end_anim = false;
}

void Animator::Update(sf::Time const& dt)
{
	if (current_animation == nullptr) return;

	current_time += dt;

	float scaledTime = (current_time.asSeconds() / current_animation->duration.asSeconds());
	auto numFrames = static_cast<int>(current_animation->frames.size());
	auto currentFrame = static_cast<int>(scaledTime * numFrames);

	if (current_animation->looping) currentFrame %= numFrames;
	else if (currentFrame >= numFrames) { currentFrame = numFrames - 1; end_anim = true; }

	sprite->setTextureRect(current_animation->frames[currentFrame]);
}

bool Animator::get_end_anim() const
{
	return end_anim;
}

sf::Sprite* Animator::get_sprite_ptr() { return sprite; }

Animator::~Animator()
{}
