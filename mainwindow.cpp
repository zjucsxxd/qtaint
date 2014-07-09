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
#include <opencv2/imgproc/imgproc.hpp>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent),
  gray(false)
{
  label = new QLabel(this);
  label->setStyleSheet("background-color: white;");
  grayscaleCheckBox = new QCheckBox("Grayscale", this);

  connect(grayscaleCheckBox, &QCheckBox::stateChanged,
          this, &MainWindow::toggleGrayscale);


  QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
  layout->addWidget(label);
  layout->addWidget(grayscaleCheckBox);

  QWidget* mainwidget = new QWidget(this);
  mainwidget->setLayout(layout);
  setCentralWidget(mainwidget);

  original = cv::imread("examples/dice.png", CV_LOAD_IMAGE_UNCHANGED);
  original.copyTo(image);

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
  image = cv::imread(filename, CV_LOAD_IMAGE_UNCHANGED);
  qDebug() << "Image size in bytes: " << image.size().area();
  refresh();
}

void MainWindow::toggleGrayscale()
{
  if(gray)
  {
    gray = false;
   original.copyTo(image);
  }
  else
  {
    gray = true;
    static cv::Mat temp;
    cv::cvtColor(image, temp, CV_BGRA2GRAY);
    static int from_to[] = {0,0, 0,1, 0,2};
    // copy grayscale channel to all three channels of the image
    cv::mixChannels(&temp, 1, &image, 1, from_to, 3);
  }
  refresh();
}
