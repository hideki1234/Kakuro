#include <QString>
#include <QtTest>
#include "../../Editor/kkrboardmanager.h"

class EditorBoardTest : public QObject
{
    Q_OBJECT

public:
    EditorBoardTest();

private Q_SLOTS:
    void testCaseBoardData();
};

EditorBoardTest::EditorBoardTest()
{
}

void EditorBoardTest::testCaseBoardData()
{
    const int cols = 10;
    const int rows = 4;
    BoardData bd{cols, rows};

    // size
    QCOMPARE(bd.getNumCols(), cols);
    QCOMPARE(bd.getNumRows(), rows);

    // defalut cell types
    QCOMPARE(bd.getCellType(0, 0), CellType::CellClue);
    QCOMPARE(bd.getCellType(0, rows-1), CellType::CellClue);
    QCOMPARE(bd.getCellType(cols-1, 0), CellType::CellClue);

    QCOMPARE(bd.getCellType(cols-1, rows-1), CellType::CellAnswer);
    QCOMPARE(bd.getCellType(cols/2, rows/2), CellType::CellAnswer);

    // default cell clue
    QCOMPARE(bd.getClueDown(1,0), NO_CLUE);
    QCOMPARE(bd.getClueRight(1,0), NO_CLUE);
    QCOMPARE(bd.getClueDown(0,2), NO_CLUE);
    QCOMPARE(bd.getClueRight(0,2), NO_CLUE);

    // default cell value
    QCOMPARE(bd.getAnswer(1,1), NO_ANSWER);
    QCOMPARE(bd.getAnswer(5,3), NO_ANSWER);

    // set cell type
    bd.setCellType(1,1,CellType::CellClue);
    bd.setCellType(5,3,CellType::CellClue);
    bd.setClueRight(1,1,36);
    bd.setClueDown(1,1,7);
    bd.setClueRight(5,3,16);
    QCOMPARE(bd.getCellType(1,1), CellType::CellClue);
    QCOMPARE(bd.getCellType(5,3), CellType::CellClue);
    QCOMPARE(bd.getClueRight(1,1), 36);
    QCOMPARE(bd.getClueDown(1,1), 7);
    QCOMPARE(bd.getClueRight(5,3), 16);
    QCOMPARE(bd.getClueDown(5,3), NO_CLUE);

    // change cell type back to answer
    bd.setCellType(1,1,CellType::CellAnswer);
    QCOMPARE(bd.getCellType(1,1), CellType::CellAnswer);
    QCOMPARE(bd.getAnswer(1,1), NO_ANSWER);

    // set answer
    bd.setAnswer(1,1,8);
    bd.setAnswer(6,3,4);
    QCOMPARE(bd.getAnswer(1,1), 8);
    QCOMPARE(bd.getAnswer(6,3), 4);
}

QTEST_APPLESS_MAIN(EditorBoardTest)

#include "tst_editorboardtest.moc"
