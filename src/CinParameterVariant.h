#ifndef CINPARAMETERVARIANT_H
#define CINPARAMETERVARIANT_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QVector>

//! Manages a parameter for an application
/*!
*/
class CinParameterVariant : public QObject
{
    Q_OBJECT 

    public:
        enum Type{UNDEFINED=0, STRING, FLOAT, INT, NUMTYPES};
        static const char *TypeNames[NUMTYPES];
        static const char *NAN_VALUE; 
        static const char *NULL_VALUE; 
        static const float NO_VALUE;
        static const float NO_PREV;
        static const float NO_NEXT;
        static const float NOT_SET;
        static const char *GetNameForType(CinParameterVariant::Type type);

        // constructor
        CinParameterVariant(const QString &name, CinParameterVariant::Type type=CinParameterVariant::UNDEFINED);

        // member function
        const QString &getName() const {return mName;}
        CinParameterVariant::Type getType() const {return mType;}

        // typeless operations
        void  getValueAsString(QString &value);
        bool  getValueAsString(QString &value, int i);
        void  incrementValue();
        void  decrementValue();
        int   getLastID() {return mValues.size() - 1;}
        int   getNumValues() {return mValues.size();}
        int   getCurID() {return mCurID;}

        void  setName(const QString &name) {mName = name;}
        // void  setType(CinParameterVariant::Type type) {mType = type;}
        bool  setToValueAt(int id);

        void  print();
        // virtual void  finalizeValues()         = 0;

        // type-specific functions
        bool recordValue(int value);
        bool recordValue(double value);
        bool recordValue(const QString &value);
        bool setValue(int value);
        bool setValue(double value);
        bool setValue(const QString &value);
        bool valueExists(int value);
        bool valueExists(double value);
        bool valueExists(const QString &value);

    signals:
        void valueChanged(const QString &value, int valueID);

    private:
        int indexOf(int value);
        int indexOf(double value);
        int indexOf(const QString &value);

        virtual bool  isValidID(int valueID);
            // type-specific functions
        void postSetValue();

        // member variables
        QString mName;
        int     mCurID=0;
        CinParameterVariant::Type mType=CinParameterVariant::UNDEFINED;
        QVector<QVariant> mValues;

};

#endif // CINPARAMETERVARIANT_H
