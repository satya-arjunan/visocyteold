if (VISOCYTE_BUILD_QT_GUI AND BUILD_SHARED_LIBS)
  ExternalData_Expand_Arguments("ParaViewData" _
    "DATA{Data/LagrangianParticleTracker/,REGEX:.*}"
    "DATA{Data/LagrangianParticleTracker/flow/,REGEX:.*}"
    "DATA{Data/LagrangianParticleTracker/surface/,REGEX:.*}"

    "DATA{Data/Baseline/LagrangianParticleTracker.png}"
    "DATA{Data/Baseline/LagrangianParticleTracker_InterA.png}"
    "DATA{Data/Baseline/LagrangianParticleTracker_InterB.png}"
    "DATA{Data/Baseline/LagrangianParticleTracker_InterF.png}"
    "DATA{Data/Baseline/LagrangianParticleTracker_InterG.png}"
    "DATA{Data/Baseline/LagrangianParticleTracker_InterH.png}"
    "DATA{Data/Baseline/LagrangianParticleTracker_InterI.png}"
    "DATA{Data/Baseline/LagrangianParticleTracker_InterJ.png}"

    "DATA{Data/Baseline/LagrangianParticleTrackerParallel.png}"
    "DATA{Data/Baseline/LagrangianParticleTrackerParallel_InterA.png}"
    "DATA{Data/Baseline/LagrangianParticleTrackerParallel_InterB.png}"

    "DATA{Data/Baseline/LagrangianParticleTrackerParallelDistributed.png}"
    "DATA{Data/Baseline/LagrangianParticleTrackerParallelDistributed_InterA.png}"

    "DATA{Data/Baseline/LagrangianParticleTrackerParallelSurfDistributed.png}"
    "DATA{Data/Baseline/LagrangianParticleTrackerParallelSurfDistributed_1.png}"

    "DATA{Data/Baseline/LagrangianParticleTrackerReseeding.png}"
    "DATA{Data/Baseline/LagrangianSurfaceHelperComposite.png}")

  visocyte_add_client_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET ParaViewData
    TEST_SCRIPTS LagrangianSeedHelperCustom.xml)
  visocyte_add_client_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET ParaViewData
    TEST_SCRIPTS LagrangianSeedHelper.xml)
  visocyte_add_client_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET ParaViewData
    TEST_SCRIPTS LagrangianSurfaceHelper.xml)
  visocyte_add_client_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET ParaViewData
    TEST_SCRIPTS LagrangianSurfaceHelperNone.xml)
  visocyte_add_client_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET ParaViewData
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_SCRIPTS LagrangianSurfaceHelperComposite.xml)
  visocyte_add_client_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET ParaViewData
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_SCRIPTS LagrangianParticleTracker.xml)
  visocyte_add_client_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET ParaViewData
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_SCRIPTS LagrangianParticleTrackerReseeding.xml)

  if (VISOCYTE_USE_MPI)
    visocyte_add_client_server_tests(
      LOAD_PLUGIN "LagrangianParticleTracker"
      TEST_DATA_TARGET ParaViewData
      BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
      TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/LagrangianParticleTrackerParallel.xml)
    visocyte_add_client_server_render_tests(
      LOAD_PLUGIN "LagrangianParticleTracker"
      TEST_DATA_TARGET ParaViewData
      BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
      TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/LagrangianParticleTrackerParallel.xml)
    visocyte_add_client_server_tests(
      LOAD_PLUGIN "LagrangianParticleTracker"
      TEST_DATA_TARGET ParaViewData
      BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
      TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/LagrangianParticleTrackerParallelDistributed.xml)
    visocyte_add_client_server_render_tests(
      LOAD_PLUGIN "LagrangianParticleTracker"
      TEST_DATA_TARGET ParaViewData
      BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
      TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/LagrangianParticleTrackerParallelDistributed.xml)
    visocyte_add_client_server_tests(
      LOAD_PLUGIN "LagrangianParticleTracker"
      TEST_DATA_TARGET ParaViewData
      BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
      TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/LagrangianParticleTrackerParallelSurfDistributed.xml)
    visocyte_add_client_server_render_tests(
      LOAD_PLUGIN "LagrangianParticleTracker"
      TEST_DATA_TARGET ParaViewData
      BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
      TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/LagrangianParticleTrackerParallelSurfDistributed.xml)
  endif ()

  visocyte_add_client_server_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET ParaViewData
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/LagrangianParticleTrackerReseeding.xml)
  visocyte_add_client_server_render_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET ParaViewData
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/LagrangianParticleTrackerReseeding.xml)
  visocyte_add_client_server_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET ParaViewData
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/LagrangianParticleTracker.xml)
  visocyte_add_client_server_render_tests(
    LOAD_PLUGIN "LagrangianParticleTracker"
    TEST_DATA_TARGET ParaViewData
    BASELINE_DIR ${VISOCYTE_TEST_BASELINE_DIR}
    TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/LagrangianParticleTracker.xml)
endif ()
