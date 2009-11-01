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

#include "vtkOpenGLContextDevice2D.h"

#include "vtkPoints2D.h"

#include "vtkFloatArray.h"

#include "vtkActor2D.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPlane.h"
#include "vtkPlaneCollection.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkProperty2D.h"
#include "vtkScalarsToColors.h"
#include "vtkUnsignedCharArray.h"
#include "vtkViewport.h"
#include "vtkWindow.h"
#include "vtkgluPickMatrix.h"

#include "vtkRenderer.h"

#ifdef VTK_USE_QT
  #include "vtkQtLabelRenderStrategy.h"
#else
  #include "vtkFreeTypeLabelRenderStrategy.h"
#endif

#include "vtkObjectFactory.h"

vtkCxxRevisionMacro(vtkOpenGLContextDevice2D, "$Revision$");
vtkStandardNewMacro(vtkOpenGLContextDevice2D);

//-----------------------------------------------------------------------------
vtkOpenGLContextDevice2D::vtkOpenGLContextDevice2D()
{
  this->Renderer = 0;
#ifdef VTK_USE_QT
  this->TextRenderer = vtkQtLabelRenderStrategy::New();
#else
  this->TextRenderer = vtkFreeTypeLabelRenderStrategy::New();
#endif
}

//-----------------------------------------------------------------------------
vtkOpenGLContextDevice2D::~vtkOpenGLContextDevice2D()
{
  this->TextRenderer->Delete();
  this->TextRenderer = 0;
}

//-----------------------------------------------------------------------------
void vtkOpenGLContextDevice2D::Begin(vtkViewport* viewport)
{
  int size[2];
  size[0] = viewport->GetSize()[0];
  size[1] = viewport->GetSize()[1];
  double *vport = viewport->GetViewport();

  // push a 2D matrix on the stack
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glDisable(GL_LIGHTING);

  glDepthMask(GL_FALSE);

  glOrtho( 0.5, size[0]+0.5,
           0.5, size[1]+0.5,
          -1, 0);

  glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
  glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
  glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
  glEnable (GL_LINE_SMOOTH);
  glEnable (GL_POINT_SMOOTH);
  glEnable (GL_POLYGON_SMOOTH);

  this->Renderer = vtkRenderer::SafeDownCast(viewport);
  this->TextRenderer->SetRenderer(this->Renderer);
  this->TextRenderer->StartFrame();
  this->Modified();
}

//-----------------------------------------------------------------------------
void vtkOpenGLContextDevice2D::End()
{
  this->TextRenderer->EndFrame();
// push a 2D matrix on the stack
  glMatrixMode( GL_PROJECTION);
  glPopMatrix();
  glMatrixMode( GL_MODELVIEW);
  glPopMatrix();
  glEnable( GL_LIGHTING);

  // Turn it back on in case we've turned it off
  glDepthMask( GL_TRUE );

  this->Modified();
}

//-----------------------------------------------------------------------------
void vtkOpenGLContextDevice2D::DrawPoly(float *f, int n)
{
  if (f && n > 0)
    {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, &f[0]);
    glDrawArrays(GL_LINE_STRIP, 0, n);
    glDisableClientState(GL_VERTEX_ARRAY);
    }
  else
    {
    vtkWarningMacro(<< "Points supplied that were not of type float.");
    }
}

//-----------------------------------------------------------------------------
void vtkOpenGLContextDevice2D::DrawPoints(float *f, int n)
{
  if (f && n > 0)
    {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, &f[0]);
    glDrawArrays(GL_POINTS, 0, n);
    glDisableClientState(GL_VERTEX_ARRAY);
    }
  else
    {
    vtkWarningMacro(<< "Points supplied that were not of type float.");
    }
}

//-----------------------------------------------------------------------------
void vtkOpenGLContextDevice2D::DrawQuad(float *f, int n)
{
  if (f && n > 0)
    {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, f);
    glDrawArrays(GL_QUADS, 0, n);
    glDisableClientState(GL_VERTEX_ARRAY);
    }
  else
    {
    vtkWarningMacro(<< "Points supplied that were not of type float.");
    }
}

//-----------------------------------------------------------------------------
void vtkOpenGLContextDevice2D::DrawText(float *point, vtkTextProperty *prop,
                                  const vtkStdString &string)
{
  int p[] = { point[0], point[1] };
  this->TextRenderer->RenderLabel(&p[0], prop, string);
}

//-----------------------------------------------------------------------------
void vtkOpenGLContextDevice2D::SetColor(unsigned char *color)
{
  glColor4ubv(color);
}

//-----------------------------------------------------------------------------
void vtkOpenGLContextDevice2D::SetColor3(unsigned char *color)
{
  glColor3ubv(color);
}

//-----------------------------------------------------------------------------
void vtkOpenGLContextDevice2D::SetPointSize(float size)
{
  glPointSize(size);
}

//-----------------------------------------------------------------------------
void vtkOpenGLContextDevice2D::SetLineWidth(float width)
{
  glLineWidth(width);
}

//-----------------------------------------------------------------------------
void vtkOpenGLContextDevice2D::SetViewExtents(float *x)
{
  glLoadIdentity();
  glOrtho( x[0], x[2],
           x[1], x[3],
          -1, 0);
}

//-----------------------------------------------------------------------------
void vtkOpenGLContextDevice2D::PushMatrix()
{
  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
}

//-----------------------------------------------------------------------------
void vtkOpenGLContextDevice2D::PopMatrix()
{
  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();
}

//-----------------------------------------------------------------------------
void vtkOpenGLContextDevice2D::SetClipping(int *x)
{
  // Test the glScissor function
  vtkDebugMacro(<< "Clipping area: " << x[0] << "\t" << x[1]
                << "\t" << x[2] << "\t" << x[3]);
  glScissor(x[0], x[1], x[2], x[3]);
  glEnable(GL_SCISSOR_TEST);
}

//-----------------------------------------------------------------------------
void vtkOpenGLContextDevice2D::DisableClipping()
{
  glDisable(GL_SCISSOR_TEST);
}

//-----------------------------------------------------------------------------
void vtkOpenGLContextDevice2D::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Renderer: ";
  if (this->Renderer)
  {
    os << endl;
    this->Renderer->PrintSelf(os, indent.GetNextIndent());
  }
  else
    {
    os << "(none)" << endl;
    }
  os << indent << "Text Renderer: ";
  if (this->Renderer)
  {
    os << endl;
    this->TextRenderer->PrintSelf(os, indent.GetNextIndent());
  }
  else
    {
    os << "(none)" << endl;
    }
}
