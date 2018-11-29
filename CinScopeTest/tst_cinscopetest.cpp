#include <QtTest>
#include <QCoreApplication>

// add necessary includes here

class CinScopeTest : public QObject
{
    Q_OBJECT

public:
    CinScopeTest();
    ~CinScopeTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

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

void CinScopeTest::test_case1()
{

}

QTEST_MAIN(CinScopeTest)

#include "tst_cinscopetest.moc"
