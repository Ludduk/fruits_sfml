//файл - основа программы - движок.

#include "Actor.h"
#include <vector>
#include <math.h>
using namespace std;
using namespace sf;

vector<Fruit*> fruits;
vector<Object*> objects;

bool create_fruit(string name, vector<Fruit*>& fruits);

int FRAME_LIM = 60;
float TIME_CONSTANT_MIL = FRAME_LIM / 1000.f;
int WIDTH = 1000, HEIGHT = 800;
int PLAYER_ID = 0;

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
        auto* s = fruits[PLAYER_ID]->get_states_ptr();
        if (pressed_keys[D] xor pressed_keys[A])
        {
            if (pressed_keys[D])
            {
                s->set_state("left_walk", false);
                s->set_state("right_walk", true);
            }
            else
            {
                s->set_state("right_walk", false);
                s->set_state("left_walk", true);
            }
        }
        else if (!pressed_keys[W] and !pressed_keys[S])
        {
            if (s->get_state("right_walk"))
            {
                s->set_state("right_walk", false);
                s->set_state("left_walk", false);
                s->set_state("left_stand", false);
                s->set_state("right_stand", true);
            }
            else if (s->get_state("left_walk"))
            {
                s->set_state("right_walk", false);
                s->set_state("left_walk", false);
                s->set_state("right_stand", false);
                s->set_state("left_stand", true);
            }
        }
        else if (pressed_keys[W] or pressed_keys[S])
        { 
            if (s->get_state("right_walk"))
            {
                s->set_state("right_walk", true);
            }
            else if (s->get_state("left_walk"))
            {
                s->set_state("left_walk", true);
            }
        }

        if (pressed_keys[ENTER] and (s->get_state("right_stand") and s->get_state("right_walk")))
            s->set_state("right_hit", true);
        else if (pressed_keys[ENTER] and (s->get_state("left_stand") and s->get_state("left_walk")))
            s->set_state("left_hit", true);
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
    const float MOVE_FORCE = 7.f;
    float rad(Vector2f& vec)
    {
        return sqrt(vec.x*vec.x + vec.y*vec.y);
    }
    const float friction = 0.7f;
    void move(Body* b, Time dt, Vector2f force = Vector2f(0.f, 0.f))
    {
        float dt_ = dt.asMilliseconds() * TIME_CONSTANT_MIL;

        force += -friction * b->vel;

        b->acc = force / b->mass;
        b->vel += b->acc * dt_;
        b->coord += b->vel * dt_;
        
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
        int id = 0;
        for (auto el : fruits)
        {
            float mass = el->get_body_ptr()->mass;
            if (id == PLAYER_ID)
                move(el->get_body_ptr(), dt, MOVE_FORCE * Vector2f(Controller::get_dir_x(), -Controller::get_dir_y()));
            else
                move(el->get_body_ptr(), dt);
            id++;
        }
    }
}
//ну, очевидно, звуки
namespace Audio {} //надеюсь, я до тебя доберусь

//отвечает за обновление спрайтов
namespace Graphics 
{
    void end_state(Fruit* fruit)
    {
        auto* s = fruit->get_states_ptr();
        auto* a = fruit->get_anim_ptr();

        if (a->get_end_anim())
        {
            s->reset_all();
            //s->set_state(a->get_current_anim_name(), false);
            Controller::refresh_states();
        }
        s->change = false;
    }
    void update(RenderWindow& win, Time dt)
    {
        Sprite* tmp;
        win.clear();
        for (auto el : fruits)
        {
            tmp = el->get_sprite_ptr();
            if (el->get_states_ptr()->change)
                el->get_anim_ptr()->switch_animation(el->get_states_ptr()->get_most_priority_state().first);
            el->get_anim_ptr()->Update(dt);
            end_state(el);
            tmp->setPosition(el->get_body_ptr()->coord);
            win.draw(*tmp);
        }
        win.display();
    }
}

//создаётся окно, начало программы
int main()
{
    RenderWindow win(VideoMode(WIDTH, HEIGHT), "Fruits");
    win.setFramerateLimit(FRAME_LIM);

    create_fruit("APPLE", fruits);
    create_fruit("APPLE", fruits);

    fruits[1]->get_body_ptr()->coord = Vector2f(100.f, 50.f);
    fruits[1]->get_anim_ptr()->switch_animation("right_stand");
    fruits[1]->get_states_ptr()->change = false;
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

