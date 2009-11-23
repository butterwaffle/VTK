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
// .NAME vtkClassDescriptor - meta-class for describing descendants of vtkObject
// .SECTION Description
// Each instance of this class keeps an inventory of member variables
// (along with functions to get and set their values) for vtkObject or
// one of its subclasses.

// .SECTION See also
// vtkMemberDescriptor

#ifndef __vtkClassDescriptor_h
#define __vtkClassDescriptor_h

#include "vtkStdString.h"
#include <vtkstd/vector>

class vtkMemberDescriptor;
template< class C_, typename V_ > class vtkMemberDescriptorImpl;
template< class C_, typename V_, int d_ > class vtkMemberDescriptorVectorImpl;

class VTK_COMMON_EXPORT vtkClassDescriptor
{
public:
  static vtkClassDescriptor* RegisterClass( vtkStdString name, vtkClassDescriptor* superclassDescriptor );

  vtkClassDescriptor();
  virtual ~vtkClassDescriptor();

  template< class C_, typename V_ >
  vtkMemberDescriptor* AddMember(
    vtkStdString name,
    bool serializable,
    typename vtkMemberDescriptorImpl<C_,V_>::GetMemberType gmeth,
    typename vtkMemberDescriptorImpl<C_,V_>::SetMemberType smeth );

  template< class C_, typename V_, int d_ >
  vtkMemberDescriptor* AddMember(
    vtkStdString name,
    bool serializable,
    typename vtkMemberDescriptorVectorImpl<C_,V_,d_>::GetMemberType gmeth,
    typename vtkMemberDescriptorVectorImpl<C_,V_,d_>::SetMemberType smeth );

  template< class C_, typename V_ >
  vtkMemberDescriptor* AddMember(
    vtkStdString name,
    bool serializable,
    typename vtkMemberDescriptorImpl<C_,V_>::GetMemberType gmeth,
    typename vtkMemberDescriptorImpl<C_,V_>::SetMemberType smeth,
    V_ min,
    V_ max );

  int GetNumberOfMembers();
  vtkMemberDescriptor* GetMemberDescriptor( int i );
  vtkMemberDescriptor* GetMemberDescriptor( vtkStdString memberName );

  vtkStdString GetName() { return this->Name; }

  vtkStdString Name;
  vtkClassDescriptor* Superclass;
  vtkstd::vector<vtkMemberDescriptor*> Descriptors;
};

#endif // __vtkClassDescriptor_h
