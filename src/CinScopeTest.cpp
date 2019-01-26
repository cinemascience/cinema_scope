#include <QtTest>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>

// add necessary includes here
#include "CinParameter.h"
#include "CinDBReader.h"
#include "CinDatabase.h"
#include "CinDBView.h"
#include "CinParameterVariant.h"
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
    void cinDatabase();
    void cinDBView();
    void rawDatabase();
    void cinParameterVariant();
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
    param.finalizeValues();

    float fResult = 0.0;
    bool result = false;

    // increment/decrement
    // param.setToValueAt(0);
    // QVERIFY(param.getValue() == 1.0);
    // param.incrementValue();
    // QVERIFY(param.getValue() == 2.0);
    // param.incrementValue();
    // QVERIFY(param.getValue() == 3.0);
    // param.decrementValue();
    // QVERIFY(param.getValue() == 2.0);
        // end wrap case
    param.setToValueAt(0);
    param.decrementValue();
    // QVERIFY(param.getValue() == 4.0);
        // end wrap case
    QVERIFY(param.getLastID() == 3);
    param.setToValueAt(param.getLastID());
    param.incrementValue();
    // QVERIFY(param.getValue() == 1.0);
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

    QSqlQuery query(db.getDatabase());
    QVERIFY(db.getParameterColumnNames() == parameters02);
    QVERIFY(db.getArtifactColumnNames() == artifacts02);

    // does this work for a database with a spaces in column names? 
    db.reset();
    db.load("../unittesting/test_spacename.cdb");
    QStringList parameters03 = {"phi", "t heta"};
    QStringList artifacts03  = {"FILE"};
    QVERIFY(db.getParameterColumnNames() == parameters03);
    QVERIFY(db.getArtifactColumnNames() == artifacts03);
/*
    qDebug() << "THIRD : " << db.getParameterColumnNames();
    QSqlRecord record = db.getDatabase().record(db.getTableName());
    for (int i=0;i<record.count();i++) 
    {
        qDebug() << "NAME: " << record.field(i).name();
    }

    query.exec("SELECT [t heta] FROM " + db.getTableName());
    qDebug() << "BEFORE TABLENAME: " << db.getTableName();
    while (query.next())
    {
        qDebug() << "VALUE: " << query.value(0).toFloat();
    }
    qDebug() << "AFTER  TABLENAME: " << db.getTableName();
*/

    db.reset();
    db.load("../unittesting/test_string.cdb");
    QStringList parameters04 = {"phi", "theta", "name"};
    QStringList artifacts04  = {"FILE"};
    QVERIFY(db.getParameterColumnNames() == parameters04);
    QVERIFY(db.getArtifactColumnNames() == artifacts04);
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

//
// example to test how database works with column names that have spaces
//
void CinScopeTest::rawDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "newconnect");
    db.open();

    QSqlQuery query(db);
    query.exec("CREATE TABLE init_table ([xValue] float, [y Value] float, [z  Value] float, [name] varchar[255])");
    query.exec("INSERT INTO init_table ([z  Value], [y Value], [xValue], [name]) VALUES (3.1, 2.1, 1.1, 'one')");
    query.exec("INSERT INTO init_table VALUES (1.2, 2.2, 3.2, 'two')");
    query.exec("INSERT INTO init_table VALUES (1.3, 2.3, 3.3, 'three')");

    query.exec("SELECT DISTINCT [xValue] FROM init_table");
    int i=0;
    QStringList x = {"1.1", "1.2", "1.3"};
    while (query.next())
    {
        QVERIFY(query.value(0) == x.at(i));
        i++;
    }

    query.exec("SELECT DISTINCT [y Value] FROM init_table");
    QStringList y = {"2.1", "2.2", "2.3"};
    i = 0;
    while (query.next())
    {
        QVERIFY(query.value(0) == y.at(i));
        i++;
    }

    query.exec("SELECT DISTINCT [z  Value] FROM init_table");
    QStringList z = {"3.1", "3.2", "3.3"};
    i = 0;
    while (query.next())
    {
        QVERIFY(query.value(0) == z.at(i));
        i++;
    }

    query.exec("SELECT [z  Value] FROM init_table WHERE [xValue]=1.1");
    while (query.next())
    {
        QVERIFY(query.value(0) == z.at(0));
    }

    query.exec("SELECT DISTINCT [name] FROM init_table");
    QStringList n = {"one", "two", "three"};
    i = 0;
    while (query.next())
    {
        QVERIFY(query.value(0) == n.at(i));
        i++;
    }
}

void CinScopeTest::cinParameterVariant()
{
    CinParameterVariant pVar("test", CinParameterVariant::INT);

    float fVal = 2.0;

    pVar.recordValue(1);
    pVar.recordValue(fVal);
    pVar.recordValue("3.0");

    QVERIFY(pVar.valueExists(1));
    QVERIFY(pVar.valueExists(9) == false);
    QVERIFY(pVar.valueExists(fVal));
    QVERIFY(pVar.valueExists((float)2.1) == false);
    QVERIFY(pVar.valueExists("3.0"));
    QVERIFY(pVar.valueExists("4.0") == false);

    QString value;
    pVar.getValueAsString(value, 2);
    QVERIFY(value == "3.0");
}



QTEST_MAIN(CinScopeTest)

// #include "tst_cinscopetest.moc"
#include "CinScopeTest.moc"
