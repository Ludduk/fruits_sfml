//файл - основа программы - движок.

#include "Actor.h"
#include <vector>
#include <math.h>
using namespace std;
using namespace sf;

vector<Fruit> fruits;
vector<Object> objects;

bool create_fruit(string name, vector<Fruit>& fruits);

int FRAME_LIM = 60;
float TIME_CONSTANT_MIL = FRAME_LIM / 1000.f;
int WIDTH = 1000, HEIGHT = 800;

//отвечает за обновление спрайтов
namespace Graphic 
{
    void update()
    {
        for (auto& el : fruits)
        {
            el.get_sprite_ptr()->setPosition(el.get_body_ptr()->coord);
        }
    }
}

//обработка движений объектов, попаданий, всё такое прочее
namespace Physics
{
    float rad(Vector2f vec)
    {
        return sqrt(vec.x*vec.x + vec.y*vec.y);
    }
    float friction = 0.5f;
    void move(Body* b, Time dt)
    {
        b->force += -friction * b->vel;

        float dt_ = dt.asMilliseconds();
        
        b->acc = b->force / b->mass;
        b->vel += b->acc * dt_;
        b->coord += b->vel * dt_;
        
        if (rad(b->vel) >= -eps and rad(b->vel) <= eps)
            b->vel *= 0.f;
    }
    void hit(Body b, float strength);
    void set_coord(Body b, Vector2f coord);
    void set_vel(Body b, Vector2f vel);
    void set_acc(Body b, Vector2f acc);

    float dist(Body f, Body s);

    bool can_move(Body b, Vector2f dvec);
    bool can_hit(Body f, Body s);

    void update(Time dt)
    {
        for (auto& el : fruits)
        {
            move(el.get_body_ptr(), dt);
        }
    }
}
//ну, очевидно, звуки
namespace Audio {} //надеюсь, я до тебя доберусь

//обработка нажатий и поведения персов в целом
namespace Controller {}

//создаётся окно, начало программы
int main()
{
    RenderWindow win(VideoMode(WIDTH, HEIGHT), "Fruits");
    win.setFramerateLimit(FRAME_LIM);

    create_fruit("APPLE", fruits);
    fruits[0].add_animation("walk_right");

    Clock clock;
    clock.restart();

    while(win.isOpen())
    {
       Event ev;
       while(win.pollEvent(ev))
       {
           if (ev.type == Event::Closed)
               win.close();
       }
       win.clear();
       Physics::update(clock.restart() * TIME_CONSTANT_MIL);
       win.display();
    }
    return 0;
}

