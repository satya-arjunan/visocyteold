###############################################################################
#
# Add python script names here.
set(PY_TESTS
  LockScalarRangeBackwardsCompatibility.py,NO_VALID
  )

visocyte_add_test_python(
  JUST_VALID
  ${PY_TESTS}
  )
