set(files
  visocyte/tpl/cinema_python/database/store.py
  visocyte/tpl/cinema_python/database/file_store.py
  visocyte/tpl/cinema_python/database/raster_wrangler.py
  visocyte/tpl/cinema_python/database/oexr_helper.py
  visocyte/tpl/cinema_python/database/vti_store.py
  visocyte/tpl/cinema_python/database/__init__.py
  visocyte/tpl/cinema_python/images/layer_rasters.py
  visocyte/tpl/cinema_python/images/lookup_tables.py
  visocyte/tpl/cinema_python/images/querymaker.py
  visocyte/tpl/cinema_python/images/camera_utils.py
  visocyte/tpl/cinema_python/images/__init__.py
  visocyte/tpl/cinema_python/images/querymaker_specb.py
  visocyte/tpl/cinema_python/images/compositor.py
  visocyte/tpl/cinema_python/__init__.py
  visocyte/tpl/cinema_python/adaptors/visocyte/pv_introspect.py
  visocyte/tpl/cinema_python/adaptors/visocyte/progress.py
  visocyte/tpl/cinema_python/adaptors/visocyte/__init__.py
  visocyte/tpl/cinema_python/adaptors/visocyte/pv_explorers.py
  visocyte/tpl/cinema_python/adaptors/visocyte/cinemareader.py
  visocyte/tpl/cinema_python/adaptors/__init__.py
  visocyte/tpl/cinema_python/adaptors/vtk/vtk_explorers.py
  visocyte/tpl/cinema_python/adaptors/vtk/__init__.py
  visocyte/tpl/cinema_python/adaptors/explorers.py)

# if we ever decide to support system cinema_python, then
# `visocyte/tpl/cinema_python` should trampoline to import the
# external `cinema_python` package.

vtk_module_add_python_package(ParaView::CinemaPython
  FILES               ${files}
  PACKAGE             "visocyte.tpl.cinema_python"
  MODULE_DESTINATION  "${VISOCYTE_PYTHON_SITE_PACKAGES_SUFFIX}")

vtk_module_add_python_module(ParaView::CinemaPython
  PACKAGES "visocyte.tpl.cinema_python")
