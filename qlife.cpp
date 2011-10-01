/*
  qlife.cpp
  
  Copyright (c) 2011, Jeremiah LaRocco jeremiah.larocco@gmail.com

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <QMainWindow>
#include "qlife.h"
#include <cmath>
#include <iostream>
#include <fstream>

#define PI (3.141592654)

QLife::QLife(QWidget *parent) : QGLWidget(parent) {
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer.start(1000/100);
    eps=0.001;
}

double QLife::x(double u, double v) {
    return u;
}

double QLife::y(double u, double v) {
    return v;
}

double QLife::z(double u, double v) {
    return 2.0*std::cos(u) * std::sin(v);
}

void QLife::timeout() {
    rotAngle += 0.5;
    if (rotAngle >= 360.0)
        rotAngle = 0.0;
    updateGL();
}

void QLife::cross(double v1[3], double v2[3], double result[3]) {
    result[0] = v1[1]*v2[2] - v2[1]*v1[2];
    result[1] = v2[0]*v1[2] - v1[0]*v2[2];
    result[2] = v1[0]*v2[1] - v2[0]*v1[1];
}
void QLife::normalize(double vect[3]) {
    double temp = std::sqrt(vect[0]*vect[0] +
                            vect[1]*vect[1] +
                            vect[2]*vect[2]);
    vect[0] /= temp;
    vect[1] /= temp;
    vect[2] /= temp;
    vect[3] /= temp;
}
void QLife::calcNormal(double u, double v, double norm[3]) {
    double v1[]={x(u+eps, v) - x(u,v),
                 y(u+eps, v) - y(u,v),
                 z(u+eps, v) - z(u,v)};
    double v2[]={x(u, v+eps) - x(u,v),
                 y(u, v+eps) - y(u,v),
                 z(u, v+eps) - z(u,v)};

    cross(v1,v2, norm);
  
    normalize(norm);
}

void QLife::initializeGL() {
    // double pi = 3.141592654;
    double uMin = -3*PI;
    double uMax = 3*PI;
    double vMin = -3*PI;
    double vMax = 3*PI;
    int uSteps = 256;
    int vSteps = 256;
    double uStep = (uMax-uMin)/uSteps;
    double vStep = (vMax-vMin)/vSteps;;
    double curU;
    double curV;
    int i,j;

    qglClearColor(Qt::black);
    glShadeModel(GL_SMOOTH);

    rotAngle = 0.0;

    mat_specular[0]=1.0;
    mat_specular[1]=1.0;
    mat_specular[2]=1.0;
    mat_specular[3]=1.0;

    mat_diffuse[0]=0.0;
    mat_diffuse[1]=1.0;
    mat_diffuse[2]=0.0;
    mat_diffuse[3]=1.0;
  
    mat_shininess[0]=50.0;

    light_position[0]=9.0;
    light_position[1]=9.0;
    light_position[2]=0.0;
    light_position[3]=1.0;

    light_color[0]=1.0;
    light_color[1]=1.0;
    light_color[2]=1.0;
    light_color[3]=1.0;

    lmodel_ambient[0]=0.2;
    lmodel_ambient[1]=0.2;
    lmodel_ambient[2]=0.2;
    lmodel_ambient[3]=1.0;
  
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
  
    listName = glGenLists(1);

    glNewList(listName, GL_COMPILE);
  
    double curNorm[4];
  
    for (i=1;i<=(vSteps-1);++i) {
        glBegin(GL_QUADS);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        curV = vMin + i*vStep;
        for (j=1; j<=(uSteps-1);++j) {
            curU = uMin + j*uStep;
      
            calcNormal(curU, curV, curNorm);
            glNormal3dv(curNorm);
            glColor3f(0, 0.9, 0);
            glVertex3f(x(curU, curV),
                       y(curU, curV),
                       z(curU, curV));
      
            calcNormal(curU+uStep, curV, curNorm);
            glNormal3dv(curNorm);
            glColor3f(0, 0.9, 0);
            glVertex3f(x(curU+uStep, curV),
                       y(curU+uStep, curV),
                       z(curU+uStep, curV));

            calcNormal(curU+uStep, curV+vStep, curNorm);
            glNormal3dv(curNorm);
            glColor3f(0, 0.9, 0);
            glVertex3f(x(curU+uStep, curV+vStep),
                       y(curU+uStep, curV+vStep),
                       z(curU+uStep, curV+vStep));

            calcNormal(curU, curV+vStep, curNorm);
            glNormal3dv(curNorm);
            glColor3f(0, 0.9, 0);
            glVertex3f(x(curU, curV+vStep),
                       y(curU, curV+vStep),
                       z(curU, curV+vStep));
        }
        glEnd();
    }
    glEndList();
}

void QLife::resizeGL(int width, int height) {
    glViewport(0,0, (GLsizei) width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //  glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5,20.0);
    gluPerspective(80, 1.0, 1.0, 140);
    glMatrixMode(GL_MODELVIEW);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void QLife::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0,15.0,15.0,
              0.0,0.0,0.0,
              0.0,0.0,1.0);

  
    glPushMatrix();
    glLoadIdentity();

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
    glPopMatrix();
    glRotatef(rotAngle, 0,0,1);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    //  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glCallList(listName);

    glFlush();
}
