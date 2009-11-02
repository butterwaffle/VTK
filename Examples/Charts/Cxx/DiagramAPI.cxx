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

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkChartActor.h"
#include "vtkObjectFactory.h"
#include "vtkChart.h"
#include "vtkContext2D.h"
#include "vtkPen.h"
#include "vtkBrush.h"
#include "vtkTextProperty.h"

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

//----------------------------------------------------------------------------
class APIDiagram : public vtkChart
{
public:
  static APIDiagram *New();
  // Paint event for the chart, called whenever the chart needs to be drawn
  virtual bool Paint(vtkContext2D *painter);
};

//----------------------------------------------------------------------------
int main( int argc, char * argv [] )
{
  // Set up a 2D chart actor, APIDiagram object andn add them to the renderer
  VTK_CREATE(vtkChartActor, actor);
  VTK_CREATE(APIDiagram, diagram);
  actor->SetChart(diagram);
  VTK_CREATE(vtkRenderer, renderer);
  renderer->SetBackground(1.0, 1.0, 1.0);
  VTK_CREATE(vtkRenderWindow, renderWindow);
  renderWindow->SetSize(800, 600);
  renderWindow->AddRenderer(renderer);
  renderer->AddActor(actor);

  VTK_CREATE(vtkRenderWindowInteractor, interactor);
  interactor->SetRenderWindow(renderWindow);
  interactor->Initialize();
  interactor->Start();

  return 0;
}

// Make our new derived class to draw a diagram
vtkStandardNewMacro(APIDiagram);
// This function draws our API diagram
bool APIDiagram::Paint(vtkContext2D *painter)
{
  // Drawing a hard wired diagram 800x600 as a demonstration of the 2D API
  painter->GetTextProp()->SetVerticalJustificationToCentered();
  painter->GetTextProp()->SetJustificationToCentered();
  painter->GetTextProp()->SetColor(0.0, 0.0, 0.0);
  painter->GetTextProp()->SetFontSize(24);
  painter->GetPen()->SetColor(0, 0, 0);

  painter->GetBrush()->SetColor(100, 255, 100);
  painter->DrawRect(100, 50, 200, 100);
  painter->DrawText(200, 100, "OpenGL");

  painter->GetBrush()->SetColor(255, 100, 0);
  painter->DrawRect(300, 50, 200, 100);
  painter->DrawText(400, 100, "Qt");

  painter->GetBrush()->SetColor(100, 0, 255);
  painter->DrawRect(500, 50, 200, 100);
  painter->DrawText(600, 100, "Cairo");

  painter->GetBrush()->SetColor(180, 180, 255);
  painter->DrawRect(100, 150, 600, 100);
  painter->DrawText(400, 200, "2D API");

  painter->GetBrush()->SetColor(255, 255, 180);
  painter->DrawRect(100, 250, 600, 200);
  painter->DrawText(400, 400, "Chart API");

  painter->GetBrush()->SetColor(180, 255, 180);
  painter->DrawRect(100, 250, 300, 100);
  painter->DrawText(250, 300, "Point Mark");

  painter->GetBrush()->SetColor(255, 255, 255);
  painter->DrawRect(100, 450, 600, 100);
  painter->DrawText(400, 500, "Chart View");
}
