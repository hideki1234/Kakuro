#include <QString>
#include <QtTest>
#include <QSignalSpy>
#include "../../Editor/metadatamanager.h"

/*
 * MetaView mock
 */
class MetaView {
    MetaDataManager * m_p;
public:
    explicit MetaView(MetaDataManager *p) : m_p(p) {}

    void setAuthor(const QString &author) {m_p->setAuthor(author);}
    void setBeginnerTime(int t) {m_p->setBeginnerTime(t);}
    void setIntermediateTime(int t) {m_p->setIntermediateTime(t);}
    void setAdvancedTime(int t) {m_p->setAdvancedTime(t);}
    void setExpertTime(int t) {m_p->setExpertTime(t);}
};

class MetaDataTest : public QObject
{
    Q_OBJECT

public:
    MetaDataTest();

private Q_SLOTS:
    void testCaseCreate();
    void testCaseRead();

    void testCaseInvalid_data();
    void testCaseInvalid();

    void testCaseValid_data();
    void testCaseValid();
};

MetaDataTest::MetaDataTest()
{
}

void MetaDataTest::testCaseCreate()
{
    MetaDataManager mgr;
    QSignalSpy spy(&mgr, &MetaDataManager::sigReset);

    QCOMPARE(spy.count(), 0);

    mgr.slCreate();

    QCOMPARE(spy.count(), 1);
    QCOMPARE(mgr.getAuthor(), "");
    QCOMPARE(mgr.getBeginnerTime(), 0);
    QCOMPARE(mgr.getIntermediateTime(), 0);
    QCOMPARE(mgr.getAdvancedTime(), 0);
    QCOMPARE(mgr.getExpertTime(), 0);
    QCOMPARE(mgr.isValid(), true);

    std::unique_ptr<MetaData> md{mgr.getMetaData()};
    QCOMPARE(md->getAuthor(), "");
    QCOMPARE(md->getBeginnerTime(), 0);
    QCOMPARE(md->getIntermediateTime(), 0);
    QCOMPARE(md->getAdvancedTime(), 0);
    QCOMPARE(md->getExpertTime(), 0);
}

void MetaDataTest::testCaseRead()
{
    MetaDataManager mgr;
    QSignalSpy spy(&mgr, &MetaDataManager::sigReset);

    QCOMPARE(spy.count(), 0);

    const QString au{"A. Name"};
    const int begin = 10000;
    const int inter = 1000;
    const int advan = 100;
    const int expert = 10;
    MetaData md_org{au, begin, inter, advan, expert};
    mgr.slRead(md_org);

    QCOMPARE(spy.count(), 1);
    QCOMPARE(mgr.getAuthor(), au);
    QCOMPARE(mgr.getBeginnerTime(), begin);
    QCOMPARE(mgr.getIntermediateTime(), inter);
    QCOMPARE(mgr.getAdvancedTime(), advan);
    QCOMPARE(mgr.getExpertTime(), expert);
    QCOMPARE(mgr.isValid(), true);

    std::unique_ptr<MetaData> md{mgr.getMetaData()};
    QCOMPARE(md->getAuthor(), au);
    QCOMPARE(md->getBeginnerTime(), begin);
    QCOMPARE(md->getIntermediateTime(), inter);
    QCOMPARE(md->getAdvancedTime(), advan);
    QCOMPARE(md->getExpertTime(), expert);
}

void MetaDataTest::testCaseInvalid_data()
{
    QTest::addColumn<QString>("author");
    QTest::addColumn<int>("beginner");
    QTest::addColumn<int>("intermediate");
    QTest::addColumn<int>("advanced");
    QTest::addColumn<int>("expert");

    QTest::newRow("large intermediate") << "N. Intermediate" << 100 << 1000 << 50 << 10;
    QTest::newRow("large advanced") << "N. Advanced" << 1000 << 100 << 200 << 10;
    QTest::newRow("large expert") << "L. Expert" << 1000 << 100 << 50 << 80;
    QTest::newRow("negative expert") << "E. Negative" << 1000 << 500 << 0 << -1;
}

void MetaDataTest::testCaseInvalid()
{
    MetaDataManager mgr;
    MetaView mv(&mgr);

    QFETCH(QString, author);
    QFETCH(int, beginner);
    QFETCH(int, intermediate);
    QFETCH(int, advanced);
    QFETCH(int, expert);

    mv.setAuthor(author);
    mv.setBeginnerTime(beginner);
    mv.setIntermediateTime(intermediate);
    mv.setAdvancedTime(advanced);
    mv.setExpertTime(expert);

    QCOMPARE(mgr.isValid(), false);

    std::unique_ptr<MetaData> pMd{mgr.getMetaData()};
    QCOMPARE(pMd, nullptr);
 }

void MetaDataTest::testCaseValid_data()
{
    QTest::addColumn<QString>("author");
    QTest::addColumn<int>("beginner");
    QTest::addColumn<int>("intermediate");
    QTest::addColumn<int>("advanced");
    QTest::addColumn<int>("expert");

    QTest::newRow("No author") << "" << 1000 << 100 << 50 << 10;
    QTest::newRow("No time") << "N. Time" << 0 << 0 << 0 << 0;
    QTest::newRow("No intermediate") << "N. Intermediate" << 1000 << 0 << 0 << 0;
    QTest::newRow("No advanced") << "N. Advanced" << 1000 << 500 << 0 << 0;
    QTest::newRow("No expert") << "N. Expert" << 1000 << 500 << 100 << 0;
}

void MetaDataTest::testCaseValid()
{
    MetaDataManager mgr;
    MetaView mv(&mgr);

    QFETCH(QString, author);
    QFETCH(int, beginner);
    QFETCH(int, intermediate);
    QFETCH(int, advanced);
    QFETCH(int, expert);

    mv.setAuthor(author);
    mv.setBeginnerTime(beginner);
    mv.setIntermediateTime(intermediate);
    mv.setAdvancedTime(advanced);
    mv.setExpertTime(expert);

    QCOMPARE(mgr.isValid(), true);

    std::unique_ptr<MetaData> pMd{mgr.getMetaData()};
    QCOMPARE(pMd->getAuthor(), author);
    QCOMPARE(pMd->getBeginnerTime(), beginner);
    QCOMPARE(pMd->getIntermediateTime(), intermediate);
    QCOMPARE(pMd->getAdvancedTime(), advanced);
    QCOMPARE(pMd->getExpertTime(), expert);
}

 QTEST_APPLESS_MAIN(MetaDataTest)

#include "tst_metadatatest.moc"
