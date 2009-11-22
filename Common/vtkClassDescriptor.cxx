#include "vtkClassDescriptor.h"
#include "vtkMemberDescriptor.h"

#include "vtkObject.h"

vtkClassDescriptor::vtkClassDescriptor()
{
}

vtkClassDescriptor::~vtkClassDescriptor()
{
  vtkstd::vector<vtkMemberDescriptor*>::iterator it;
  for ( it = this->Descriptors.begin(); it != this->Descriptors.end(); ++ it )
    {
    delete *it;
    }
}

vtkClassDescriptor* vtkClassDescriptor::RegisterClass( vtkStdString name, vtkClassDescriptor* superclassDescriptor )
{
  vtkClassDescriptor* self = new vtkClassDescriptor;
  self->Name = name;
  self->Superclass = superclassDescriptor;
  return self;
}

int vtkClassDescriptor::GetNumberOfMembers()
{
  int nd = static_cast<int>( this->Descriptors.size() );
  if ( this->Superclass )
    {
    nd += this->Superclass->GetNumberOfMembers();
    }
  return nd;
}

vtkMemberDescriptor* vtkClassDescriptor::GetMemberDescriptor( int i )
{
  if ( i < 0 )
    {
    return 0;
    }
  int nd = static_cast<int>( this->Descriptors.size() );
  if ( i >= nd )
    { // Place superclass descriptors after this class' descriptors.
    if ( this->Superclass )
      {
      return this->Superclass->GetMemberDescriptor( i - nd );
      }
    return 0;
    }
  return this->Descriptors[i];
}

vtkMemberDescriptor* vtkClassDescriptor::GetMemberDescriptor( vtkStdString memberName )
{
  vtkstd::vector<vtkMemberDescriptor*>::iterator it;
  for ( it = this->Descriptors.begin(); it != this->Descriptors.end(); ++ it )
    {
    if ( (*it)->Name == memberName )
      {
      return *it;
      }
    }
  if ( this->Superclass )
    {
    return this->Superclass->GetMemberDescriptor( memberName );
    }
  return 0;
}

