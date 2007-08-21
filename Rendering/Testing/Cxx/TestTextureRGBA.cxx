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

// .NAME Test of vtkGLSLShaderDeviceAdapter
// .SECTION Description
// this program tests the shader support in vtkRendering.

#include "vtkPlaneSource.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkTexture.h"
#include "vtkImageData.h"
#include "vtkPNGReader.h"
#include "vtkTestUtilities.h"
#include "vtkRegressionTestImage.h"

int TestTextureRGBA(int argc, char *argv[])
{
  char* fname = vtkTestUtilities::ExpandDataFileName(
      argc, argv, "Data/textureRGBA.png");

  vtkPNGReader *PNGReader = vtkPNGReader::New();
  PNGReader->SetFileName(fname);
  PNGReader->Update();

  vtkTexture *texture = vtkTexture::New();
  texture->SetInputConnection(PNGReader->GetOutputPort());
  PNGReader->Delete();
  texture->InterpolateOn();

  vtkPlaneSource *planeSource = vtkPlaneSource::New();
  planeSource->Update();

  vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
  mapper->SetInput(planeSource->GetOutput());
  planeSource->Delete();

  vtkActor *actor = vtkActor::New();
  actor->GetProperty()->SetTexture("texture",texture);
  texture->Delete();
  actor->SetMapper(mapper);
  mapper->Delete();

  vtkRenderer *renderer = vtkRenderer::New();
  renderer->AddActor(actor);
  actor->Delete();
  renderer->SetBackground(0.5,0.7,0.7);

  vtkRenderWindow *renWin = vtkRenderWindow::New();
  renWin->AddRenderer(renderer);

  vtkRenderWindowInteractor *interactor = vtkRenderWindowInteractor::New();
  interactor->SetRenderWindow(renWin);

  renWin->SetSize(400,400);
  renWin->Render();
  interactor->Initialize();
  renWin->Render();

  int retVal = vtkRegressionTestImage( renWin );
  if( retVal == vtkRegressionTester::DO_INTERACTOR)
    {
    interactor->Start();
    }

  renderer->Delete();
  renWin->Delete();
  interactor->Delete();

  return !retVal;
}
