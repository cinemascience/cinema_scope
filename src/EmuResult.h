#ifndef EMURESULT_H 
#define EMURESULT_H 

#include <QString>
#include <QVector>

//! A class that manages emu data for the application 
/*!
*/
class EmuResult
{
    public:
        EmuResult();
        ~EmuResult();

        void setName(const QString &name) {mName = name;}
        void setNotes(const QString &notes) {mNotes = notes;}
        void setResults(const QVector<double> &results) {mResults = results;}
        void setInputs(const QVector<double> &inputs) {mInputs = inputs;}

        const QString &getName()    {return mName;}
        const QString &getNotes()   {return mNotes;}
        const QVector<double> &getInputs()  {return mInputs;}
        const QVector<double> &getResults() {return mResults;}

    private:
        QString mName;
        QString mNotes;
        QVector<double> mInputs;
        QVector<double> mResults;
};

#endif // EMURESULT_H
