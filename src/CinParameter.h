#ifndef CINPARAMETER_H
#define CINPARAMETER_H

#include <QObject>
#include <QString>
#include <vector>

//! Manages a parameter for an application
/*!
*/
class CinParameter : public QObject
{
    Q_OBJECT 

    public:
        enum Type{UNDEFINED=0, STRING, FLOAT, INT, NUMTYPES};
        static const char *TypeNames[NUMTYPES];
        static const float NO_VALUE;
        static const float NO_PREV;
        static const float NO_NEXT;
        static const float NOT_SET;
        static const char *GetNameForType(CinParameter::Type type);

        // constructor
        CinParameter(const QString &name, CinParameter::Type type=CinParameter::UNDEFINED);

        // member function
        const QString &getName() const {return mName;}
        CinParameter::Type getType() const {return mType;}

        // float getMin() const {return mMin;}
        // float getMax() const {return mMax;}
            // typeless operations
        void  getValueAsString(QString &value);
        bool  getValueAsString(QString &value, int i);
        int   getCurID() {return mCurID;}
        int   getLastID() {return mValues.size() - 1;}
        int   getNumValues() {return mValues.size();}
        void  incrementValue();
        void  decrementValue();

        void  setName(const QString &name)     {mName = name;}
        void  setType(CinParameter::Type type) {mType = type;}
        bool  setToValueAt(int id);

        // type-specific functions
        void  recordValue(float value);
        void  setMin(float min)                {mMin  = min;}
        void  setMax(float max)                {mMax  = max;}
        bool  setValue(float value);


        void  print();
        void  finalizeValues();

    signals:
        void valueChanged(const QString &value, int valueID);

    private:
        float getValue() const {return mCurValue;}
        bool  valueAt(float &value, int valueID);
        bool  isValidID(int valueID);
            // type-specific functions
        int   getIDForValue(float value);
        bool  valueExists(float value);

        // member variables
        QString mName;
        CinParameter::Type mType=CinParameter::UNDEFINED;
        float   mMin;
        float   mMax;
        float   mCurValue;
        int     mCurID;
        std::vector<float> mValues;
};

#endif // CINPARAMETER_H
