#include "Animator.h"
#include "Actor.h"
#include <math.h>

float get_rad(sf::Vector2f const vector)
{
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);;
}

sf::Vector2f normalization(sf::Vector2f const vector)
{
    sf::Vector2f norm_vector = vector;
    float rad = get_rad(norm_vector);
    if (rad != 0)
    {
        norm_vector.x /= rad;
        norm_vector.y /= rad;
        return norm_vector;
    }
    return vector;
}

sf::Vector2f rading_vector(sf::Vector2f const vector)
{
    sf::Vector2f norm_vector = vector;
    float rad = get_rad(norm_vector);
    if (rad != 0)
    {
        norm_vector.x /= rad;
        norm_vector.y /= rad;
        return rad * norm_vector;
    }
    return vector;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 800), "My window");

    sf::Sprite sprite;

    sprite.setPosition(sf::Vector2f(500.f, 400.f));
    Actor<Fruit> actor(sprite, TYPES::FRUIT, 10.f, 100.f, 50.f);
   
    actor.add_animation("walk_right", APPLE_ANIMATIONS_PATH, sf::seconds(0.8), true, sf::Vector2i(0, 0), sf::Vector2i(80, 48), 6, 1);

    actor.create_relations("walk_right", Fruit::FRUIT_STATES::MOVE_RIGHT);

    actor.add_animation("walk_left", APPLE_ANIMATIONS_PATH, sf::seconds(0.8), true, sf::Vector2i(0, 49), sf::Vector2(80, 48), 6, 1);

    actor.create_relations("walk_left", Fruit::FRUIT_STATES::MOVE_LEFT);

    actor.add_animation("stand_left", APPLE_ANIMATIONS_PATH, sf::seconds(0.8), true, sf::Vector2i(0, 4 * 48 + 1), sf::Vector2(80, 48), 4, 1);

    actor.create_relations("stand_left", Fruit::FRUIT_STATES::STAND_LEFT);

    sf::Clock clock;
    clock.restart();
   
    auto a = actor.get_obj_ptr();
    a->set_state(Fruit::FRUIT_STATES::MOVE_LEFT, true);
    a->set_state(Fruit::FRUIT_STATES::MOVE_RIGHT, false);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::D:
                    actor.get_obj_ptr()->set_state(Fruit::FRUIT_STATES::MOVE_RIGHT, true);
                    actor.get_obj_ptr()->set_state(Fruit::FRUIT_STATES::MOVE_LEFT, false);
                    actor.get_obj_ptr()->set_state(Fruit::FRUIT_STATES::STAND_LEFT, false);
                    break;
                case sf::Keyboard::A:
                    actor.get_obj_ptr()->set_state(Fruit::FRUIT_STATES::MOVE_LEFT, true);
                    actor.get_obj_ptr()->set_state(Fruit::FRUIT_STATES::MOVE_RIGHT, false);
                    actor.get_obj_ptr()->set_state(Fruit::FRUIT_STATES::STAND_LEFT, false);
                    break;
                case sf::Keyboard::W:
                    actor.get_obj_ptr()->set_state(Fruit::FRUIT_STATES::STAND_LEFT, true);
                    actor.get_obj_ptr()->set_state(Fruit::FRUIT_STATES::MOVE_RIGHT, false);                   
                    actor.get_obj_ptr()->set_state(Fruit::FRUIT_STATES::MOVE_LEFT, false);                   
                    break;
                case sf::Keyboard::S:
                    
                    break;
                default:
                    break;
                }
            }
        }
        window.clear();
        actor.Update(clock.restart());
        window.draw(sprite);
        window.display();
    }

    return 0;
}

//int main()
//{
//    const int FRAME_RATE = 60;
//    const float FRAME_TIME_MILL = 1000.f / FRAME_RATE;
//    sf::RenderWindow window(sf::VideoMode(1000, 800), "My window");
//    window.setFramerateLimit(FRAME_RATE);
//    window.setVerticalSyncEnabled(true);
//
//    sf::Time apple_time = sf::Time::Zero;
//    sf::Time time = sf::Time::Zero;
//    sf::Time delta_time;
//    sf::Clock clock;
//    
//    Animator anim(apple);
//
//    auto& walk_right_anim = anim.create_animation("walk_right", "images\\apple_animation.png", sf::seconds(0.5), true);
//    walk_right_anim.add_frames(sf::Vector2i(0, 0), sf::Vector2i(48, 48), 6, 1);
//
//    auto& stand_right_anim = anim.create_animation("stand_right", "images\\apple_animation.png", sf::seconds(1), true);
//    stand_right_anim.add_frames(sf::Vector2i(0, 96), sf::Vector2i(64, 48), 5, 1);
//
//    auto& walk_left_anim = anim.create_animation("walk_left", "images\\apple_animation.png", sf::seconds(0.5), true);
//    walk_left_anim.add_frames(sf::Vector2i(0, 48), sf::Vector2i(48, 48), 6, 1);
//
//    auto& stand_left_anim = anim.create_animation("stand_left", "images\\apple_animation.png", sf::seconds(1), true);
//    stand_left_anim.add_frames(sf::Vector2i(0, 144), sf::Vector2i(64, 48), 5, 1);
//
//
//    sf::Vector2f position = sf::Vector2f(500.f, 400.f);
//    apple.setPosition(position);
//    apple.setScale(2, 2);
//    anim.switch_animation("stand_right");
//    anim.Update(sf::seconds(0));
//
//    sf::Vector2f vec = sf::Vector2f(0.f, 0.f);
//    sf::Vector2f vel = sf::Vector2f(0.f, 0.f);
//    sf::Vector2f acel = sf::Vector2f(0.f, 0.f);
//    sf::Vector2f force = sf::Vector2f(0.f, 0.f);
//    sf::Vector2f frac = sf::Vector2f(0.f, 0.f);
//
//    const float mass = 10.f;
//    
//    const float FRAC_COEF = 0.7f;
//    //const float MAX_VEL = 5.f;
//    const float DELTA_FORCE = 2.f;
//    //std::cout << MAX_VEL << '\n';
//
//    bool D_pressed = false,
//         A_pressed = false,
//         W_pressed = false,
//         S_pressed = false;
//    
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//            else if (event.type == sf::Event::KeyPressed)
//            {
//                switch (event.key.code)
//                {
//                case sf::Keyboard::D:
//                    D_pressed = true;
//                    break;
//                case sf::Keyboard::A:
//                    A_pressed = true;
//                    break;
//                case sf::Keyboard::W:
//                    W_pressed = true;
//                    break;
//                case sf::Keyboard::S:
//                    S_pressed = true;
//                    break;
//                default:
//                    break;
//                }
//            }
//            else if (event.type == sf::Event::KeyReleased)
//            {
//                switch (event.key.code)
//                {
//                case sf::Keyboard::D:
//                    D_pressed = false;
//                    break;
//                case sf::Keyboard::A:
//                    A_pressed = false;
//                    break;
//                case sf::Keyboard::W:
//                    W_pressed = false;
//                    break;
//                case sf::Keyboard::S:
//                    S_pressed = false;
//                    break;
//                default:
//                    break;
//                }
//            }
//        }
//        delta_time = clock.restart();
//        apple_time += delta_time;
//
//        std::cout << get_rad(vel) << '\n';
//        { // APPLE BLOCK
//            force.x = DELTA_FORCE * (D_pressed - A_pressed);
//            force.y = DELTA_FORCE * (S_pressed - W_pressed);
//
//            force += -FRAC_COEF * vel;
//
//            acel = force / mass;
//
//            //acel = normalization(acel) * DELTA_FORCE / mass;
//
//            vel += acel * static_cast<float>(apple_time.asMilliseconds() / (FRAME_TIME_MILL));
//            // эту строку нужно добавить для реализации "угла над землёй"
//            //vel.y /= 1.2f; 
//
//            if (get_rad(vel) <= 0.1f)
//                vel = sf::Vector2f(0.f, 0.f);
//            
//            position += vel * static_cast<float>(apple_time.asMilliseconds() / (FRAME_TIME_MILL));
//
//            apple.setPosition(position);
//            { //ANIMATION
//                std::string anim_name = anim.get_current_anim_name();
//                if (acel.x > 0 and anim_name.compare("walk_right"))
//                    anim.switch_animation("walk_right");
//                else if (acel.x < 0 and anim_name.compare("walk_left"))
//                    anim.switch_animation("walk_left");
//                else if (acel.x == 0 and acel.y == 0 and (anim_name.compare("stand_left") and anim_name.compare("stand_right")))
//                    anim.switch_animation(!anim_name.compare("walk_left") ? "stand_left" : "stand_right");
//                else if (acel.x == 0 and acel.y != 0 and (anim_name.compare("walk_left") and anim_name.compare("walk_right")))
//                    anim.switch_animation(!anim_name.compare("stand_left") ? "walk_left" : "walk_right");
//
//                anim.Update(sf::seconds(apple_time.asSeconds()));
//            }
//            
//            apple_time = sf::seconds(0);
//        }
//        window.clear(sf::Color::White);
//
//        window.draw(apple);
//
//        window.display();
//    }
//
//
//    return 0;
//}
