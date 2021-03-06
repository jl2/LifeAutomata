/*
  simplelifeconfig.h
  
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

#ifndef SIMPLE_LIFE_CONFIG_INCLUDE_H
#define SIMPLE_LIFE_CONFIG_INCLUDE_H

#include <QDialog>

#include "simplelife.h"

class QPushButton;
class QLineEdit;
class QLabel;
class QSettings;

class SimpleLifeConfig : public QDialog {
    Q_OBJECT;
public:
    SimpleLifeConfig(SimpleLife *sl, QSettings *sets=0, QWidget *parent = 0);

public slots:
    void finish();

private:
    QLineEdit *widthEdit;
    QLineEdit *heightEdit;
    QLineEdit *probEdit;

    QLineEdit *redEdit;
    QLineEdit *greenEdit;
    QLineEdit *blueEdit;

    QPushButton *okayButton;
    QPushButton *cancelButton;

    SimpleLife *life;
    QSettings *settings;
};

#endif
