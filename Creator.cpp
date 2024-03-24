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
//ПРОБЛЕМА - СУДЯ ПО ВСЕМУ НЕ РАСПОЗНАЁТ FLOAT, НУЖНО ПОТОМ ИСПРАВИТЬ
bool parse(string name, float& mass, float& max_health, float& strength)
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
                            return false;
                    }
            }
        }
    }
    in.close();
    return start and end;
}

//создаёт фрукт по шаблону, который передаётся в файле
bool create_fruit(std::string name, vector<Fruit>& fruits)
{
    Sprite* sprite = new Sprite;
    float mass, max_health, strength;
    if (!parse(name, mass, max_health, strength))
    {
        print_info("parse error");
        return false;
    }
    fruits.push_back(Fruit(sprite, mass, max_health, strength));
    return true;
}


