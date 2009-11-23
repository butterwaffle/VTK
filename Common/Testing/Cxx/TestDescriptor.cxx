#include "vtkInstantiator.h"
#include "vtkParametricDini.h"
#include "vtkDoubleArray.h"
#include "vtkType.h"
#include "vtkClassDescriptor.h"
#include "vtkMemberDescriptor.h"

static void vtkPrintMembers( vtkObject* obj )
{
  cout << obj->GetClassName() << "\n";
  int nd = obj->GetNumberOfDescriptors();
  for ( int i = 0; i < nd; ++ i )
    {
    vtkMemberDescriptor* mdesc = obj->GetDescriptor( i );
    cout
      << "  " << mdesc->GetClassDescriptor()->GetName().c_str() << "::"
      << mdesc->GetName().c_str() << ":";
    for ( int j = 0; j < mdesc->GetNumberOfComponents(); ++ j )
      {
      vtkVariant val = mdesc->GetValue( obj, j );
      cout << " " << ( val.GetType() == VTK_UNSIGNED_CHAR ? vtkVariant( val.ToInt() ) : val );
      }
    cout << "\n";
    }
}

int TestDescriptor( int argc, char* argv[] )
{
  if ( argc > 1 )
    {
    vtkObject* obj = vtkInstantiator::CreateInstance( argv[1] );
    if ( obj )
      {
      vtkPrintMembers( obj );
      obj->Delete();
      }
    }
  else
    {
    vtkParametricDini* pdini = vtkParametricDini::New();
    vtkPrintMembers( pdini );
    pdini->Delete();

    vtkDoubleArray* darr = vtkDoubleArray::New();
    vtkPrintMembers( darr );
    darr->Delete();
    }

  return 0;
}
