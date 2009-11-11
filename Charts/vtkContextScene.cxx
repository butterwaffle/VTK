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

#include "vtkContextScene.h"

#include "vtkContextItem.h"

// Get my new commands
#include "vtkCommand.h"

#include "vtkAnnotationLink.h"
#include "vtkInteractorStyle.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"
#include "vtkInteractorStyleRubberBand2D.h"
#include "vtkObjectFactory.h"

// My STL containers
#include <vtkstd/vector>

//-----------------------------------------------------------------------------
// Minimal command class to handle callbacks.
class vtkContextScene::Command : public vtkCommand
{
public:
  Command(vtkContextScene *scene) { this->Target = scene; }

  virtual void Execute(vtkObject *caller, unsigned long eventId, void *callData)
    {
    if (this->Target)
      {
      vtkInteractorStyle *style = vtkInteractorStyle::SafeDownCast(caller);
      vtkRenderWindowInteractor *interactor =
          vtkRenderWindowInteractor::SafeDownCast(style->GetInteractor());

      int x = interactor->GetEventPosition()[0];
      int y = interactor->GetEventPosition()[1];

      switch (eventId)
        {
        case vtkCommand::MouseMoveEvent :
          this->Target->MouseMoveEvent(x, y);
          break;
        case vtkCommand::LeftButtonPressEvent :
          this->Target->LeftButtonPressEvent(x, y);
          break;
        case vtkCommand::LeftButtonReleaseEvent :
          this->Target->LeftButtonReleaseEvent(x, y);
          break;
        case vtkCommand::SelectionChangedEvent :
          this->Target->ProcessSelectionEvent(caller, callData);
          break;
        default:
          this->Target->ProcessEvents(caller, eventId, callData);
        }
      }
    }

  void SetTarget(vtkContextScene* t) { this->Target = t; }

  vtkContextScene *Target;
};

//-----------------------------------------------------------------------------
// Minimal storage class for STL containers etc.
class vtkContextScene::Private
{
public:
  vtkstd::vector<vtkContextItem *> items;
  vtkstd::vector<bool> itemState;
  int itemMousePressCurrent; // Index of the item with a current mouse down
};

//-----------------------------------------------------------------------------
vtkCxxRevisionMacro(vtkContextScene, "$Revision$");
vtkStandardNewMacro(vtkContextScene);
vtkCxxSetObjectMacro(vtkContextScene, AnnotationLink, vtkAnnotationLink);
vtkCxxSetObjectMacro(vtkContextScene, Window, vtkRenderWindow);

//-----------------------------------------------------------------------------
vtkContextScene::vtkContextScene()
{
  this->Observer = new vtkContextScene::Command(this);
  this->Storage = new Private;
  this->Storage->itemMousePressCurrent = -1;
  this->AnnotationLink = NULL;
  this->Window = NULL;
}

//-----------------------------------------------------------------------------
vtkContextScene::~vtkContextScene()
{
  delete this->Observer;
  this->Observer = 0;
  delete this->Storage;
  this->Storage = 0;
}

//-----------------------------------------------------------------------------
bool vtkContextScene::Paint(vtkContext2D *painter)
{
  unsigned int size = this->Storage->items.size();
  for (unsigned int i = 0; i < size; ++i)
    {
    this->Storage->items[i]->Paint(painter);
    }
}

//-----------------------------------------------------------------------------
void vtkContextScene::AddItem(vtkContextItem *item)
{
  this->Storage->items.push_back(item);
  this->Storage->itemState.push_back(false);
}

//-----------------------------------------------------------------------------
int vtkContextScene::NumberOfItems()
{
  return this->Storage->items.size();
}

//-----------------------------------------------------------------------------
vtkContextItem * vtkContextScene::GetItem(int index)
{
  if (index < this->NumberOfItems())
    {
    return this->Storage->items[index];
    }
}

//-----------------------------------------------------------------------------
void vtkContextScene::AddInteractorStyle(vtkInteractorStyle *interactor)
{
  interactor->AddObserver(vtkCommand::SelectionChangedEvent, this->Observer);
  interactor->AddObserver(vtkCommand::AnyEvent, this->Observer);
}

//-----------------------------------------------------------------------------
void vtkContextScene::ProcessEvents(vtkObject* caller, unsigned long eventId,
                             void* callData)
{
  cout << "ProcessEvents called! " << caller->GetClassName() << "\t"
      << vtkCommand::GetStringFromEventId(eventId)
      << "\n\t" << vtkInteractorStyleRubberBand2D::SafeDownCast(caller)->GetInteraction() << endl;
  return;
}

//-----------------------------------------------------------------------------
void vtkContextScene::ProcessSelectionEvent(vtkObject* caller, void* callData)
{
  cout << "ProcessSelectionEvent called! " << caller << "\t" << callData << endl;
  unsigned int *rect = reinterpret_cast<unsigned int *>(callData);
  cout << "Rect:";
  for (int i = 0; i < 5; ++i)
    {
    cout << "\t" << rect[i];
    }
  cout << endl;
}

//-----------------------------------------------------------------------------
void vtkContextScene::MouseMoveEvent(int x, int y)
{
  unsigned int size = this->Storage->items.size();
  if (this->Storage->itemMousePressCurrent >= 0)
    {
    this->Storage->items[this->Storage->itemMousePressCurrent]->MouseMoveEvent(x, y);
    if (this->Window)
      {
      this->Window->Render();
      }
    return;
    }
  for (unsigned int i = 0; i < size; ++i)
    {
    if (this->Storage->items[i]->Hit(x, y))
      {
      if (this->Storage->itemState[i])
        {
        this->Storage->items[i]->MouseMoveEvent(x, y);
        }
      else
        {
        this->Storage->itemState[i] = true;
        this->Storage->items[i]->MouseEnterEvent(x, y);
        }
      }
    else
      {
      if (this->Storage->itemState[i])
        {
        this->Storage->itemState[i] = false;
        this->Storage->items[i]->MouseLeaveEvent(x, y);
        }
      }
    }
  if (this->Window)
    {
    this->Window->Render();
    }
  this->InvokeEvent(vtkCommand::RenderEvent);
}

//-----------------------------------------------------------------------------
void vtkContextScene::LeftButtonPressEvent(int x, int y)
{
  unsigned int size = this->Storage->items.size();
  for (unsigned int i = 0; i < size; ++i)
    {
    if (this->Storage->items[i]->Hit(x, y))
      {
      if (this->Storage->items[i]->LeftMouseButtonPressEvent(x, y))
        {
        // The event was accepted - stop propagating
        this->Storage->itemMousePressCurrent = i;
        return;
        }
      }
    }
}

//-----------------------------------------------------------------------------
void vtkContextScene::LeftButtonReleaseEvent(int x, int y)
{
  if (this->Storage->itemMousePressCurrent >= 0)
    {
    this->Storage->items[this->Storage->itemMousePressCurrent]->LeftMouseButtonReleaseEvent(x, y);
    this->Storage->itemMousePressCurrent = -1;
    }
}

//-----------------------------------------------------------------------------
void vtkContextScene::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  // Print out the chart's geometry if it has been set
  os << indent << "Widthxheight: " << this->Geometry[0] << "\t" << this->Geometry[1]
     << endl;
}
