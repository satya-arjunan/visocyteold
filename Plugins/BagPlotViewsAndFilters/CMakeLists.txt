set(qrc_files)

if (VISOCYTE_BUILD_QT_GUI)
  list(APPEND qrc_files
    BagPlotViewsAndFilters.qrc)
endif ()

visocyte_add_plugin(BagPlotViewsAndFilters
  REQUIRED_ON_SERVER
  VERSION "1.0"
  SERVER_MANAGER_XML BagPlotViewsAndFilters.xml
  UI_RESOURCES ${qrc_files}
  MODULES BagPlotViewsAndFilters::BagPlotViewsAndFiltersBagPlot
  MODULE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/BagPlot/vtk.module")

if (BUILD_TESTING)
  add_subdirectory(Testing)
endif()
