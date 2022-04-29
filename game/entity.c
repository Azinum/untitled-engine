// entity.c

void entity_init(Entity* e, v3 pos, v3 scale) {
  e->id = UINT32_MAX;
  e->pos = pos;
  e->scale = scale;
  e->mesh_id = -1;
  e->texture_id = -1;
  e->type = ENTITY_NONE;
}

void entities_update_and_render(Game* game) {
  Entity* e = &game->entities[1];
  Assets* assets = &game->assets;
  for (u32 i = 1; i < game->entity_count; ++i, ++e) {
    switch (e->type) {
      case ENTITY_BOBBER: {
        e->pos.y += (0.1f * game->dt) * sinf(4 * game->total_time);
        break;
      }
      default:
        break;
    }
    // TODO(lucas): add null texture and null mesh so there is no need to do these kinds of checks
    if (e->mesh_id >= 0) {
      i32 mesh = assets->mesh[e->mesh_id];
      if (e->texture_id >= 0) {
        Texture texture = assets->texture[e->texture_id];
        render_model(mesh, &texture, e->pos, e->scale);
      }
    }
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
