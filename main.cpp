//файл - основа программы - движок.

#include "Actor.h"
#include "Land.cpp"
#include <vector>
#include <math.h>
#include <algorithm>
using namespace std;
using namespace sf;

vector<Fruit*> fruits;
vector<Object*> objects;

Land* land;

bool create_fruit(string name, vector<Fruit*>& fruits);

int FRAME_LIM = 60;
float TIME_CONSTANT_MIL = FRAME_LIM / 1000.f;
int PLAYER_ID = 0;
Fruit* PLAYER_PTR = nullptr;


Vector2f get_abs_body_coord(Body* body)
{
    return body->coord + Vector2f(body->area.left, body->area.top);
}

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

    bool is_right(Fruit* frt)
    {
        auto s = frt->get_states_ptr();
        return s->get_state("right_stand") or s->get_state("right_walk") 
            or s->get_state("right_hit_1") or s->get_state("right_hit_2")
            or s->get_state("right_hit_3") or s->get_state("right_dash");
    }

    bool is_left(Fruit *frt)
    {
        auto s = frt->get_states_ptr();
        return s->get_state("left_stand") or s->get_state("left_walk") 
            or s->get_state("left_hit_1") or s->get_state("left_hit_2")
            or s->get_state("left_hit_3") or s->get_state("left_dash");

    }
    
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
            if (is_right(PLAYER_PTR))
            {
                 s->set_state("right_hit_1", true);
                 s->set_state("left_hit_1", false);
            }
            else if (is_left(PLAYER_PTR))
            {
                s->set_state("left_hit_1", true);
                s->set_state("right_hit_1", false);
            }
            else if (s->get_state("right_hit_1") or s->get_state("left_hit_1"))
            {

            }
        }

        if (pressed_keys[int(KEYS::SPACE)])
        {
           if (is_right(PLAYER_PTR))
           {
               s->set_state("right_dash", true);
               s->set_state("left_dash", false);
           }
           else if (is_left(PLAYER_PTR))
           {
               s->set_state("left_dash", true);
               s->set_state("right_dash", false);
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

    bool can_move(Body* body, float x, float y)
    {
        IntRect tmp(x, y, body->area.width, body->area.height);
        IntRect tmp_el;
        for (auto el : land->restr_areas)
        {
            if (tmp.intersects(el->area))
                return false;
        }
        Body* el_b = nullptr;
        for (auto el : fruits)
        {
            el_b = el->get_body_ptr();
            if (el_b != body)
            {
                tmp_el = IntRect(el_b->coord.x, el_b->coord.y, el_b->area.width, el_b->area.height);
                if (tmp.intersects(tmp_el))
                    return false;
            }
        }
        return true;
    }


    const float friction = 0.9f;
    float coef = 1.f;
    void move(Body* b, Time dt, Vector2f force = Vector2f(0.f, 0.f))
    {
        coef = 1.f;
        float dt_ = dt.asMilliseconds() * TIME_CONSTANT_MIL;

        auto corr_force = force - (friction) * b->vel;
        b->acc = corr_force / b->mass;
        b->vel += b->acc * dt_;

        if (abs((abs(force.x) - abs(force.y)) < eps))
            coef = 1 / SQRT2;
        Vector2f vel_y_compress = b->vel;
        vel_y_compress.y *= 0.6f;
       
        Vector2f pred_coord = b->coord + vel_y_compress * coef * dt_;

        if (can_move(b, pred_coord.x, b->coord.y))
            b->coord.x = pred_coord.x;
        if (can_move(b, b->coord.x, pred_coord.y))
            b->coord.y = pred_coord.y;

        
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

    bool is_hitting(Fruit* frt)
    {
        auto s = frt->get_states_ptr();

        return s->get_state("right_hit_1") or s->get_state("left_hit_1")
            or s->get_state("right_hit_2") or s->get_state("left_hit_2")
            or s->get_state("right_hit_3") or s->get_state("left_hit_3");
    }

    bool is_dash(Fruit* frt)
    {
        auto s = frt->get_states_ptr();

        return s->get_state("right_dash") or s->get_state("left_dash");
    }

    void update(Time dt)
    {
        for (auto el : fruits)
        {
            auto body = el->get_body_ptr();
            auto state = el->get_states_ptr();

            float dash_coef = 1.f;
            if (is_dash(el))
                dash_coef = 2.f;

            float mass = body->mass;
            if (el == PLAYER_PTR)
                 move(body, dt, (!is_hitting(el)) * dash_coef * MOVE_FORCE * Vector2f(Controller::get_dir_x(), -Controller::get_dir_y()));
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

    bool coord_compare(Fruit* f, Fruit* s)
    {
        auto f_body = f->get_body_ptr(),
             s_body = s->get_body_ptr();
        return f_body->coord.y/* + f_body->area.top + f_body->area.height*/ < s_body->coord.y/* + s_body->area.top + s_body->area.height*/;
    }
  
    void draw_back(RenderWindow& win)
    {
        win.draw(*land->back);
    }

    void draw_front(RenderWindow& win)
    {
        win.draw(*land->front);
    }

    void create_hittings(Fruit* fruit)
    {
        
    }
    
    void update(RenderWindow& win, Time dt)
    {
        sort(fruits.begin(), fruits.end(), coord_compare);
        RectangleShape rec_f(Vector2f(0, 1));
        Sprite* sprite;
        win.clear(Color::White);
        draw_back(win);
        for (auto el : fruits)
        {
            sprite = el->get_sprite_ptr();
            auto anim = el->get_anim_ptr();
            auto states = el->get_states_ptr();
            auto body = el->get_body_ptr();

            create_hittings(el);

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
            sprite->setPosition(body->coord - Vector2f(body->area.left, body->area.top));
            win.draw(*sprite);

            /*rec_f.setSize(Vector2f(body->area.width, body->area.height));
            rec_f.setPosition(body->coord);
            rec_f.setFillColor(Color(0, 255, 0, 100));
            win.draw(rec_f);*/
        }
        draw_front(win);
    }
}

//создаётся окно, начало программы
int main()
{
    RenderWindow win(VideoMode(WIDTH, HEIGHT), "Fruits", Style::Close);
    win.setFramerateLimit(FRAME_LIM);
    win.setPosition(Vector2i(400, 300));
    
    land = new Land(BACK_PATH, FRONT_PATH);
    land->create_restr_area(IntRect(0, 0, WIDTH, 90));
    land->create_restr_area(IntRect(0, 0, 45, HEIGHT));
    land->create_restr_area(IntRect(WIDTH - 25, 0, 25, HEIGHT));
    land->create_restr_area(IntRect(0, HEIGHT - 20, WIDTH, 20));

    /*RectangleShape rec(Vector2f(WIDTH, 20));
    rec.setPosition(Vector2f(0, HEIGHT - 20));
    rec.setFillColor(Color(255, 0, 0, 100));*/

    create_fruit("APPLE", fruits);
    create_fruit("BANANA", fruits);

    fruits[0]->get_states_ptr()->set_state("right_stand", true);
    fruits[0]->get_body_ptr()->coord = Vector2f(500.f, 400.f);
    PLAYER_PTR = fruits[0];
    fruits[1]->get_body_ptr()->coord = Vector2f(400.f, 300.f);
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

