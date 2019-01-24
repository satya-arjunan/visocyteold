set(files
  visocyte/web/__init__.py
  visocyte/web/_default_proxies.py
  visocyte/web/camera.py
  visocyte/web/data_converter.py
  visocyte/web/data_writer.py
  visocyte/web/dataset_builder.py
  visocyte/web/decorators.py
  visocyte/web/helper.py
  visocyte/web/protocols.py
  visocyte/web/pv_wslink.py
  visocyte/web/test_server.py)

vtk_module_add_python_package(Visocyte::WebPython
  FILES               ${files}
  PACKAGE             "visocyte.web"
  MODULE_DESTINATION  "${VISOCYTE_PYTHON_SITE_PACKAGES_SUFFIX}")

vtk_module_add_python_module(Visocyte::WebPython
  PACKAGES "visocyte.web")
