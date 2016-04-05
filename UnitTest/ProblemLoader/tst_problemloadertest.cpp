#include <QString>
#include <QtTest>
#include <memory>
#include "../../Kakuro/problemdata.h"

namespace pd = problemdata;

class ProblemLoaderTest : public QObject
{
    Q_OBJECT
    const QString m_dataPath;

public:
    ProblemLoaderTest();

private Q_SLOTS:
    void testCaseSmall();
};

ProblemLoaderTest::ProblemLoaderTest()
    : m_dataPath(SRCDIR "data/")
{
}

void ProblemLoaderTest::testCaseSmall()
{
    const QString dataFileName{m_dataPath + "small.kkr"};
    std::unique_ptr<pd::ProblemData> pData{pd::ProblemData::problemLoader(dataFileName)};

    QVERIFY2(pData.get() != nullptr, "problemLoader should return something");
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(ProblemLoaderTest)

#include "tst_problemloadertest.moc"
