#include "EmuEmulator.h"
#include "test.emu/params.h"
#include <time.h>
#include <QVector>

EmuEmulator::EmuEmulator()
{
    mYStar = (double *)(malloc(neta * sizeof(double)));
    mResults.resize(neta);

    for (int i=0;i<neta;i++)
    {
        mYStar[i] = 0.0;
    }

    emuInit();
}

EmuEmulator::~EmuEmulator()
{
    free(mYStar);
    mYStar = NULL;
}


void EmuEmulator::emulate(const QVector<double> &inputs)
{
    double xstar[p];
    // double ystar[neta];
    double ysd[neta];
    double wstar[peta];
    double wvarstar[peta];
    double *ysamp = (double *)(malloc(neta * samp * sizeof(double)));

    gsl_rng *r = gsl_rng_alloc(gsl_rng_taus2); // GSL's Taus generator
    gsl_rng_set(r, time(NULL)); // Initialize the GSL generator with time

    // inefficient, but safe
    // TODO: check size of inputs
    for (int i=0;i<p;i++)
    {
        xstar[i] = inputs[i];
    }

    emu(r, xstar, mYStar, ysd, wstar, wvarstar, ysamp); 

    for (int i=0;i<neta;i++)
    {
        mResults[i] = mYStar[i];
    }

    gsl_rng_free(r);
    free(ysamp);
}
