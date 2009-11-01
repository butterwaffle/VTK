#include "vtkCurveMark.h"

#include "vtkBrush.h"
#include "vtkContext2D.h"
#include "vtkDoubleArray.h"
#include "vtkInformation.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPen.h"
#include "vtkType.h"
#include "vtkVariantArray.h"

vtkStandardNewMacro(vtkCurveMark);
vtkCxxRevisionMacro(vtkCurveMark,"$Revision:$");

vtkCurveMark::vtkCurveMark()
{
  this->SetupParameters();
}

vtkCurveMark::~vtkCurveMark()
{
}

void vtkCurveMark::PrintSelf( ostream& os, vtkIndent indent )
{
  this->Superclass::PrintSelf( os, indent );
}

void vtkCurveMark::SetupParameters()
{
  vtkInformation* pinfo;
  int pnum;

  // NOTE: The order of AddParameter() calls must match vtkPointMarkStatBox::Parameters!

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

  /*
  vtkVariant defaultJoinStyle( CAIRO_LINE_JOIN_MITER );
  int joinStyleBounds[2] = { CAIRO_LINE_JOIN_MITER, CAIRO_LINE_JOIN_BEVEL };
  pinfo = vtkInformation::New();
  pinfo->Set( vtkPointMark::PARAMETER_NAME(), "Join Style" );
  pinfo->Set( vtkPointMark::PARAMETER_TYPE(), VTK_INT );
  pinfo->Set( vtkPointMark::PARAMETER_NUMBER_OF_COMPONENTS(), 1 );
  pinfo->Set( vtkPointMark::PARAMETER_INTEGER_BOUNDS(), joinStyleBounds, 2 );
  pnum = this->AddParameter( pinfo );
  pinfo->Delete();
  this->BindParameter( pnum, defaultJoinStyle );

  vtkVariant defaultCapStyle( CAIRO_LINE_CAP_BUTT );
  int capStyleBounds[2] = { CAIRO_LINE_CAP_BUTT, CAIRO_LINE_CAP_SQUARE };
  pinfo = vtkInformation::New();
  pinfo->Set( vtkPointMark::PARAMETER_NAME(), "Cap Style" );
  pinfo->Set( vtkPointMark::PARAMETER_TYPE(), VTK_INT );
  pinfo->Set( vtkPointMark::PARAMETER_NUMBER_OF_COMPONENTS(), 1 );
  pinfo->Set( vtkPointMark::PARAMETER_INTEGER_BOUNDS(), capStyleBounds, 2 );
  pnum = this->AddParameter( pinfo );
  pinfo->Delete();
  this->BindParameter( pnum, defaultCapStyle );
  */
}

void vtkCurveMark::DrawMarks( vtkContext2D* ctxt,
  vtkDataArray* xc, int xcomp, vtkDataArray* yc, int ycomp,
  vtkIdType ti, vtkIdType te, vtkIdType ts )
{
  // We've overridden DrawMarks because the vtkPointMark version may try to
  // turn each point into a glyph instance drawn with a display list (when
  // no parameters vary). We can't have that since our marks depend on 2
  // consecutive points.
  //
  // FIXME: We could rifle through lines and use a single DrawPolyline() to
  //        render consecutive curves that have no changes in style.
  this->DrawMarksInternal( ctxt, xc, xcomp, yc, ycomp, ti, te, ts );
}

void vtkCurveMark::DrawMarksInternal( vtkContext2D* ctxt,
  vtkDataArray* xc, int xcomp, vtkDataArray* yc, int ycomp,
  vtkIdType ti, vtkIdType te, vtkIdType ts )
{
  if ( ti > te )
    return;

  double origFillColor[4];
  double origLineColor[4];
  //int origJoinStyle;
  //int origCapStyle;
  double origLineThickness = ctxt->GetPen()->GetWidth();
  ctxt->GetPen()->GetColorF( origLineColor );
  ctxt->GetBrush()->GetColorF( origFillColor );

  double* tvx;
  double* tvy;
  double oldLineThickness;
  double lineThickness;
  int joinStyle;
  int oldJoinStyle;
  int capStyle;
  int oldCapStyle;
  double fillColor[4];
  double lineColor[4];
  double oldLineColor[4];
  double oldFillColor[4];
  vtkVariantArray* var = vtkVariantArray::New();

  vtkIdType pid = ti;

  tvx = xc->GetTuple( pid );
  tvy = yc->GetTuple( pid );

  double x0, y0;
  x0 = tvx[xcomp];
  y0 = tvy[ycomp];
  this->GetParameterValuesForTuple( pid, var );

  oldLineThickness = var->GetValue( 0 ).ToDouble();
  for ( int i = 0; i < 4; ++ i )
    {
    oldLineColor[i] = var->GetValue( i + 1 ).ToDouble();
    oldFillColor[i] = var->GetValue( i + 5 ).ToDouble();
    }
  oldJoinStyle = var->GetValue( 9 ).ToInt();
  oldCapStyle = var->GetValue( 10 ).ToInt();
  ctxt->GetPen()->SetWidth( oldLineThickness );
  ctxt->GetPen()->SetColorF( oldLineColor[0], oldLineColor[1], oldLineColor[2], oldLineColor[3] );
  //cairo_set_line_join( cr, static_cast<cairo_line_join_t>( oldJoinStyle ) );
  //cairo_set_line_cap( cr, static_cast<cairo_line_cap_t>( oldCapStyle ) );

  for ( pid += ts; pid < te; pid += ts )
    {
    // Fetch the coordinates for this mark's center.
    tvx = xc->GetTuple( pid );
    tvy = yc->GetTuple( pid );
    ctxt->DrawLine( x0, y0, tvx[xcomp], tvy[ycomp] );
    x0 = tvx[xcomp];
    y0 = tvy[ycomp];

    // Fetch parameter values for this point mark.
    this->GetParameterValuesForTuple( pid, var );

    lineThickness = var->GetValue( 0 ).ToDouble();
    for ( int i = 0; i < 4; ++ i )
      {
      lineColor[i] = var->GetValue( i + 1 ).ToDouble();
      fillColor[i] = var->GetValue( i + 5 ).ToDouble();
      }
    joinStyle = var->GetValue( 9 ).ToInt();
    capStyle = var->GetValue( 10 ).ToInt();

    if ( lineThickness != oldLineThickness )
      {
      ctxt->GetPen()->SetWidth( lineThickness );
      oldLineThickness = lineThickness;
      }
    /*
    if ( joinStyle != oldJoinStyle )
      {
      cairo_set_line_join( cr, static_cast<cairo_line_join_t>( joinStyle ) );
      oldJoinStyle = joinStyle;
      }
    if ( capStyle != oldCapStyle )
      {
      cairo_set_line_cap( cr, static_cast<cairo_line_cap_t>( capStyle ) );
      oldCapStyle = capStyle;
      }
      */
    if (
      lineColor[0] != oldLineColor[0] ||
      lineColor[1] != oldLineColor[1] ||
      lineColor[2] != oldLineColor[2] ||
      lineColor[3] != oldLineColor[3] )
      {
      ctxt->GetPen()->SetColorF( lineColor[0], lineColor[1], lineColor[2], lineColor[3] );
      for ( int i = 0; i < 4; ++ i )
        {
        oldLineColor[i] = lineColor[i];
        }
      }
    }
  //cairo_stroke( cr );
  var->Delete();
}

