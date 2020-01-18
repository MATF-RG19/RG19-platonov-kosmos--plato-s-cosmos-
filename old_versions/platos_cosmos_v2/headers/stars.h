#ifndef _STARS_H_
#define _STARS_H_

#include <math.h>
#include <time.h>
#include <GL/glut.h>

#define MAXSTARS 400
#define MAXPOS 10000
#define MAXWARP 10
#define MAXANGLES 6000

#undef PI               
#define PI 3.141592657

typedef struct _starRec {   //struktura koja opisuje zvezdicu
  GLint type;
  float x[2], y[2], z[2];
  float offsetX,  offsetY, offsetR, rotation;
} starRec;


float Sin(float angle);

float Cos(float angle);

void NewStar(GLint n, GLint d);

void RotatePoint(float *x, float *y, float rotation);

void ShowStar(GLint n);

void MoveStars(void);

GLenum StarPoint(GLint n);

void UpdateStars(void);

void ShowStars(void);

#endif