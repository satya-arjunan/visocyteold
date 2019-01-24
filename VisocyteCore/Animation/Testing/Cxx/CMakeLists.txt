vtk_add_test_cxx(vtkPVAnimationCxxTests tests
  NO_DATA NO_VALID NO_OUTPUT
  VisocyteCoreAnimationPrintSelf.cxx
  )
vtk_test_cxx_executable(vtkPVAnimationCxxTests tests)
