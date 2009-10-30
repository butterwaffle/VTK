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

#include "vtkQtContextDevice2D.h"

#include "vtkMath.h"
#include "vtkObjectFactory.h"

#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkProperty2D.h"
#include "vtkUnsignedCharArray.h"
#include "vtkStdString.h"

#include "vtkObjectFactory.h"

#include <QPainter>
#include <QPointF>
#include <QTransform>
#include <QPainterPath>

vtkCxxRevisionMacro(vtkQtContextDevice2D, "$Revision$");
vtkStandardNewMacro(vtkQtContextDevice2D);

//-----------------------------------------------------------------------------
vtkQtContextDevice2D::vtkQtContextDevice2D()
{

}

//-----------------------------------------------------------------------------
vtkQtContextDevice2D::~vtkQtContextDevice2D()
{

}

//-----------------------------------------------------------------------------
void vtkQtContextDevice2D::Begin(QPainter* painter)
{
  this->Painter = painter;
}

//-----------------------------------------------------------------------------
void vtkQtContextDevice2D::End()
{
  this->Painter = 0;
}

//-----------------------------------------------------------------------------
void vtkQtContextDevice2D::DrawPoly(float *f, int n)
{
  if (f && n > 0)
    {
    // Is this safe???
    QPointF qPoints[n];
    for (int i = 0; i < n; ++i)
      {
      qPoints[i] = QPointF(f[2*i], f[2*i+1]);
      }

    this->Painter->drawPolyline(&qPoints[0], n);
    }
  else
    {
    vtkWarningMacro(<< "Points supplied that were not of type float.");
    }
}

//-----------------------------------------------------------------------------
void vtkQtContextDevice2D::DrawPoints(float *f, int n)
{
  if (f && n > 0)
    {

    }
  else
    {
    vtkWarningMacro(<< "Points supplied that were not of type float.");
    }
}

//-----------------------------------------------------------------------------
void vtkQtContextDevice2D::DrawText(float *point, vtkTextProperty *prop,
                                  const vtkStdString &string)
{
  int p[] = { point[0], point[1] };
  vtkStdString buf = string;
  const char *text = buf;
  this->Painter->drawText(point[0], point[1], QString(text));
  //this->TextRenderer->RenderLabel(&p[0], prop, string);
}

//-----------------------------------------------------------------------------
void vtkQtContextDevice2D::SetColor(unsigned char *color)
{

}

//-----------------------------------------------------------------------------
void vtkQtContextDevice2D::SetColor3(unsigned char *color)
{

}


//-----------------------------------------------------------------------------
void vtkQtContextDevice2D::SetPointSize(float size)
{

}

//-----------------------------------------------------------------------------
void vtkQtContextDevice2D::SetLineWidth(float width)
{

}

//-----------------------------------------------------------------------------
void vtkQtContextDevice2D::SetViewExtents(float *x)
{
/*  QTransform tranform( , , ,
                       , , ,
                       , , );
*/
  cout << "SetViewExtents: " << x[0] << ", " << x[1] << ", " << x[2] << ", "
      << x[3] << endl;
  this->Painter->setWindow(x[0], x[3], x[2], -x[1]);
}

//-----------------------------------------------------------------------------
void vtkQtContextDevice2D::PushMatrix()
{
  this->Painter->save();
}

//-----------------------------------------------------------------------------
void vtkQtContextDevice2D::PopMatrix()
{
  this->Painter->restore();
}

//-----------------------------------------------------------------------------
void vtkQtContextDevice2D::SetClipping(int *x)
{
  // Test the glScissor function

}

//-----------------------------------------------------------------------------
void vtkQtContextDevice2D::DisableClipping()
{

}

//-----------------------------------------------------------------------------
void vtkQtContextDevice2D::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

