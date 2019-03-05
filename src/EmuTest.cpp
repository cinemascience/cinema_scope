#include <QtTest>
#include <QCoreApplication>

// add necessary includes here
#include "EmuDatabase.h"
#include "EmuResult.h"

//! CinemaScope Unit Testing Class
class EmuTest : public QObject
{
    Q_OBJECT

public:
    EmuTest();
    ~EmuTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void database();
};


EmuTest::EmuTest()
{

}

EmuTest::~EmuTest()
{

}

void EmuTest::initTestCase()
{

}

void EmuTest::cleanupTestCase()
{

}

void EmuTest::database()
{
    EmuDatabase database;

    // QVERIFY(param.getLastID() == 3);
}

QTEST_MAIN(EmuTest)

#include "EmuTest.moc"
