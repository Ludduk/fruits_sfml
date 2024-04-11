#include "General.h"

using namespace sf;
using namespace std;


struct DamageArea
{
    IntRect area;
    float damage;

    DamageArea(IntRect area_ = IntRect(0, 0, 10, 10), float damage_ = 10.f): area(area_), damage(damage_){}   
    bool operator == (DamageArea& a)
    {
        return area == a.area and damage == a.damage;
    }
};

struct RestrictArea
{
    IntRect area;

    RestrictArea(IntRect area_ = IntRect(0, 0, 100, 100)): area(area_){}

    bool operator == (RestrictArea& a)
    {
        return area == a.area;
    }
};

struct Map
{
    Sprite* spr;
    Texture* texture;
    vector<DamageArea*> dam_areas;
    vector<RestrictArea*> restr_areas;
    
    Map(string text_name, Vector2f coord)
    {
        if (!texture->loadFromFile(text_name))
        {
            print_info("map texture file is not found");
            return;
        }
        spr->setTexture(*texture);
        spr->setPosition(coord);
    }

    void move(Vector2f new_coord)
    {
        spr->setPosition(new_coord);
    }

    void create_dam_area(IntRect area, float damage)
    {
        DamageArea* dam = new DamageArea(area, damage);
        dam_areas.push_back(dam);
    }

    void delete_dam_area(DamageArea area)
    {
        DamageArea* found = nullptr;
        for (int i = 0; i < dam_areas.size(); i++)
        {
            if (*dam_areas[i] == area)
            {
                dam_areas.erase(dam_areas.begin() + i);
                return;
            }
        }
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

    ~Map()
    {
        for (auto el : restr_areas)
            delete el;
        for (auto el : dam_areas)
            delete el;
    }
};
