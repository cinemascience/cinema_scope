#ifndef CINDBFACTORY_H 
#define CINDBFACTORY_H 

#include <QString>


class CinParamSet;
class CinDBInterface;

//! Class that builds a set of objects needed to interact with Cinema data 
/*!
*/
class CinDBFactory
{
    public:
        // constructor
        CinDBFactory();

        // member functions
        static CinDBInterface *BuildDBInterface();

    private:
};

#endif // CINDBFACTORY_H
