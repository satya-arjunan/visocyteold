/*=========================================================================

  Program:   Visocyte
  Module:    vtkSMDeserializerXMLCache.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.visocyte.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkSMDeserializerXMLCache
 * @brief   deserializes proxies from their XML states.
 *
 * vtkSMDeserializerXMLCache is used to deserialize proxies from previously
 * stored XML states.
*/

#ifndef vtkSMDeserializerXMLCache_h
#define vtkSMDeserializerXMLCache_h

#include "vtkPVServerManagerCoreModule.h" //needed for exports
#include "vtkSMDeserializerXML.h"

class vtkPVXMLElement;
class vtkSMProxy;
class vtkSMProxyLocator;
class vtkSMSession;

class VTKPVSERVERMANAGERCORE_EXPORT vtkSMDeserializerXMLCache : public vtkSMDeserializerXML
{
public:
  static vtkSMDeserializerXMLCache* New();
  vtkTypeMacro(vtkSMDeserializerXMLCache, vtkSMDeserializerXML);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Allow to register a given XML state for a given proxy GlobalId
   */
  virtual void CacheXMLProxyState(vtkTypeUInt32 id, vtkPVXMLElement* xml);

protected:
  vtkSMDeserializerXMLCache();
  ~vtkSMDeserializerXMLCache() override;

  // Friend to access NewProxy().
  friend class vtkSMProxyLocator;

  /**
   * Locate the XML for the proxy with the given id.
   */
  vtkPVXMLElement* LocateProxyElement(vtkTypeUInt32 id) override;

private:
  vtkSMDeserializerXMLCache(const vtkSMDeserializerXMLCache&) = delete;
  void operator=(const vtkSMDeserializerXMLCache&) = delete;

  class vtkInternal;
  vtkInternal* Internals;
};

#endif
