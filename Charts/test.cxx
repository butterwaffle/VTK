#include "QVTKWidget.h"

#include "vtkChartXY.h"
#include "vtkChartActor.h"
#include "vtkTable.h"
#include "vtkDoubleArray.h"
#include "vtkContext2D.h"
#include "vtkOpenGLContextDevice2D.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

#include <QApplication>

#include <math.h>

int main( int argc, char* argv[] )
{
  QApplication app( argc, argv );

  vtkTable* tab = vtkTable::New();
  vtkDoubleArray* xcol = vtkDoubleArray::New();
  vtkDoubleArray* ycol = vtkDoubleArray::New();
  xcol->SetName( "x" );
  ycol->SetName( "y" );
  tab->AddColumn( xcol );
  tab->AddColumn( ycol );
  xcol->FastDelete();
  ycol->FastDelete();
  for ( int i = 0; i < 256; ++ i )
    {
    double x, y;
    x = i / 255.;
    y = sin( x );
    xcol->InsertNextValue( x );
    ycol->InsertNextValue( y );
    }

  //vtkChartXY* chart = vtkChartXY::SafeDownCast( vtkChartXY::New() );
  /*
  vtkContext2D* ctxt = vtkContext2D::New();
  vtkContextDevice2D* cdev = vtkOpenGLContextDevice2D::New();
  //chart->SetTable( tab, "x", "y" );
  chart->Paint();
  */
  vtkChartActor* actor = vtkChartActor::New();
  QVTKWidget* widg = new QVTKWidget();
  vtkRenderer* rr = vtkRenderer::New();
  widg->GetRenderWindow()->SetMultiSamples( 16 );
  widg->GetRenderWindow()->AddRenderer( rr );
  rr->AddActor( actor );
  rr->SetBackground( 1, 1, 1 );

  widg->show();
  app.exec();

  tab->Delete();
  rr->Delete();
  actor->Delete();
}
