/*
  lifewidget.cpp
  
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

#include "lifewidget.h"

#include <cmath>

#define PI (3.141592654)

LifeWidget::LifeWidget(QWidget *parent) : QGLWidget(parent) {
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));

    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));

    timer.start(1000/100);
}

void LifeWidget::timeout() {
    updateGL();
}

void LifeWidget::initializeGL() {

    qglClearColor(Qt::black);
    glShadeModel(GL_SMOOTH);
}

void LifeWidget::resizeGL(int width, int height) {
    glViewport(0,0, (GLsizei) width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //  glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5,20.0);
    gluPerspective(80, 1.0, 1.0, 140);
    glMatrixMode(GL_MODELVIEW);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void LifeWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glFlush();
}
