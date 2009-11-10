/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile$

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

#include "vtkSmartPointer.h"

#include "vtkChartActor.h"
#include "vtkChart.h"
#include "vtkPlot.h"
#include "vtkTable.h"
#include "vtkDelimitedTextReader.h"

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QHBoxLayout>

#include "QVTKWidget.h"
#include "vtkQtTableView.h"

// The QWidget form of a vtkChart
#include "vtkQtChart.h"

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

//----------------------------------------------------------------------------
int main(int argc, char * argv [])
{
  // Qt initialization
  QApplication app(argc, argv);
  QMainWindow *mainWindow = new QMainWindow;
  mainWindow->setGeometry(0, 0, 1150, 600);

  // QVTK set up and initialization
  QVTKWidget *qvtkWidget = new QVTKWidget(mainWindow);

  // Set up my 2D world...
  VTK_CREATE(vtkChartActor, actor); // This contains a chart object

  VTK_CREATE(vtkDelimitedTextReader, reader);
  reader->SetFieldDelimiterCharacters(",");
  reader->SetStringDelimiter('"');
  reader->SetUseStringDelimiter(true);
  reader->SetFileName("plot.csv");
  reader->SetHaveHeaders(true);
  reader->SetDetectNumericColumns(true);
  reader->Update();

  // Add multiple line plots, setting the colors etc
  vtkTable *table = reader->GetOutput();
  table->Update();
  vtkChart *chart = actor->GetChart();
  vtkPlot *line = chart->AddPlot(vtkChart::LINE);
  line->DebugOn();
  line->SetInput(table, 0, 1);
  line->SetColor(255, 0, 0, 255);

  // Now set up the renderer, background color
  VTK_CREATE(vtkRenderer, renderer);
  renderer->SetBackground(1.0, 1.0, 1.0);
  vtkRenderWindow *renderWindow = vtkRenderWindow::New();
  renderWindow->AddRenderer(renderer);
  qvtkWidget->SetRenderWindow(renderWindow);

  // Add the chart, which is derived from a vtkProp
  renderer->AddActor(actor);

  // Now lets try to add a table view
  QWidget *widget = new QWidget(mainWindow);
  QHBoxLayout *layout = new QHBoxLayout(widget);
  VTK_CREATE(vtkQtTableView, tableView);
  tableView->SetSplitMultiComponentColumns(true);
  tableView->AddRepresentationFromInput(table);
  tableView->Update();
  layout->addWidget(qvtkWidget, 2);
  layout->addWidget(tableView->GetWidget());
  mainWindow->setCentralWidget(widget);

  // Now show the application and start the event loop
  mainWindow->show();

  return app.exec();
}
