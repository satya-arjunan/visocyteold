# Check that matplotlib is available
include(FindPythonModules)
find_python_module(matplotlib matplotlib_found)

# Set variables to make the testing functions.
set(_vtk_build_test "visocyte")
set(${_vtk_build_test}_TEST_LABELS VISOCYTE)

ExternalData_Expand_Arguments(VisocyteData _
  "DATA{${visocyte_test_data_directory_input}/Data/can.ex2}"
  "DATA{${visocyte_test_data_directory_input}/Data/multicomb_0.vts}")

# only enable TestPythonAnnotationFilter test if numpy is available
find_python_module(numpy numpy_found)
if (numpy_found)
  set(visocyte_python_args
    --data=${visocyte_test_data_directory_output}/can.ex2)
  # Add pvpython tests
  visocyte_add_test_python(
    NO_DATA NO_VALID NO_OUTPUT NO_RT
    TestPythonAnnotationFilter.py
    TestPythonAnnotationFilterNoMerge.py
    TestAnnotateAttributeData.py
    CinemaAERTest.py
    )
  visocyte_add_test_python(
    NO_VALID NO_OUTPUT NO_RT
    AppendAttributes.py
    UnstructuredCellTypePythonCalculator.py
    )
  visocyte_add_test_python(
    NO_VALID NO_RT
    FileSeriesWriterSubTimeSteps.py
    )
  unset(visocyte_python_args)

  if (VISOCYTE_ENABLE_MATPLOTLIB AND matplotlib_found)
    # add Matplotlib tests only if matplotlib was found at configure time.
    visocyte_add_test_python(
      NO_DATA NO_RT
      TestPythonViewMatplotlibScript.py
      )
  endif()

  visocyte_add_test_python(
    NO_DATA NO_RT
    TestPythonViewNumpyScript.py
    )
endif()

visocyte_add_test_python(
  NO_DATA NO_RT
  ColorBy.py
  OutputImageResolution.py
  PartialArrayIndication.py
  ResetSession.py
  TestClipCylinder.py
  TestColorHistogram.py
  TestGeometryBoundsClobber.py
  TestHoudiniWriter.py
  TestLineChartLabelPrefix.py
  TestPythonViewScript.py
)

visocyte_add_test_python(
  NO_DATA NO_VALID NO_OUTPUT NO_RT
  TestResetProperty.py
  PointPicking.py
)

visocyte_add_test_python(
  NO_VALID NO_RT
  ExportCSV.py
  Plot3DReader.py
)

# These tests could run safely in serial and parallel.
set(PVBATCH_TESTS
  BatchAxesGrid.py
  PLYWriter.py
  VolumeCellSelection.py
)
if(NOT APPLE)
  # temporarily disable on mac due to bugs in
  # ProjectedTetraHedraMapper (visocyte/visocyte#17620).
  list(APPEND PVBATCH_TESTS
    StructuredGridVolumeRendering.py)
endif()

foreach (tfile IN LISTS PVBATCH_TESTS)
  get_filename_component(test ${tfile} NAME_WE)
  set(${test}_ARGS
    --state ${CMAKE_CURRENT_SOURCE_DIR}/${test}.pvsm)
endforeach ()
if (VISOCYTE_USE_MPI AND VTK_MPIRUN_EXE)
  # run the tests in parallel
  set(${}_NUMPROCS 3)
  visocyte_add_test_pvbatch_mpi(
    JUST_VALID
    ${PVBATCH_TESTS}
    )

  if (numpy_found)
    visocyte_add_test_pvbatch_mpi(
      NO_DATA NO_VALID NO_OUTPUT NO_RT
      TestAnnotateAttributeData.py
      )
  endif()
else()
  # run the test serially
  visocyte_add_test_pvbatch(
    JUST_VALID
    ${PVBATCH_TESTS}
    )
  if (numpy_found)
    visocyte_add_test_pvbatch(
      NO_DATA NO_VALID NO_OUTPUT NO_RT
      TestAnnotateAttributeData.py
      )
  endif()
endif()


visocyte_add_test_driven(
  JUST_VALID
  TestCompositedGeometryCulling.py
)

# Python Multi-servers test
# => Only for shared build as we dynamically load plugins
if(BUILD_SHARED_LIBS)
  set(TestMultiServersConfig_ARGS
    --test-multi-servers 2
    )
  set(TestMultiServersRemoteProxy_ARGS
    --test-multi-servers 3
    )
  set(${_vtk_build_test}_TEST_LABELS VISOCYTE)

  visocyte_add_test_driven(
    NO_DATA NO_VALID NO_OUTPUT NO_RT
    TestMultiServersConfig.py
    TestMultiServersRemoteProxy.py
    TestRemoteProgrammableFilter.py
    )
endif()
