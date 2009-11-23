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

#ifndef __vtkClassDescriptor_txx
#define __vtkClassDescriptor_txx

#include "vtkClassDescriptor.h"
#include "vtkMemberDescriptor.h"
#include "vtkMemberDescriptor.txx"

template< class C_, typename V_ >
vtkMemberDescriptor* vtkClassDescriptor::AddMember(
  vtkStdString name, bool serializable,
  typename vtkMemberDescriptorImpl<C_,V_>::GetMemberType gmeth,
  typename vtkMemberDescriptorImpl<C_,V_>::SetMemberType smeth )
{
  vtkMemberDescriptor* mdesc = new vtkMemberDescriptorImpl<C_,V_>( name, serializable, gmeth, smeth );
  mdesc->Class = this;
  this->Descriptors.push_back( mdesc );
  return mdesc;
}

template< class C_, typename V_, int d_ >
vtkMemberDescriptor* vtkClassDescriptor::AddMember(
  vtkStdString name, bool serializable,
  typename vtkMemberDescriptorVectorImpl<C_,V_,d_>::GetMemberType gmeth,
  typename vtkMemberDescriptorVectorImpl<C_,V_,d_>::SetMemberType smeth )
{
  vtkMemberDescriptor* mdesc = new vtkMemberDescriptorVectorImpl<C_,V_,d_>( name, serializable, gmeth, smeth );
  mdesc->Class = this;
  this->Descriptors.push_back( mdesc );
  return mdesc;
}

template< class C_, typename V_ >
vtkMemberDescriptor* vtkClassDescriptor::AddMember(
  vtkStdString name, bool serializable,
  typename vtkMemberDescriptorImpl<C_,V_>::GetMemberType gmeth,
  typename vtkMemberDescriptorImpl<C_,V_>::SetMemberType smeth,
  V_ min, V_ max )
{
  vtkMemberDescriptor* mdesc = new vtkClampedMemberDescriptorImpl<C_,V_>( name, serializable, gmeth, smeth, min, max );
  mdesc->Class = this;
  this->Descriptors.push_back( mdesc );
  return mdesc;
}

#endif // __vtkClassDescriptor_txx
