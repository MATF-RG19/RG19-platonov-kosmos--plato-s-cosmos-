#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>

#include "../headers/stars.h"
  
GLenum doubleBuffer;
GLint windWidth = 800, windHeight = 600;

//float speed = 1.0; //uvecavanje brzine ce se odvijati postepeno

starRec stars[MAXSTARS];        //niz svih zvezdica
float sinTable[MAXANGLES];

static void Init(void){
  float angle;
  GLint n;

  srand((unsigned int) time(NULL));

  for (n = 0; n < MAXSTARS; n++) {
    NewStar(n, 100);
  }

  angle = 0.0;
  for (n = 0; n <= MAXANGLES; n++) {
    sinTable[n] = sin(angle);
    angle += PI / (MAXANGLES / 2.0);
  }

  glClearColor(0.0, 0.0, 0.0, 0.0);

  glDisable(GL_DITHER);
}

void Reshape(int width, int height){
  windWidth = width;
  windHeight = height;

  glViewport(0, 0, windWidth, windHeight);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-0.5, windWidth + 0.5, -0.5, windHeight + 0.5);
  glMatrixMode(GL_MODELVIEW);
}

static void Key(unsigned char key, int x, int y){
  switch (key) {
    case 27: exit(0);
  }
}

void Idle(void){
  MoveStars();
  UpdateStars();
  glutPostRedisplay();
}

void Display(void){
  ShowStars();
  if (doubleBuffer) {
    glutSwapBuffers();
  } else {
    glFlush();
  }
}

void Visible(int state){
  if (state == GLUT_VISIBLE) {
    glutIdleFunc(Idle);
  } else {
    glutIdleFunc(NULL);
  }
}

static void Args(int argc, char **argv){
  GLint i;

  doubleBuffer = GL_TRUE;

  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-sb") == 0) {
      doubleBuffer = GL_FALSE;
    } else if (strcmp(argv[i], "-db") == 0) {
      doubleBuffer = GL_TRUE;
    }
  }
}


int main(int argc, char **argv){
  GLenum type;

  glutInitWindowSize(windWidth, windHeight);
  glutInitWindowPosition(200, 200);
  glutInit(&argc, argv);
  Args(argc, argv);

  type = GLUT_RGB;
  type |= (doubleBuffer) ? GLUT_DOUBLE : GLUT_SINGLE;
  glutInitDisplayMode(type);
  glutCreateWindow("Platonov kosmos");

  Init();

  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Key);
  glutVisibilityFunc(Visible);
  glutDisplayFunc(Display);
  glutMainLoop();
  return 0;
}