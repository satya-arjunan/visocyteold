if (PARAVIEW_BUILD_QT_GUI AND BUILD_SHARED_LIBS)
  ExternalData_Expand_Arguments("VisocyteData" _
    "DATA{Data/LagrangianParticleTracker/,REGEX:.*}"
    "DATA{Data/LagrangianParticleTracker/flow/,REGEX:.*}"
    "DATA{Data/LagrangianParticleTracker/surface/,REGEX:.*}")

  visocyte_add_client_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS LagrangianSeedHelperCustom.xml)
  visocyte_add_client_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS LagrangianSeedHelper.xml)
  visocyte_add_client_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS LagrangianSurfaceHelper.xml)
  visocyte_add_client_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET VisocyteData
    TEST_SCRIPTS LagrangianSurfaceHelperNone.xml)
  visocyte_add_client_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET VisocyteData
    BASELINE_DIR ${PARAVIEW_TEST_BASELINE_DIR}
    TEST_SCRIPTS LagrangianSurfaceHelperComposite.xml)
  visocyte_add_client_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET VisocyteData
    BASELINE_DIR ${PARAVIEW_TEST_BASELINE_DIR}
    TEST_SCRIPTS LagrangianParticleTracker.xml)
  visocyte_add_client_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET VisocyteData
    BASELINE_DIR ${PARAVIEW_TEST_BASELINE_DIR}
    TEST_SCRIPTS LagrangianParticleTrackerReseeding.xml)

  if (PARAVIEW_USE_MPI)
    visocyte_add_client_server_tests(
      LOAD_PLUGIN "LagrangianParticleTracker"
      TEST_DATA_TARGET VisocyteData
      BASELINE_DIR ${PARAVIEW_TEST_BASELINE_DIR}
      TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/LagrangianParticleTrackerParallel.xml)
    visocyte_add_client_server_render_tests(
      LOAD_PLUGIN "LagrangianParticleTracker"
      TEST_DATA_TARGET VisocyteData
      BASELINE_DIR ${PARAVIEW_TEST_BASELINE_DIR}
      TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/LagrangianParticleTrackerParallel.xml)
    visocyte_add_client_server_tests(
      LOAD_PLUGIN "LagrangianParticleTracker"
      TEST_DATA_TARGET VisocyteData
      BASELINE_DIR ${PARAVIEW_TEST_BASELINE_DIR}
      TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/LagrangianParticleTrackerParallelDistributed.xml)
    visocyte_add_client_server_render_tests(
      LOAD_PLUGIN "LagrangianParticleTracker"
      TEST_DATA_TARGET VisocyteData
      BASELINE_DIR ${PARAVIEW_TEST_BASELINE_DIR}
      TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/LagrangianParticleTrackerParallelDistributed.xml)
    visocyte_add_client_server_tests(
      LOAD_PLUGIN "LagrangianParticleTracker"
      TEST_DATA_TARGET VisocyteData
      BASELINE_DIR ${PARAVIEW_TEST_BASELINE_DIR}
      TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/LagrangianParticleTrackerParallelSurfDistributed.xml)
    visocyte_add_client_server_render_tests(
      LOAD_PLUGIN "LagrangianParticleTracker"
      TEST_DATA_TARGET VisocyteData
      BASELINE_DIR ${PARAVIEW_TEST_BASELINE_DIR}
      TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/LagrangianParticleTrackerParallelSurfDistributed.xml)
  endif ()

  visocyte_add_client_server_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET VisocyteData
    BASELINE_DIR ${PARAVIEW_TEST_BASELINE_DIR}
    TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/LagrangianParticleTrackerReseeding.xml)
  visocyte_add_client_server_render_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET VisocyteData
    BASELINE_DIR ${PARAVIEW_TEST_BASELINE_DIR}
    TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/LagrangianParticleTrackerReseeding.xml)
  visocyte_add_client_server_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET VisocyteData
    BASELINE_DIR ${PARAVIEW_TEST_BASELINE_DIR}
    TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/LagrangianParticleTracker.xml)
  visocyte_add_client_server_render_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET VisocyteData
    BASELINE_DIR ${PARAVIEW_TEST_BASELINE_DIR}
    TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/LagrangianParticleTracker.xml)
endif ()
