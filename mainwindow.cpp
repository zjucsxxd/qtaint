/**
The MIT License (MIT)

Copyright (c) 2014 Ruben Van Boxem

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 **/

#include "mainwindow.h"

#include <QDebug>
#include <QLabel>
#include <QBoxLayout>
#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent),
  gray(false)
{
  label = new QLabel(this);
  grayscaleCheckBox = new QCheckBox("Grayscale", this);

  connect(grayscaleCheckBox, &QCheckBox::stateChanged,
          this, &MainWindow::toggleGrayscale);


  QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
  layout->addWidget(label);
  layout->addWidget(grayscaleCheckBox);

  QWidget* mainwidget = new QWidget(this);
  mainwidget->setLayout(layout);

  setCentralWidget(mainwidget);

  // must set central widget max size here, not QMainWindow's
  label->setMaximumSize(800,600);
}

void MainWindow::loadImage(const char* filename)
{
  original.load(filename);
  displayed = original;
  label->setPixmap(QPixmap::fromImage(displayed, Qt::MonoOnly));
  qDebug() << "Image size in bytes: " << original.byteCount();
}

void MainWindow::toggleGrayscale()
{
  if(gray)
  {
    gray = false;
    displayed = original;
  }
  else
  {
    gray = true;
    displayed = original.convertToFormat(QImage::Format_MonoLSB);
  }
  label->setPixmap(QPixmap::fromImage(displayed));
  label->repaint();
}
