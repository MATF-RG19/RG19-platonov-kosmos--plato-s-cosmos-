#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../headers/stars.h"

extern float sinTable[MAXANGLES];
extern starRec stars[MAXSTARS];        //niz svih zvezdica
GLint windW = 800, windH = 600;
float speed = 1.0;
GLint starCount = MAXSTARS / 2;

enum {
  STREAK = 0,
  CIRCLE = 1
};

float Sin(float angle){ return (sinTable[(GLint) angle]); }

float Cos(float angle){ return (sinTable[((GLint) angle + (MAXANGLES / 4)) % MAXANGLES]); }

void NewStar(GLint n, GLint d){
  if (rand() % 4 == 0) {
    stars[n].type = CIRCLE;
  } else {
    stars[n].type = STREAK;
  }
  stars[n].x[0] = (float) (rand() % MAXPOS - MAXPOS / 2);
  stars[n].y[0] = (float) (rand() % MAXPOS - MAXPOS / 2);
  stars[n].z[0] = (float) (rand() % MAXPOS + d);
  stars[n].x[1] = stars[n].x[0];
  stars[n].y[1] = stars[n].y[0];
  stars[n].z[1] = stars[n].z[0];
  if (rand() % 4 == 0){
    stars[n].offsetX = 0.0;
    stars[n].offsetY = 0.0;
    stars[n].offsetR = 0.0;
  }
}

void RotatePoint(float *x, float *y, float rotation){
  float tmpX, tmpY;

  tmpX = *x * Cos(rotation) - *y * Sin(rotation);
  tmpY = *y * Cos(rotation) + *x * Sin(rotation);
  *x = tmpX;
  *y = tmpY;
}

void ShowStar(GLint n){
  float x0, y0, x1, y1, width;
  GLint i;

  x0 = stars[n].x[0] * windW / stars[n].z[0];
  y0 = stars[n].y[0] * windH / stars[n].z[0];
  RotatePoint(&x0, &y0, stars[n].rotation);
  x0 += windW / 2.0;
  y0 += windH / 2.0;

  if (x0 >= 0.0 && x0 < windW && y0 >= 0.0 && y0 < windH) {
    if (stars[n].type == STREAK) {
      x1 = stars[n].x[1] * windW / stars[n].z[1];
      y1 = stars[n].y[1] * windH / stars[n].z[1];
      RotatePoint(&x1, &y1, stars[n].rotation);
      x1 += windW / 2.0;
      y1 += windH / 2.0;

      glLineWidth(MAXPOS / 100.0 / stars[n].z[0] + 1.0);
      glColor3f(1.0, (MAXWARP - speed) / MAXWARP, (MAXWARP - speed) / MAXWARP);
      if (fabs(x0 - x1) < 1.0 && fabs(y0 - y1) < 1.0) {
        glBegin(GL_POINTS);
        glVertex2f(x0, y0);
        glEnd();
      } else {
        glBegin(GL_LINES);
        glVertex2f(x0, y0);
        glVertex2f(x1, y1);
        glEnd();
      }
    } else {
      width = MAXPOS / 10.0 / stars[n].z[0] + 1.0;
      glColor3f(0.0, 0.0, 0.8); //plave zvezdice
      glBegin(GL_POLYGON);
      for (i = 0; i < 8; i++) {
        float x = x0 + width * Cos((float) i * MAXANGLES / 8.0);
        float y = y0 + width * Sin((float) i * MAXANGLES / 8.0);
        glVertex2f(x, y);
      };
      glEnd();
    }
  }
}

void MoveStars(void){
  float offset;
  GLint n;

  offset = speed * 60.0;

  for (n = 0; n < starCount; n++) {
    stars[n].x[1] = stars[n].x[0];
    stars[n].y[1] = stars[n].y[0];
    stars[n].z[1] = stars[n].z[0];
    stars[n].x[0] += stars[n].offsetX;
    stars[n].y[0] += stars[n].offsetY;
    stars[n].z[0] -= offset;
    stars[n].rotation += stars[n].offsetR;
    if (stars[n].rotation > MAXANGLES) {
      stars[n].rotation = 0.0;
    }
  }
}

GLenum StarPoint(GLint n){
  float x0, y0;

  x0 = stars[n].x[0] * windW / stars[n].z[0];
  y0 = stars[n].y[0] * windH / stars[n].z[0];
  RotatePoint(&x0, &y0, stars[n].rotation);
  x0 += windW / 2.0;
  y0 += windH / 2.0;

  if (x0 >= 0.0 && x0 < windW && y0 >= 0.0 && y0 < windH) {
    return GL_TRUE;
  } else {
    return GL_FALSE;
  }
}

void UpdateStars(void){
  GLint n;

  glClear(GL_COLOR_BUFFER_BIT);

  for (n = 0; n < starCount; n++) {
    if (stars[n].z[0] > speed || (stars[n].z[0] > 0.0 && speed < MAXWARP)) {
      if (StarPoint(n) == GL_FALSE) {
        NewStar(n, MAXPOS);
      }
    } else {
      NewStar(n, MAXPOS);
    }
  }
}

void ShowStars(void){
  GLint n;

  glClear(GL_COLOR_BUFFER_BIT);

  for (n = 0; n < starCount; n++) {
    if (stars[n].z[0] > speed || (stars[n].z[0] > 0.0 && speed < MAXWARP)) {
      ShowStar(n);
    }
  }
}