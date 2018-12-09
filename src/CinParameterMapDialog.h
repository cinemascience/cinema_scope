#ifndef CINPARAMETERMAPDIALOG_H 
#define CINPARAMETERMAPDIALOG_H 

#include <QObject>
#include <QDialog>
#include "CinArtifactSet.h"
#include "CinParamSet.h"

class CinDBView;
class CinImageView;
class QComboBox;

//! Allows user to change the names of parameters mapped internally 
/*!
 *
*/
class CinParameterMapDialog : public QDialog
{
    Q_OBJECT

    public:
        // constructor
        CinParameterMapDialog(QWidget *parent);
        void connect(CinDBView *view, CinImageView *imageView);

    public slots:
        void onAccept();
        void onReject();

    private:
        CinDBView    *mView=NULL;
        CinImageView *mImageView=NULL;
        QComboBox    *mVert=NULL;
        QComboBox    *mHor=NULL;
        QComboBox    *mArtifact=NULL;
};

#endif // CINPARAMETERMAPDIALOG_H
