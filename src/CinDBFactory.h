#ifndef CINDBFACTORY_H 
#define CINDBFACTORY_H 

#include <QString>


class CinParamSet;
class CinDBView;

//! Builds a set of objects needed to interact with Cinema data 
/*!
*/
class CinDBFactory
{
    public:
        // constructor
        CinDBFactory();

        // member functions
        static CinDBView *BuildDBView();

    private:
};

#endif // CINDBFACTORY_H
