SET(KIT Charts)
SET(UKIT CHARTS)
SET(KIT_TCL_LIBS vtkHybridTCL ${VTK_TK_LIBRARIES})
SET(KIT_PYTHON_LIBS vtkHybridPythonD)
SET(KIT_JAVA_LIBS vtkHybridJava)

IF (JAVA_AWT_LIBRARY)
  SET(KIT_JAVA_LIBS ${KIT_JAVA_LIBS} ${JAVA_AWT_LIBRARY})
ENDIF (JAVA_AWT_LIBRARY)

SET(KIT_INTERFACE_LIBRARIES vtkHybrid)
SET(KIT_LIBS vtkIO vtkftgl
  ${VTK_FREETYPE_LIBRARIES}
)

SET(Kit_SRCS
  vtkChartActor.cxx
  vtkAxis.cxx
  vtkBlockItem.cxx
  vtkBrush.cxx
  vtkChart.cxx
  vtkChartXY.cxx
  vtkContext2D.cxx
  vtkContextActor.cxx
  vtkContextDevice2D.cxx
  vtkContextItem.cxx
  vtkContextMapper2D.cxx
  vtkContextScene.cxx
  vtkCurveMark.cxx
  vtkOpenGLContextDevice2D.cxx
  vtkPen.cxx
  vtkPlot.cxx
  vtkPlotGrid.cxx
  vtkPlotLine.cxx
  vtkPointMark.cxx
  vtkPointMarkTick.cxx
  vtkPointMarkStatBox.cxx
  vtkPoints2D.cxx
  vtkQtContextDevice2D.cxx
  vtkQtChart.cxx
  )

SET(MocHeaders
  vtkQtChart.h
  )

INCLUDE_DIRECTORIES(${MOC_INCLUDE_DIRS})

QT4_WRAP_CPP(MocSrcs ${MocHeaders})

SET(Kit_SRCS
  ${Kit_SRCS}
  ${MocSrcs}
  )

SET_SOURCE_FILES_PROPERTIES(
  vtkChart
  vtkContextDevice2D
  vtkContextItem
  vtkContextMapper2D
  vtkPlot
  vtkPointMark
  vtkQtChart
  moc_vtkQtChart
  ABSTRACT
  )

SET_SOURCE_FILES_PROPERTIES(
  vtkQtChart
  moc_vtkQtChart
  WRAP_EXCLUDE
  )

INCLUDE(${QT_USE_FILE})

#-----------------------------------------------------------------------------
# Include CMake code common to all kits.
INCLUDE(${VTK_CMAKE_DIR}/KitCommonBlock.cmake)
#-----------------------------------------------------------------------------

#TARGET_LINK_LIBRARIES(vtkCharts ${QT_LIBRARIES})

  add_executable( chart MACOSX_BUNDLE
    test.cxx
  )
  target_link_libraries( chart vtkCharts QVTK )
