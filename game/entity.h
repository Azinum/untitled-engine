// entity.h

#ifndef _ENTITY_H
#define _ENTITY_H

typedef enum Entity_type {
  ENTITY_NONE = 0,
  ENTITY_BOBBER,
} Entity_type;

typedef struct Entity {
  u32 id;
  v3 pos;
  v3 scale;

  i32 model_id;
  Texture texture;

  Entity_type type;
} Entity;

struct Game;

void entity_init(Entity* e, v3 pos, v3 scale);

void entities_update_and_render(struct Game* game);

Entity* get_null_entity(struct Game* game);

Entity* entity_push(struct Game* game, v3 pos);

#endif // _ENTITY_H
