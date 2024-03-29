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
//                               avtSILGenerator.C                           //
// ************************************************************************* //

#include <avtSILGenerator.h>

#include <stdio.h>

#include <avtDatabaseMetaData.h>
#include <avtSILEnumeratedNamespace.h>
#include <avtSILRangeNamespace.h>
#include <avtSIL.h>

#include <DebugStream.h>
#include <ImproperUseException.h>
#include <TimingsManager.h>

#include <cstring>
#include <cstdlib>
#include <map>
#include <utility>

using std::string;
using std::vector;
using std::map;
using std::pair;

// Prototypes
static int GroupSorter(const void *, const void *);

// ****************************************************************************
//  Method: avtSILGenerator::CreateSIL
//
//  Purpose:
//      Creates a pretty good SIL based on avtDatabaseMetaData.
//
//  Arguments:
//      md      The database meta-data.
//      sil     The SIL to populate.
//
//  Notes:      This routine was originally implemented in avtGenericDatabase.
//
//  Programmer: Hank Childs
//  Creation:   September 6, 2002
//
//  Modifications:
//
//    Hank Childs, Wed Dec  4 08:21:13 PST 2002
//    Make SIL matrices instead of large SIL objects.
//
//    Hank Childs, Fri Aug  1 21:38:52 PDT 2003
//    Add support for curves.
//
//    Jeremy Meredith, August 25, 2005
//    Added group origin.
//
//    Jeremy Meredith, Mon Aug 28 16:25:07 EDT 2006
//    Added scalar enumeration types.
//
//    Hank Childs, Wed Dec 19 08:39:46 PST 2007
//    Add timing information.
//
//    Dave Bremer, Tue Apr  1 16:43:16 PDT 2008
//    Passed in a flag to AddSubsets to control the use of SIL Arrays.
//    They don't perform well when used with a SIL that has many
//    collections, because when the avtSILSet is created on demand,
//    the SIL's collections need to be examined to see if they contain
//    the set, so maps out can be added to the set.  I disabled their 
//    use if the mesh has any groups, although it might be optimal to
//    allow them for small numbers of groups.
//
//    Mark C. Miller, Mon Apr 14 15:28:11 PDT 2008
//    Changed interface to enumerated scalar
//
//    Mark C. Miller, Wed Aug 26 11:08:21 PDT 2009
//    Removed custom SIL stuff.
//
//    Hank Childs, Tue Dec  8 08:34:22 PST 2009
//    Added some data members that enable smaller SILs.
//
//    Cyrus Harrison, Wed Aug 25 08:35:22 PDT 2010
//    Support selection of domains, even if we only have
//    a single domain.
//
//    Kathleen Biagas, Thu Aug 22 10:00:11 PDT 2013
//    Pass groupNames to AddGroups call.
//
//    Mark C. Miller, Tue May 22 18:34:15 PDT 2018
//    Support curves re-interpreted from multi-block meshes.
// ****************************************************************************

void
avtSILGenerator::CreateSIL(avtDatabaseMetaData *md, avtSIL *sil)
{
    int t0 = visitTimer->StartTimer();
    int   i;

    //
    // We will need these when we set up the matrices at the end.
    //
    vector< vector<int> > domainListList;
    vector< vector<int> > matListList;

    //
    // Add all of the meshes to the set.
    //
    int numMeshes = md->GetNumMeshes();
    for (i = 0 ; i < numMeshes ; i++)
    {
        //
        // Create the mesh and add it to the SIL.
        //
        const avtMeshMetaData *mesh = md->GetMesh(i);
        int id = -1;
        avtSILSet_p set = new avtSILSet(mesh->name, id);
        int topIndex = sil->AddWhole(set);

        const avtMaterialMetaData *mat = md->GetMaterialOnMesh(mesh->name);
        bool useArrays = (mat == NULL);

        //
        // Since we have this mesh handy, add all of the subdomains if they
        // exist.
        //
        vector<int> domainList;
        vector<int> groupList;
        if (mesh->numGroups > 0)
        {
            groupList = AddGroups(sil, topIndex, mesh->numGroups, 
                        mesh->groupOrigin, mesh->groupPieceName, 
                        mesh->groupTitle, mesh->groupNames);
        }
        int t1 = visitTimer->StartTimer();
        AddSubsets(sil, topIndex, mesh->numBlocks, mesh->blockOrigin,
                    domainList, mesh->blockTitle, mesh->blockPieceName,
                    mesh->blockNames, mesh->blockNameScheme, useArrays);
        visitTimer->StopTimer(t1, "Adding subsets");
        if (mesh->numGroups > 0)
        {
            AddGroupCollections(sil, topIndex, mesh->numGroups,
                        domainList, mesh->groupIds, mesh->groupIdsBasedOnRange,
                        mesh->blockTitle, groupList);
        }
        domainListList.push_back(domainList);

        //
        // Add material related sets if they exist
        //
        vector<int> matList;
        if (mat != NULL)
        {
            int id = -1;
            AddMaterials(sil, topIndex, mat->name, mat->materialNames,
                         matList, id);

            //
            // Add the species if they exist
            //
            const avtSpeciesMetaData *spec = md->GetSpeciesOnMesh(mesh->name);
            if (spec != NULL)
            {
                if (spec->numMaterials != mat->numMaterials)
                    EXCEPTION0(ImproperUseException);
 
                AddSpecies(sil, topIndex, mat->materialNames,
                           spec->name, spec, id);
            }
        }
        matListList.push_back(matList);

        //
        // Add scalar enumerations if they exist
        //

        for (int j=0; j<md->GetNumScalars(); j++)
        {
            const avtScalarMetaData *smd = md->GetScalar(j);
            if (smd->GetEnumerationType() != avtScalarMetaData::None &&
                smd->meshName == mesh->name)
            {
                AddEnumScalars(sil, topIndex, smd);
            }
        }

        //
        // IF WE EVER WANT TO STOP USING SIL MATRICES, THE BELOW CODE CAN BE
        // USED IN LIEU OF THEM (AND COMMENT OUT THE SIL MATRIX CODE).
        //
        //  AddMaterialSubsets(sil, domainList, mesh->numBlocks,
        //                     mesh->blockOrigin, matList, mat->name,
        //                     mesh->blockNames, mat->materialNames);
        //
    }
 
    for (i = 0 ; i < md->GetNumCurves() ; i++)
    {
        const avtCurveMetaData *curve = md->GetCurve(i);

        // Look for telltale signs its a mesh re-interpreted as a curve. If not
        // it is the simple ole' curve case.
        if (!(curve->name.substr(0, 14) == "Scalar_Curves/" &&
            curve->name.substr(14, string::npos) == curve->from1DScalarName))
        {
            avtSILSet_p set = new avtSILSet(curve->name, 0);
            sil->AddWhole(set);
            continue;
        }

        // Assume its a mesh re-interpreted as a curve object and get the mesh.
        // If that fails, treat it like the simple ole' curve case.
        const avtMeshMetaData *mesh = md->GetMesh(md->MeshForVar(curve->from1DScalarName));
        if (!mesh)
        {
            avtSILSet_p set = new avtSILSet(curve->name, 0);
            sil->AddWhole(set);
            continue;
        }

        // Arriving here, this curve object is a re-interpretation of a 1D
        // mesh object. So, handle the possibility that it is a multi-block
        // curve and create its subsets.
        bool const useArrays = false;
        int id = -1;
        avtSILSet_p set = new avtSILSet(curve->name, id);
        int topIndex = sil->AddWhole(set);
        vector<int> domainList;
        if (mesh->numBlocks >= 1)
            AddSubsets(sil, topIndex, mesh->numBlocks, mesh->blockOrigin,
                domainList, mesh->blockTitle, mesh->blockPieceName,
                mesh->blockNames, mesh->blockNameScheme, false);
    }
    
    //
    // Note that the SIL matrices must be added last, so we have to pull this
    // out as its own 'for' loop.
    //
    for (i = 0 ; i < numMeshes ; i++)
    {
        const avtMeshMetaData *mesh = md->GetMesh(i);
        const avtMaterialMetaData *mat = md->GetMaterialOnMesh(mesh->name);

        if (mat != NULL)
        {
            avtSILMatrix_p matrix = new avtSILMatrix(domainListList[i],
                                                SIL_DOMAIN, mesh->blockTitle,
                                                matListList[i], SIL_MATERIAL,
                                                mat->name);
            sil->AddMatrix(matrix);
        }
    }

    visitTimer->StopTimer(t0, "SIL generator to execute");
}


// ****************************************************************************
//  Method: avtSILGenerator::AddSubsets
//
//  Purpose:
//      Adds the subsets to a whole.
//
//  Arguments:
//      sil      The sil to add these sets to.
//      parent   The index of parent of all of these subsets.
//      num      The number of subsets.
//      origin   The origin of the subset numbers.
//      list     A list of all of the indices for each of the subsets created.
//      names    A list of each of the block names.
//      title    The title for the subsets.
//      unit     The prefix for each subset name.
//      cat      The category role tag for these subsets in context of parent.
//
//  Notes:      This routine was originally implemented in avtGenericDatabase.
//
//  Programmer:  Hank Childs
//  Creation:    September 6, 2002
//
//  Modifications:
//
//     Mark C. Miller, September 3, 2003
//     Added cat argument, renamed 2nd arg to 'parent'
//
//     Mark C. Miller, September 14, 2003
//     Added onlyCreateSets argument
//
//     Dave Bremer, Mon Feb 12 17:20:43 PST 2007
//     Added support for format strings.
//
//     Dave Bremer, Wed Dec 19 12:18:03 PST 2007
//     Added code to use an avtSILArray in place of a bunch of sets.
//
//     Dave Bremer, Thu Mar 27 16:39:04 PDT 2008
//     Extended the use of avtSILArrays to the case in which the
//     domains all have explicit names.
//
//     Dave Bremer, Tue Apr  1 16:43:16 PDT 2008
//     Added a flag to AddSubsets to control the use of SIL Arrays.
//     They don't perform well when used with a SIL that has many
//     collections, because when the avtSILSet is created on demand,
//     the SIL's collections need to be examined to see if they contain
//     the set, so maps out can be added to the set.
//
//     Hank Childs, Mon Dec  7 14:26:38 PST 2009
//     Add flags for AMR efficiency.
//
// ****************************************************************************

void
avtSILGenerator::AddSubsets(avtSIL *sil, int parent, int num, int origin,
                            vector<int> &list, const string &title,
                            const string &unit, const vector<string> &names,
                            const NameschemeAttributes &namescheme,
                            bool useSILArrays, SILCategoryRole cat, 
                            bool onlyCreateSets)
{
    list.reserve(list.size() + num);
    if (useSILArrays && !onlyCreateSets)
    {
        debug5 << "Using SIL arrays to improve efficiency" << endl;
        int iFirstSet = sil->GetNumSets();
        avtSILArray_p  pArray;

        if (namescheme.GetNamescheme() != "")
            pArray = new avtSILArray(namescheme, num, origin, (cat==SIL_DOMAIN),
                                     title, cat, parent);
        else if (names.size() == (size_t)num)
            pArray = new avtSILArray(names, num, origin, (cat==SIL_DOMAIN),
                                     title, cat, parent);
        else
            pArray = new avtSILArray(unit,  num, origin, (cat==SIL_DOMAIN),
                                     title, cat, parent);
        sil->AddArray(pArray);

        for (int ii = 0; ii < num; ii++)
            list.push_back(iFirstSet + ii);
    }
    else
    {
        debug5 << "Not using SIL arrays to construct SIL, likely because there "
               << "are materials involved." << endl;
        for (int i = 0 ; i < num ; i++)
        {
            char name[1024];
            if (names.size() == (size_t)num)
            {
                strcpy(name, names[i].c_str());
            }
            else
            {
                if (strstr(unit.c_str(), "%") != NULL)
                    sprintf(name, unit.c_str(), i+origin);
                else
                    sprintf(name, "%s%d", unit.c_str(), i+origin);
            }
    
            // determine "identifier" for the set (only "domains" get non -1) 
            int ident = -1;
            if (cat == SIL_DOMAIN)
            ident = i; 
    
            avtSILSet_p set = new avtSILSet(name, ident);
    
            int dIndex = sil->AddSubset(set);
            list.push_back(dIndex);
        }
        if (!onlyCreateSets)
        {
            //
            // Create a namespace and a collection.  The collection owns the
            // namespace after it is registered (so no leaks).
            //
            avtSILEnumeratedNamespace *ns = new avtSILEnumeratedNamespace(list);
            avtSILCollection_p coll = new avtSILCollection(title, cat, parent, ns);
        
            sil->AddCollection(coll);
        }
    }
}


// ****************************************************************************
//  Method: avtSILGenerator::AddGroups
//
//  Purpose:
//      Adds the groups to the SIL.  Hook them up with the domains so that
//      turning off a domain turns off all the domains underneath it.
//
//  Arguments:
//      sil      The sil to add these sets to.
//      top      The index of the top level set.
//      domList  The indices to the domains in the SIL.
//      groupIds The group index of each domain.
//      gTitle   The title of the groups.
//      piece    The name of one set in the group.
//      bTitle   The title of the blocks.
//
//  Notes:      This routine was originally implemented in avtGenericDatabase.
//
//  Programmer:  Hank Childs
//  Creation:    September 6, 2002
//
//  Modifications:
//    Jeremy Meredith, Fri Feb 28 10:19:35 PST 2003
//    Swapped the order of a conjunctive test to prevent an ABR.
//
//    Jeremy Meredith, Wed Aug 24 12:42:53 PDT 2005
//    Added an origin.
//
//    Dave Bremer, Mon Feb 12 17:20:43 PST 2007
//    Added support for format strings.
// 
//    Hank Childs, Mon Dec  7 14:26:38 PST 2009
//    Add flags for AMR efficiency.  Separate out the collection code to its
//    own routine.
//
//    Kathleen Biagas, Thu Aug 22 10:00:35 PDT 2013
//    Added groupNames argument. If empty or size doesn't match numGroups,
//    names will be generated as before.
//
// ****************************************************************************
 
std::vector<int>
avtSILGenerator::AddGroups(avtSIL *sil, int top, int numGroups, int origin,
                           const std::string &piece, const std::string &gTitle,
                           const std::vector< std::string > &gNames)
{
    int  i;
 
    //
    // Start off by adding each group to the SIL.
    //
    vector<int> groupList;
    for (i = 0 ; i < numGroups ; i++)
    {
        char name[1024];
        if (!gNames.empty() && gNames.size() == (size_t)numGroups)
            strcpy(name, gNames[i].c_str()); 
        else if (strstr(piece.c_str(), "%") != NULL)
            sprintf(name, piece.c_str(), i+origin);
        else
            sprintf(name, "%s%d", piece.c_str(), i+origin);
 
        avtSILSet_p set = new avtSILSet(name, -1);
 
        int gIndex = sil->AddSubset(set);
        groupList.push_back(gIndex);
    }

    //
    // Create a namespace and a collection.  The collection owns the
    // namespace after it is registered (so no leaks).
    //
    avtSILEnumeratedNamespace *ns = new avtSILEnumeratedNamespace(groupList);
    avtSILCollection_p coll = new avtSILCollection(gTitle, SIL_BLOCK, top, ns);
    sil->AddCollection(coll);

    return groupList;
}
 

// ****************************************************************************
//  Method: avtSILGenerator::AddGroupCollections
//
//  Purpose:
//      The groups were added in "AddGroups".  This builds the collections 
//      (maps) underneath the groups to the blocks.
//
//  Programmer: Hank Childs
//  Creation:   December 11, 2009
//
// ****************************************************************************

void
avtSILGenerator::AddGroupCollections(avtSIL *sil, int top, int numGroups, 
        const vector<int> &domList, const vector<int> &groupIds,
        const vector<int> &groupIdsBasedOnRange,
        const string &bTitle,
        const vector<int> &groupList)
{
    int  i;
    int t1 = visitTimer->StartTimer();
   
    if (groupIdsBasedOnRange.size() > 0)
    {
        for (size_t i = 0 ; i < groupIdsBasedOnRange.size()-1 ; i++)
        {
            int min, max;
            int startSet = sil->GetNumSets()-groupIdsBasedOnRange[groupIdsBasedOnRange.size()-1];
            min = startSet + groupIdsBasedOnRange[i];
            max = startSet + groupIdsBasedOnRange[i+1]-1;

            avtSILRangeNamespace *ns = new avtSILRangeNamespace(groupList[i],min,max);
            avtSILCollection_p coll = new avtSILCollection(bTitle, SIL_DOMAIN,
                                                           groupList[i], ns);
            sil->AddCollection(coll);
        }
    }
    else
    {
     
        //
        // Things aren't very well sorted here -- we want all of the domains,
        // sorted by groupId.  Let's try to be efficient and use the qsort 
        // routine provided by stdlib to do this.
        //
        int nDoms = (int)domList.size();
        int *records = new int[2*nDoms];
        for (i = 0 ; i < nDoms ; i++)
        {
            records[2*i]   = i;            // The domain.
            records[2*i+1] = groupIds[i];  // The group.
        }
        qsort(records, nDoms, 2*sizeof(int), GroupSorter);
     
        //
        // Now create the collections that fall under each group.
        //
        int current = 0;
        for (i = 0 ; i < numGroups ; i++)
        {
            vector<int> thisGroupsList;
            while (current < nDoms && records[2*current+1] <= i)
            {
                if (records[2*current+1] == i)
                {
                    thisGroupsList.push_back(domList[records[2*current]]);
                }
                current++;
            }
     
            if (thisGroupsList.size() > 0)
            {
                avtSILEnumeratedNamespace *ns = 
                    new avtSILEnumeratedNamespace(thisGroupsList);
                avtSILCollection_p coll = 
                    new avtSILCollection(bTitle, SIL_DOMAIN, groupList[i], ns);
                sil->AddCollection(coll);
            }
        }
 
        delete [] records;
    }
    visitTimer->StopTimer(t1, "Adding groups to SIL (includes qsort)");
}
 
 
// ****************************************************************************
//  Method: avtSILGenerator::AddMaterials
//
//  Purpose:
//      Adds a material to the top level.
//
//  Arguments:
//      sil        The sil to add the materials to.
//      top        The index of the whole mesh.
//      name       The name of the material (ie "mat1", not "copper")
//      matnames   The name of the individual materials (ie "copper", "steel")
//      list       A place to store the subsets we have created.
//      id         An id to be used for all of the sets (-1 okay).
//
//  Notes:      This routine was originally implemented in avtGenericDatabase.
//
//  Programmer: Hank Childs
//  Creation:   September 6, 2002
//
// ****************************************************************************
 
void
avtSILGenerator::AddMaterials(avtSIL *sil, int top, const string &name,
                                 const vector<string> &matnames,
                                 vector<int> &list, int id)
{
    int numMats = (int)matnames.size();
    for (int i = 0 ; i < numMats ; i++)
    {
        avtSILSet_p set = new avtSILSet(matnames[i], id);
        int mIndex = sil->AddSubset(set);
        list.push_back(mIndex);
    }
 
    //
    // Create a namespace and a collection.  The collection owns the namespace
    // after it is registered (so no leaks).
    //
    avtSILEnumeratedNamespace *ns = new avtSILEnumeratedNamespace(list);
    avtSILCollection_p coll = new avtSILCollection(name, SIL_MATERIAL,
                                                   top, ns);
 
    sil->AddCollection(coll);
}
 
 
// ****************************************************************************
//  Method: avtSILGenerator::AddSpecies
//
//  Purpose:
//      Adds species to a material.
//
//  Arguments:
//      sil        The sil to add the species to.
//      top        The index of the whole
//      matnames   The names of all amterials
//      name       The name of the species object (ie "spec1", not "oxygen")
//      species    The species metadata object.
//      id         An id to be used for all of the sets (-1 okay).
//
//  Notes:      This routine was originally implemented in avtGenericDatabase.
//
//  Programmer: Jeremy Meredith
//  Creation:   September 6, 2002
//
//  Modifications:
//    Brad Whitlock, Thu Mar 8 09:45:06 PDT 2007
//    I made it use the automatically generated avtSpeciesMetaData object.
//
// ****************************************************************************
 
void
avtSILGenerator::AddSpecies(avtSIL *sil, int top,
                               const vector<string> &matnames,
                               const string &name,
                               const avtSpeciesMetaData *species,
                               int id)
{
    vector<int> list;
    int numMats = (int)matnames.size();
    for (int i = 0 ; i < numMats ; i++)
    {
        if(i >= 0 && i < species->GetNumSpecies())
        {
            const vector<string> &specnames = species->GetSpecies(i).speciesNames;
            int numSpecs = (int)specnames.size();
            for (int j = 0 ; j < numSpecs; j++)
            {
                char n[1024];
                sprintf(n, "Mat %s, Spec %s", matnames[i].c_str(),
                                              specnames[j].c_str());
                avtSILSet_p set = new avtSILSet(n, id);
                int mIndex = sil->AddSubset(set);
                list.push_back(mIndex);
            }
        }
    }
 
    //
    // Create a namespace and a collection.  The collection owns the namespace
    // after it is registered (so no leaks).
    //
    avtSILEnumeratedNamespace *ns = new avtSILEnumeratedNamespace(list);
    avtSILCollection_p coll = new avtSILCollection(name, SIL_SPECIES,
                                                   top, ns);
 
    sil->AddCollection(coll);
}


// ****************************************************************************
//  Method: avtSILGenerator::AddMaterialSubsets
//
//  Purpose:
//      Adds a layer of material-domains and connects them up with the correct
//      domain and material.
//
//  Arguments:
//      sil         The SIL to add the material-domains to.
//      domList     The list of indices for the domains.
//      blocks      The number of blocks for the domains.
//      origin      The origin of the block numbering.
//      matList     The list of indices for the materials.
//      mname       The name of the material (ie "mat1", not "copper").
//      blocknames  A list of each of the block names.
//      matnames    The names of the actual materials (ie "copper", "steel").
//
//  Notes:      This routine was originally implemented in avtGenericDatabase.
//
//  Programmer: Hank Childs
//  Creation:   September 6, 2002
//
// ****************************************************************************
 
void
avtSILGenerator::AddMaterialSubsets(avtSIL *sil, const vector<int> &domList,
                               int blocks, int origin,
                               const vector<int> &matList, const string &mname,
                               const vector<string> &blocknames,
                               const vector<string> &matnames)
{
    int   i, j;
 
    //
    // Start off by creating all of the material domains and adding them to
    // the sil.
    //
    int nmat =(int) matnames.size();
    vector<int>  matdomlist;
    for (i = 0 ; i < blocks ; i++)
    {
        for (j = 0 ; j < nmat ; j++)
        {
            //
            // Create a domain for this domain, this material.
            //
            char matdom_name[1024];
            if (blocknames.size() == (size_t)blocks)
            {
                sprintf(matdom_name, "Dom=%s,Mat=%s",
                        blocknames[i].c_str(), matnames[j].c_str());
            }
            else
            {
                sprintf(matdom_name, "Dom=%d,Mat=%s",
                        i+origin,matnames[j].c_str());
            }

            //
            // Create the set and add it to the SIL.
            //
            avtSILSet_p set = new avtSILSet(matdom_name, i);
            int mdIndex = sil->AddSubset(set);
 
            //
            // Store the index of the set so we can set up a collection later.
            //
            matdomlist.push_back(mdIndex);
        }
    }
 
    //
    // Set up all of the collections relating the domains to
    // the material-domains.
    //
    for (i = 0 ; i < blocks ; i++)
    {
        //
        // We want all of the materials on block i.
        //
        vector<int> mdlist;
        for (j = 0 ; j < nmat ; j++)
        {
            mdlist.push_back(matdomlist[i*nmat+j]);
        }
 
        //
        // Create a namespace and a collection.  The collection owns the
        // namespace after it is registered (so no leaks).
        //
        avtSILEnumeratedNamespace *ns = new avtSILEnumeratedNamespace(mdlist);
        int dl = domList[i];
        avtSILCollection_p coll = new avtSILCollection(mname, SIL_MATERIAL,
                                                       dl, ns);
 
        sil->AddCollection(coll);
    }
 
    //
    // Set up all of the collections relating the materials to
    // the material-domains.
    //
    for (i = 0 ; i < nmat ; i++)
    {
        //
        // We want all of the blocks on material i.
        //
        vector<int> mdlist;
        for (j = 0 ; j < blocks ; j++)
        {
            mdlist.push_back(matdomlist[j*nmat+i]);
        }
 
        //
        // Create a namespace and a collection.  The collection owns the
        // namespace after it is registered (so no leaks).
        //
        avtSILEnumeratedNamespace *ns = new avtSILEnumeratedNamespace(mdlist);
        avtSILCollection_p coll = new avtSILCollection("domains", SIL_DOMAIN,
                                                       matList[i], ns);
 
        sil->AddCollection(coll);
    }
}


// ****************************************************************************
//  Method:  avtSILGenerator::AddEnumScalarSubgraph
//
//  Purpose: Add SIL collection structure representing the graph of an
//  enumerated scalar.
//
//  Programmer:  Mark C. Miller 
//  Creation:    March 26, 2008
//
// ****************************************************************************

static void
AddEnumScalarSubgraph(avtSIL *sil,
                      int silTop, int enumTop, const string enumTopName,
                      const vector<int> &graphEdges,
                      const vector<string> &graphEdgeNames,
                      const vector<int> &graphEdgeNameIndexs,
                      const vector<int> &setIDs)
{
    // Map of the children using an edge name so to create a SIL for
    // each edge name.
  
    // Of the pair the first  is vector<int> childSetIDs;
    // Of the pair the second is vector<int> childEnumIDs;
    map< string, pair< vector<int>, vector<int> > > children;

    //
    // Find all the child sets of the given subgraphTop set.
    //
    if (enumTop == -1)
    {
        //
        // First, find all the top-level enum sets (those that do NOT appear as the
        // 'tail' of an edge). All tails are at the 'odd' indices in the edge list.
        //
        vector<bool> isTopEnum(setIDs.size(), true);
        for (size_t i = 1; i < graphEdges.size(); i+=2)
            isTopEnum[graphEdges[i]] = false;

        for (int i = 0; i < (int)setIDs.size(); i++)
        {
            if (isTopEnum[i])
            {
                children[enumTopName].first.push_back( setIDs[i] );
                children[enumTopName].second.push_back( i );
            }
        }
    }
    else
    {
        for (size_t i = 0; i < graphEdges.size(); i+=2)
        {
            if (graphEdges[i] == enumTop)
            {
                string edgeName;
                int index = graphEdgeNameIndexs[i/2]; // Edge list is 2x 

                // No edge name index so use the top name
                if( index == -1 )
                  edgeName = enumTopName;
                // Have an edge name index edge index so get the name
                else
                  edgeName = graphEdgeNames[index];

                children[edgeName].first.push_back( setIDs[graphEdges[i+1]] );
                children[edgeName].second.push_back( graphEdges[i+1] );
            }
        }
    }

    //
    // Add this collection of children to the SIL 
    //
    for( const auto child : children )
    {
        const string edgeName = child.first;
        const vector<int> &childSetIDs = child.second.first;
        const vector<int> &childEnumIDs = child.second.second;
        
        avtSILEnumeratedNamespace *ns = new avtSILEnumeratedNamespace(childSetIDs);
        avtSILCollection_p coll = new avtSILCollection(edgeName, SIL_ENUMERATION,
                                                       silTop, ns);
        sil->AddCollection(coll);

        //
        // Recurse on the children
        //
        for (size_t i = 0; i < childSetIDs.size(); i++)
        {
            const string name = sil->GetSILSet(childSetIDs[i])->GetName();
            AddEnumScalarSubgraph(sil, childSetIDs[i], childEnumIDs[i], name, 
                                  graphEdges, graphEdgeNames, graphEdgeNameIndexs,
                                  setIDs);
        }
    }
}


// ****************************************************************************
//  Method:  avtSILGenerator::AddEnumScalars
//
//  Purpose:
//    Adds collections for an enumerated scalar to the SIL.
//
//  Arguments:
//      sil        The sil to add the species to.
//      top        The index of the whole
//      smd        The meta data for the enumerated scalar
//
//  Programmer:  Jeremy Meredith
//  Creation:    August 28, 2006
//
//  Modifications:
//
//    Mark C. Miller, Wed Mar 26 16:23:27 PDT 2008
//    Added support for enumerated scalars w/graphs
//
// ****************************************************************************
void
avtSILGenerator::AddEnumScalars(avtSIL *sil, int top,
                                const avtScalarMetaData *smd)
{
    int nEnums = (int)smd->enumNames.size();
    vector<int> enumList;
    for (int k=0; k<nEnums; k++)
    {
        char name[1024];
        sprintf(name, "%s", smd->enumNames[k].c_str());
        avtSILSet_p set = new avtSILSet(name, -1);
        int dIndex = sil->AddSubset(set);
        enumList.push_back(dIndex);
    }

    if (smd->enumGraphEdges.size() > 0)
    {
        AddEnumScalarSubgraph(sil, top, -1, smd->name,
                              smd->enumGraphEdges,
                              smd->enumGraphEdgeNames,
                              smd->enumGraphEdgeNameIndexs,
                              enumList);
    }
    else
    {
        avtSILEnumeratedNamespace *ns = new avtSILEnumeratedNamespace(enumList);
        avtSILCollection_p coll = new avtSILCollection(smd->name, SIL_ENUMERATION,
                                                       top, ns);
        sil->AddCollection(coll);
    }
}


// ****************************************************************************
//  Function: GroupSorter
//
//  Purpose:
//      This sorts records that have two entries -- a domain number and a group
//      id.  This is a routine that is then fed into qsort.  Its only value
//      added is to provide the >, <, ==.
//
//  Arguments:
//      arg1    The first record.
//      arg2    The second record.
//
//  Returns:    <0 if arg1<arg2, 0 if arg1==arg2, >0 if arg2>arg1.
//
//  Programmer: Hank Childs
//  Creation:   June 24, 2002
//
// ****************************************************************************

int
GroupSorter(const void *arg1, const void *arg2)
{
    const int *r1 = (const int *) arg1;
    const int *r2 = (const int *) arg2;

    if (r1[1] > r2[1])
    {
        return 1;
    }
    else if (r2[1] > r1[1])
    {
        return -1;
    }
    else if (r1[0] > r2[0])
    {
        return 1;
    }
    else if (r2[0] > r1[0])
    {
        return -1;
    }

    return 0;
}
