#include "vtkPointMarkTick.h"

#include "vtkContext2D.h"
#include "vtkBrush.h"
#include "vtkPen.h"
#include "vtkDoubleArray.h"
#include "vtkInformation.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkType.h"
#include "vtkVariantArray.h"

vtkStandardNewMacro(vtkPointMarkTick);
vtkCxxRevisionMacro(vtkPointMarkTick,"$Revision:$");

vtkPointMarkTick::vtkPointMarkTick()
{
  this->SetupParameters();
}

vtkPointMarkTick::~vtkPointMarkTick()
{
}

void vtkPointMarkTick::PrintSelf( ostream& os, vtkIndent indent )
{
  this->Superclass::PrintSelf( os, indent );
}

void vtkPointMarkTick::SetupParameters()
{
  vtkInformation* pinfo;
  int pnum;

  // NOTE: The order of AddParameter() calls must match vtkPointMarkTick::Parameters!

  vtkVariant defaultLineThickness( 0.002 );
  double lineThicknessBounds[2] = { 0., VTK_DOUBLE_MAX };
  pinfo = vtkInformation::New();
  pinfo->Set( vtkPointMark::PARAMETER_NAME(), "Line Thickness" );
  pinfo->Set( vtkPointMark::PARAMETER_TYPE(), VTK_DOUBLE );
  pinfo->Set( vtkPointMark::PARAMETER_NUMBER_OF_COMPONENTS(), 1 );
  pinfo->Set( vtkPointMark::PARAMETER_REAL_BOUNDS(), lineThicknessBounds, 2 );
  pnum = this->AddParameter( pinfo );
  pinfo->Delete();
  this->BindParameter( pnum, defaultLineThickness );

  vtkVariant defaultLength( 0.002 );
  double lengthBounds[2] = { 0., VTK_DOUBLE_MAX };
  pinfo = vtkInformation::New();
  pinfo->Set( vtkPointMark::PARAMETER_NAME(), "Length" );
  pinfo->Set( vtkPointMark::PARAMETER_TYPE(), VTK_DOUBLE );
  pinfo->Set( vtkPointMark::PARAMETER_NUMBER_OF_COMPONENTS(), 1 );
  pinfo->Set( vtkPointMark::PARAMETER_REAL_BOUNDS(), lengthBounds, 2 );
  pnum = this->AddParameter( pinfo );
  pinfo->Delete();
  this->BindParameter( pnum, defaultLength );

  vtkVariant defaultJustification( 0 );
  double justificationBounds[2] = { 0, 1 }; // 0 is centered, 1 is not
  pinfo = vtkInformation::New();
  pinfo->Set( vtkPointMark::PARAMETER_NAME(), "Justification" );
  pinfo->Set( vtkPointMark::PARAMETER_TYPE(), VTK_INT );
  pinfo->Set( vtkPointMark::PARAMETER_NUMBER_OF_COMPONENTS(), 1 );
  pinfo->Set( vtkPointMark::PARAMETER_REAL_BOUNDS(), justificationBounds, 2 );
  pnum = this->AddParameter( pinfo );
  pinfo->Delete();
  this->BindParameter( pnum, defaultJustification );

  vtkVariant defaultAngle( 0.0 );
  double angleBounds[2];
  angleBounds[0] = - vtkMath::DoublePi();
  angleBounds[1] =   vtkMath::DoublePi();
  pinfo = vtkInformation::New();
  pinfo->Set( vtkPointMark::PARAMETER_NAME(), "Angle" );
  pinfo->Set( vtkPointMark::PARAMETER_TYPE(), VTK_DOUBLE );
  pinfo->Set( vtkPointMark::PARAMETER_NUMBER_OF_COMPONENTS(), 1 );
  pinfo->Set( vtkPointMark::PARAMETER_REAL_BOUNDS(), angleBounds, 2 );
  pnum = this->AddParameter( pinfo );
  pinfo->Delete();
  this->BindParameter( pnum, defaultAngle );

  double black[4] = { 0., 0., 0., 1. };
  vtkDoubleArray* col0 = vtkDoubleArray::New();
  col0->SetNumberOfComponents( 4 );
  col0->InsertNextTuple( black );
  vtkVariant defaultLineColor( col0 );
  col0->Delete();
  double lineColorBounds[8] = { 0., 1.,   0., 1.,   0., 1.,   0., 1. };
  pinfo = vtkInformation::New();
  pinfo->Set( vtkPointMark::PARAMETER_NAME(), "Line Color" );
  pinfo->Set( vtkPointMark::PARAMETER_TYPE(), VTK_DOUBLE );
  pinfo->Set( vtkPointMark::PARAMETER_NUMBER_OF_COMPONENTS(), 4 );
  pinfo->Set( vtkPointMark::PARAMETER_REAL_BOUNDS(), lineColorBounds, 8 );
  pnum = this->AddParameter( pinfo );
  pinfo->Delete();
  this->BindParameter( pnum, defaultLineColor );

}

void vtkPointMarkTick::DrawMarksInternal( vtkContext2D* ctxt,
  vtkDataArray* xc, int xcomp, vtkDataArray* yc, int ycomp,
  vtkIdType ti, vtkIdType te, vtkIdType ts )
{
  double* tvx;
  double* tvy;
  double oldLineThickness = -1.;
  double lineThickness;
  double length;
  int justification;
  double angle;
  double oldAngle = VTK_DOUBLE_MAX; // out of range
  float lineColor[4];
  double dx = 1., dy = 0.;
  vtkVariantArray* var = vtkVariantArray::New();

  for ( vtkIdType pid = ti; pid < te; pid += ts )
    {
    // Fetch the coordinates for this mark's center.
    tvx = xc->GetTuple( pid );
    tvy = yc->GetTuple( pid );

    // Fetch parameter values for this point mark.
    this->GetParameterValuesForTuple( pid, var );
    lineThickness = var->GetValue( 0 ).ToDouble();
    length = var->GetValue( 1 ).ToDouble();
    justification = var->GetValue( 2 ).ToInt();
    angle = var->GetValue( 3 ).ToDouble();
    for ( int c = 0; c < 4; ++ c )
      {
      lineColor[c] = var->GetValue( 4 + c ).ToDouble();
      }

    bool drawLine = lineColor[3] > 0. && lineThickness > 0.;
    if ( ! drawLine )
      {
      continue; // skip this mark!
      }

    // Draw the mark
    if ( oldLineThickness != lineThickness )
      ctxt->GetPen()->SetWidth( lineThickness );
    oldLineThickness = lineThickness;
    ctxt->GetPen()->SetColorF( lineColor[0], lineColor[1], lineColor[2], lineColor[3] );
    if ( angle != oldAngle )
      { // these are expensive and we expect angle to be constant for most uses.
      dx = cos( angle );
      dy = sin( angle );
      oldAngle = angle;
      }
    if ( justification )
      {
      ctxt->DrawLine( tvx[xcomp], tvy[ycomp], tvx[xcomp] + length * dx, tvy[ycomp] + length * dy );
      }
    else
      {
      ctxt->DrawLine(
        tvx[xcomp] - length * dx / 2., tvy[ycomp] - length * dy / 2.,
        tvx[xcomp] + length * dx / 2., tvy[ycomp] + length * dy / 2. );
      }
    }

  var->Delete();
}


