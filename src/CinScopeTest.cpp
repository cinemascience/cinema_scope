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
#include "CinParamSet.h"
// #include "CinParameterVariant.h"
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
    void cinParamSet();
    void rawDatabase();
    void cinParameter();
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
    // param.finalizeValues();

    // float fResult = 0.0;
    // bool result = false;

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

    // float data 
    db.reset();
    db.load("../unittesting/test_paramtypes.cdb");
    QStringList parameters05 = {"phi", "theta", "iphi", "itheta", "sphi", "stheta"};
    QStringList artifacts05  = {"FILE"};
    QVERIFY(db.getParameterColumnNames() == parameters05);
    QVERIFY(db.getArtifactColumnNames() == artifacts05);
    QSqlQuery query_02(db.getDatabase());
    query_02.exec("SELECT [FILE] FROM cinema WHERE [phi]='-180.0' AND [theta]='-90.1' AND [iphi]='180' AND [itheta]='-180' AND [sphi]='one' AND [stheta]='three'");
    while (query_02.next())
    {
        QVERIFY(query_02.value(0) == "image/-180.1/-90.1/180/-180/one/three/image.png");
    }
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

void CinScopeTest::cinParamSet()
{
    CinParamSet params;

    params.add("int", CinParameter::INT);
    params.add("float", CinParameter::FLOAT);
    params.add("string", CinParameter::STRING);

    QVERIFY(params.getNumParameters() == 3);
    QVERIFY(params.parameterExists("int"));
    QVERIFY(params.parameterExists("float"));
    QVERIFY(params.parameterExists("string"));
    QVERIFY(params.parameterExists("nothing") == false);

    CinParameter*param = NULL;
    QVERIFY(params.getParameter("int") != NULL);
    QVERIFY(params.getParameter("float") != NULL);
    QVERIFY(params.getParameter("string") != NULL);
    QVERIFY(params.getParameter("nothing") == NULL);

    param = params.getParameter("int");
    param->recordValue(1);
    param->recordValue(3);
    param->recordValue(5);
    QVERIFY(param->valueExists(0) == false);
    QVERIFY(param->valueExists(1));
    QVERIFY(param->valueExists(3));
    QVERIFY(param->valueExists(5));
    QVERIFY(param->valueExists(7) == false);

    param = params.getParameter("float");
    param->recordValue(1.0);
    param->recordValue(3.0);
    param->recordValue(5.0);
    QVERIFY(param->valueExists(0.1) == false);
    QVERIFY(param->valueExists(1.0));
    QVERIFY(param->valueExists(3.0));
    QVERIFY(param->valueExists(5.0));
    QVERIFY(param->valueExists(7.1) == false);

    param = params.getParameter("string");
    param->recordValue("first");
    param->recordValue("second");
    param->recordValue("third");
    QVERIFY(param->valueExists("zero") == false);
    QVERIFY(param->valueExists("first"));
    QVERIFY(param->valueExists("second"));
    QVERIFY(param->valueExists("third"));
    QVERIFY(param->valueExists("this is long") == false);
    QString value;
    // param->getValueAsString(value);
    // QVERIFY(value == "first");
    QVERIFY(param->setToValueAt(2));
    param->getValueAsString(value);
    QVERIFY(value == "third");
    // set and get the current value
    QVERIFY(param->setToValueAt(1));
    param->getValueAsString(value);
    QVERIFY(value == "second");
    // get a specific value
    param->getValueAsString(value, 0);
    QVERIFY(value == "first");

    // test out of bounds queries
    QVERIFY(param->getValueAsString(value, -1) == false);
    QVERIFY(param->getValueAsString(value, 10) == false);
    QVERIFY(param->setToValueAt(-1) == false);
    QVERIFY(param->setToValueAt(3) == false);

    // clear and test
    params.clear();
    QVERIFY(params.getNumParameters() == 0);
}

void CinScopeTest::cinParameter()
{
    CinParameter pVar("test", CinParameter::INT);

    pVar.recordValue(1);
    pVar.recordValue(2.0);
    pVar.recordValue("3.0");
    pVar.recordValue("this");

    QVERIFY(pVar.valueExists(1));
    QVERIFY(pVar.valueExists(9) == false);
    QVERIFY(pVar.valueExists(2.0));
    QVERIFY(pVar.valueExists(2.1) == false);
    QVERIFY(pVar.valueExists("3.0"));
    QVERIFY(pVar.valueExists("4.0") == false);
    QVERIFY(pVar.valueExists("this"));
    QVERIFY(pVar.valueExists("that") == false);

    // Null and NaN testing
    QVERIFY(pVar.valueExists(CinParameter::NULL_VALUE) == false);
    QVERIFY(pVar.valueExists(CinParameter::NAN_VALUE) == false);
    QVERIFY(pVar.setValue(CinParameter::NULL_VALUE) == false);
    QVERIFY(pVar.setValue(CinParameter::NAN_VALUE) == false);
        // now we create them
    pVar.recordValue(CinParameter::NULL_VALUE);
    pVar.recordValue(CinParameter::NAN_VALUE);
    QVERIFY(pVar.valueExists(CinParameter::NULL_VALUE));
    QVERIFY(pVar.valueExists(CinParameter::NAN_VALUE));
    QVERIFY(pVar.setValue(CinParameter::NULL_VALUE));
    QVERIFY(pVar.setValue(CinParameter::NAN_VALUE));

    // set and get
    QVERIFY(pVar.setValue(1));
    QVERIFY(pVar.setValue(15) == false);
    QVERIFY(pVar.setValue(2.0));
    QVERIFY(pVar.setValue(3.0) == false);
    QVERIFY(pVar.setValue("this"));
    QVERIFY(pVar.setValue("that") == false);

    QString value;
    pVar.getValueAsString(value, 2);
    QVERIFY(value == "3.0");

    pVar.print();
}



QTEST_MAIN(CinScopeTest)

// #include "tst_cinscopetest.moc"
#include "CinScopeTest.moc"
