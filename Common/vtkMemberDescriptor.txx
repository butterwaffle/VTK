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

#ifndef __vtkMemberDescriptor_txx
#define __vtkMemberDescriptor_txx

#include "vtkVariant.h"
#include "vtkMemberDescriptor.h"
#include "vtkTypeTraits.h"

template< class C_ >
class VTK_COMMON_EXPORT vtkMemberDescriptorVoidPImpl : public vtkMemberDescriptor
{
public:
  virtual int GetType()
    {
    return VTK_OPAQUE;
    }
  virtual vtkVariant GetValue( vtkObject* cls )
    {
    return (static_cast<C_*>( cls )->*GetMethod)();
    }
  virtual vtkVariant GetValue( vtkObject* cls, int vtkNotUsed(component) )
    {
    return (static_cast<C_*>( cls )->*GetMethod)();
    }
  virtual void SetValue( vtkObject* cls, vtkVariant val )
    {
    (static_cast<C_*>( cls )->*SetMethod)( vtkVariantConverter( val ) );
    }
  virtual void SetValue( vtkObject* cls, int vtkNotUsed(component), vtkVariant val )
    {
    (static_cast<C_*>( cls )->*SetMethod)( vtkVariantConverter( val ) );
    }
protected:
  friend class vtkClassDescriptor;
  typedef void (C_::*SetMemberType)( void* );
  typedef void* (C_::*GetMemberType)();

  vtkMemberDescriptorVoidPImpl( vtkStdString name, bool serializable, GetMemberType gmeth, SetMemberType smeth )
    : vtkMemberDescriptor( name, serializable )
    {
    // NB: C_::ClassDescriptor hasn't been set inside C_::PrepareDescriptor yet, so the next won't work:
    // this->Class = C_::ClassDescriptor;
    this->Class = 0;
    this->SetMethod = smeth;
    this->GetMethod = gmeth;
    }

  SetMemberType SetMethod;
  GetMemberType GetMethod;
};

template< class C_, typename V_ >
class VTK_COMMON_EXPORT vtkMemberDescriptorImpl : public vtkMemberDescriptor
{
public:
  virtual int GetType()
    {
    vtkTypeTraits<V_> traits;
    return traits.VTKTypeID();
    }
  virtual vtkVariant GetValue( vtkObject* cls )
    {
    return (static_cast<C_*>( cls )->*GetMethod)();
    }
  virtual vtkVariant GetValue( vtkObject* cls, int vtkNotUsed(component) )
    {
    return (static_cast<C_*>( cls )->*GetMethod)();
    }
  virtual void SetValue( vtkObject* cls, vtkVariant val )
    {
    (static_cast<C_*>( cls )->*SetMethod)( vtkVariantConverter( val ) );
    }
  virtual void SetValue( vtkObject* cls, int vtkNotUsed(component), vtkVariant val )
    {
    (static_cast<C_*>( cls )->*SetMethod)( vtkVariantConverter( val ) );
    }
protected:
  friend class vtkClassDescriptor;
  typedef void (C_::*SetMemberType)( V_ );
  typedef V_ (C_::*GetMemberType)();

  vtkMemberDescriptorImpl( vtkStdString name, bool serializable, GetMemberType gmeth, SetMemberType smeth )
    : vtkMemberDescriptor( name, serializable )
    {
    // NB: C_::ClassDescriptor hasn't been set inside C_::PrepareDescriptor yet, so the next won't work:
    // this->Class = C_::ClassDescriptor;
    this->Class = 0;
    this->SetMethod = smeth;
    this->GetMethod = gmeth;
    }

  SetMemberType SetMethod;
  GetMemberType GetMethod;
};

template< class C_, typename V_ >
class VTK_COMMON_EXPORT vtkClampedMemberDescriptorImpl : public vtkMemberDescriptorImpl<C_,V_>
{
public:
  virtual bool GetClamped() { return true; }
  virtual vtkVariant GetMin()
    {
    return vtkVariant( this->Min );
    }
  virtual vtkVariant GetMin( int vtkNotUsed(component) )
    {
    return vtkVariant( this->Min );
    }
  virtual vtkVariant GetMax()
    {
    return vtkVariant( this->Max );
    }
  virtual vtkVariant GetMax( int vtkNotUsed(component) )
    {
    return vtkVariant( this->Max );
    }
protected:
  friend class vtkClassDescriptor;

  vtkClampedMemberDescriptorImpl( vtkStdString name, bool serializable, typename vtkMemberDescriptorImpl<C_,V_>::GetMemberType gmeth, typename vtkMemberDescriptorImpl<C_,V_>::SetMemberType smeth, V_ min, V_ max )
    : vtkMemberDescriptorImpl<C_,V_>( name, serializable, gmeth, smeth )
    {
    this->Min = min;
    this->Max = max;
    }

  V_ Min;
  V_ Max;
};

template< class C_, typename V_, int d_ >
class VTK_COMMON_EXPORT vtkMemberDescriptorVectorImpl : public vtkMemberDescriptor
{
public:
  virtual int GetType()
    {
    vtkTypeTraits<V_> traits;
    return traits.VTKTypeID();
    }
  virtual int GetNumberOfComponents()
    { return d_; }
  virtual vtkVariant GetValue( vtkObject* cls )
    {
    return this->GetValue( cls, 0 );
    }
  virtual vtkVariant GetValue( vtkObject* cls, int component )
    {
    V_ tmp[d_];
    (static_cast<C_*>( cls )->*GetMethod)( tmp );
    return tmp[component];
    }
  virtual void SetValue( vtkObject* cls, vtkVariant val )
    {
    this->SetValue( cls, 0, val );
    }
  virtual void SetValue( vtkObject* cls, int component, vtkVariant val )
    {
    V_ tmp[d_];
    (static_cast<C_*>( cls )->*GetMethod)( tmp );
    tmp[component] = vtkVariantConverter( val );
    (static_cast<C_*>( cls )->*SetMethod)( tmp );
    }
protected:
  friend class vtkClassDescriptor;
  typedef void (C_::*SetMemberType)( V_[d_] );
  typedef void (C_::*GetMemberType)( V_[d_] );

  vtkMemberDescriptorVectorImpl( vtkStdString name, bool serializable, GetMemberType gmeth, SetMemberType smeth )
    : vtkMemberDescriptor( name, serializable )
    {
    // NB: C_::ClassDescriptor hasn't been set inside C_::PrepareDescriptor yet, so the next won't work:
    // this->Class = C_::ClassDescriptor;
    this->Class = 0;
    this->SetMethod = smeth;
    this->GetMethod = gmeth;
    }

  SetMemberType SetMethod;
  GetMemberType GetMethod;
};

template< class C_, typename V_, int d_ >
class VTK_COMMON_EXPORT vtkClampedMemberDescriptorVectorImpl : public vtkMemberDescriptorVectorImpl<C_,V_,d_>
{
public:
  virtual bool GetClamped() { return true; }
  virtual vtkVariant GetMin()
    {
    return this->GetMin( 0 );
    }
  virtual vtkVariant GetMax()
    {
    return this->GetMax( 0 );
    }
  virtual vtkVariant GetMin( int component )
    {
    return vtkVariant( this->Min[component] );
    }
  virtual vtkVariant GetMax( int component )
    {
    return vtkVariant( this->Max[component] );
    }
protected:
  friend class vtkClassDescriptor;
  typedef void (C_::*SetMemberType)( V_[d_] );
  typedef void (C_::*GetMemberType)( V_[d_] );

  vtkClampedMemberDescriptorVectorImpl( vtkStdString name, bool serializable, GetMemberType gmeth, SetMemberType smeth, V_ min[d_], V_ max[d_] )
    : vtkMemberDescriptorVectorImpl<C_,V_,d_>( name, serializable, gmeth, smeth ), Min( min ), Max( max )
    {
    }

  V_ Min[d_];
  V_ Max[d_];
};

template< class C_, typename V_ >
class VTK_COMMON_EXPORT vtkMemberDescriptorObjImpl : public vtkMemberDescriptor
{
public:
  virtual int GetType()
    {
    return VTK_OBJECT;
    }
  virtual vtkVariant GetValue( vtkObject* cls )
    {
    return (static_cast<C_*>( cls )->*GetMethod)();
    }
  virtual vtkVariant GetValue( vtkObject* cls, int vtkNotUsed(component) )
    {
    return (static_cast<C_*>( cls )->*GetMethod)();
    }
  virtual void SetValue( vtkObject* cls, vtkVariant val )
    {
    V_* ptr = V_::SafeDownCast( val.ToVTKObject() );
    (static_cast<C_*>( cls )->*SetMethod)( ptr );
    }
  virtual void SetValue( vtkObject* cls, int vtkNotUsed(component), vtkVariant val )
    {
    V_* ptr = V_::SafeDownCast( val.ToVTKObject() );
    (static_cast<C_*>( cls )->*SetMethod)( ptr );
    }
protected:
  friend class vtkClassDescriptor;
  typedef void (C_::*SetMemberType)( V_* );
  typedef V_* (C_::*GetMemberType)();

  vtkMemberDescriptorObjImpl( vtkStdString name, bool serializable, GetMemberType gmeth, SetMemberType smeth )
    : vtkMemberDescriptor( name, serializable )
    {
    // NB: C_::ClassDescriptor hasn't been set inside C_::PrepareDescriptor yet, so the next won't work:
    // this->Class = C_::ClassDescriptor;
    this->Class = 0;
    this->SetMethod = smeth;
    this->GetMethod = gmeth;
    }

  SetMemberType SetMethod;
  GetMemberType GetMethod;
};


#endif // __vtkMemberDescriptor_txx
