#ifndef DESCRIPTIONS_H
# define DESCRIPTIONS_H

# include <stdint.h>
# undef swap
# include <vector>
# include <string>

# include "dice.h"
# include "npc.h"
# include "object.h"

typedef class dungeon dungeon_t;

uint32_t parse_descriptions(dungeon_t *d);
uint32_t print_descriptions(dungeon_t *d);
uint32_t destroy_descriptions(dungeon_t *d);

typedef enum object_type {
    objtype_no_type,
    objtype_WEAPON,
    objtype_OFFHAND,
    objtype_RANGED,
    objtype_LIGHT,
    objtype_ARMOR,
    objtype_HELMET,
    objtype_CLOAK,
    objtype_GLOVES,
    objtype_BOOTS,
    objtype_AMULET,
    objtype_RING,
    objtype_SCROLL,
    objtype_BOOK,
    objtype_FLASK,
    objtype_GOLD,
    objtype_AMMUNITION,
    objtype_FOOD,
    objtype_WAND,
    objtype_CONTAINER
} object_type_t;

extern const char object_symbol[];

class monster_description {
private:
    std::string name, description;
    char symbol;
    std::vector<uint32_t> color;
    uint32_t abilities;
    dice speed, hitpoints, damage;
    int rendered, killed;
public:
    monster_description() : name(),       description(), symbol(0),   color(0),
                            abilities(0), speed(),       hitpoints(), damage()
    {
    }
    void set(const std::string &name,
             const std::string &description,
             const char symbol,
             const std::vector<uint32_t> &color,
             const dice &speed,
             const uint32_t abilities,
             const dice &hitpoints,
             const dice &damage);
    void set_rendered(int value) { rendered = value; }
    void set_killed(int value) { killed = value; }
    std::ostream &print(std::ostream &o);
    char get_symbol() { return symbol; }
    uint32_t get_abilities() { return abilities; }
    int get_rendered() { return rendered; }
    int get_killed() { return killed; }
    inline npc gen_monst_from_desc()
    {
        npc new_mon;

        new_mon.name = name;
        new_mon.description = description;
        new_mon.symbol = symbol;
        new_mon.color = color;
        new_mon.abilities = abilities;
        new_mon.speed = speed.roll();
        new_mon.hitpoints = hitpoints.roll();
        new_mon.damage = damage;

        return new_mon;
    }

    inline void gen_monst_from_desc(npc *monst)
    {
        monst->name = name;
        monst->description = description;
        monst->symbol = symbol;
        monst->color = color;
        monst->abilities = abilities;
        monst->speed = speed.roll();
        monst->hitpoints = hitpoints.roll();
        monst->damage = damage;
    }
};

class object_description {
private:
    std::string name, description;
    object_type_t type;
    uint32_t color;
    dice hit, damage, dodge, defence, weight, speed, attribute, value;
public:
    object_description() : name(),    description(), type(objtype_no_type),
                           color(0),  hit(),         damage(),
                           dodge(),   defence(),     weight(),
                           speed(),   attribute(),   value()
    {
    }
    void set(const std::string &name,
             const std::string &description,
             const object_type_t type,
             const uint32_t color,
             const dice &hit,
             const dice &damage,
             const dice &dodge,
             const dice &defence,
             const dice &weight,
             const dice &speed,
             const dice &attrubute,
             const dice &value);
    std::ostream &print(std::ostream &o);
    /* Need all these accessors because otherwise there is a *
     * circular dependancy that is difficult to get around.  */
    inline const std::string &get_name() const { return name; }
    inline const std::string &get_description() const { return description; }
    inline const object_type_t get_type() const { return type; }
    inline const uint32_t get_color() const { return color; }
    inline const dice &get_hit() const { return hit; }
    inline const dice &get_damage() const { return damage; }
    inline const dice &get_dodge() const { return dodge; }
    inline const dice &get_defence() const { return defence; }
    inline const dice &get_weight() const { return weight; }
    inline const dice &get_speed() const { return speed; }
    inline const dice &get_attribute() const { return attribute; }
    inline const dice &get_value() const { return value; }
    inline object create_object_instance()
    {
        object o;

        o.name = name;
        o.description = description;
        o.type = object_symbol[type];
        o.color = color;
        o.hit = hit.roll();
        o.dodge = dodge.roll();
        o.defence = defence.roll();
        o.weight = weight.roll();
        o.speed = speed.roll();
        o.attribute = attribute.roll();
        o.value = value.roll();
        o.damage = damage;

        return o;
    }
    inline void create_object_instance(object *o)
    {
        o->name = name;
        o->description = description;
        o->type = object_symbol[type];
        o->color = color;
        o->hit = hit.roll();
        o->dodge = dodge.roll();
        o->defence = defence.roll();
        o->weight = weight.roll();
        o->speed = speed.roll();
        o->attribute = attribute.roll();
        o->value = value.roll();
        o->damage = damage;
    }
};

std::ostream &operator<<(std::ostream &o, monster_description &m);
std::ostream &operator<<(std::ostream &o, object_description &od);

#endif
