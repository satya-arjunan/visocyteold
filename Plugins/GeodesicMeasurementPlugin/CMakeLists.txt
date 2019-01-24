project( GeodesicMeasurementPlugin )
cmake_minimum_required( VERSION 3.3 )

if (NOT Visocyte_BINARY_DIR)
  find_package( Visocyte REQUIRED )
  include( ${VISOCYTE_USE_FILE} )
endif()

add_subdirectory( FmmMesh )

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/FmmMesh
  ${CMAKE_CURRENT_BINARY_DIR}/FmmMesh
  ${VTK_INCLUDE_DIRS}
  )

add_visocyte_plugin( GeodesicMeasurement "1.0"
  SERVER_MANAGER_XML GeodesicMeasurement.xml
  SERVER_MANAGER_SOURCES
    vtkPolyDataGeodesicDistance.cxx
    vtkFastMarchingGeodesicDistance.cxx
    vtkFastMarchingGeodesicPath.cxx
    vtkGeodesicsBetweenPoints.cxx
)

target_link_libraries( GeodesicMeasurement LINK_PRIVATE FmmMesh )
