// entity.c

void entity_init(Entity* e, v3 pos, v3 scale) {
  e->id = UINT32_MAX;
  e->pos = pos;
  e->scale = scale;
  e->model_id = -1;
  e->texture = (Texture) { .width = 0, .height = 0, .id = -1, };
  e->type = ENTITY_NONE;
}

void entities_update_and_render(Game* game) {
  Entity* e = &game->entities[1];
  for (u32 i = 1; i < game->entity_count; ++i, ++e) {
    switch (e->type) {
      case ENTITY_BOBBER: {
        e->pos.y += (0.1f * game->dt) * sinf(4 * game->total_time);
        break;
      }
      default:
        break;
    }
    render_model(e->model_id, &e->texture, e->pos, e->scale);
  }
}

Entity* get_null_entity(Game* game) {
  return &game->entities[0];
}

Entity* entity_push(Game* game, v3 pos) {
  if (game->entity_count < MAX_ENTITY) {
    Entity* e = &game->entities[game->entity_count++];
    entity_init(e, pos, V3(1, 1, 1));
    e->id = game->entity_id++;
    return e;
  }
  return get_null_entity(game);
}
