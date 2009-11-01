#include "vtkPointMarkStatBox.h"

#include "vtkBrush.h"
#include "vtkContext2D.h"
#include "vtkDoubleArray.h"
#include "vtkInformation.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPen.h"
#include "vtkType.h"
#include "vtkVariantArray.h"

vtkStandardNewMacro(vtkPointMarkStatBox);
vtkCxxRevisionMacro(vtkPointMarkStatBox,"$Revision:$");

vtkPointMarkStatBox::vtkPointMarkStatBox()
{
  this->SetupParameters();
}

vtkPointMarkStatBox::~vtkPointMarkStatBox()
{
}

void vtkPointMarkStatBox::PrintSelf( ostream& os, vtkIndent indent )
{
  this->Superclass::PrintSelf( os, indent );
}

void vtkPointMarkStatBox::SetupParameters()
{
  vtkInformation* pinfo;
  int pnum;

  // NOTE: The order of AddParameter() calls must match vtkPointMarkStatBox::Parameters!

  vtkVariant defaultDotSize( 0.002 );
  double dotSizeBounds[2] = { 0., VTK_DOUBLE_MAX };
  pinfo = vtkInformation::New();
  pinfo->Set( vtkPointMark::PARAMETER_NAME(), "Dot Size" );
  pinfo->Set( vtkPointMark::PARAMETER_TYPE(), VTK_DOUBLE );
  pinfo->Set( vtkPointMark::PARAMETER_NUMBER_OF_COMPONENTS(), 1 );
  pinfo->Set( vtkPointMark::PARAMETER_REAL_BOUNDS(), dotSizeBounds, 2 );
  pnum = this->AddParameter( pinfo );
  pinfo->Delete();
  this->BindParameter( pnum, defaultDotSize );

  vtkVariant defaultLineThickness( 0.003 );
  double lineThicknessBounds[2] = { 0., VTK_DOUBLE_MAX };
  pinfo = vtkInformation::New();
  pinfo->Set( vtkPointMark::PARAMETER_NAME(), "Line Thickness" );
  pinfo->Set( vtkPointMark::PARAMETER_TYPE(), VTK_DOUBLE );
  pinfo->Set( vtkPointMark::PARAMETER_NUMBER_OF_COMPONENTS(), 1 );
  pinfo->Set( vtkPointMark::PARAMETER_REAL_BOUNDS(), lineThicknessBounds, 2 );
  pnum = this->AddParameter( pinfo );
  pinfo->Delete();
  this->BindParameter( pnum, defaultLineThickness );

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

  double white[4] = { 1., 1., 1., 1. };
  vtkDoubleArray* col1 = vtkDoubleArray::New();
  col1->SetNumberOfComponents( 4 );
  col1->InsertNextTuple( white );
  vtkVariant defaultFillColor( col1 );
  col1->Delete();
  double fillColorBounds[8] = { 0., 1.,   0., 1.,   0., 1.,   0., 1. };
  pinfo = vtkInformation::New();
  pinfo->Set( vtkPointMark::PARAMETER_NAME(), "Fill Color" );
  pinfo->Set( vtkPointMark::PARAMETER_TYPE(), VTK_DOUBLE );
  pinfo->Set( vtkPointMark::PARAMETER_NUMBER_OF_COMPONENTS(), 4 );
  pinfo->Set( vtkPointMark::PARAMETER_REAL_BOUNDS(), fillColorBounds, 8 );
  pnum = this->AddParameter( pinfo );
  pinfo->Delete();
  this->BindParameter( pnum, defaultFillColor );

  vtkVariant defaultBoxWidth( 0.01 );
  double boxWidthBounds[2] = { 0., VTK_DOUBLE_MAX };
  pinfo = vtkInformation::New();
  pinfo->Set( vtkPointMark::PARAMETER_NAME(), "Box Width" );
  pinfo->Set( vtkPointMark::PARAMETER_TYPE(), VTK_DOUBLE );
  pinfo->Set( vtkPointMark::PARAMETER_NUMBER_OF_COMPONENTS(), 1 );
  pinfo->Set( vtkPointMark::PARAMETER_REAL_BOUNDS(), boxWidthBounds, 2 );
  pnum = this->AddParameter( pinfo );
  pinfo->Delete();
  this->BindParameter( pnum, defaultBoxWidth );

  vtkVariant defaultBoxAscent( 0.008 );
  double boxAscentBounds[2] = { 0., VTK_DOUBLE_MAX };
  pinfo = vtkInformation::New();
  pinfo->Set( vtkPointMark::PARAMETER_NAME(), "Box Ascent" );
  pinfo->Set( vtkPointMark::PARAMETER_TYPE(), VTK_DOUBLE );
  pinfo->Set( vtkPointMark::PARAMETER_NUMBER_OF_COMPONENTS(), 1 );
  pinfo->Set( vtkPointMark::PARAMETER_REAL_BOUNDS(), boxAscentBounds, 2 );
  pnum = this->AddParameter( pinfo );
  pinfo->Delete();
  this->BindParameter( pnum, defaultBoxAscent );

  vtkVariant defaultBoxDescent( 0.006 );
  double boxDescentBounds[2] = { 0., VTK_DOUBLE_MAX };
  pinfo = vtkInformation::New();
  pinfo->Set( vtkPointMark::PARAMETER_NAME(), "Box Descent" );
  pinfo->Set( vtkPointMark::PARAMETER_TYPE(), VTK_DOUBLE );
  pinfo->Set( vtkPointMark::PARAMETER_NUMBER_OF_COMPONENTS(), 1 );
  pinfo->Set( vtkPointMark::PARAMETER_REAL_BOUNDS(), boxDescentBounds, 2 );
  pnum = this->AddParameter( pinfo );
  pinfo->Delete();
  this->BindParameter( pnum, defaultBoxDescent );

  vtkVariant defaultWhiskerAscent( 0.015 );
  double whiskerAscentBounds[2] = { 0., VTK_DOUBLE_MAX };
  pinfo = vtkInformation::New();
  pinfo->Set( vtkPointMark::PARAMETER_NAME(), "Whisker Ascent" );
  pinfo->Set( vtkPointMark::PARAMETER_TYPE(), VTK_DOUBLE );
  pinfo->Set( vtkPointMark::PARAMETER_NUMBER_OF_COMPONENTS(), 1 );
  pinfo->Set( vtkPointMark::PARAMETER_REAL_BOUNDS(), whiskerAscentBounds, 2 );
  pnum = this->AddParameter( pinfo );
  pinfo->Delete();
  this->BindParameter( pnum, defaultWhiskerAscent );

  vtkVariant defaultWhiskerDescent( 0.02 );
  double whiskerDescentBounds[2] = { 0., VTK_DOUBLE_MAX };
  pinfo = vtkInformation::New();
  pinfo->Set( vtkPointMark::PARAMETER_NAME(), "Whisker Descent" );
  pinfo->Set( vtkPointMark::PARAMETER_TYPE(), VTK_DOUBLE );
  pinfo->Set( vtkPointMark::PARAMETER_NUMBER_OF_COMPONENTS(), 1 );
  pinfo->Set( vtkPointMark::PARAMETER_REAL_BOUNDS(), whiskerDescentBounds, 2 );
  pnum = this->AddParameter( pinfo );
  pinfo->Delete();
  this->BindParameter( pnum, defaultWhiskerDescent );

}

void vtkPointMarkStatBox::DrawMarksInternal( vtkContext2D* ctxt,
  vtkDataArray* xc, int xcomp, vtkDataArray* yc, int ycomp,
  vtkIdType ti, vtkIdType te, vtkIdType ts )
{
  if ( ti > te )
    return;

  double* tvx;
  double* tvy;
  double oldLineThickness = -1.;
  double dotSize;
  double lineThickness;
  float fillColor[4];
  float lineColor[4];
  double boxWidth;
  double boxAscent;
  double boxDescent;
  double whiskerAscent;
  double whiskerDescent;
  vtkVariantArray* var = vtkVariantArray::New();

  double origLineColor[4];
  double origFillColor[4];
  double origLineThickness = ctxt->GetPen()->GetWidth();
  ctxt->GetPen()->GetColorF( origLineColor );
  ctxt->GetBrush()->GetColorF( origFillColor );

  for ( vtkIdType pid = ti; pid < te; pid += ts )
    {
    // Fetch the coordinates for this mark's center.
    tvx = xc->GetTuple( pid );
    tvy = yc->GetTuple( pid );

    // Fetch parameter values for this point mark.
    this->GetParameterValuesForTuple( pid, var );
    dotSize = var->GetValue( 0 ).ToDouble();
    lineThickness = var->GetValue( 1 ).ToDouble();
    for ( int c = 0; c < 4; ++ c )
      {
      lineColor[c] = var->GetValue( 2 + c ).ToDouble();
      fillColor[c] = var->GetValue( 6 + c ).ToDouble();
      }
    boxWidth = var->GetValue( 10 ).ToDouble();
    boxAscent = var->GetValue( 11 ).ToDouble();
    boxDescent = var->GetValue( 12 ).ToDouble();
    whiskerAscent = var->GetValue( 13 ).ToDouble();
    whiskerDescent = var->GetValue( 14 ).ToDouble();
    bool drawFill = fillColor[3] > 0.;
    bool drawLine = lineColor[3] > 0. && lineThickness > 0.;

    if ( ! drawFill && ! drawLine )
      {
      continue; // skip this mark!
      }

    // Draw the mark
    if ( oldLineThickness != lineThickness )
      ctxt->GetPen()->SetWidth( lineThickness );
    oldLineThickness = lineThickness;
    ctxt->GetPen()->SetColorF( lineColor[0], lineColor[1], lineColor[2], lineColor[3] );
    ctxt->DrawLine(
      tvx[xcomp], tvy[ycomp] - whiskerDescent,
      tvx[xcomp], tvy[ycomp] + whiskerAscent );
    ctxt->DrawLine(
      tvx[xcomp] - boxWidth / 4., tvy[ycomp] - whiskerDescent,
      tvx[xcomp] + boxWidth / 4., tvy[ycomp] - whiskerDescent );
    ctxt->DrawLine(
      tvx[xcomp] - boxWidth / 4., tvy[ycomp] + whiskerAscent,
      tvx[xcomp] + boxWidth / 4., tvy[ycomp] + whiskerAscent );
    ctxt->GetBrush()->SetColorF( fillColor[0], fillColor[1], fillColor[2], fillColor[3] );
    if ( drawFill )
      {
      ctxt->DrawRect( tvx[xcomp] - boxWidth / 2., tvy[ycomp] - boxDescent, boxWidth, boxAscent + boxDescent );
      }
    if ( oldLineThickness != dotSize )
      ctxt->GetPen()->SetWidth( dotSize );
    oldLineThickness = dotSize;
    ctxt->DrawPoint( tvx[xcomp], tvy[ycomp] );
    /* Was:
    cairo_set_source_rgba( cr, lineColor[0], lineColor[1], lineColor[2], lineColor[3] );
    cairo_stroke( cr );
    cairo_arc( cr, tvx[xcomp], tvy[ycomp], dotSize, 0., 2. * vtkMath::Pi() );
    cairo_fill( cr );
    */
    }

  ctxt->GetPen()->SetWidth( origLineThickness );
  ctxt->GetPen()->SetColorF( origLineColor );
  ctxt->GetBrush()->SetColorF( origFillColor );
  var->Delete();
}


