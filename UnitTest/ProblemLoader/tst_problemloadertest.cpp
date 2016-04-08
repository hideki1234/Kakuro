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
     * invalid header
     */
    void testCaseInvalidSig();
    void testCaseShortSig();
    void testCaseShortVer();
    /*
     * version 0
     */
    // error cases
    void testCaseVer0ShortSize();
    void testCaseVer0InvalidAns();
    void testCaseVer0InvalidDown();
    void testCaseVer0InvalidRight();
    void testCaseVer0InvalidType();
    // normal cases
    void testCaseVer0Small();
    void testCaseVer0_9x3();
    /*
     * version 1
     */
    // error cases
    void testCaseVer1InvalidAns();
    void testCaseVer1InvalidRight();
    void testCaseVer1InvalidType();
    // normal cases
    void testCaseVer1_2x2();
    void testCaseVer1_9x3();
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

void ProblemLoaderTest::testCaseVer0_9x3()
{
    const QString dataFileName{m_dataPath + "ver0_9x3.kkr"};
    std::unique_ptr<pd::ProblemData> pData{pd::ProblemData::problemLoader(dataFileName)};

    QVERIFY2(pData.get() != nullptr, "problemLoader should return something");

    // size
    QCOMPARE(pData->getNumCols(), 10);
    QCOMPARE(pData->getNumRows(), 4);

    // cells
    int x, y;
    x = 0; y = 0;
    QCOMPARE(pData->getCellType(x,y), pd::CellType::CellClue);
    QCOMPARE(pData->getClueRight(x,y), pd::CLOSED_CLUE);
    QCOMPARE(pData->getClueDown(x,y), pd::CLOSED_CLUE);

    x = 3; y = 1;
    QCOMPARE(pData->getCellType(x,y), pd::CellType::CellClue);
    QCOMPARE(pData->getClueRight(x,y), 4);
    QCOMPARE(pData->getClueDown(x,y), 4);

    x = 8; y = 0;
    QCOMPARE(pData->getCellType(x,y), pd::CellType::CellClue);
    QCOMPARE(pData->getClueRight(x,y), pd::CLOSED_CLUE);
    QCOMPARE(pData->getClueDown(x,y), 21);

    x = 7; y = 3;
    QCOMPARE(pData->getCellType(x,y), pd::CellType::CellClue);
    QCOMPARE(pData->getClueRight(x,y), 9);
    QCOMPARE(pData->getClueDown(x,y), pd::CLOSED_CLUE);

    x = 1; y = 2;
    QCOMPARE(pData->getCellType(x,y), pd::CellType::CellAnswer);
    QCOMPARE(pData->getAnswer(x,y), 8);
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

void ProblemLoaderTest::testCaseVer1_9x3()
{
    const QString dataFileName{m_dataPath + "ver1_9x3.kkr"};
    std::unique_ptr<pd::ProblemData> pData{pd::ProblemData::problemLoader(dataFileName)};

    QVERIFY2(pData.get() != nullptr, "problemLoader should return something");

    // size
    QCOMPARE(pData->getNumCols(), 10);
    QCOMPARE(pData->getNumRows(), 4);

    // cells
    int x, y;
    x = 0; y = 0;
    QCOMPARE(pData->getCellType(x,y), pd::CellType::CellClue);
    QCOMPARE(pData->getClueRight(x,y), pd::CLOSED_CLUE);
    QCOMPARE(pData->getClueDown(x,y), pd::CLOSED_CLUE);

    x = 6; y = 1;
    QCOMPARE(pData->getCellType(x,y), pd::CellType::CellClue);
    QCOMPARE(pData->getClueRight(x,y), 10);
    QCOMPARE(pData->getClueDown(x,y), 17);

    x = 5; y = 0;
    QCOMPARE(pData->getCellType(x,y), pd::CellType::CellClue);
    QCOMPARE(pData->getClueRight(x,y), pd::CLOSED_CLUE);
    QCOMPARE(pData->getClueDown(x,y), 6);

    x = 4; y = 3;
    QCOMPARE(pData->getCellType(x,y), pd::CellType::CellClue);
    QCOMPARE(pData->getClueRight(x,y), 9);
    QCOMPARE(pData->getClueDown(x,y), pd::CLOSED_CLUE);

    x = 9; y = 2;
    QCOMPARE(pData->getCellType(x,y), pd::CellType::CellAnswer);
    QCOMPARE(pData->getAnswer(x,y), 4);
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
