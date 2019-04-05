r"""
    This module is a VisocyteWeb server application used only for automated testing
"""

import os

# import visocyte modules.
from visocyte.web import pv_wslink
from visocyte.web import protocols as pv_protocols
from wslink import server

try:
    import argparse
except ImportError:
    # since  Python 2.6 and earlier don't have argparse, we simply provide
    # the source for the same as _argparse and we use it instead.
    from vtk.util import _argparse as argparse


# =============================================================================
# Create custom Pipeline Manager class to handle clients requests
# =============================================================================

class _TestServer(pv_wslink.PVServerProtocol):

    dataDir = os.getcwd()
    canFile = None
    authKey = "wslink-secret"
    dsHost = None
    dsPort = 11111
    rsHost = None
    rsPort = 11111
    rcPort = -1
    fileToLoad = None
    groupRegex = "[0-9]+\\.[0-9]+\\.|[0-9]+\\."
    excludeRegex = "^\\.|~$|^\\$"
    plugins = None
    filterFile = None
    colorPalette = None
    proxies = None
    allReaders = True
    saveDataDir = os.getcwd()
    viewportScale=1.0
    viewportMaxWidth=2560
    viewportMaxHeight=1440

    def initialize(self):
        # Bring used components
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebStartupRemoteConnection(_TestServer.dsHost, _TestServer.dsPort, _TestServer.rsHost, _TestServer.rsPort, _TestServer.rcPort))
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebStartupPluginLoader(_TestServer.plugins))
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebFileListing(_TestServer.dataDir, "Home", _TestServer.excludeRegex, _TestServer.groupRegex))
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebProxyManager(allowedProxiesFile=_TestServer.proxies, baseDir=_TestServer.dataDir, fileToLoad=_TestServer.fileToLoad, allowUnconfiguredReaders=_TestServer.allReaders))
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebColorManager(pathToColorMaps=_TestServer.colorPalette))
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebMouseHandler())
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebViewPort()) # _TestServer.viewportScale, _TestServer.viewportMaxWidth, _TestServer.viewportMaxHeight
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebViewPortImageDelivery())
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebViewPortGeometryDelivery())
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebTimeHandler())
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebSelectionHandler())
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebWidgetManager())
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebKeyValuePairStore())
        self.registerVtkWebProtocol(pv_protocols.VisocyteWebSaveData(baseSavePath=_TestServer.saveDataDir))

        # Update authentication key to use
        self.updateSecret(_TestServer.authKey)

# =============================================================================
# Main: Parse args and start server
# =============================================================================

if __name__ == "__main__":
    # Create argument parser
    parser = argparse.ArgumentParser(description="Visocyte Web Test Server")

    # Add arguments
    server.add_arguments(parser)
    args = parser.parse_args()

    # Start server
    server.start_webserver(options=args, protocol=_TestServer)
