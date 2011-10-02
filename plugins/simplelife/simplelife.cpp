/*
  simplelife.cpp
  
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

#include <QtGui>
#include <QDebug>

#include <QSettings>

#include <cstdlib>

#include "simplelife.h"

#include "simplelifeconfig.h"

size_t randUInt(size_t min, size_t max) {
    return ((std::rand()%(max-min)) + min);
}

SimpleLife::SimpleLife() : width(128), height(128), prob(0.4), r(0),g(1),b(1) {
}

void SimpleLife::readSettings(QSettings *sets) {
    width = sets->value("simple_width", 128).toInt();
    height = sets->value("simple_height", 128).toInt();

    prob = sets->value("simple_initial_fill", 0.4).toFloat();

    r = sets->value("simple_red", 0.0).toFloat();
    g = sets->value("simple_green", 0.8).toFloat();
    b = sets->value("simple_blue", 0.4).toFloat();

    reset();
}

void SimpleLife::configure(QWidget *parent, QSettings *sets) {
    SimpleLifeConfig *cfgDlg = new SimpleLifeConfig(this, sets, parent);
    cfgDlg->show();
}

SimpleLife::~SimpleLife() {
    array.clear();
}

QString SimpleLife::name() {
    return tr("Simple Life");
}

QString SimpleLife::description() {
    return tr("Traditional Conway's game of life.");
}

bool SimpleLife::allowViewManipulation() {
    return false;
}

void SimpleLife::initView() {
    glClearColor(0,0,0,0);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_LIGHTING);
    
    glDisable(GL_LIGHT0);
}

void SimpleLife::resizeView(int width, int height) {
    glViewport(0,0, (GLsizei) width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 100,
               0, 100);

    glMatrixMode(GL_MODELVIEW);

    glClear(GL_COLOR_BUFFER_BIT);
}

bool SimpleLife::evolve() {
    // qDebug() << "Evolving";

    int h = height;
    int w = width;

    std::vector< std::vector<bool> > rval;
    
    for (int i=0; i<height; ++i) {
        rval.push_back(std::vector<bool>());
        rval[i].resize(width, false);
    }

    for (int i=0; i<h; ++i) {
        for (int j=0; j<w; ++j) {
            int num = countNeighbors(i,j);

            if (array[i][j]) {
                if ((num < 2) || (num > 3)) {
                    rval[i][j] = false;
                } else {
                    rval[i][j] = true;
                }
            } else {
                if (num == 3) {
                    rval[i][j] = true;
                } else {
                    rval[i][j] = false;
                }
            }
        }
    }
    array = rval;
    return false;
}

void SimpleLife::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // glTranslatef(-0.5*width, -0.5*height, 0);
    float dx = 100.0/width;
    float dy = 100.0/height;

    // qDebug() << "Drawing";
    for (int i=0; i < height; ++i) {
        float cy = i*dy;
        for (int j=0; j<width; ++j) {
            
            if (array[i][j]) {
                float cx = j*dx;
                
                glBegin(GL_QUADS);
                glColor3f(r,g,b);
                glVertex2f(cx, cy);
                glVertex2f(cx+dx, cy);
                glVertex2f(cx+dx, cy+dy);
                glVertex2f(cx, cy+dy);
                glEnd();
            }
        }
    }
    glFlush();
}

void SimpleLife::reset() {
    array.clear();
    for (int i=0; i<height; ++i) {
        array.push_back(std::vector<bool>());
        array[i].resize(width, false);
    }
    int num = prob*width*height;
    for (int i=0;i<num; ++i) {
        size_t ri = randUInt(0, height);
        size_t rj = randUInt(0, width);
        array[ri][rj] = true;
    }
    
}
int SimpleLife::countNeighbors(int i, int j) {
    int w = width;
    int h = height;

    int num = 0;
    int up = i-1>0 ? i-1 : h-1;
    int down = i+1 <h-1 ? i+1 : 0;
    int left = j-1>0 ? j-1 : w-1;
    int right = j+1 < w-1 ? j+1 : 0;
    
    num += array[up][j];
    num += array[down][j];
    num += array[i][left];
    num += array[i][right];
    num += array[up][left];
    num += array[up][right];
    num += array[down][left];
    num += array[down][right];
    return num;
}

void SimpleLife::setRGB(double red, double green, double blue) {
    r = red;
    g = green;
    b = blue;
}

void SimpleLife::getRGB(double &red, double &green, double &blue) {
    red = r;
    green = g;
    blue = b;
}


void SimpleLife::setProb(double probability) {
    prob = probability;
}

void SimpleLife::getProb(double &probability) {
    probability = prob;
}

void SimpleLife::setDim(int w, int h) {
    width = w;
    height = h;
}
void SimpleLife::getDim(int &w, int &h) {
    w = width;
    h = height;
}


Q_EXPORT_PLUGIN2(simplelife, SimpleLife)
