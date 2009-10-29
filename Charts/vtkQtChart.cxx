#include "vtkQtChart.h"

#include <QPaintEvent>
#include <QPainter>

#include "vtkChartXY.h"
#include "vtkContext2D.h"
#include "vtkQtContextDevice2D.h"

vtkQtChart::vtkQtChart(QWidget* parent, Qt::WFlags f)
{
  this->Chart = vtkChartXY::New();
  this->Painter = vtkContext2D::New();
  this->PaintDevice = vtkQtContextDevice2D::New();
  this->Painter->Begin(this->PaintDevice);

  this->setBackgroundRole(QPalette::Light);
  this->setAutoFillBackground(true);
}

vtkQtChart::~vtkQtChart()
{
  this->Chart->Delete();
  this->Chart = 0;
  this->Painter->GetDevice()->Delete();
  this->Painter->Delete();
  this->Painter = 0;
}

void vtkQtChart::resizeEvent(QResizeEvent* event)
{
  // Do nothing...
}

void vtkQtChart::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);

  painter.setWindow(0, this->height(), this->width(), -this->height());

  int geometry[] = { this->width(), this->height(),
                     40,      40,
                     20,      20 };
  this->Chart->SetGeometry(&geometry[0]);

  this->PaintDevice->Begin(&painter);
  this->Chart->Paint(this->Painter);
  this->PaintDevice->End();
}
