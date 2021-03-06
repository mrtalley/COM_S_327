#include <vector>
#include <cstring>

#include "object.h"
#include "dungeon.h"
#include "utils.h"

object::object(const object_description &o, pair_t p, object *next) :
  name(o.get_name()),
  description(o.get_description()),
  type(o.get_type()),
  color(o.get_color()),
  damage(o.get_damage()),
  hit(o.get_hit().roll()),
  dodge(o.get_dodge().roll()),
  defence(o.get_defence().roll()),
  weight(o.get_weight().roll()),
  speed(o.get_speed().roll()),
  attribute(o.get_attribute().roll()),
  value(o.get_value().roll()),
  seen(false),
  next(next)
{
  position[dim_x] = p[dim_x];
  position[dim_y] = p[dim_y];
}

object::~object()
{
  if (next) {
    delete next;
  }
}

int set_object_price(object *o)
{
  int dmg_avg = 0;
  for(int i = 0; i < 5; i++) {
    dmg_avg += o->roll_dice();
  }
  dmg_avg = dmg_avg / 5;

  if(dmg_avg > 100) {
    return rand() % (30 - 20 + 1) + 20;
  } else if(dmg_avg > 50) {
    return rand() % (20 - 10 + 1) + 10;
  } else {
    return rand() % (10) + 1;
  }
}

void gen_object_store(dungeon_t *d, int x, int y)
{
  object *o;
  pair_t p;
  const std::vector<object_description> &v = d->object_descriptions;
  const object_description &od = v[rand_range(0, v.size() - 1)];

  p[dim_y] = y;
  p[dim_x] = x;
  o = new object(od, p, d->objmap[y][x]);

  /* TODO: add initialization for in_store and price */
  o->set_in_store(true);
  o->set_price(set_object_price(o));

  d->objmap[p[dim_y]][p[dim_x]] = o;
}

int gen_store_objects(dungeon_t *d)
{
  uint32_t i, j;

  int y_size = d->rooms[2].size[dim_y];
  int y_pos = d->rooms[2].position[dim_y];
  int x_size = d->rooms[2].size[dim_x];
  int x_pos = d->rooms[2].position[dim_x];
  int num_store_objects = 0;

  for (i = y_pos + 1; i < y_pos + y_size - 1; i++)
  {
    for (j = x_pos + 1; j < x_pos + x_size - 1; j++)
    {
      gen_object_store(d, j, i);
      num_store_objects++;
    }
  }

  return num_store_objects;
}

void gen_object(dungeon_t *d)
{
  object *o;
  uint32_t room;
  pair_t p;
  const std::vector<object_description> &v = d->object_descriptions;
  const object_description &od = v[rand_range(0, v.size() - 1)];

  do {
    room = rand_range(0, d->num_rooms - 1);
  } while(room == 2);

  do {
    p[dim_y] = rand_range(d->rooms[room].position[dim_y],
                          (d->rooms[room].position[dim_y] +
                           d->rooms[room].size[dim_y] - 1));
    p[dim_x] = rand_range(d->rooms[room].position[dim_x],
                          (d->rooms[room].position[dim_x] +
                           d->rooms[room].size[dim_x] - 1));
  } while (mappair(p) > ter_stairs);

  o = new object(od, p, d->objmap[p[dim_y]][p[dim_x]]);

  o->set_in_store(false);
  o->set_price(set_object_price(o));

  d->objmap[p[dim_y]][p[dim_x]] = o;
}

void gen_objects(dungeon_t *d)
{
  uint32_t i;

  memset(d->objmap, 0, sizeof (d->objmap));

  int store_objs = gen_store_objects(d);

  for (i = 0; i < d->max_objects; i++) {
    gen_object(d);
  }

  d->num_objects = d->max_objects + store_objs;
}

char object::get_symbol()
{
  return next ? '&' : object_symbol[type];
}

uint32_t object::get_color()
{
  return color;
}

const char *object::get_name()
{
  return name.c_str();
}

int32_t object::get_speed()
{
  return speed;
}

int32_t object::roll_dice()
{
  return damage.roll();
}

void destroy_objects(dungeon_t *d)
{
  uint32_t y, x;

  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      if (d->objmap[y][x]) {
        delete d->objmap[y][x];
        d->objmap[y][x] = 0;
      }
    }
  }
}

int32_t object::get_type()
{
  return type;
}

uint32_t object::is_equipable()
{
  return type >= objtype_WEAPON && type <= objtype_RING;
}

uint32_t object::is_removable()
{
  return 1;
}

uint32_t object::is_dropable()
{
  return 1;
}

uint32_t object::is_destructable()
{
  return 1;
}

int32_t object::get_eq_slot_index()
{
  if (type < objtype_WEAPON ||
      type > objtype_RING) {
    return -1;
  }

  return type - 1;
}

void object::to_pile(dungeon_t *d, pair_t location)
{
  next = (object *) d->objmap[location[dim_y]][location[dim_x]];
  d->objmap[location[dim_y]][location[dim_x]] = this;
}
