#include "Actor.h"
#include <fstream>

using namespace sf;
using namespace std;

bool is_count(string line)
{
    for (int i = 0; i < line.size(); i++)
    {
        if (!isdigit(line[i]) and line[i] != '.' and line[i] != '-')
            return false;
    }
    return true;
}

bool parse_chars(string name, float& mass, float& max_health, float& strength)
{
    int counter = 0;
    bool start = false;
    bool end = false;
    ifstream in(TEMPLATES_PATH);
    if (!in.is_open())
        return false;

    string word;
    while (in >> word and !start)
    {
        if(word.compare(name) == 0)
        {
            start = true;
            while (in >> word and !end)
            {
                if (is_count(word))
                    switch (counter)
                    {
                        case 0:
                            mass = stof(word);
                            counter++;
                            break;
                        case 1:
                            max_health = stof(word);
                            counter++;
                            break;
                        case 2:
                            strength = stof(word);
                            end = true;
                            break;
                        default:
                            in.close();
                            return false;
                    }
            }
        }
    }
    in.close();
    return start and end;
}

bool animating_fruit(string name, Fruit* frt)
{
    string word = "";
    ifstream in(TEMPLATES_PATH);
    if (!in.is_open())
        return false;
    //находим нужный блок
    while (in >> word and word.compare(name) != 0);
    //спускаемся до куска с анимациями
    while (in >> word and word.compare("-") != 0);
    
    if (word.compare("-") != 0)
    {
        in.close();
        return false;
    }

    string path = "";
    in >> path;
    Vector2i start(-1, -1), frame(-1, -1);
    float x, y;
    float time;
    int loop, frames_count, rankes;
    in >> x >> y;
    frame.x = x;
    frame.y = y;
    bool not_empty = false;
    string anim_name;
    int priority;
    while (in >> word)
    {
        if (word.compare("[") == 0)
        {
            not_empty = true;
            in >> anim_name;
            in >> priority;
            in >> time;
            
            in >> loop;
            in >> x >> y;
            start.x = x;
            start.y = y;

            in >> frames_count >> rankes;
            in >> word;
            
            if (word.compare("]") != 0)
            {
                in.close();
                return false;
            }
        }
        else
        {
            in.close();
            return not_empty;
        }
        auto& animation = frt->get_anim_ptr()->create_animation(anim_name, path, seconds(time), loop == 1);
        animation.add_frames(start, frame, frames_count, rankes);
        frt->get_states_ptr()->create_state(anim_name, priority);
    }
    in.close();
    return not_empty;
}

//создаёт фрукт по шаблону, который передаётся в файле
bool create_fruit(string name, vector<Fruit*>& fruits)
{
    Sprite* sprite = new Sprite;
    float mass, max_health, strength;
    if (!parse_chars(name, mass, max_health, strength))
    {
        print_info("parse error");
        return false;
    }
    sprite->setScale(2.5f, 2.5f);
    Fruit* frt = new Fruit(sprite, mass, max_health, strength);
    if (!animating_fruit(name, frt))
    {
        print_info("animating error");
        return false;
    }
    //frt->get_anim_ptr()->switch_animation(frt->get_states_ptr()->get_most_priority_state().first);
    fruits.push_back(frt);
    return true;
}


