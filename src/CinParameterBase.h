#ifndef CINPARAMETERBASE_H
#define CINPARAMETERBASE_H

#include <QObject>
#include <QString>
#include <vector>

//! Manages a parameter for an application
/*!
*/
class CinParameterBase : public QObject
{
    Q_OBJECT 

    public:
        enum Type{UNDEFINED=0, STRING, FLOAT, INT, NUMTYPES};
        static const char *TypeNames[NUMTYPES];
        static const float NO_VALUE;
        static const float NO_PREV;
        static const float NO_NEXT;
        static const float NOT_SET;
        static const char *GetNameForType(CinParameterBase::Type type);

        // constructor
        CinParameterBase(const QString &name, CinParameterBase::Type type=CinParameterBase::UNDEFINED);

        // member function
        const QString &getName() const {return mName;}
        CinParameterBase::Type getType() const {return mType;}

        // typeless operations
        virtual void  getValueAsString(QString &value) = 0;
        virtual bool  getValueAsString(QString &value, int i) = 0;
        virtual void  incrementValue() = 0;
        virtual void  decrementValue() = 0;
        virtual int   getLastID()      = 0;
        virtual int   getNumValues()   = 0;
                int   getCurID() {return mCurID;}

                void  setName(const QString &name) {mName = name;}
                void  setType(CinParameterBase::Type type) {mType = type;}
        virtual bool  setToValueAt(int id)     = 0;

        virtual void  print()                  = 0;
        virtual void  finalizeValues()         = 0;

        // type-specific functions
        virtual void  recordValue(float value) = 0;
        virtual void  setMin(float min)        = 0; 
        virtual void  setMax(float max)        = 0; 
        virtual bool  setValue(float value)    = 0;


    signals:
        void valueChanged(const QString &value, int valueID);

    private:
        virtual void getValue(float &value) const = 0;
        virtual bool  valueAt(float &value, int valueID) = 0;

        virtual bool  isValidID(int valueID) = 0;
            // type-specific functions
        virtual int   getIDForValue(float value) = 0;
        virtual bool  valueExists(float value) = 0;

        // member variables
        QString mName;
        CinParameterBase::Type mType=CinParameterBase::UNDEFINED;
        int     mCurID;
};

#endif // CINPARAMETERBASE_H
