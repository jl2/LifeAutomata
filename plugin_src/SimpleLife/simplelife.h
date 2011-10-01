/*
  simplelife.h
  
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

#ifndef SIMPLE_LIFE_INCLUDE_H
#define SIMPLE_LIFE_INCLUDE_H

#include <QObject>
#include <QWidget>

#include <GL/gl.h>
#include <GL/glu.h>

#include "lifeplugin.h"

class SimpleLife : public QObject, public LifePlugin {
    Q_OBJECT;
    Q_INTERFACES(LifePlugin);
  
public:
    SimpleLife();
    ~SimpleLife();

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

    void setDim(int w, int h);
    void getDim(int &w, int &h);
    
private:
    int countNeighbors(int i, int j);

private:
    std::vector< std::vector<bool> > array;
    int width, height;
    double prob;
    double r,g,b;
};

#endif
