#ifndef EMU_H 
#define EMU_H 

#include <QVector>

//! A class that manages emu computation and metadata
/*!
*/
class EmuEmulator
{
    public:
        EmuEmulator();
        ~EmuEmulator();

        void emulate(const QVector<double> &inputs);
        QVector<double> &getResults() {return mResults;}

        int getNumInputs();
        int getInputDim();
        bool getInputMinMax(int ID, QVector<double> &values);

    private:
        bool isValidInputID(int ID);

        double *mYStar=0;
        QVector<double> mResults;
};

#endif // EMU_H
