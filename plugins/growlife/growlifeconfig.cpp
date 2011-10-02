/*
  growlifeconfig.cpp
  
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

#include "growlifeconfig.h"

GrowLifeConfig::GrowLifeConfig(GrowLife *sl,
                                   QSettings *sets,
                                   QWidget *parent) : QDialog(parent),
                                                      life(sl),
                                                      settings(sets) {

    QGridLayout *layout = new QGridLayout();
    int curRow = 0;

    int width, height, depth;
    life->getDim(width, height, depth);

    layout->addWidget(new QLabel(tr("Width")), curRow, 0);
    widthEdit = new QLineEdit(tr("%1").arg(width));
    layout->addWidget(widthEdit, curRow, 1);
    curRow+=1;
    
    layout->addWidget(new QLabel(tr("Height")), curRow, 0);
    heightEdit = new QLineEdit(tr("%1").arg(height));
    layout->addWidget(heightEdit, curRow, 1);
    curRow+=1;

    layout->addWidget(new QLabel(tr("Depth")), curRow, 0);
    depthEdit = new QLineEdit(tr("%1").arg(depth));
    layout->addWidget(depthEdit, curRow, 1);
    curRow+=1;

    double prob;
    life->getProb(prob);
    layout->addWidget(new QLabel(tr("Percent fill")), curRow, 0);
    probEdit = new QLineEdit(tr("%1").arg(prob,0,'g', 3));
    layout->addWidget(probEdit, curRow, 1);
    curRow += 1;

    double r,g,b;
    life->getRGB(r,g,b);
    
    layout->addWidget(new QLabel(tr("Red")), curRow, 0);
    redEdit = new QLineEdit(tr("%1").arg(r,0,'g', 3));
    layout->addWidget(redEdit, curRow, 1);
    curRow += 1;

    layout->addWidget(new QLabel(tr("Green")), curRow, 0);
    greenEdit = new QLineEdit(tr("%1").arg(g,0,'g', 3));
    layout->addWidget(greenEdit, curRow, 1);
    curRow += 1;
    
    layout->addWidget(new QLabel(tr("Blue")), curRow, 0);
    blueEdit = new QLineEdit(tr("%1").arg(b,0,'g', 3));
    layout->addWidget(blueEdit, curRow, 1);
    curRow += 1;
    // QPushButton *colorPicker = new QPushButton("");
    // colorPicker->
        
    okayButton = new QPushButton(tr("Okay"));
    layout->addWidget(okayButton, curRow, 0);
    connect(okayButton, SIGNAL(clicked()), this, SLOT(finish()));

    cancelButton = new QPushButton(tr("Cancel"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    layout->addWidget(cancelButton, curRow, 1);
    curRow += 1;

    setLayout(layout);
}

void GrowLifeConfig::finish() {
    int newWidth = widthEdit->text().toInt();
    int newHeight = heightEdit->text().toInt();
    int newDepth = depthEdit->text().toInt();

    double newProb = probEdit->text().toDouble();
    double newRed = redEdit->text().toDouble();
    double newGreen = greenEdit->text().toDouble();
    double newBlue = blueEdit->text().toDouble();

    if (settings) {
        settings->setValue("grow_width", newWidth);
        settings->setValue("grow_height", newHeight);
        settings->setValue("grow_depth", newDepth);
        settings->setValue("grow_initial_fill", newProb);

        settings->value("grow_red", newRed);
        settings->value("grow_green", newGreen);
        settings->value("grow_blue", newBlue);

        settings->sync();
    }

    life->setDim(newWidth, newHeight, newDepth);
    life->setProb(newProb);
    life->setRGB(newRed, newGreen, newBlue);

    this->close();

    life->reset();
}
