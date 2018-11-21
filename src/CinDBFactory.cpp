#include "CinDBFactory.h"
#include "CinDBInterface.h"
#include "CinDatabase.h"

#include <QString>

CinDBFactory::CinDBFactory()
{
}

CinDBInterface *CinDBFactory::BuildDBInterface()
{
    CinDBInterface *interface = new CinDBInterface();
    CinDatabase    *database  = new CinDatabase();

    interface->setDatabase(database);

    return interface;
}
