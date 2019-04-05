#include "VisocyteDocumentationInitializer.h"

#include <QObject>
#include <QtPlugin>

void visocyte_documentation_initialize()
{
  Q_INIT_RESOURCE(visocyte_documentation);
}
