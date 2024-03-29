/*****************************************************************************
*
* Copyright (c) 2000 - 2018, Lawrence Livermore National Security, LLC
* Produced at the Lawrence Livermore National Laboratory
* LLNL-CODE-442911
* All rights reserved.
*
* This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
* full copyright notice is contained in the file COPYRIGHT located at the root
* of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
*
* Redistribution  and  use  in  source  and  binary  forms,  with  or  without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of  source code must  retain the above  copyright notice,
*    this list of conditions and the disclaimer below.
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
*    documentation and/or other materials provided with the distribution.
*  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
*    be used to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
* ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
* LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
* DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
* SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

// ************************************************************************* //
//                            avtPixieFileFormat.h                           //
// ************************************************************************* //

#ifndef AVT_PIXIE_FILE_FORMAT_H
#define AVT_PIXIE_FILE_FORMAT_H

#include <avtPixieOptions.h>
#include <avtMTSDFileFormat.h>

class DBOptionsAttributes;

// Define this symbol BEFORE including hdf5.h to indicate the HDF5 code
// in this file uses version 1.6 of the HDF5 API. This is harmless for
// versions of HDF5 before 1.8 and ensures correct compilation with
// version 1.8 and thereafter. When, and if, the HDF5 code in this file
// is explicitly upgraded to the 1.8 API, this symbol should be removed.
#define H5_USE_16_API
#include <vtk_hdf5.h>
#include <vector>
#include <map>

// ****************************************************************************
//  Class: avtPixieFileFormat
//
//  Purpose:
//      Reads in Pixie files as a plugin to VisIt.
//
//  Programmer: Brad Whitlock
//  Creation:   Fri Aug 13 14:31:43 PST 2004
//
//  Modifications:
//    Brad Whitlock, Thu Aug 26 09:54:15 PDT 2004
//    Added support for meshes that have coordinates.
//
//    Brad Whitlock, Wed Sep 15 17:04:01 PST 2004
//    Rewrote some aspects of support for meshes with coordinates so the
//    "coords" attributes are used.
//
//    Eric Brugger, Tue Oct 26 08:12:40 PDT 2004
//    The pixie format was modified so that values defined on the curvilinear
//    mesh were nodal instead of zonal.  One artifact of this change was that
//    now those arrays are 2*nx*ny instead of 1*nx*ny in the case of 2d
//    variables.  In addition to modifying the interpretation of those values
//    I modified the reader to the read the appropriate hyperslab in the 2d
//    case and modified a number of methods to pass around the hyperslab
//    information.
//
//    Eric Brugger, Mon Nov 29 15:50:18 PST 2004
//    Modified the reader to handle gaps in the cycle numbering (e.g. allowing
//    0, 10, 20, 30 instead of requiring 0, 1, 2, 3).
//
//    Mark C. Miller, Mon Apr  4 14:55:14 PDT 2005
//    Added rawExpressionString data member to support expressions
//
//    Mark C. Miller, Tue May 17 18:48:38 PDT 2005
//    Added timeState arg to PopulateDatabaseMetaData to satisfy new interface
//
//    Luis Chacon, Tue Mar 2 10:02:00 EST 2010
//    Added double vector time_val (set up in GetVariableList)
//
//    Jean Favre, Wed Dec 21 11:48:20 CET 2011
//    Added support for parallel reading via hyperslabs
//    Added a start[3], count[3] to VarInfo for parallel hyperslabs
//    Added a start_no_ghost[3], count_no_ghost[3]
//    Added reading options to select hyperslab cutting method
//
// ****************************************************************************

class avtPixieFileFormat : public avtMTSDFileFormat
{
    struct TraversalInfo
    {
        avtPixieFileFormat *This;
        int                level;
        std::string        path;
        bool               hasCoords;
        std::string        coordX;
        std::string        coordY;
        std::string        coordZ;
    };

    struct VarInfo
    {
        bool        timeVarying;
        hsize_t     dims[3];
        hsize_t     start[3];
        hsize_t     count[3];
        hsize_t     start_no_ghost[3];
        hsize_t     count_no_ghost[3];
        hid_t       nativeVarType;
        std::string fileVarName;
        bool        hasCoords;
        std::string coordX;
        std::string coordY;
        std::string coordZ;
    };

    typedef std::map<std::string, VarInfo> VarInfoMap;
public:
                       avtPixieFileFormat(const char *, DBOptionsAttributes *);
    virtual           ~avtPixieFileFormat();

    virtual void           GetCycles(std::vector<int> &);
    virtual void           GetTimes(std::vector<double> &);

    virtual int            GetNTimesteps(void);

    virtual const char    *GetType(void)   { return "Pixie"; };
    virtual void           FreeUpResources(void); 
    virtual void           ActivateTimestep(int ts);

    virtual vtkDataSet    *GetMesh(int, const char *);
    virtual vtkDataArray  *GetVar(int, const char *);

protected:
    void                   Initialize();
    void                   DetermineVarDimensions(const VarInfo &info,
                                                  hsize_t *hyperslabDims,
                                                  int *varDims,
                                                  int &nVarDims) const;
    bool                   MeshIsCurvilinear(const std::string &) const;
    vtkDataSet            *CreatePointMesh(int timestate, const VarInfo &,
                                           const hsize_t *hyperslabDims,
                                           const int *varDims,
                                           int nVarDims) const;
    vtkDataSet            *CreateCurvilinearMesh(int timestate, const VarInfo &,
                                                 const hsize_t *hyperslabDims,
                                                 const int *varDims,
                                                 int nVarDims);
    bool                   ReadCoordinateFields(int timestate, const VarInfo &,
                                                float *coords[3],
                                                const hsize_t *dims,
                                                int nDims) const;
    bool                   ReadVariableFromFile(int timestate,
                                                const std::string &,
                                                const VarInfo &it,
                                                const hsize_t *dims,
                                                void *dest) const;

    void                   AddGhostCellInfo(const VarInfo &info, vtkDataSet *ds);
    void                   PartitionDims();

    virtual void           PopulateDatabaseMetaData(avtDatabaseMetaData *, int);

    hid_t                  fileId;
    VarInfoMap             variables;
    VarInfoMap             meshes;
    int                    nTimeStates;
    bool                   haveMeshCoords;
    std::string            rawExpressionString;
    std::string            timeStatePrefix;
    std::vector<int>       cycles;
    std::vector<double>    time_val;

    static herr_t VisitLinks(hid_t, const char *, const H5L_info_t *, void *);
    static herr_t GetVariableList(hid_t, const char *, void *);

    PixieDBOptions::PartitioningDirection partitioning;
    bool                                  duplicateData;
};

#endif
