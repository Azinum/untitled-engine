// camera.h

#ifndef _CAMERA_H
#define _CAMERA_H

typedef struct Camera {
  v3 pos;
  v3 target;
  v3 up;
  v3 right;
  v3 forward;

  f32 pitch;
  f32 yaw;
  f32 roll;
} Camera;

extern Camera camera;

void camera_init(v3 position);

void camera_update();

#endif
