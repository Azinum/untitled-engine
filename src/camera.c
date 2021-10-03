// camera.c

#define WORLD_UP V3(0, 1, 0)

Camera camera = {0};

void camera_init(v3 pos, Projection mode) {
  camera.pos = camera.target = pos;
  camera.up = WORLD_UP;
  camera.right = V3(1, 0, 0);
  camera.forward = V3(0, 0, 1);

  camera.pitch = 180;
  camera.yaw = 90;
  camera.roll = 0;

  camera.mode = mode;
}

void camera_update_projection() {
  i32 window_width = platform_window_width();
  i32 window_height = platform_window_height();
  perspective_proj = perspective(g_camera_fov, (f32)window_width / window_height, g_camera_near, g_camera_far);
  // orthogonal_proj = orthographic(0, window_width, window_height, 0, -1.0f, 1.0f);
  orthogonal_proj = orthographic(0, window_width, window_height, 0, -1.0f, 1.0f);

  switch (camera.mode) {
    case PERSPECTIVE: {
      projection = &perspective_proj;
      break;
    }
    case ORTHOGONAL: {
      projection = &orthogonal_proj;
      break;
    }
    default:
      break;
  }
}

void camera_set_projection_mode(Projection mode) {
  camera.mode = mode;
  camera_update_projection();
}

void camera_update() {
  v3 dir = V3(
    cos(radians(camera.yaw)) * cos(radians(camera.pitch)),
    sin(radians(camera.pitch)),
    sin(radians(camera.yaw)) * cos(radians(camera.pitch))
  );

  camera.forward = v3_normalize(dir);
  camera.right = v3_normalize(v3_cross(camera.forward, WORLD_UP));
  camera.up = v3_normalize(v3_cross(camera.right, camera.forward));

  view = look_at(camera.pos, V3_OP(camera.pos, camera.forward, +), camera.up);
}
