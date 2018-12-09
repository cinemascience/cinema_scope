#include <QtTest>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>

// add necessary includes here
#include "CinParameter.h"
#include "CinDBReader.h"
#include "CinParameterMap.h"
#include "CinDatabase.h"
#include "CinDBView.h"
#include <vector>

//! CinemaScope Unit Testing Class
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
    void parameterMap();
    void cinDatabase();
    void cinDBView();
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
    QString dbPath = "../unittesting/simple_load.cdb";
    QString table  = "cinema";
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    QString connection = database.connectionName(); 
    database.open();

    int result = reader.load(database, dbPath, table);
    QVERIFY(result == CinDBReader::DatabaseLoaded);
    QVERIFY(reader.hasCSVFile() == true);
    QVERIFY(reader.hasSettingsFile() == true);

    // clean up
    database = QSqlDatabase();
    database.removeDatabase(connection);
}


void CinScopeTest::parameterMap()
{
    CinParameterMap map;
    QString phiString, thetaString, failString;

    // check initial
    QVERIFY(not map.getInput("phi", phiString));
    QVERIFY(not map.getInput("theta", thetaString));
    QVERIFY(not map.getInput("fail", failString));

    // insert some data
    map.map("zhi", "phi");
    map.map("zheta", "theta");

    // recheck 
    QVERIFY(map.getInput("phi", phiString));
    QVERIFY(phiString == "zhi");
    QVERIFY(map.getInput("theta", thetaString));
    QVERIFY(thetaString == "zheta");
    QVERIFY(not map.getInput("fail", failString));
}

void CinScopeTest::cinDatabase()
{
    CinDatabase db;

    db.load("../unittesting/simple_load.cdb");
    QStringList parameters01 = {"phi", "theta", "xValue", "yValue", "zValue"};
    QStringList artifacts01  = {"FILE"};

    QVERIFY(db.getParameterColumnNames() == parameters01);
    QVERIFY(db.getArtifactColumnNames() == artifacts01);

    db.reset();
    db.load("../unittesting/test_parameter.cdb");
    QStringList parameters02 = {"zhi", "zheta"};
    QStringList artifacts02  = {"FILE", "FILE_cylinder"};

    QVERIFY(db.getParameterColumnNames() == parameters02);
    QVERIFY(db.getArtifactColumnNames() == artifacts02);
}

void CinScopeTest::cinDBView()
{
    CinDBView view;
    CinDatabase db;

    db.load("../unittesting/simple_load.cdb");
        // TODO decide if views should have pointers or references
    view.setDatabase(&db);
    view.initializeAttributes();

    // check the contents of the database
    QVERIFY(view.parameterExists("phi"));
    QVERIFY(view.parameterExists("theta"));
    QVERIFY(view.artifactExists("FILE"));

    db.reset();
    db.load("../unittesting/test_parameter.cdb");
    view.initializeAttributes();

    // check the contents of the database
    QVERIFY(view.parameterExists("zhi"));
    QVERIFY(view.parameterExists("zheta"));
    QVERIFY(view.artifactExists("FILE"));
    QVERIFY(view.artifactExists("FILE_cylinder"));
        // it should no longer include these values
    QVERIFY(not view.parameterExists("phi"));
    QVERIFY(not view.parameterExists("theta"));
}



QTEST_MAIN(CinScopeTest)

// #include "tst_cinscopetest.moc"
#include "CinScopeTest.moc"
