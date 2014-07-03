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
#include <QVector>

#include <opencv2/highgui/highgui.hpp>

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

  image = cv::imread("examples/dice.png", -1); // negative to load image as-is, with transparency

  // must set central widget max size here, not QMainWindow's
  label->setMaximumSize(800,600);
}

// move elsewhere
QVector<QRgb> grayscale_colortable()
{
  QVector<QRgb> result(256);
  for (int i = 0; i < 256; ++i)
     result[i] = qRgb(i, i, i);

  return result;
}

void MainWindow::refresh()
{
  switch(image.type())
  {
    case CV_8UC4:
      displayed = QPixmap::fromImage(QImage(image.data, image.cols, image.rows, image.step, QImage::Format_ARGB32));
      break;
    case CV_8UC3:
      displayed = QPixmap::fromImage(QImage(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888).rgbSwapped());
      break;
    case CV_8UC1:
    {
      static QVector<QRgb> color_table = grayscale_colortable();
      QImage temp(image.data, image.cols, image.rows, image.step, QImage::Format_Indexed8);
      temp.setColorTable(color_table);
      displayed = QPixmap::fromImage(temp);
      break;
    }
    default:
      qWarning() << "Image type not handled" << image.type();
      break;
  }
  label->setPixmap((displayed));
}

void MainWindow::loadImage(const char* filename)
{
  image = cv::imread(filename, -1); // -1 = as-is, keeping transparency
  qDebug() << "Image size in bytes: " << image.size().area();
  refresh();
}

void MainWindow::toggleGrayscale()
{
  if(gray)
  {
    gray = false;
    //displayed = original;
  }
  else
  {
    gray = true;
    //displayed = original.convertToFormat(QImage::Format_MonoLSB);
  }
  //label->setPixmap(QPixmap::fromImage(displayed));
  //label->repaint();
}
