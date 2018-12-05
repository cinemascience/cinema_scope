#include <QtTest>
#include <QCoreApplication>

// add necessary includes here
#include "CinParameter.h"
#include "CinDBReader.h"
#include <vector>

class CinScopeTest : public QObject
{
    Q_OBJECT

public:
    CinScopeTest();
    ~CinScopeTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void parameter();
    void databaseReader();
};


CinScopeTest::CinScopeTest()
{

}

CinScopeTest::~CinScopeTest()
{

}

void CinScopeTest::initTestCase()
{

}

void CinScopeTest::cleanupTestCase()
{

}

void CinScopeTest::parameter()
{
    // testing CinParameter class
    CinParameter param("test", CinParameter::FLOAT);
    param.recordValue(1.0);
    param.recordValue(4.0);
    param.recordValue(2.0);
    param.recordValue(3.0);
    param.sortValues();

    float fResult = 0.0;
    bool result = false;

    // previous
    result = param.getPrevValue(1.0, fResult);
    QVERIFY(result == false);

    result = param.getPrevValue(1.5, fResult);
    QVERIFY(result == true);
    QVERIFY(fResult == 1.0);

    result = param.getPrevValue(4.5, fResult);
    QVERIFY(result == true);
    QVERIFY(fResult == 4.0);

    // next
    result = param.getNextValue(4.0, fResult);
    QVERIFY(result == false);

    result = param.getNextValue(5.0, fResult);
    QVERIFY(result == false);

    result = param.getNextValue(0.0, fResult);
    QVERIFY(result == true);
    QVERIFY(fResult == 1.0);

    result = param.getNextValue(2.5, fResult);
    QVERIFY(result == true);
    QVERIFY(fResult == 3.0);
}

void CinScopeTest::databaseReader()
{
    // test of database reader to be inserted here
    CinDBReader reader;

    QVERIFY(true == true);
}

QTEST_MAIN(CinScopeTest)

// #include "tst_cinscopetest.moc"
#include "CinScopeTest.moc"
