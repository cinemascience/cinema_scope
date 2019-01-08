#include "CinParameterMapDialog.h"
#include "CinDBView.h"
#include "CinImageView.h"
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QDebug>

CinParameterMapDialog::CinParameterMapDialog(QWidget *parent)
{
}

void CinParameterMapDialog::connect(CinDBView *view, CinImageView *imageView)
{
    mView = view;
    mImageView = imageView;

    this->setModal(true);

    QDialogButtonBox *buttonBox = new QDialogButtonBox();
    QPushButton *cancel = new QPushButton();

    buttonBox->addButton(QDialogButtonBox::Ok);
    buttonBox->addButton(QDialogButtonBox::Cancel);
    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(onReject()));

    QLabel *message = new QLabel("CinemaScope hasn't found parameters<br> it expected to automatically control the following events. <br><br>Select the parameters you'd like to use instead.");
    QGridLayout *grid   = new QGridLayout();
    QFormLayout *layout = new QFormLayout();
    grid->addWidget(message);
    grid->addLayout(layout, 1, 0);

    mVert = new QComboBox();
    mHor  = new QComboBox();
    mArtifact = new QComboBox();
    const QStringList &p = mView->getDatabase()->getParameterColumnNames();
    for (int i=0;i<p.count();i++) {
        mVert->addItem(p.at(i));
        mHor->addItem(p.at(i));
    }
    mHor->setCurrentText(mImageView->getHorizontalParameter());
    mVert->setCurrentText(mImageView->getVerticalParameter());

    const QStringList &a = mView->getDatabase()->getArtifactColumnNames();
    if (a.count() == 0)
    {
        // use the parameters
        for (int i=0;i<p.count();i++) {
            mArtifact->addItem(p.at(i));
        }
    } else {
        // use the artifacts
        for (int i=0;i<a.count();i++) {
            mArtifact->addItem(a.at(i));
        }
    }
    mArtifact->setCurrentText(mView->getArtifact());

    layout->addRow("Mouse Vertical", mVert);
    layout->addRow("Mouse Horizontal", mHor);
    layout->addRow("Artifact", mArtifact);

    layout->addRow("", buttonBox);

    this->setLayout(grid);
    this->resize(400, 200);
}

void CinParameterMapDialog::onAccept()
{
    mView->setArtifact(mArtifact->currentText());
    mImageView->setHorizontalParameter(mHor->currentText());
    mImageView->setVerticalParameter(mVert->currentText());

    done(QDialog::Accepted);
}

void CinParameterMapDialog::onReject()
{
    qDebug() << "REJECT";

    done(QDialog::Rejected);
}

