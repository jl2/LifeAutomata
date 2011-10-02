/*
  lifeplugin.h
  
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

#ifndef LIFE_PLUGIN_H
#define LIFE_PLUGIN_H

class QSettings;
class QWidget;

class LifePlugin {
public:
    virtual ~LifePlugin() {};
    virtual QString name()=0;
    virtual QString description()=0;

    virtual bool allowViewManipulation()=0;
    virtual void initView()=0;
    virtual void resizeView(int width, int height)=0;
    virtual bool evolve()=0;
    virtual void draw()=0;
    virtual void reset()=0;
    virtual void readSettings(QSettings *sets) = 0;
    virtual void configure(QWidget *parent,QSettings *sets) = 0;

    virtual void zoom(double) {};
    virtual void rotate(double, double, double) {};
    
};

Q_DECLARE_INTERFACE(LifePlugin, "com.jlarocco.LifePlugin/0.1")

#endif
