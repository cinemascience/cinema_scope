#ifndef CINPARAMETER_H
#define CINPARAMETER_H

#include <QObject>
#include <QString>
#include <vector>

//! Manages a parameter 
/*!
 *  A set of parameters defines the input values for a Cinema Database
 *  This class manages communication about these parameters as they
 *  change in a Qt application.
*/
class CinParameter : public QObject
{
    Q_OBJECT 

    public:
        enum Type{UNDEFINED=0, STRING, FLOAT, INT, NUMTYPES};
        static const char *TypeNames[NUMTYPES];
        static const float NO_PREV;
        static const float NO_NEXT;
        static const float NOT_SET;
        static const char *GetNameForType(CinParameter::Type type);

        // constructor
        CinParameter(const QString &name, CinParameter::Type type=CinParameter::UNDEFINED, 
                        float min=CinParameter::NOT_SET, float max=CinParameter::NOT_SET, 
                        float cur=CinParameter::NOT_SET);

        // member function
        bool getNextValue(float value, float &next);
        bool getPrevValue(float value, float &prev);
        const QString &getName() const {return mName;}
        CinParameter::Type getType() const {return mType;}
        float getMin() const {return mMin;}
        float getMax() const {return mMax;}
        float getValue() const {return mValue;}

        void  setName(const QString &name)     {mName  = name;}
        void  setType(CinParameter::Type type) {mType  = type;}
        void  setMin(float min)                {mMin   = min;}
        void  setMax(float max)                {mMax   = max;}
        void  setValue(float value)            {mValue = value;}
        void  setToValueAt(int id);
        void  recordValue(float value);
        bool  valueExists(float value);

        int   getNumValues();
        // TODO check i for validity
        float valueAt(int i) { return mValues[i]; }
        bool valueAsString(QString &value, int i);

        void  print();
        void  sortValues();

    signals:
        void valueChanged(float value, int valueID);
        void valueChanged(const QString &name, float value);

    private:
        // member variables
        QString mName;
        CinParameter::Type mType=CinParameter::UNDEFINED;
        float   mMin;
        float   mMax;
        float   mValue;
        std::vector<float> mValues;
};

#endif // CINPARAMETER_H
