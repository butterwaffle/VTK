#include "vtkPointMark.h"

#include "vtkAbstractArray.h"
#include "vtkRenderer.h"
#include "vtkDataArray.h"
#include "vtkDoubleArray.h"
#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationKey.h"
#include "vtkInformationIntegerKey.h"
#include "vtkInformationIntegerVectorKey.h"
#include "vtkInformationDoubleVectorKey.h"
#include "vtkInformationStringKey.h"
#include "vtkVariantArray.h"

#include <vtkstd/vector>

vtkCxxRevisionMacro(vtkPointMark,"$Revision:$");

vtkInformationKeyMacro(vtkPointMark,PARAMETER_TYPE,Integer);
vtkInformationKeyMacro(vtkPointMark,PARAMETER_NAME,String);
vtkInformationKeyMacro(vtkPointMark,PARAMETER_NUMBER_OF_COMPONENTS,Integer);
vtkInformationKeyMacro(vtkPointMark,PARAMETER_INTEGER_BOUNDS,IntegerVector);
vtkInformationKeyMacro(vtkPointMark,PARAMETER_REAL_BOUNDS,DoubleVector);

class vtkParameterBinding
{
public:
  vtkParameterBinding()
    {
    this->ArrayValue = 0;
    this->ArrayComponent = 0;
    }
  ~vtkParameterBinding()
    {
    if ( this->ArrayValue )
      this->ArrayValue->Delete();
    }
  void Bind( vtkAbstractArray* val, int component )
    {
    if ( this->ArrayValue && this->ArrayValue != val )
      {
      this->ArrayValue->Delete();
      }
    this->ArrayValue = val;
    this->ArrayComponent = component;
    if ( this->ArrayValue )
      {
      this->ArrayValue->Register( 0 );
      }
    }
  void Bind( vtkVariant& val )
    {
    if ( this->ArrayValue )
      this->ArrayValue->Delete();
    this->ConstValue = val;
    }
  int GetValue( vtkVariant& constVal, vtkAbstractArray*& arrayVal, int& component )
    {
    if ( this->ArrayValue )
      {
      arrayVal = this->ArrayValue;
      component = this->ArrayComponent;
      return 1;
      }
    constVal = this->ConstValue;
    return 0;
    }
  bool IsConstant()
    {
    return this->ArrayValue ? false : true;
    }
  vtkVariant ConstValue;
  vtkAbstractArray* ArrayValue;
  int ArrayComponent;
};

class vtkPointMarkInternal
{
public:
  vtkPointMarkInternal()
    {
    this->XCoords = 0;
    this->YCoords = 0;
    this->XComponent = this->YComponent = 0;
    this->TupleStride = this->TupleStart = this->TupleEnd = 0;
    }
  ~vtkPointMarkInternal()
    {
    for ( vtkstd::vector<vtkInformation*>::iterator it = this->Parameters.begin(); it != this->Parameters.end(); ++ it )
      {
      if ( *it )
        {
        (*it)->Delete();
        }
      }
    if ( this->XCoords )
      {
      this->XCoords->Delete();
      }
    if ( this->YCoords )
      {
      this->YCoords->Delete();
      }
    }
  vtkstd::vector<vtkInformation*> Parameters;
  vtkstd::vector<vtkParameterBinding> Values;
  vtkDataArray* XCoords;
  vtkDataArray* YCoords;
  vtkIdType XComponent;
  vtkIdType YComponent;
  vtkIdType TupleStart;
  vtkIdType TupleEnd;
  vtkIdType TupleStride;
};

vtkPointMark::vtkPointMark()
{
  this->P = new vtkPointMarkInternal();
}

vtkPointMark::~vtkPointMark()
{
  delete this->P;
}

void vtkPointMark::PrintSelf( ostream& os, vtkIndent indent )
{
  this->Superclass::PrintSelf( os, indent );
  os << indent << "P: " << this->P << "\n";
}

int vtkPointMark::GetNumberOfParameters()
{
  return static_cast<int>( this->P->Parameters.size() );
}

const char* vtkPointMark::GetParameterName( int param )
{
  if ( param < 0 || param > this->GetNumberOfParameters() )
    return 0;

  return this->P->Parameters[param]->Get( vtkPointMark::PARAMETER_NAME() );
}

int vtkPointMark::GetParameterHandle( const char* paramName )
{
  int i = 0;
  for ( vtkstd::vector<vtkInformation*>::iterator it = this->P->Parameters.begin(); it != this->P->Parameters.end(); ++ it, ++ i )
    {
    const char* pname = (*it)->Get( vtkPointMark::PARAMETER_NAME() );
    if ( pname && ! strcmp( pname, paramName ) )
      return i;
    }
  return -1;
}

vtkInformation* vtkPointMark::GetParameterInformation( int param )
{
  if ( param < 0 || param > this->GetNumberOfParameters() )
    return 0;

  return this->P->Parameters[param];
}

void vtkPointMark::ResetParameters()
{
  // Release references to parameters and their values.
  vtkstd::vector<vtkInformation*>::size_type i;
  for ( i = 0; i < this->P->Parameters.size(); ++ i )
    {
    this->P->Parameters[i]->Delete();
    if ( this->P->Values[i].ArrayValue )
      {
      this->P->Values[i].ArrayValue->Delete();
      this->P->Values[i].ArrayValue = 0;
      }
    }
  // Empty the lists of parameters and their values.
  this->P->Parameters.clear();
  this->P->Values.clear();
  // Ask the subclass to add the parameters and set their default values again.
  this->SetupParameters();
}

void vtkPointMark::BindParameter( int param, vtkVariant value )
{
  if ( param < 0 || param > this->GetNumberOfParameters() )
    return;

  this->P->Values[param].Bind( value );
}

void vtkPointMark::BindParameter( int param, vtkAbstractArray* values, int component )
{
  if ( param < 0 || param > this->GetNumberOfParameters() )
    return;

  this->P->Values[param].Bind( values, component );
}

int vtkPointMark::GetParameterValue( int param, vtkVariant& constVal, vtkAbstractArray*& arrayVal, int& component )
{
  if ( param < 0 || param > this->GetNumberOfParameters() )
    return -1;

  return this->P->Values[param].GetValue( constVal, arrayVal, component );
}

void vtkPointMark::DrawMarks( vtkContext2D* ctxt,
  vtkDataArray* xc, int xcomp, vtkDataArray* yc, int ycomp,
  vtkIdType ti, vtkIdType te, vtkIdType ts )
{
  if ( ! ctxt )
    {
    vtkErrorMacro( "You must call DrawMarks with a non-NULL vtkContext2D." )
    return;
    }

#if 0
  // FIXME: The new 2D drawing API doesn't support anything like display lists or cairo save/restore
  if ( this->AreAllParametersConstant() )
    {
    cairo_t* cr = ren->GetContext();
    cairo_save( cr );

    // Prepare a temporary array so we can call DrawMarksInternal to produce a singly mark at the origin.
    vtkDoubleArray* origin = vtkDoubleArray::New();
    origin->SetNumberOfComponents( 2 );
    origin->SetNumberOfTuples( 1 );
    origin->SetTuple2( 0, 0., 0. );
    // Draw the mark at the origin and turn it into a pattern source.
    cairo_push_group( cr );
    this->DrawMarksInternal( ctxt, origin, 0, origin, 1,   0, 1, 1 );
    cairo_pattern_t* marker = cairo_pop_group( cr );
    origin->Delete();
    // Loop over all the points and paint the pattern there.
    vtkIdType i;
    double xo[2];
    double xr[2];
    double xp[2] = { 0., 0. };
    double* tvx;
    double* tvy;
    tvx = xc->GetTuple( ti );
    tvy = yc->GetTuple( ti );
    xo[0] = tvx[xcomp];
    xo[1] = tvy[ycomp];
    for ( i = ti; i < te; i += ts )
      {
      tvx = xc->GetTuple( i );
      tvy = yc->GetTuple( i );
      xr[0] = tvx[xcomp] - xp[0]; xr[1] = tvy[ycomp] - xp[1];
      xp[0] = tvx[xcomp]; xp[1] = tvy[ycomp];
      cairo_translate( cr, xr[0], xr[1] );
      cairo_set_source( cr, marker );
      cairo_paint( cr );
      }
    // FIXME: Since we cairo_restore() below, do we need to translate?
    xr[0] = xo[0] - xp[0]; xr[1] = xo[1] - xp[1];
    cairo_translate( cr, xr[0], xr[1] );
    xr[0] = - xo[0]; xr[1] = - xo[1];
    cairo_translate( cr, xr[0], xr[1] );

    cairo_restore( cr );
    }
  else
#endif // 0
    {
    this->DrawMarksInternal( ctxt, xc, xcomp, yc, ycomp, ti, te, ts );
    }
}

int vtkPointMark::AreAllParametersConstant()
{
  for ( vtkstd::vector<vtkParameterBinding>::iterator it = this->P->Values.begin(); it != this->P->Values.end(); ++ it )
    {
    if ( ! it->IsConstant() )
      {
      return 0;
      }
    }
  return 1;
}

void vtkPointMark::GetParameterValuesForTuple( vtkIdType pt, vtkVariantArray* pvals )
{
  int pvi = 0;
  vtkstd::vector<vtkInformation*>::iterator pit = this->P->Parameters.begin();
  vtkAbstractArray* aval;
  vtkVariant cval;
  int component;
  for ( vtkstd::vector<vtkParameterBinding>::iterator vit = this->P->Values.begin(); vit != this->P->Values.end(); ++ vit, ++ pit )
    {
    int numberOfComponents = (*pit)->Get( vtkPointMark::PARAMETER_NUMBER_OF_COMPONENTS() );
    if ( vit->GetValue( cval, aval, component ) )
      { // a varying binding:
      vtkIdType vno = aval->GetNumberOfComponents() * pt + component; // where does the pt-th tuple start?
      for ( int i = 0; i < numberOfComponents; ++ i, ++ vno )
        pvals->InsertVariantValue( pvi ++, aval->GetVariantValue( vno ) );
      }
    else
      { // a constant binding:
      if ( numberOfComponents == 1 )
        {
        pvals->InsertVariantValue( pvi ++, cval );
        }
      else
        {
        aval = cval.ToArray();
        for ( int i = 0; i < numberOfComponents; ++ i )
          pvals->InsertVariantValue( pvi ++, aval->GetVariantValue( i ) );
        }
      }
    }
}

void vtkPointMark::SetCoordinates(
  vtkDataArray* xCoords, int xComponent,
  vtkDataArray* yCoords, int yComponent,
  vtkIdType tStart, vtkIdType tEnd, vtkIdType tStride )
{
  bool wasModified = false;
  vtkPointMarkInternal* p = this->P;
  if ( p->XCoords != xCoords )
    {
    if ( p->XCoords )
      {
      p->XCoords->UnRegister( this );
      }
    p->XCoords = xCoords;
    if ( p->XCoords )
      {
      p->XCoords->Register( this );
      }
    wasModified = true;
    }
  if ( p->XComponent != xComponent )
    {
    p->XComponent = xComponent;
    wasModified = true;
    }
  if ( p->YCoords != yCoords )
    {
    if ( p->YCoords )
      {
      p->YCoords->UnRegister( this );
      }
    p->YCoords = yCoords;
    if ( p->YCoords )
      {
      p->YCoords->Register( this );
      }
    wasModified = true;
    }
  if ( p->YComponent != yComponent )
    {
    p->YComponent = yComponent;
    wasModified = true;
    }
  if ( p->TupleStart != tStart )
    {
    p->TupleStart = tStart;
    wasModified = true;
    }
  if ( p->TupleEnd != tEnd )
    {
    p->TupleEnd = tEnd;
    wasModified = true;
    }
  if ( p->TupleStride != tStride )
    {
    p->TupleStride = tStride;
    wasModified = true;
    }
  if ( wasModified )
    {
    this->Modified();
    }
}

int vtkPointMark::AddParameter( vtkInformation* paramInfo )
{
  if ( ! paramInfo )
    {
    return -1;
    }

  vtkParameterBinding paramBinding;
  int i = static_cast<int>( this->P->Parameters.size() );
  this->P->Parameters.push_back( paramInfo );
  this->P->Values.push_back( paramBinding );
  paramInfo->Register( this );
  return i;
}

