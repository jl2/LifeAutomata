/*
  threedimlife.h
  
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

#ifndef THREE_DIM_LIFE_INCLUDE_H
#define THREE_DIM_LIFE_INCLUDE_H

#include <QObject>
#include <QWidget>

#include <GL/gl.h>
#include <GL/glu.h>

#include "lifeplugin.h"

static const size_t NUM_MATERIALS=2;
static const size_t NUM_LIGHTS=1;

static const size_t LINE_MAT=0;
static const size_t BOX_MAT=1;

typedef std::vector<bool>       vector_1d;
typedef std::vector<vector_1d> vector_2d;
typedef std::vector<vector_2d> vector_3d;


class ThreeDimLife : public QObject, public LifePlugin {
    Q_OBJECT;
    Q_INTERFACES(LifePlugin);
  
public:
    ThreeDimLife();
    ~ThreeDimLife();

    virtual QString name();
    virtual QString description();

    virtual bool allowViewManipulation();
    virtual void initView();
    virtual void resizeView(int width, int height);
    virtual void evolve();
    virtual void draw();
    virtual void reset();

    virtual void readSettings(QSettings *sets);
    virtual void configure(QWidget *parent, QSettings *sets);

    void setRGB(double red, double green, double blue);
    void getRGB(double &red, double &green, double &blue);

    void setProb(double probability);
    void getProb(double &prob);

    void setDim(int w, int h, int d);
    void getDim(int &w, int &h, int &d);
    
private:
    int countNeighbors(int i, int j, int k);

private:
    vector_3d array;

    int width, height, depth;
    double prob;
    double r,g,b;

    void initLights();
    void initMaterials();

    // Arrays to hold light properties
    GLfloat light_position[NUM_LIGHTS][4];
    GLfloat light_color[NUM_LIGHTS][4];
    GLfloat lmodel_ambient[NUM_LIGHTS][4];

    GLfloat rotationX,rotationY,rotationZ;
    GLfloat translate;

    // Arrays to hold material properties
    GLfloat mat_specular[NUM_MATERIALS][4];
    GLfloat mat_shininess[NUM_MATERIALS][1];
    GLfloat mat_diffuse[NUM_MATERIALS][4];
    GLfloat mat_ambient[NUM_MATERIALS][4];

};

#endif
