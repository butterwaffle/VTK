#ifndef VTKQTCHART_H
#define VTKQTCHART_H

#include <QWidget>

#include "vtkObject.h"

class vtkChart;
class vtkContext2D;
class vtkQtContextDevice2D;

class VTK_CHARTS_EXPORT vtkQtChart : public QWidget
{
  Q_OBJECT
public:
  vtkQtChart(QWidget* parent = NULL, Qt::WFlags f = 0);
  virtual ~vtkQtChart();

  vtkChart *chart() { return this->Chart; }

protected:
  virtual void resizeEvent(QResizeEvent* event);
  virtual void paintEvent(QPaintEvent* event);

  vtkChart *Chart;
  vtkContext2D *Painter;
  vtkQtContextDevice2D *PaintDevice;
};

#endif // VTKQTCHART_H
