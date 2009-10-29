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

#include "vtkChartXY.h"

#include "vtkContext2D.h"
#include "vtkContextDevice2D.h"
#include "vtkPoints2D.h"

#include "vtkPlot.h"
#include "vtkPlotLine.h"

#include "vtkAxis.h"
#include "vtkPlotGrid.h"

#include "vtkTable.h"
#include "vtkAbstractArray.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"

#include "vtkObjectFactory.h"

#include "vtkStdString.h"
#include "vtkTextProperty.h"

// My STL containers
#include <vtkstd/vector>

//-----------------------------------------------------------------------------
class vtkChartXYPrivate
{
  public:
    vtkstd::vector<vtkPlot *> plots; // Charts can contain multiple plots of data
};

//-----------------------------------------------------------------------------
vtkCxxRevisionMacro(vtkChartXY, "$Revision$");

//-----------------------------------------------------------------------------
vtkStandardNewMacro(vtkChartXY);

//-----------------------------------------------------------------------------
vtkChartXY::vtkChartXY()
{
  this->XAxis = vtkAxis::New();
  this->YAxis = vtkAxis::New();
  this->Grid = vtkPlotGrid::New();
  this->Grid->SetXAxis(this->XAxis);
  this->Grid->SetYAxis(this->YAxis);
  this->ChartPrivate = new vtkChartXYPrivate;
}

//-----------------------------------------------------------------------------
vtkChartXY::~vtkChartXY()
{
  for (int i = 0; i < this->ChartPrivate->plots.size(); ++i)
    {
    this->ChartPrivate->plots[i]->Delete();
    }
  delete this->ChartPrivate;
  this->ChartPrivate = 0;

  this->XAxis->Delete();
  this->XAxis = 0;
  this->YAxis->Delete();
  this->YAxis = 0;
  this->Grid->Delete();
  this->Grid = 0;
}

//-----------------------------------------------------------------------------
void vtkChartXY::SetGeometry(int *p)
{
  vtkChart::SetGeometry(p);

  // This is where we set the axes up too
  float p1[] = { p[2], p[3], p[0]-p[4], p[3] };
  float p2[] = { p[2], p[3], p[2], p[1]-p[5] };
  this->XAxis->SetPoint1(p[2]     , p[3]     );
  this->XAxis->SetPoint2(p[0]-p[4], p[3]     );
  this->YAxis->SetPoint1(p[2]     , p[3]     );
  this->YAxis->SetPoint2(p[2]     , p[1]-p[5]);
}

//-----------------------------------------------------------------------------
bool vtkChartXY::Paint(vtkContext2D *painter)
{
  // This is where everything should be drawn, or dispatched to other methods.
  vtkDebugMacro(<< "Paint event called in vtkChartXY.");

  // This method could be optimized if only certain regions needed painting.

  // Set up the x and y axes - should be congigured based on data
  this->XAxis->SetMinimum(0.0);
  this->XAxis->SetMaximum(7.0);
  this->XAxis->SetNumberOfTicks(8);
  this->XAxis->SetLabel("X Axis");
  this->YAxis->SetMinimum(0.0);
  this->YAxis->SetMaximum(1.0);
  this->YAxis->SetNumberOfTicks(5);
  this->YAxis->SetLabel("Y Axis");

  // Draw a hard wired grid right now - this should be configurable
  painter->SetColor(0.8, 0.8, 0.8, 1.0);
  painter->SetLineWidth(1.0);
  this->Grid->Paint(painter);

  // The origin of the plot area
  float xOrigin = this->Geometry[2];
  float yOrigin = this->Geometry[3];

  // Get the scale for the plot area from the x and y axes
  float *min = this->XAxis->GetPoint1();
  float *max = this->XAxis->GetPoint2();
  float xScale = (this->XAxis->GetMaximum() - this->XAxis->GetMinimum()) /
                 (max[0] - min[0]);
  min = this->YAxis->GetPoint1();
  max = this->YAxis->GetPoint2();
  float yScale = (this->YAxis->GetMaximum() - this->YAxis->GetMinimum()) /
                 (max[1] - min[1]);

  // Set up the scaling for the plot area
  float plot[4];
  plot[0] = this->XAxis->GetMinimum() - xOrigin * xScale;
  plot[2] = this->XAxis->GetMaximum() + this->Geometry[4] * xScale;
  plot[1] = this->YAxis->GetMinimum() - yOrigin * yScale;
  plot[3] = this->YAxis->GetMaximum() + this->Geometry[5] * yScale;
  painter->GetDevice()->SetViewExtents(&plot[0]);

  // Clip drawing while plotting
  int clip[4];
  clip[0] = xOrigin;
  clip[1] = yOrigin;
  clip[2] = this->Geometry[0] - this->Geometry[4];
  clip[3] = this->Geometry[1] - this->Geometry[5];
  painter->GetDevice()->SetClipping(&clip[0]);

  // Now iterate through the plots
  int n = this->ChartPrivate->plots.size();
  for (int i = 0; i < n; ++i)
    {
    this->ChartPrivate->plots[i]->Paint(painter);
    }

  // Stop clipping and reset back to screen coordinates
  painter->GetDevice()->DisableClipping();
  painter->GetDevice()->PopMatrix();

  // Set the color and width, draw the axes, color and width push to axis props
  painter->SetColor(0.0, 0.0, 0.0, 1.0);
  painter->SetLineWidth(1.0);
  this->XAxis->Paint(painter);
  this->YAxis->Paint(painter);

  // Now let's try to render some test text
  vtkPoints2D *p = vtkPoints2D::New();
  p->SetNumberOfPoints(1);
  p->SetPoint(0, 175, 10);
  vtkTextProperty *prop = vtkTextProperty::New();
  prop->SetFontSize(15);
  prop->SetFontFamilyAsString("Arial");
  prop->SetColor(0.0, 0.0, 0.0);
  painter->SetPointSize(1);
  painter->DrawPoint(300, 300);
  prop->SetOrientation(0.0);
  p->SetPoint(0, 300, 300);
  painter->DrawText(p, prop, "Test Text");
  painter->SetColor(1.0, 0.0, 0.0, 1.0);
  p->SetPoint(0, 300, 310);
  painter->DrawPoint(300, 310);
  prop->SetJustificationToCentered();
  painter->DrawText(p, prop, "Test Text");
  painter->SetColor(0.0, 0.0, 1.0, 1.0);
  painter->DrawPoint(300, 320);
  p->SetPoint(0, 300, 320);
  prop->SetJustificationToRight();
  painter->DrawText(p, prop, "Test Text");

  // And a semi-transparent rectangle
  painter->SetColor(0.75, 0.0, 0.0, 0.65);
  painter->DrawRect(70, 100, 50, 200);
  painter->DrawQuad(200, 100, 200, 120,
                    250, 140, 300, 80);

}

//-----------------------------------------------------------------------------
void vtkChartXY::RenderPlots(vtkContext2D *painter)
{
  // This function ensures that the correct view transforms are in place for
  // the plot functions before calling paint on each one.
  float x[] = { -0.1, -0.1, 10.0, 2.1 };
  painter->GetDevice()->SetViewExtents(&x[0]);
}

//-----------------------------------------------------------------------------
vtkPlot * vtkChartXY::AddPlot(vtkChart::Type type)
{
  switch (type)
    {
    case LINE:
      vtkPlotLine *line = vtkPlotLine::New();
      this->ChartPrivate->plots.push_back(line);
      return line;
      break;
    }
}

//-----------------------------------------------------------------------------
vtkIdType vtkChartXY::GetNumberPlots()
{

}

//-----------------------------------------------------------------------------
void vtkChartXY::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "X Axis: ";
  if (this->XAxis)
    {
    os << endl;
    this->XAxis->PrintSelf(os, indent.GetNextIndent());
    }
    else
    {
    os << "(none)" << endl;
    }
  os << indent << "Y Axis: ";
  if (this->YAxis)
    {
    os << endl;
    this->YAxis->PrintSelf(os, indent.GetNextIndent());
    }
    else
    {
    os << "(none)" << endl;
    }
  if (this->ChartPrivate)
    {
    os << indent << "Number of plots: " << this->ChartPrivate->plots.size()
       << endl;
    for (int i = 0; i < this->ChartPrivate->plots.size(); ++i)
      {
      os << indent << "Plot " << i << ":" << endl;
      this->ChartPrivate->plots[i]->PrintSelf(os, indent.GetNextIndent());
      }
    }

}
