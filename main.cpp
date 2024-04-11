//файл - основа программы - движок.

#include "Actor.h"
#include <vector>
#include <math.h>
#include <algorithm>
using namespace std;
using namespace sf;

vector<Fruit*> fruits;
vector<Object*> objects;

bool create_fruit(string name, vector<Fruit*>& fruits);

int FRAME_LIM = 60;
float TIME_CONSTANT_MIL = FRAME_LIM / 1000.f;
int WIDTH = 1000, HEIGHT = 800;
int PLAYER_ID = 0;
Fruit* PLAYER_PTR = nullptr;

//обработка нажатий и поведения персов в целом
namespace Controller
{
    enum KEYS
    {
        W,
        A,
        S,
        D,
        SPACE,
        ENTER
    };

    
    bool* pressed_keys = new bool[ENTER + 1];
    
    int get_dir_x() { return pressed_keys[D] - pressed_keys[A]; }

    int get_dir_y() { return pressed_keys[W] - pressed_keys[S]; }
 
    void refresh_states()
    {
        auto* s = PLAYER_PTR->get_states_ptr();

        if (pressed_keys[D] xor pressed_keys[A])
        {
            s->set_state("right_stand", false);
            s->set_state("left_stand", false);
            if (pressed_keys[D])
            {
                s->set_state("left_walk", false);
                s->set_state("right_walk", true);
            }
            else if (pressed_keys[A])
            {
                s->set_state("right_walk", false);
                s->set_state("left_walk", true);
            }
        }
        else
        {
            if (pressed_keys[W] xor pressed_keys[S])
            {
                if (s->get_state("right_stand"))
                {
                    s->set_state("right_walk", true);
                }
                else if (s->get_state("left_stand"))
                {
                    s->set_state("left_walk", true);
                }
            }
            else
            {
                if (!pressed_keys[D] and s->get_state("right_walk"))
                {
                    s->set_state("right_walk", false);
                    s->set_state("right_stand", true);
                    s->set_state("left_stand", false);
                }
                if (!pressed_keys[A] and s->get_state("left_walk"))
                {
                    s->set_state("left_walk", false);
                    s->set_state("left_stand", true);
                    s->set_state("right_stand", false);
                }
            }
        }

        if (pressed_keys[int(KEYS::ENTER)])
        {
            if (s->get_state("right_stand") or s->get_state("right_walk"))
            {
                 s->set_state("right_hit", true);
                 s->set_state("left_hit", false);
            }
            else if (s->get_state("left_stand") or s->get_state("left_walk"))
            {
                s->set_state("left_hit", true);
                s->set_state("right_hit", false);
            }
        }
    }

    void refresh_keys()
    {
        pressed_keys[int(KEYS::W)] = Keyboard::isKeyPressed(Keyboard::W);
        pressed_keys[int(KEYS::A)] = Keyboard::isKeyPressed(Keyboard::A);
        pressed_keys[int(KEYS::S)] = Keyboard::isKeyPressed(Keyboard::S);
        pressed_keys[int(KEYS::D)] = Keyboard::isKeyPressed(Keyboard::D);
        pressed_keys[int(KEYS::SPACE)] = Keyboard::isKeyPressed(Keyboard::Space);
        pressed_keys[int(KEYS::ENTER)] = Keyboard::isKeyPressed(Keyboard::Enter);

        refresh_states();
    }

 }



//обработка движений объектов, попаданий, всё такое прочее
namespace Physics
{
    const float MOVE_FORCE = 5.f;
    float rad(Vector2f vec)
    {
        return sqrt(vec.x*vec.x + vec.y*vec.y);
    }

    void norm(Vector2f& vec)
    {
        float r = rad(vec);
        if (r > -eps and r < eps)
            return;
        vec.x /= r;
        vec.y /= r;

        vec *= r;
    }
    const float friction = 0.9f;
    float coef = 1.f;
    void move(Body* b, Time dt, Vector2f force = Vector2f(0.f, 0.f))
    {
        coef = 1.f;
        float dt_ = dt.asMilliseconds() * TIME_CONSTANT_MIL;

        auto corr_force = force -friction * b->vel;
        b->acc = corr_force / b->mass;
        b->vel += b->acc * dt_;

        if (abs((abs(force.x) - abs(force.y)) < eps))
            coef = 1 / SQRT2;
        
        b->coord += b->vel * coef * dt_;

        if (rad(b->vel) > -eps and rad(b->vel) < eps)
            b->vel = Vector2f(0.f, 0.f);
    }
    void hit(Body b, float strength);
    void set_coord(Body b, Vector2f coord);
    void set_vel(Body b, Vector2f vel);
    void set_acc(Body b, Vector2f acc);

    float dist(Body f, Body s);

    bool can_move(Body b, Vector2f dvec);
    bool can_hit(Body f, Body s);

    //пока сомнительная вышла идея, но пусть будет
    string dir(Body* b)
    {
        if (b->acc.x > 2 * eps)
            return "right";
        else if (b->acc.x < (-2)*eps)
            return "left";
        return "stands";
    }

    void update(Time dt)
    {
        for (auto el : fruits)
        {
            auto body = el->get_body_ptr();
            float mass = body->mass;
            if (el == PLAYER_PTR)
                move(body, dt, MOVE_FORCE * Vector2f(Controller::get_dir_x(), -Controller::get_dir_y()));
            else
                move(body, dt);
        }
    }
}
//ну, очевидно, звуки
namespace Audio {} //надеюсь, я до тебя доберусь

//отвечает за обновление спрайтов
namespace Graphics 
{
    RectangleShape rectangle(Vector2f(0, 0));
    RectangleShape hit(Vector2f(0, 0));

    bool coord_compare(Fruit* f, Fruit* s)
    {
        auto f_body = f->get_body_ptr(),
             s_body = s->get_body_ptr();
        return f_body->coord.y + f_body->area.top + f_body->area.height < s_body->coord.y + s_body->area.top + s_body->area.height;
    }
   
    
    void update(RenderWindow& win, Time dt)
    {
        sort(fruits.begin(), fruits.end(), coord_compare);
        Sprite* sprite;
        win.clear(Color::White);
        for (auto el : fruits)
        {
            sprite = el->get_sprite_ptr();
            auto anim = el->get_anim_ptr();
            auto states = el->get_states_ptr();
            auto body = el->get_body_ptr();
            if (anim->get_end_anim())
                states->set_state(anim->get_current_anim_name(), false);

            if (states->change)
            {
                states->change = false;
                string curr_name = states->get_most_priority_active_state().first;
                if (curr_name.compare(anim->get_current_anim_name()) != 0)
                    anim->switch_animation(curr_name);
            }
            anim->Update(dt);
            sprite->setPosition(body->coord);
            win.draw(*sprite);
            

            if (states->get_state("right_hit"))
            {
                hit.setPosition(Vector2f(body->area.width, body->area.height - el->hitting_area.y) + sprite->getPosition() + Vector2f(body->area.left, body->area.top));
                hit.setSize(Vector2f(el->hitting_area));
                hit.setFillColor(Color(255, 0, 0, 128));
                win.draw(hit);
            }
            else if (states->get_state("left_hit"))
            {
                hit.setPosition(Vector2f(-el->hitting_area.x, body->area.height - el->hitting_area.y) + sprite->getPosition() + Vector2f(body->area.left, body->area.top));
                hit.setSize(Vector2f(el->hitting_area));
                hit.setFillColor(Color(255, 0, 0, 128));
                win.draw(hit);
            }
            rectangle.setPosition(sprite->getPosition() + Vector2f(body->area.left, body->area.top));
            rectangle.setSize(Vector2f(body->area.width, body->area.height));
            rectangle.setFillColor(Color(0, 255, 0, 128));
            win.draw(rectangle);
       
        }
    }
}

//создаётся окно, начало программы
int main()
{
    RenderWindow win(VideoMode(WIDTH, HEIGHT), "Fruits");
    win.setFramerateLimit(FRAME_LIM);

    create_fruit("APPLE", fruits);
    create_fruit("BANANA", fruits);

    fruits[0]->get_states_ptr()->set_state("right_stand", true);
    PLAYER_PTR = fruits[0];
    fruits[1]->get_body_ptr()->coord = Vector2f(100.f, 50.f);
    fruits[1]->get_states_ptr()->set_state("right_walk", true);
    
    Clock frame_time;
    frame_time.restart();


    while(win.isOpen())
    {
       Event ev;
       while(win.pollEvent(ev))
       {
           if (ev.type == Event::Closed)
               win.close();
       }
       Controller::refresh_keys();
       Graphics::update(win, frame_time.getElapsedTime());
       Physics::update(frame_time.getElapsedTime());
       frame_time.restart();
       win.display();
    }
    return 0;
}

