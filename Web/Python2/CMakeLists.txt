set(files
  visocyte/web/ipython.py
  visocyte/web/pv_web_catalyst.py
  visocyte/web/webgl.py)

vtk_module_add_python_package(Visocyte::PVWebPython2
  FILES               ${files}
  PACKAGE             "visocyte.web"
  MODULE_DESTINATION  "${VISOCYTE_PYTHON_SITE_PACKAGES_SUFFIX}")

vtk_module_add_python_module(Visocyte::PVWebPython2
  PACKAGES "visocyte.web")
