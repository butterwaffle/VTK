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

#include "vtkFloatArray.h"

#include "vtkObjectFactory.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

vtkCxxRevisionMacro(vtkContext2D, "$Revision$");

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
  float x[] = { x1,    y1,
                x1+x2, y1,
                x1+x2, y1+y2,
                x1,    y1+y2 };
  this->Device->DrawRect(&x[0], 4);
}

//-----------------------------------------------------------------------------
void vtkContext2D::DrawQuad(float x1, float y1, float x2, float y2,
                            float x3, float y3, float x4, float y4)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  float p[] = { x1, y1, x2, y2, x3, y3, x4, y4 };
  this->Device->DrawRect(&p[0], 4);
}

//-----------------------------------------------------------------------------
void vtkContext2D::DrawQuad(float *p)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  this->Device->DrawRect(p, 4);
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
void vtkContext2D::SetColor(unsigned char r, unsigned char g, unsigned char b,
                            unsigned char a)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  this->Device->SetColor(r, g, b, a);
}

//-----------------------------------------------------------------------------
void vtkContext2D::SetColor(unsigned char r, unsigned char g, unsigned char b)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  this->Device->SetColor(r, g, b);
}

//-----------------------------------------------------------------------------
void vtkContext2D::SetColor(double r, double g, double b, double a)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  this->Device->SetColor(static_cast<unsigned char>(r*255),
                         static_cast<unsigned char>(g*255),
                         static_cast<unsigned char>(b*255),
                         static_cast<unsigned char>(a*255));
}

//-----------------------------------------------------------------------------
void vtkContext2D::SetColor(double r, double g, double b)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  this->Device->SetColor(static_cast<unsigned char>(r*255),
                         static_cast<unsigned char>(g*255),
                         static_cast<unsigned char>(b*255));
}

//-----------------------------------------------------------------------------
void vtkContext2D::SetPointSize(float size)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  this->Device->SetPointSize(size);
}

//-----------------------------------------------------------------------------
void vtkContext2D::SetLineWidth(float width)
{
  if (!this->Device)
    {
    vtkErrorMacro(<< "Attempted to paint with no active vtkContextDevice2D.");
    return;
    }
  this->Device->SetLineWidth(width);
}

//-----------------------------------------------------------------------------
vtkContext2D::vtkContext2D()
{
  this->Device = NULL;
}

//-----------------------------------------------------------------------------
vtkContext2D::~vtkContext2D()
{
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
}

