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

LifeWidget::LifeWidget(LifePlugin *plug, QWidget *parent) : QGLWidget(parent), curPlugin(plug), curIter(0) {
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));

    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));

}

void LifeWidget::setPlugin(LifePlugin *newPlugin) {
    // What needs clean up?
    curPlugin = newPlugin;
    curPlugin->reset();
    curPlugin->initView();
    curPlugin->resizeView(curWidth, curHeight);
}

// void LifeWidget::configure() {
    
// }

void LifeWidget::timeout() {
    if (curPlugin) {
        curIter++;
        curPlugin->evolve();
        emit iterationDone(curIter);
    }
    updateGL();
}

void LifeWidget::initializeGL() {
    if (curPlugin) {
        curPlugin->initView();
    } else {
        qglClearColor(Qt::black);
        glShadeModel(GL_SMOOTH);
    }
}

void LifeWidget::resizeGL(int width, int height) {
    curWidth = width;
    curHeight = height;
    if (curPlugin) {
        curPlugin->resizeView(width, height);
    } else {
        glViewport(0,0, (GLsizei) width, (GLsizei)height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(80, 1.0, 1.0, 140);
        glMatrixMode(GL_MODELVIEW);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void LifeWidget::paintGL() {
    if (curPlugin) {
        curPlugin->draw();
    } else {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glFlush();
    }
}
void LifeWidget::stop() {
    timer.stop();
}
void LifeWidget::start() {
    timer.start(1000/100);
}
void LifeWidget::reset() {
    if (curPlugin) {
        curIter = 0;
        curPlugin->reset();
        emit iterationDone(curIter);
    }
    updateGL();
}

void LifeWidget::resetView() {
    if (curPlugin) {
        curPlugin->initView();
    }
}
