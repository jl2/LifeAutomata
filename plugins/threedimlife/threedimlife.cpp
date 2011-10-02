/*
  threedimlife.cpp
  
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

#include "threedimlife.h"

#include "threedimlifeconfig.h"

size_t randUInt(size_t min, size_t max) {
    return ((std::rand()%(max-min)) + min);
}

ThreeDimLife::ThreeDimLife() : width(32), height(32), depth(32), prob(0.4), r(0),g(1),b(1) {
    zoomAmount=75;
    rotateX = 45.0;
    rotateY = 45.0;
    rotateZ = 0.0;
}

void ThreeDimLife::readSettings(QSettings *sets) {
    width = sets->value("three_dim_width", 32).toInt();
    height = sets->value("three_dim_height", 32).toInt();
    depth = sets->value("three_dim_depth", 32).toInt();

    prob = sets->value("three_dim_initial_fill", 0.4).toFloat();

    r = sets->value("three_dim_red", 0.0).toFloat();
    g = sets->value("three_dim_green", 0.8).toFloat();
    b = sets->value("three_dim_blue", 0.4).toFloat();

    reset();
}

void ThreeDimLife::configure(QWidget *parent, QSettings *sets) {
    ThreeDimLifeConfig *cfgDlg = new ThreeDimLifeConfig(this, sets, parent);
    cfgDlg->show();
}

ThreeDimLife::~ThreeDimLife() {
    array.clear();
}

QString ThreeDimLife::name() {
    return tr("3D Life");
}

QString ThreeDimLife::description() {
    return tr("Traditional Conway's game of life.");
}

bool ThreeDimLife::allowViewManipulation() {
    return true;
}

void ThreeDimLife::initLights() {
    light_position[0][0]=0.0;
    light_position[0][1]=0.0;
    light_position[0][2]=30.0;
    light_position[0][3]=1.0;
  
    light_position[1][0]=0.0;
    light_position[1][1]=0.0;
    light_position[1][2]=-30.0;
    light_position[1][3]=1.0;
  
    for (size_t i=0;i<NUM_LIGHTS; ++i) {
        light_color[i][0]=1.0;
        light_color[i][1]=1.0;
        light_color[i][2]=1.0;
        light_color[i][3]=1.0;
        lmodel_ambient[i][0]=0.4;
        lmodel_ambient[i][1]=0.4;
        lmodel_ambient[i][2]=0.4;
        lmodel_ambient[i][3]=1.0;
    }

    glEnable(GL_LIGHTING);
    
    glEnable(GL_LIGHT0);
    
    //   glEnable(GL_LIGHT1);
}
void ThreeDimLife::initMaterials() {
    // lines
    mat_specular[LINE_MAT][0]=0.0;
    mat_specular[LINE_MAT][1]=0.0;
    mat_specular[LINE_MAT][2]=0.0;
    mat_specular[LINE_MAT][3]=1.0;
  
    mat_shininess[LINE_MAT][0]=80.0;

    mat_diffuse[LINE_MAT][0]=0.0;
    mat_diffuse[LINE_MAT][1]=0.0;
    mat_diffuse[LINE_MAT][2]=0.0;
    mat_diffuse[LINE_MAT][3]=1.0;
  
    mat_ambient[LINE_MAT][0] = 0.0;
    mat_ambient[LINE_MAT][1] = 0.0;
    mat_ambient[LINE_MAT][2] = 0.0;
    mat_ambient[LINE_MAT][3] = 1.0;

    // closed spots
    mat_specular[BOX_MAT][0]=1.0;
    mat_specular[BOX_MAT][1]=0.125;
    mat_specular[BOX_MAT][2]=0.125;
    mat_specular[BOX_MAT][3]=1.0;

    mat_shininess[BOX_MAT][0]=100.0;
  
    mat_diffuse[BOX_MAT][0]=0.5;
    mat_diffuse[BOX_MAT][1]=0.5;
    mat_diffuse[BOX_MAT][2]=0.5;
    mat_diffuse[BOX_MAT][3]=1.0;
  
    mat_ambient[BOX_MAT][0] = 0.130;
    mat_ambient[BOX_MAT][1] = 0.130;
    mat_ambient[BOX_MAT][2] = 0.130;
    mat_ambient[BOX_MAT][3] = 1.0;
}

void ThreeDimLife::initView() {
    glClearColor(0,0,0,0);
    glShadeModel(GL_SMOOTH);
    glShadeModel(GL_FLAT);
    
    glPolygonMode(GL_FRONT, GL_FILL);
    
    glEnable(GL_POLYGON_OFFSET_FILL);
    
    glEnable(GL_DEPTH_TEST);
    
    initLights();
    initMaterials();
}

void ThreeDimLife::resizeView(int width, int height) {
    glViewport(0,0, (GLsizei) width, (GLsizei)height);
  
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80, 1.0, 1.0, 180);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // glViewport(0,0, (GLsizei) width, (GLsizei)height);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // gluOrtho2D(0, 100,
    //            0, 100);

    // glMatrixMode(GL_MODELVIEW);

    // glClear(GL_COLOR_BUFFER_BIT);
}

void ThreeDimLife::evolve() {
    // qDebug() << "Evolving";

    int h = height;
    int w = width;
    int d = depth;

    vector_3d rval;

    rval.resize(height);
    for (int i=0; i<height; ++i) {
        rval[i].resize(width);
        // rval.push_back(std::vector< std::vector<bool> >() );
        // array[i].resize(width);
        for (int j=0;j<width; ++j) {
            // array[i][j].push_back(std::vector<bool>());
            rval[i][j].resize(depth, false);
        }
    }

    for (int i=0; i<h; ++i) {
        for (int j=0; j<w; ++j) {
            for (int k=0; k<d; ++k) {
                int num = countNeighbors(i,j, k);

                if (array[i][j][k]) {
                    if ((num < 2) || (num > 3)) {
                        rval[i][j][k] = false;
                    } else {
                        rval[i][j][k] = true;
                    }
                } else {
                    if (num == 3) {
                        rval[i][j][k] = true;
                    } else {
                        rval[i][j][k] = false;
                    }
                }
            }
        }
    }
    array = rval;
}

void ThreeDimLife::draw() {
    // Rotate/translate the projection matrix
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    
    glTranslatef(0.0,0.0,-(zoomAmount+5));
    glRotatef(rotateX, 1.0, 0.0, 0.0);
    glRotatef(rotateY, 0.0, 1.0, 0.0);
    glRotatef(rotateZ, 0.0, 0.0, 1.0);

    // Switch to modelview mode and draw the scene
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Setup the lights
    glLightfv(GL_LIGHT0, GL_POSITION, light_position[0]);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color[0]);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_color[0]);
  
    //   glLightfv(GL_LIGHT1, GL_POSITION, light_position[1]);
    //   glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color[1]);
    //   glLightfv(GL_LIGHT1, GL_SPECULAR, light_color[1]);
  
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient[0]);
    glLoadIdentity();


    static GLfloat cubeCorners[] = {0.0f, 1.0f, 1.0f,
                                     1.0f, 1.0f, 1.0f,
                                    1.0f, 0.0f, 1.0f,
                                    0.0f, 0.0f, 1.0f,
                                    0.0f, 1.0f, 0.0f,
                                    1.0f, 1.0f, 0.0f,
                                    1.0f, 0.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f,
    };
    static GLubyte indexes[] = {0, 1, 2, 3,
                                4, 5, 1, 0,
                                3, 2, 6, 7,
                                5, 4, 7, 6,
                                1, 5, 6, 2,
                                4, 0, 3, 7,
    };


    // glTranslatef(-0.5*width, -0.5*height, 0);
    float dx = 100.0/width;
    float dy = 100.0/height;
    float dz = 100.0/depth;

    // qDebug() << "Drawing";
    for (int i=0; i < height; ++i) {
        float cy = i*dy;
        for (int j=0; j<width; ++j) {
            float cx = j*dx;
            
            for (int k=0;k<depth; ++k) {
                float cz = k*dz;

                if (array[i][j][k]) {
                    
                    // Draw the box
                    glPushMatrix();
                    // glScalef(0.25, 0.25, 0.25);
                    glTranslatef(i,
                                 j,
                                 k);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse[BOX_MAT]);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient[BOX_MAT]);
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                    glEnableClientState(GL_VERTEX_ARRAY);

                    glVertexPointer(3, GL_FLOAT, 0, cubeCorners);

                    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indexes);
    
                    glLineWidth(1.0);

                    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse[LINE_MAT]);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient[LINE_MAT]);

                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);// Draw the outline
                    glVertexPointer(3, GL_FLOAT, 0, cubeCorners);
                    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indexes);
    
                    glLineWidth(1.0);
                    glPopMatrix();
                
                
                    // glBegin(GL_QUADS);
                    // glColor3f(r,g,b);
                    // glVertex2f(cx, cy);
                    // glVertex2f(cx+dx, cy);
                    // glVertex2f(cx+dx, cy+dy);
                    // glVertex2f(cx, cy+dy);
                    // glEnd();
                }
            }
        }
    }

    // Reset to how we found things
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    glMatrixMode(GL_MODELVIEW);

    glFlush();
}

void ThreeDimLife::reset() {
    array.clear();
    array.resize(height);
    for (int i=0; i<height; ++i) {
        array[i].resize(width);
        // rval.push_back(std::vector< std::vector<bool> >() );
        // array[i].resize(width);
        for (int j=0;j<width; ++j) {
            // array[i][j].push_back(std::vector<bool>());
            array[i][j].resize(depth, false);
        }
    }

    // for (int i=0; i<height; ++i) {
    //     array.push_back(std::vector< std::vector<bool> >() );
    //     // array[i].resize(width);
    //     for (int j=0;j<width; ++j) {
    //         array[i][j].push_back(std::vector<bool>());
    //         array[i][j].resize(width, false);
    //     }
    // }

    // for (int i=0; i<height; ++i) {
    //     array.push_back(std::vector< std::vector<bool> >());
    //     array[i].resize(width);
    //     for (int j=0;j<depth; ++j) {
    //         array[i][j].push_back(std::vector<bool>());
    //         array[i][j].resize(width);
    //     }
    // }
    int num = prob*width*height*depth;
    for (int i=0;i<num; ++i) {
        size_t ri = randUInt(0, height);
        size_t rj = randUInt(0, width);
        size_t rk = randUInt(0, depth);
        array[ri][rj][rk] = true;
    }
    
}
int ThreeDimLife::countNeighbors(int i, int j, int k) {
    int w = width;
    int h = height;

    int num = 0;
    int up = i-1>0 ? i-1 : h-1;
    int down = i+1 <h-1 ? i+1 : 0;
    int left = j-1>0 ? j-1 : w-1;
    int right = j+1 < w-1 ? j+1 : 0;
    
    num += array[up][j][k];
    num += array[down][j][k];
    num += array[i][left][k];
    num += array[i][right][k];
    num += array[up][left][k];
    num += array[up][right][k];
    num += array[down][left][k];
    num += array[down][right][k];

    return num;
}

void ThreeDimLife::setRGB(double red, double green, double blue) {
    r = red;
    g = green;
    b = blue;
}

void ThreeDimLife::getRGB(double &red, double &green, double &blue) {
    red = r;
    green = g;
    blue = b;
}


void ThreeDimLife::setProb(double probability) {
    prob = probability;
}

void ThreeDimLife::getProb(double &probability) {
    probability = prob;
}

void ThreeDimLife::setDim(int w, int h, int d) {
    width = w;
    height = h;
    depth = d;
}
void ThreeDimLife::getDim(int &w, int &h, int &d) {
    w = width;
    h = height;
    d = depth;
}
void ThreeDimLife::zoom(double amt) {
    zoomAmount += amt;
    if (zoomAmount < 10) zoomAmount = 10.0;
}

void wrap(double &val, double min, double max) {
    if (val < min) val = max;
    if (val > max) val = min;
}

void ThreeDimLife::rotate(double x, double y, double z) {
    rotateX += x;
    rotateY += y;
    rotateZ += z;
    wrap(x,0,360);
    wrap(y,0,360);
    wrap(z,0,360);
}


Q_EXPORT_PLUGIN2(threedimlife, ThreeDimLife)
