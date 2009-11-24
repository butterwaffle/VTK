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

#include "vtkContext2D.h"

#include "vtkPoints2D.h"
#include "vtkContextDevice2D.h"
#include "vtkPen.h"
#include "vtkBrush.h"

#include "vtkFloatArray.h"

#include "vtkObjectFactory.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

vtkCxxRevisionMacro(vtkContext2D, "$Revision$");
vtkCxxSetObjectMacro(vtkContext2D, Pen, vtkPen);
vtkCxxSetObjectMacro(vtkContext2D, Brush, vtkBrush);

//-----------------------------------------------------------------------------
vtkStandardNewMacro(vtkContext2D);

//-----------------------------------------------------------------------------
bool vtkContext2D::Begin(vtkContextDevice2D *device)
{
  this->Device = device;
  this->Modified();
}

//-----------------------------------------------------------------------------
bool vtkContext2D::End()
{
  this->Device->End();
  this->Device = NULL;
  this->Modified();
}

//-----------------------------------------------------------------------------
void vtkContext2D::DrawLine(float x1, float y1, float x2, float y2)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  float x[] = { x1, y1, x2, y2 };

  this->ApplyPen();
  this->Device->DrawPoly(&x[0], 2);
}

//-----------------------------------------------------------------------------
void vtkContext2D::DrawLine(float p[4])
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }

  this->ApplyPen();
  this->Device->DrawPoly(&p[0], 2);
}

//-----------------------------------------------------------------------------
void vtkContext2D::DrawLine(vtkPoints2D *points)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  if (points->GetNumberOfPoints() < 2)
    {
    vtkErrorMacro(<< "Attempted to paint a line with <2 points.");
    return;
    }
  float *f = vtkFloatArray::SafeDownCast(points->GetData())->GetPointer(0);

  this->ApplyPen();
  this->Device->DrawPoly(f, 2);
}

//-----------------------------------------------------------------------------
void vtkContext2D::DrawPoly(float *x, float *y, int n)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  float p[2*n];
  for (int i = 0; i < n; ++i)
    {
    p[2*i]   = x[i];
    p[2*i+1] = y[i];
    }

  this->ApplyPen();
  this->Device->DrawPoly(&p[0], n);
}

//-----------------------------------------------------------------------------
void vtkContext2D::DrawPoly(vtkPoints2D *points)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  // Construct an array with the correct coordinate packing for OpenGL.
  int n = points->GetNumberOfPoints();

  // If the points are of type float then call OpenGL directly
  float *f = vtkFloatArray::SafeDownCast(points->GetData())->GetPointer(0);

  this->ApplyPen();
  this->Device->DrawPoly(f, n);
}

//-----------------------------------------------------------------------------
void vtkContext2D::DrawPoly(float *points, int n)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  if (n < 2)
    {
    vtkErrorMacro(<< "Attempted to paint a line with <2 points.");
    return;
    }

  this->ApplyPen();
  this->Device->DrawPoly(points, n);
}

//-----------------------------------------------------------------------------
void vtkContext2D::DrawPoint(float x, float y)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  float p[] = { x, y };

  this->ApplyPen();
  this->Device->DrawPoints(&p[0], 1);
}

//-----------------------------------------------------------------------------
void vtkContext2D::DrawPoints(float *x, float *y, int n)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  float p[2*n];
  for (int i = 0; i < n; ++i)
    {
    p[2*i]   = x[i];
    p[2*i+1] = y[i];
    }

  this->ApplyPen();
  this->Device->DrawPoints(&p[0], n);
}

//-----------------------------------------------------------------------------
void vtkContext2D::DrawPoints(vtkPoints2D *points)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  // Construct an array with the correct coordinate packing for OpenGL.
  int n = points->GetNumberOfPoints();

  // If the points are of type float then call OpenGL directly
  float *f = vtkFloatArray::SafeDownCast(points->GetData())->GetPointer(0);

  this->ApplyPen();
  this->Device->DrawPoints(f, n);
}

//-----------------------------------------------------------------------------
void vtkContext2D::DrawPoints(float *points, int n)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }

  this->ApplyPen();
  this->Device->DrawPoints(points, n);
}

//-----------------------------------------------------------------------------
void vtkContext2D::DrawRect(float x1, float y1, float x2, float y2)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  float p[] = { x1,    y1,
                x1+x2, y1,
                x1+x2, y1+y2,
                x1,    y1+y2 };

  // Draw the filled area of the rectangle.
  this->ApplyBrush();
  this->Device->DrawQuad(&p[0], 4);

    // Draw the outline now.
  this->ApplyPen();
  this->Device->DrawPoly(p, 4);
  float closeLine[] = { p[0], p[1], p[6], p[7] };
  this->Device->DrawPoly(&closeLine[0], 2);
}

//-----------------------------------------------------------------------------
void vtkContext2D::DrawQuad(float x1, float y1, float x2, float y2,
                            float x3, float y3, float x4, float y4)
{
  float p[] = { x1, y1, x2, y2, x3, y3, x4, y4 };
  this->DrawQuad(&p[0]);
}

//-----------------------------------------------------------------------------
void vtkContext2D::DrawQuad(float *p)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }

  // Draw the filled area of the quad.
  this->ApplyBrush();
  this->Device->DrawQuad(p, 4);

  // Draw the outline now.
  this->ApplyPen();
  this->Device->DrawPoly(p, 4);
  float closeLine[] = { p[0], p[1], p[6], p[7] };
  this->Device->DrawPoly(&closeLine[0], 2);
}

//-----------------------------------------------------------------------------
void vtkContext2D::DrawText(vtkPoints2D *point, vtkTextProperty *prop,
                            const vtkStdString &string)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  float *f = vtkFloatArray::SafeDownCast(point->GetData())->GetPointer(0);
  this->Device->DrawText(f, prop, string);
}

//-----------------------------------------------------------------------------
void vtkContext2D::DrawText(int x, int y, vtkTextProperty *prop,
                            const vtkStdString &string)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  float f[] = { x, y };
  this->Device->DrawText(&f[0], prop, string);
}

//-----------------------------------------------------------------------------
void vtkContext2D::DrawText(vtkPoints2D *point, vtkTextProperty *prop,
                            const char *string)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  float *f = vtkFloatArray::SafeDownCast(point->GetData())->GetPointer(0);
  vtkStdString str = string;
  this->Device->DrawText(f, prop, str);
}

//-----------------------------------------------------------------------------
void vtkContext2D::DrawText(int x, int y, vtkTextProperty *prop,
                            const char *string)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  float f[] = { x, y };
  vtkStdString str = string;
  this->Device->DrawText(&f[0], prop, str);
}

//-----------------------------------------------------------------------------
inline void vtkContext2D::ApplyPen()
{
  this->Device->SetColor(this->Pen->GetColor());
  this->Device->SetLineWidth(this->Pen->GetWidth());
  this->Device->SetPointSize(this->Pen->GetWidth());
}

//-----------------------------------------------------------------------------
inline void vtkContext2D::ApplyBrush()
{
  this->Device->SetColor(this->Brush->GetColor());
}

//-----------------------------------------------------------------------------
vtkContext2D::vtkContext2D()
{
  this->Device = NULL;
  this->Pen = vtkPen::New();
  this->Brush = vtkBrush::New();
}

//-----------------------------------------------------------------------------
vtkContext2D::~vtkContext2D()
{
  this->Pen->Delete();
  this->Pen = 0;
  this->Brush->Delete();
  this->Brush = 0;
}

//-----------------------------------------------------------------------------
void vtkContext2D::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Context Device: ";
  if (this->Device)
    {
    os << endl;
    this->Device->PrintSelf(os, indent.GetNextIndent());
    }
  else
    {
    os << "(none)" << endl;
    }
  os << indent << "Pen: ";
  this->Pen->PrintSelf(os, indent.GetNextIndent());
  os << indent << "Brush: ";
  this->Brush->PrintSelf(os, indent.GetNextIndent());
}
