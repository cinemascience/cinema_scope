#include "CinDBFactory.h"
#include "CinDBView.h"
#include "CinDatabase.h"

#include <QString>

CinDBFactory::CinDBFactory()
{
}

CinDBView *CinDBFactory::BuildDBView()
{
    CinDBView   *view = new CinDBView();
    CinDatabase *db   = new CinDatabase();

    view->setDatabase(db);

    return view;
}
