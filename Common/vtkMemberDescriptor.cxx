#include "vtkMemberDescriptor.h"

vtkMemberDescriptor::vtkMemberDescriptor( vtkStdString name, bool serializable )
{
  this->Name = name;
  this->Serializable = serializable;
}

vtkMemberDescriptor::~vtkMemberDescriptor()
{
}

