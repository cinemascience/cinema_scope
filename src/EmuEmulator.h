#ifndef EMU_H 
#define EMU_H 

#include <QVector>

//! A class that manages emu computation 
/*!
*/
class EmuEmulator
{
    public:
        EmuEmulator();
        ~EmuEmulator();

        void emulate(const QVector<double> &inputs);
        QVector<double> &getResults() {return mResults;}

    private:
        double *mYStar=0;
        QVector<double> mResults;
};

#endif // EMU_H
