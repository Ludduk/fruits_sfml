#include "General.h"

using namespace sf;
using namespace std;


struct RestrictArea
{
    IntRect area;

    RestrictArea(IntRect area_ = IntRect(0, 0, 100, 100)): area(area_){}

    bool operator == (RestrictArea& a)
    {
        return area == a.area;
    }
};

struct Land 
{
    Sprite* back, *front;
    Texture back_t, front_t;
    vector<RestrictArea*> restr_areas;
    
    Land(string back_path, string front_path)
    {
        if (!back_t.loadFromFile(back_path))
        {
            print_info("map texture file is not found");
            return;
        }
        if (!front_t.loadFromFile(front_path))
        {
            print_info("map texture file is not found");
            return;
        }       
       
        back = new Sprite();
        front = new Sprite();
        back->setTexture(back_t);

        float scaleX = float(WIDTH) / back->getTexture()->getSize().x;
        float scaleY = float(HEIGHT) / back->getTexture()->getSize().y;
        
        back->setScale(scaleX, scaleY);
        back->setPosition(Vector2f(0, 0));
        front->setTexture(front_t);
        front->setScale(scaleX, scaleY);
        back->setPosition(Vector2f(0, back->getTexture()->getSize().y - front->getTexture()->getSize().y));
    }

   
    void create_restr_area(IntRect area)
    {
        RestrictArea* restr = new RestrictArea(area);
        restr_areas.push_back(restr);
    }

    void delete_restr_area(RestrictArea area)
    {
        RestrictArea* found = nullptr;
        for (int i = 0; i < restr_areas.size(); i++)
        {
            if (*restr_areas[i] == area)
            {
                restr_areas.erase(restr_areas.begin() + i);
                return;
            }
        }
    }

    ~Land()
    {
        for (auto el : restr_areas)
            delete el;
    }
};
