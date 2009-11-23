#include "vtkClassDescriptor.h"
#include "vtkMemberDescriptor.h"

#include "vtkObject.h"

#include <vtkstd/map>

// Initialize static members for storing class descriptors.
vtkClassDescriptorP* vtkClassDescriptor::ClassInternals = 0;

class vtkClassDescriptorP : public vtkstd::map<vtkStdString,vtkClassDescriptor*>
{
};

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
  if ( ! vtkClassDescriptor::ClassInternals )
    {
    vtkClassDescriptor::ClassInternals = new vtkClassDescriptorP;
    atexit( vtkClassDescriptor::CleanupClassDescriptors );
    }
  vtkClassDescriptor* self = new vtkClassDescriptor;
  self->Name = name;
  self->Superclass = superclassDescriptor;
  vtkstd::map<vtkStdString,vtkClassDescriptor*>::iterator it = vtkClassDescriptor::ClassInternals->find( name );
  if ( it != vtkClassDescriptor::ClassInternals->end() )
    { // Oops, already registered.
    delete self;
    return it->second;
    }
  (*vtkClassDescriptor::ClassInternals)[name] = self;
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

vtkClassDescriptor* vtkClassDescriptor::GetClassDescriptor( const char* className )
{
  if ( ! vtkClassDescriptor::ClassInternals )
    {
    vtkClassDescriptor::ClassInternals = new vtkClassDescriptorP;
    atexit( vtkClassDescriptor::CleanupClassDescriptors );
    }
  vtkstd::map<vtkStdString,vtkClassDescriptor*>::iterator it = vtkClassDescriptor::ClassInternals->find( className );
  if ( it == vtkClassDescriptor::ClassInternals->end() )
    {
    return 0;
    }
  return it->second;
}

void vtkClassDescriptor::CleanupClassDescriptors()
{
  if ( vtkClassDescriptor::ClassInternals )
    {
    delete vtkClassDescriptor::ClassInternals;
    vtkClassDescriptor::ClassInternals = 0;
    }
}

