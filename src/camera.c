// camera.c

#define WORLD_UP V3(0, 1, 0)

Camera camera = {0};

void camera_init(v3 position) {
  camera.position = position;
  camera.up = WORLD_UP;
  camera.right = V3(1, 0, 0);
  camera.forward = V3(0, 0, 1);

  camera.pitch = 180;
  camera.yaw = 90;
  camera.roll = 0;
}

void camera_update() {

  // TODO: Clamp pitch rotation

  v3 dir = V3(
    cos(radians(camera.yaw)) * cos(radians(camera.pitch)),
    sin(radians(camera.pitch)),
    sin(radians(camera.yaw)) * cos(radians(camera.pitch))
  );

  camera.forward = v3_normalize(dir);
  camera.right = v3_normalize(v3_cross(camera.forward, WORLD_UP));
  camera.up = v3_normalize(v3_cross(camera.right, camera.forward));

  view = look_at(camera.position, V3_OP(camera.position, camera.forward, +), camera.up);
}
