// platform_glfw.c
// Use glfw as a platform layer 'backend'

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

Keycode KEY_ESCAPE = GLFW_KEY_ESCAPE;

Keycode KEY_0 = GLFW_KEY_0;
Keycode KEY_1 = GLFW_KEY_1;
Keycode KEY_2 = GLFW_KEY_2;
Keycode KEY_3 = GLFW_KEY_3;
Keycode KEY_4 = GLFW_KEY_4;
Keycode KEY_5 = GLFW_KEY_5;
Keycode KEY_6 = GLFW_KEY_6;
Keycode KEY_7 = GLFW_KEY_7;
Keycode KEY_8 = GLFW_KEY_8;
Keycode KEY_9 = GLFW_KEY_9;

Keycode KEY_LAST = GLFW_KEY_LAST;
#define MAX_KEY GLFW_KEY_LAST

i8 mouse_state = 0;
u8 key_down[MAX_KEY] = {0};
u8 key_pressed[MAX_KEY] = {0};

typedef struct Window {
  WINDOW_COMMON;

  void* window;
} Window;

Window window;

static void framebuffer_callback(GLFWwindow* glfw_window, i32 width, i32 height);

void framebuffer_callback(GLFWwindow* glfw_window, i32 width, i32 height) {
  if (window.fb_callback) {
    window.fb_callback(width, height);
  }
}

i32 platform_open_window(i32 width, i32 height, u8 vsync, u8 fullscreen, const char* title) {
  window.width = width;
  window.height = height;
  window.fullscreen = fullscreen;
  window.fb_callback = NULL;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#if __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  window.window = glfwCreateWindow(window.width, window.height, title, window.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
  if (!window.window) {
    fprintf(stderr, "Failed to create GLFW window\n");
    return ERR;
  }

  glfwSetFramebufferSizeCallback(window.window, framebuffer_callback);
  glfwMakeContextCurrent(window.window);

  glfwSwapInterval(vsync ? 1 : 0);
  return NO_ERR;
}

void platform_set_framebuffer_callback(FbCallback fb_callback) {
  if (fb_callback) {
    window.fb_callback = fb_callback;
  }
}

i32 platform_handle_events() {
  glfwPollEvents();

  for (i32 key_index = 0; key_index < KEY_LAST; ++key_index) {
    i32 key_state = glfwGetKey(window.window, key_index);
    if (key_state == GLFW_PRESS) {
      key_pressed[key_index] = !key_down[key_index];
      key_down[key_index] = 1;
    }
    else {
      key_down[key_index] = 0;
      key_pressed[key_index] = 0;
    }
  }

  return 0;
}

void platform_swap_buffers() {
  glfwSwapBuffers(window.window);
}

i32 platform_close_window() {
  if (window.window) {
    glfwDestroyWindow(window.window);
    glfwTerminate();
  }
  return NO_ERR;
}
