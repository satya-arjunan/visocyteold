r"""visocyteweb_wslink is the visocyte-specific subclass
    of vtkweb_wslink that provides the PVWeb Application
"""

from vtkmodules.web import wslink as vtk_wslink
from visocyte.modules.vtkPVWebCore import vtkPVWebApplication
from visocyte.web import protocols as pv_protocols

class PVServerProtocol(vtk_wslink.ServerProtocol):

    def __init__(self):
        vtk_wslink.ServerProtocol.__init__(self)
        # if (vtk_wslink.imageCapture):
        #   self.unregisterLinkProtocol(vtk_wslink.imageCapture)
        # vtk_wslink.imageCapture = pv_protocols.VisocyteWebViewPortImageDelivery()
        # self.registerLinkProtocol(vtk_wslink.imageCapture)
        # vtk_wslink.imageCapture.setApplication(self.getApplication())

    def initApplication(self):
        return vtkPVWebApplication()
