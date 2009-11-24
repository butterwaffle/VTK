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
#include "vtkPen.h"
#include "vtkBrush.h"
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

#include "vtkMath.h"
#include "vtkPointMarkTick.h"
#include "vtkPointMarkStatBox.h"

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
  painter->GetPen()->SetColorF(0.8, 0.8, 0.8);
  painter->GetPen()->SetWidth(1.0);
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
  painter->GetDevice()->PushMatrix();
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
  painter->GetPen()->SetColorF(0.0, 0.0, 0.0, 1.0);
  painter->GetPen()->SetWidth(1.0);
  this->XAxis->Paint(painter);
  this->YAxis->Paint(painter);

  // And a semi-transparent rectangle
  painter->GetBrush()->SetColorF(0.75, 0.0, 0.0, 0.65);
  painter->GetPen()->SetColor(0, 0, 0, 255);
  painter->DrawRect(70, 100, 50, 200);
  painter->DrawQuad(200, 100, 200, 120,
                    250, 140, 300, 80);

  // Try out the tick-mark point marker:
#define NTICKS 64
  vtkFloatArray* ticks = vtkFloatArray::New();
  ticks->SetNumberOfComponents( 4 );
  ticks->SetNumberOfTuples( NTICKS );
  for ( int i = 0; i < NTICKS; ++ i )
    {
    float x, y, a, t;
    x = i / ( NTICKS - 1. );
    y = sin( 2. * vtkMath::Pi() * x );
    a = ( 0.5 + 5 * x * x ) * vtkMath::Pi();
    t = 10. * x * ( 1. - x ) * ( 1. - 4. * x ) * ( 1 - 4. * x ) + 0.75;
    ticks->SetTuple4( i, 800 * x + 80, 200 * y + 280, a, t );
    }
  vtkPointMarkTick* tickMarks = vtkPointMarkTick::New();
  tickMarks->BindParameter( "Length", 15 );
  tickMarks->BindParameter( "Angle", ticks, 2 ); // Angle of tick to horizontal.
  tickMarks->BindParameter( "Line Thickness", ticks, 3 ); // Thickness of tick mark line.
  tickMarks->DrawMarks( painter, ticks, 0, ticks, 1, 0, NTICKS, 1 );

  vtkPointMarkStatBox* statMarks = vtkPointMarkStatBox::New();
  statMarks->BindParameter( "Dot Size", 3. );
  statMarks->BindParameter( "Line Thickness", 1 );
  //statMarks->BindParameter( "Line Color", lineColor );
  //statMarks->BindParameter( "Fill Color", fillColor );
  statMarks->BindParameter( "Box Width", 10. );
  statMarks->BindParameter( "Box Ascent", ticks, 3 );
  statMarks->BindParameter( "Box Descent", ticks, 3 );
  statMarks->BindParameter( "Whisker Ascent", 20. );
  statMarks->BindParameter( "Whisker Descent", 30. );
  statMarks->DrawMarks( painter, ticks, 1, ticks, 0, 0, NTICKS, 1 );

  tickMarks->Delete();
  statMarks->Delete();
  ticks->Delete();

  return true;
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