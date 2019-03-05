#include "EmuEmulator.h"
#include "test.emu/params.h"
#include <time.h>
#include <QVector>
#include <QDebug>

int EmuEmulator::getNumInputs()
{
    return emu::p;
}

int EmuEmulator::getInputDim()
{
    return emu::m;
}

bool EmuEmulator::isValidInputID(int ID)
{
    bool result = false;

    if ((ID >= 0) && (ID < getNumInputs()))
    {
        result = true;
    }

    return result;
}

bool EmuEmulator::getInputMinMax(int ID, QVector<double> &values)
{
    bool result = false;

    if (isValidInputID(ID))
    {
        result = true;
        values.resize(2);
        values[0] = emu::xmin[ID];
        values[1] = emu::xmax[ID];
    }

    return result;
}

EmuEmulator::EmuEmulator()
{
    mYStar = (double *)(malloc(emu::neta * sizeof(double)));
    mResults.resize(emu::neta);

    for (int i=0;i<emu::neta;i++)
    {
        mYStar[i] = 0.0;
    }

    emu::emuInit();
}

EmuEmulator::~EmuEmulator()
{
    free(mYStar);
    mYStar = NULL;
}


void EmuEmulator::emulate(const QVector<double> &inputs)
{
    double xstar[emu::p];
    // double ystar[neta];
    double ysd[emu::neta];
    double wstar[emu::peta];
    double wvarstar[emu::peta];
    double *ysamp = (double *)(malloc(emu::neta * emu::samp * sizeof(double)));

    gsl_rng *r = gsl_rng_alloc(gsl_rng_taus2); // GSL's Taus generator
    gsl_rng_set(r, time(NULL)); // Initialize the GSL generator with time

    // inefficient, but safe
    // TODO: check size of inputs
    for (int i=0;i<emu::p;i++)
    {
        xstar[i] = inputs[i];
    }

    emu::emu(r, xstar, mYStar, ysd, wstar, wvarstar, ysamp); 

    for (int i=0;i<emu::neta;i++)
    {
        mResults[i] = mYStar[i];
    }

    gsl_rng_free(r);
    free(ysamp);
}
