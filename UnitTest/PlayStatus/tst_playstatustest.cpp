#include <QString>
#include <QtTest>
#include <QSignalSpy>
#include <QThread>
#include "../../Kakuro/playstatus.h"

Q_DECLARE_METATYPE(playstatus::Status)
//Q_ENUMS(playstatus::Status)

namespace ps = playstatus;
namespace pd = problemdata;

class PlayStatusTest : public QObject
{
    Q_OBJECT

public:
    PlayStatusTest();

private Q_SLOTS:
    void initTestCase();
    void testStatusChanged();
    void testElapsedTime();
};

PlayStatusTest::PlayStatusTest()
{
}

void PlayStatusTest::initTestCase()
{
}

void PlayStatusTest::testStatusChanged()
{
    ps::PlayStatus target;
    qRegisterMetaType<playstatus::Status>("Status");
    QSignalSpy spy(&target, &ps::PlayStatus::statusChanged);

    QCOMPARE(spy.count(), 0);

    // nothing should happen without data
    target.playPressed();
    QCOMPARE(spy.count(), 0);

    // NODATA -> READY
    std::shared_ptr<pd::ProblemData> pMockPd{pd::ProblemData::problemLoader("dummy")};
    target.updateData(pMockPd);
    QCOMPARE(spy.count(), 1);

    // READY -> INPLAY
    target.playPressed();
    QCOMPARE(spy.count(), 2);

    // INPLAY -> PAUSED
    target.playPressed();
    QCOMPARE(spy.count(), 3);

    // PAUSED -> INPLAY
    target.playPressed();
    QCOMPARE(spy.count(), 4);

    // INPLAY -> DONE
    target.solved();
    QCOMPARE(spy.count(), 5);

    ps::Status s;
    s = qvariant_cast<ps::Status>(spy.at(0).at(0));
    QCOMPARE(s, ps::READY);
    s = qvariant_cast<ps::Status>(spy.at(1).at(0));
    QCOMPARE(s, ps::INPLAY);
    s = qvariant_cast<ps::Status>(spy.at(2).at(0));
    QCOMPARE(s, ps::PAUSED);
    s = qvariant_cast<ps::Status>(spy.at(3).at(0));
    QCOMPARE(s, ps::INPLAY);
    s = qvariant_cast<ps::Status>(spy.at(4).at(0));
    QCOMPARE(s, ps::DONE);
}

void PlayStatusTest::testElapsedTime()
{
    static const unsigned long WAIT_MS = 100ul;
    ps::PlayStatus target;

    qint64 s;

    // NODATA
    s = target.getElapsedTime();
    QThread::msleep(WAIT_MS);
    QCOMPARE(target.getElapsedTime(), s);

    // READY
    std::shared_ptr<pd::ProblemData> pMockPd{pd::ProblemData::problemLoader("dummy")};
    target.updateData(pMockPd);
    s = target.getElapsedTime();
    QThread::msleep(WAIT_MS);
    QCOMPARE(target.getElapsedTime(), s);

    // INPLAY
    target.playPressed();
    s = target.getElapsedTime();
    QThread::msleep(WAIT_MS);
    QVERIFY(target.getElapsedTime() > s);

    // PAUSED
    target.playPressed();
    s = target.getElapsedTime();
    QThread::msleep(WAIT_MS);
    QCOMPARE(target.getElapsedTime(), s);

    // INPLAY again
    target.playPressed();
    s = target.getElapsedTime();
    QThread::msleep(WAIT_MS);
    QVERIFY(target.getElapsedTime() > s);

    // DONE
    target.solved();
    s = target.getElapsedTime();
    QThread::msleep(WAIT_MS);
    QCOMPARE(target.getElapsedTime(), s);
}

QTEST_APPLESS_MAIN(PlayStatusTest)

#include "tst_playstatustest.moc"
