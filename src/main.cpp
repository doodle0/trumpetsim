#include "draw.h"

#include <cmath>
#include <emscripten/emscripten.h>
#include <exception>
#include <iostream>

#include <emscripten.h>
#include <emscripten/html5.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>

/* #region Globals */

const int INIT_WIDTH = 800, INIT_HEIGHT = 450;

SDL_Window *window;
SDL_GLContext context;

int windowWidth = INIT_WIDTH, windowHeight = INIT_HEIGHT;

int t, mouseX, mouseY;
struct {
  float dist, theta, phi;
} cam = {3, 0, 0};

/* #endregion */

/* #region Utility functions */

float lerpf(float x, float in1, float in2, float out1, float out2) {
  return (x - in1) / (in2 - in1) * (out2 - out1) + out1;
}

/* #endregion */

/* #region Update and draw functions */

void update() {
  // Handle events
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_QUIT:
      std::terminate();
      break;
    case SDL_KEYDOWN:
      std::cout << "Got SDL_KEYDOWN\n";
      break;
    }
  }

  // Update current tick and mouse position
  t = SDL_GetTicks();
  SDL_GetMouseState(&mouseX, &mouseY);

  // Update the camera position for rendering
  cam.theta = lerpf(mouseX, 0, windowWidth, -M_PI, M_PI);
  cam.phi = lerpf(mouseY, 0, windowHeight, 0, M_PI_2);
}

void draw() {
  glViewport(0, 0, windowWidth, windowHeight);
  glClearColor(0, 0, 0, 1);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (double)windowWidth / windowHeight, 0.1, 50.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // Using gluLookAt seems to be buggy...
  // gluLookAt(0, cam.y, cam.z, 0, 0, 0, 0, 1, 0);
  glTranslatef(0, 0, -cam.dist);
  glRotatef(cam.phi / M_PI * 180, 1, 0, 0);
  glRotatef(cam.theta / M_PI * 180, 0, 1, 0);

  for (int i = -50; i <= 50; i++) {
    float x = (float)i / 25;
    glColor3f(lerpf(i, -50, 50, 1, 0), 1.0f, lerpf(i, -50, 50, 0, 1));
    drawBoreSegment(x, (cos(x * M_PI) + 5) / 25, 20);
  }

  drawAxes();

  glFlush();
  SDL_GL_SwapWindow(window);
}

/* #endregion */

/* #region Emscripten callback functions */

EM_BOOL
onFullscreenchange(int eventType,
                   const EmscriptenFullscreenChangeEvent *fullscreenChangeEvent,
                   void *userData) {
  if (fullscreenChangeEvent->isFullscreen) {
    std::cout << "Entering fullscreen\n";
    // Changing video mode is presumably done by Emscripten;
    // we only need to resize the window to the size of our current screen
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    SDL_SetWindowSize(window, dm.w, dm.h);
  } else {
    std::cout << "Leaving fullscreen\n";
    SDL_SetWindowSize(window, INIT_WIDTH, INIT_HEIGHT);
  }

  SDL_GetWindowSize(window, &windowWidth, &windowHeight);
  std::cout << "  New window size: " << windowWidth << "x" << windowHeight << '\n';

  return 0;
}

/* #endregion */

/* #region Main control flow */

void mainInit() {
  std::cout << "Initializing SDL window...";

  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("Trumpet Simulation", 0, 0, INIT_WIDTH, INIT_HEIGHT,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  context = SDL_GL_CreateContext(window);
  // SDL_GL_MakeCurrent(window, context);

  std::cout << "complete.\n";
}

void mainLoop() {
  update();
  draw();
}

int main() {
  mainInit();
  emscripten_set_fullscreenchange_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT,
                                           NULL, 0, onFullscreenchange);
  emscripten_set_main_loop(mainLoop, 0, true);
}

/* #endregion */