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
    /*
     * intentional errors
     */
    // common
    void testCaseInvalidSig();
    void testCaseShortSig();
    void testCaseShortVer();
    // version 0
    void testCaseVer0ShortSize();
    void testCaseVer0InvalidAns();
    void testCaseVer0InvalidDown();
    void testCaseVer0InvalidRight();
    void testCaseVer0InvalidType();
    // version 1
    void testCaseVer1_2x2();
    void testCaseVer1InvalidAns();
    void testCaseVer1InvalidRight();
    void testCaseVer1InvalidType();

    /*
     * normal cases
     */
    // version 0
    void testCaseVer0Small();
};

ProblemLoaderTest::ProblemLoaderTest()
    : m_dataPath(SRCDIR "data/")
{
}

void ProblemLoaderTest::testCaseInvalidSig()
{
    const QString dataFileName{m_dataPath + "invalidSig.kkr"};
    std::unique_ptr<pd::ProblemData> pData{pd::ProblemData::problemLoader(dataFileName)};
    QVERIFY2(pData.get() == nullptr, "Invalid signature of problem data");
}

void ProblemLoaderTest::testCaseShortSig()
{
    const QString dataFileName{m_dataPath + "shortSig.kkr"};
    std::unique_ptr<pd::ProblemData> pData{pd::ProblemData::problemLoader(dataFileName)};
    QVERIFY2(pData.get() == nullptr, "Data ends before signature ends");
}

void ProblemLoaderTest::testCaseShortVer()
{
    const QString dataFileName{m_dataPath + "shortVer.kkr"};
    std::unique_ptr<pd::ProblemData> pData{pd::ProblemData::problemLoader(dataFileName)};
    QVERIFY2(pData.get() == nullptr, "Data ends before version number ends");
}

void ProblemLoaderTest::testCaseVer0ShortSize()
{
    const QString dataFileName{m_dataPath + "ver0ShortSize.kkr"};
    std::unique_ptr<pd::ProblemData> pData{pd::ProblemData::problemLoader(dataFileName)};
    QVERIFY2(pData.get() == nullptr, "Data ends before version number ends");
}

void ProblemLoaderTest::testCaseVer0InvalidAns()
{
    const QString dataFileName{m_dataPath + "ver0InvalidAns.kkr"};
    std::unique_ptr<pd::ProblemData> pData{pd::ProblemData::problemLoader(dataFileName)};
    QVERIFY2(pData.get() == nullptr, "Incorrect answer value");
}

void ProblemLoaderTest::testCaseVer0InvalidDown()
{
    const QString dataFileName{m_dataPath + "ver0InvalidDown.kkr"};
    std::unique_ptr<pd::ProblemData> pData{pd::ProblemData::problemLoader(dataFileName)};
    QVERIFY2(pData.get() == nullptr, "Incorrect clue down value");
}

void ProblemLoaderTest::testCaseVer0InvalidRight()
{
    const QString dataFileName{m_dataPath + "ver0InvalidRight.kkr"};
    std::unique_ptr<pd::ProblemData> pData{pd::ProblemData::problemLoader(dataFileName)};
    QVERIFY2(pData.get() == nullptr, "Incorrect clue right value");
}

void ProblemLoaderTest::testCaseVer0InvalidType()
{

    const QString dataFileName{m_dataPath + "ver0InvalidType.kkr"};
    std::unique_ptr<pd::ProblemData> pData{pd::ProblemData::problemLoader(dataFileName)};
    QVERIFY2(pData.get() == nullptr, "Incorrect cell type");
}

void ProblemLoaderTest::testCaseVer0Small()
{
    const QString dataFileName{m_dataPath + "ver0Small.kkr"};
    std::unique_ptr<pd::ProblemData> pData{pd::ProblemData::problemLoader(dataFileName)};

    QVERIFY2(pData.get() != nullptr, "problemLoader should return something");

    // size
    QCOMPARE(pData->getNumCols(), 3);
    QCOMPARE(pData->getNumRows(), 3);

    // cells
    QCOMPARE(pData->getCellType(0,0), pd::CellType::CellClue);
    QCOMPARE(pData->getClueRight(0,0), pd::CLOSED_CLUE);
    QCOMPARE(pData->getClueDown(0,0), pd::CLOSED_CLUE);

    QCOMPARE(pData->getCellType(1,0), pd::CellType::CellClue);
    QCOMPARE(pData->getClueRight(1,0), pd::CLOSED_CLUE);
    QCOMPARE(pData->getClueDown(1,0), 4);

    QCOMPARE(pData->getCellType(2,0), pd::CellType::CellClue);
    QCOMPARE(pData->getClueRight(2,0), pd::CLOSED_CLUE);
    QCOMPARE(pData->getClueDown(2,0), 7);

    QCOMPARE(pData->getCellType(0,1), pd::CellType::CellClue);
    QCOMPARE(pData->getClueRight(0,1), 3);
    QCOMPARE(pData->getClueDown(0,1), pd::CLOSED_CLUE);

    QCOMPARE(pData->getCellType(1,1), pd::CellType::CellAnswer);
    QCOMPARE(pData->getAnswer(1,1), 1);

    QCOMPARE(pData->getCellType(2,1), pd::CellType::CellAnswer);
    QCOMPARE(pData->getAnswer(2,1), 2);

    QCOMPARE(pData->getCellType(0,2), pd::CellType::CellClue);
    QCOMPARE(pData->getClueRight(0,2), 8);
    QCOMPARE(pData->getClueDown(0,2), pd::CLOSED_CLUE);

    QCOMPARE(pData->getCellType(1,2), pd::CellType::CellAnswer);
    QCOMPARE(pData->getAnswer(1,2), 3);

    QCOMPARE(pData->getCellType(2,2), pd::CellType::CellAnswer);
    QCOMPARE(pData->getAnswer(2,2), 5);
}

void ProblemLoaderTest::testCaseVer1_2x2()
{
    const QString dataFileName{m_dataPath + "ver1_2x2.kkr"};
    std::unique_ptr<pd::ProblemData> pData{pd::ProblemData::problemLoader(dataFileName)};

    QVERIFY2(pData.get() != nullptr, "problemLoader should return something");

    // size
    QCOMPARE(pData->getNumCols(), 3);
    QCOMPARE(pData->getNumRows(), 3);

    // cells
    QCOMPARE(pData->getCellType(0,0), pd::CellType::CellClue);
    QCOMPARE(pData->getClueRight(0,0), pd::CLOSED_CLUE);
    QCOMPARE(pData->getClueDown(0,0), pd::CLOSED_CLUE);

    QCOMPARE(pData->getCellType(1,0), pd::CellType::CellClue);
    QCOMPARE(pData->getClueRight(1,0), pd::CLOSED_CLUE);
    QCOMPARE(pData->getClueDown(1,0), 3);

    QCOMPARE(pData->getCellType(2,0), pd::CellType::CellClue);
    QCOMPARE(pData->getClueRight(2,0), pd::CLOSED_CLUE);
    QCOMPARE(pData->getClueDown(2,0), 12);

    QCOMPARE(pData->getCellType(0,1), pd::CellType::CellClue);
    QCOMPARE(pData->getClueRight(0,1), 4);
    QCOMPARE(pData->getClueDown(0,1), pd::CLOSED_CLUE);

    QCOMPARE(pData->getCellType(1,1), pd::CellType::CellAnswer);
    QCOMPARE(pData->getAnswer(1,1), 1);

    QCOMPARE(pData->getCellType(2,1), pd::CellType::CellAnswer);
    QCOMPARE(pData->getAnswer(2,1), 3);

    QCOMPARE(pData->getCellType(0,2), pd::CellType::CellClue);
    QCOMPARE(pData->getClueRight(0,2), 11);
    QCOMPARE(pData->getClueDown(0,2), pd::CLOSED_CLUE);

    QCOMPARE(pData->getCellType(1,2), pd::CellType::CellAnswer);
    QCOMPARE(pData->getAnswer(1,2), 2);

    QCOMPARE(pData->getCellType(2,2), pd::CellType::CellAnswer);
    QCOMPARE(pData->getAnswer(2,2), 9);
}

void ProblemLoaderTest::testCaseVer1InvalidAns()
{
    const QString dataFileName{m_dataPath + "ver1InvalidAns.kkr"};
    std::unique_ptr<pd::ProblemData> pData{pd::ProblemData::problemLoader(dataFileName)};
    QVERIFY2(pData.get() == nullptr, "Incorrect answer value");
}

void ProblemLoaderTest::testCaseVer1InvalidRight()
{
    const QString dataFileName{m_dataPath + "ver1InvalidRight.kkr"};
    std::unique_ptr<pd::ProblemData> pData{pd::ProblemData::problemLoader(dataFileName)};
    QVERIFY2(pData.get() == nullptr, "Incorrect clue right value");
}

void ProblemLoaderTest::testCaseVer1InvalidType()
{
    const QString dataFileName{m_dataPath + "ver1InvalidType.kkr"};
    std::unique_ptr<pd::ProblemData> pData{pd::ProblemData::problemLoader(dataFileName)};
    QVERIFY2(pData.get() == nullptr, "Incorrect cell type");
}

QTEST_APPLESS_MAIN(ProblemLoaderTest)

#include "tst_problemloadertest.moc"
