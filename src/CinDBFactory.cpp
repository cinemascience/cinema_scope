#include "CinDBFactory.h"
#include "CinDBView.h"
#include "CinDatabase.h"

#include <QString>

CinDBFactory::CinDBFactory()
{
}

/*! \brief Build objects to manage a database and a view of it
 *
*/
CinDBView *CinDBFactory::BuildDBView()
{
    // TODO check with Ollie: should this return references?
    CinDBView   *view = new CinDBView();
    CinDatabase *db   = new CinDatabase();

    view->setDatabase(db);

    return view;
}
