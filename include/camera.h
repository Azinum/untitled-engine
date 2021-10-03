// camera.h

#ifndef _CAMERA_H
#define _CAMERA_H

typedef enum Projection {
  PERSPECTIVE = 0,
  ORTHOGONAL,
} Projection;

typedef struct Camera {
  v3 pos;
  v3 target;
  v3 up;
  v3 right;
  v3 forward;

  f32 pitch;
  f32 yaw;
  f32 roll;

  Projection mode;
} Camera;

extern Camera camera;

void camera_init(v3 pos, Projection mode);

void camera_update_projection();

void camera_set_projection_mode(Projection mode);

void camera_update();

#endif
