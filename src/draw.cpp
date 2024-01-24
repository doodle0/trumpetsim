#include "draw.h"

#include <cmath>

#include <GL/gl.h>
#include <GL/glu.h>

void drawBoreSegment(float x, float r, float slices) {
  glBegin(GL_LINE_LOOP);

  for (int i = 0; i < slices; i++) {
    float theta = (float)i / slices * M_PI * 2;
    glVertex3f(x, r * sin(theta), r * cos(theta));
  }

  glEnd();
}

void drawSphere(float radius, float yoffset) {
  const int slices = 20, stacks = 20;

  glBegin(GL_LINE_STRIP);

  for (int i = 0; i < stacks; i++) {
    float phi = (float)i / stacks * M_PI;

    for (int j = 0; j < slices; j++) {
      float theta = (float)j / slices * 2.0 * M_PI;

      float x = radius * sin(phi) * cos(theta);
      float y = radius * cos(phi) + yoffset;
      float z = radius * sin(phi) * sin(theta);

      glVertex3f(x, y, z);
    }
  }

  glEnd();
}

void drawAxes() {
  glColor3f(1, 0, 0);
  glBegin(GL_LINES);
  glVertex3f(0, 0, 0);
  glVertex3f(1, 0, 0);
  glEnd();

  glColor3f(0, 1, 0);
  glBegin(GL_LINES);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 1, 0);
  glEnd();

  glColor3f(0, 0, 1);
  glBegin(GL_LINES);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 1);
  glEnd();
}
