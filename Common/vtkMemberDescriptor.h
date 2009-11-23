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
// .NAME vtkMemberDescriptor - provide uniform access to vtkObject member variables
// .SECTION Description
// Each instance of this class represents a single member variable of
// vtkObject or one of its subclasses.
// This class contains generic methods to set and get the values of this member
// variable given an instance of the class.

// .SECTION See also
// vtkClassDescriptor

#ifndef __vtkMemberDescriptor_h
#define __vtkMemberDescriptor_h

#include "vtkStdString.h"
#include "vtkVariant.h"

class vtkObject;
class vtkClassDescriptor;

class VTK_COMMON_EXPORT vtkMemberDescriptor
{
public:
  vtkStdString GetName() { return this->Name; }
  virtual int GetNumberOfComponents() { return 1; }
  virtual vtkVariant GetValue( vtkObject* instance, int component ) = 0;
  virtual void SetValue( vtkObject* instance, int component, vtkVariant val ) = 0;
  virtual vtkVariant GetValue( vtkObject* instance )
    { return this->GetValue( instance, 0 ); }
  virtual void SetValue( vtkObject* instance, vtkVariant val )
    { this->SetValue( instance, 0, val ); }
  bool GetSerializable() { return this->Serializable; }
  virtual vtkVariant GetDefault() { return this->Default; }
  virtual void SetDefault( vtkVariant val ) { this->Default = val; }
  vtkClassDescriptor* GetClassDescriptor() { return this->Class; }
  virtual bool GetClamped() { return false; }
  virtual vtkVariant GetMin() { return vtkVariant(); }
  virtual vtkVariant GetMax() { return vtkVariant(); }

protected:
  vtkMemberDescriptor( vtkStdString name, bool serializable );
  virtual ~vtkMemberDescriptor();

  friend class vtkClassDescriptor;

  vtkStdString Name;
  bool Serializable;
  vtkVariant Default;
  vtkClassDescriptor* Class;
};

#endif // __vtkMemberDescriptor_h
