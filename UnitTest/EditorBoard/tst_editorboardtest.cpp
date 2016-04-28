#include <QString>
#include <QtTest>
#include "../../Editor/kkrboardmanager.h"
#include <QSignalSpy>

/*
 * mock
 */
class KkrBoardView {
    std::shared_ptr<KkrBoardManager> m_kbm;

public:
    explicit KkrBoardView(std::shared_ptr<KkrBoardManager> kbm);
    ~KkrBoardView();

    void setCellType(int c, int r, CellType ct) {m_kbm->setCellType(c,r,ct);}
    void setClueRight(int c, int r, int cl) {m_kbm->setClueRight(c,r,cl);}
    void setClueDown(int c, int r, int cl) {m_kbm->setClueDown(c,r,cl);}
    void setAnswer(int c, int r, int a) { m_kbm->setAnswer(c,r,a);}
};

KkrBoardView::KkrBoardView(std::shared_ptr<KkrBoardManager> kbm)
    : m_kbm(kbm)
{
}

KkrBoardView::~KkrBoardView()
{
}

/*
 * test class
 */
class EditorBoardTest : public QObject
{
    Q_OBJECT

public:
    EditorBoardTest();

private Q_SLOTS:
    void testCaseBoardData();
    void testCaseBoardDataCopy();
    void testCaseBoardDataMgrNew();
    void testCaseBoardDataMgrRead();
    void testCaseBoardDataMgrSet();
    void testCaseBoardDataMgrGet();
    void testCaseBoardDataMgrValid();
    void testCaseBoardDataMgrInvalidNoClue();
    void testCaseBoardDataMgrInvalidNoAnswer();
    void testCaseBoardDataMgrInvalidWrongSum();
};

EditorBoardTest::EditorBoardTest()
{
}

void EditorBoardTest::testCaseBoardData()
{
    constexpr int cols = 10;
    constexpr int rows = 4;
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
    QCOMPARE(bd.getClueDown(1,0), EMPTY_CLUE);
    QCOMPARE(bd.getClueRight(1,0), EMPTY_CLUE);
    QCOMPARE(bd.getClueDown(0,2), EMPTY_CLUE);
    QCOMPARE(bd.getClueRight(0,2), EMPTY_CLUE);

    // default cell value
    QCOMPARE(bd.getAnswer(1,1), EMPTY_ANSWER);
    QCOMPARE(bd.getAnswer(5,3), EMPTY_ANSWER);

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
    QCOMPARE(bd.getClueDown(5,3), EMPTY_CLUE);

    // change cell type back to answer
    bd.setCellType(1,1,CellType::CellAnswer);
    QCOMPARE(bd.getCellType(1,1), CellType::CellAnswer);
    QCOMPARE(bd.getAnswer(1,1), EMPTY_ANSWER);

    // set answer
    bd.setAnswer(1,1,8);
    bd.setAnswer(6,3,4);
    QCOMPARE(bd.getAnswer(1,1), 8);
    QCOMPARE(bd.getAnswer(6,3), 4);
}

void EditorBoardTest::testCaseBoardDataCopy()
{
    // set up source
    const int cols = 10;
    const int rows = 4;
    BoardData bd{cols, rows};
    bd.setCellType(1,1,CellType::CellClue);
    bd.setClueRight(1,1,30);
    bd.setClueDown(1,1,5);
    bd.setAnswer(2,2,5);

    BoardData bd2{bd};
    QCOMPARE(bd2.getCellType(1,1), CellType::CellClue);
    QCOMPARE(bd2.getClueRight(1,1), 30);
    QCOMPARE(bd2.getClueDown(1,1), 5);
    QCOMPARE(bd2.getAnswer(2,2), 5);
}

void EditorBoardTest::testCaseBoardDataMgrNew()
{
    KkrBoardManager kbm;
    QSignalSpy spy(&kbm, &KkrBoardManager::sigReset);
    QCOMPARE(spy.count(), 0);

    constexpr int cols = 4;
    constexpr int rows = 10;
    kbm.slCreate(cols, rows);
    QCOMPARE(spy.count(), 1);

    // size
    QCOMPARE(kbm.getNumCols(), cols);
    QCOMPARE(kbm.getNumRows(), rows);

    // defalut cell types
    QCOMPARE(kbm.getCellType(0, 0), CellType::CellClue);
    QCOMPARE(kbm.getCellType(0, rows-1), CellType::CellClue);
    QCOMPARE(kbm.getCellType(cols-1, 0), CellType::CellClue);

    QCOMPARE(kbm.getCellType(cols-1, rows-1), CellType::CellAnswer);
    QCOMPARE(kbm.getCellType(cols/2, rows/2), CellType::CellAnswer);

    // default cell clue
    QCOMPARE(kbm.getClueDown(0,0), CLOSED_CLUE);
    QCOMPARE(kbm.getClueRight(0,0), CLOSED_CLUE);
    QCOMPARE(kbm.getClueDown(1,0), EMPTY_CLUE);
    QCOMPARE(kbm.getClueRight(1,0), CLOSED_CLUE);
    QCOMPARE(kbm.getClueDown(0,2), CLOSED_CLUE);
    QCOMPARE(kbm.getClueRight(0,2), EMPTY_CLUE);

    // default cell value
    QCOMPARE(kbm.getAnswer(1,1), EMPTY_ANSWER);
    QCOMPARE(kbm.getAnswer(3,5), EMPTY_ANSWER);

    // reset
    constexpr int cols2 = 6;
    constexpr int rows2 = 13;
    kbm.slCreate(cols2, rows2);
    QCOMPARE(spy.count(), 2);

    // size
    QCOMPARE(kbm.getNumCols(), cols2);
    QCOMPARE(kbm.getNumRows(), rows2);
}

void EditorBoardTest::testCaseBoardDataMgrRead()
{
    KkrBoardManager kbm;
    QSignalSpy spy(&kbm, &KkrBoardManager::sigReset);
    QCOMPARE(spy.count(), 0);

    constexpr int cols0 = 4;
    constexpr int rows0 = 10;
    kbm.slCreate(cols0, rows0);

    /*
     * source board data
     */
    constexpr int cols = 10;
    constexpr int rows = 4;
    std::shared_ptr<BoardData> pBd{new BoardData{cols, rows}};

    // set cell type
    pBd->setCellType(1,1,CellType::CellClue);
    pBd->setCellType(5,3,CellType::CellClue);
    pBd->setClueRight(1,1,36);
    pBd->setClueDown(1,1,7);
    pBd->setClueRight(5,3,16);

    // set answer
    pBd->setAnswer(2,2,8);
    pBd->setAnswer(6,3,4);

    // update data of manager
    kbm.slRead(pBd);
    QCOMPARE(spy.count(), 2);

    // size
    QCOMPARE(kbm.getNumCols(), pBd->getNumCols());
    QCOMPARE(kbm.getNumRows(), pBd->getNumRows());

    // contents
    bool allMatched = true;
    int badC = -1;
    int badR = -1;
    for(int c = 0; c < pBd->getNumCols(); ++c) {
        for(int r = 0; r < pBd->getNumRows(); ++r) {
            if(kbm.getCellType(c,r) != pBd->getCellType(c,r)) {
                badC = c; badR = r;
                allMatched = false;
                goto endContentMatch;
            }
            if(kbm.getCellType(c,r) == CellType::CellAnswer)
                allMatched = kbm.getAnswer(c,r) == pBd->getAnswer(c,r);
            else
                allMatched = kbm.getClueRight(c,r) == pBd->getClueRight(c,r)
                        && kbm.getClueDown(c,r) == pBd->getClueDown(c,r);
            if(!allMatched) {
                badC = c; badR = r;
                goto endContentMatch;
            }
        }
    }
endContentMatch:
    QVERIFY2(allMatched, QString::asprintf("cell(%d,%d)", badC, badR).toLatin1().constData());
}

void EditorBoardTest::testCaseBoardDataMgrSet()
{
    constexpr int cols = 10;
    constexpr int rows = 4;
    std::shared_ptr<KkrBoardManager> pKbm{new KkrBoardManager};
    pKbm->slCreate(cols, rows);

    KkrBoardView kbv{pKbm};

    // set cell type
    kbv.setCellType(1,1,CellType::CellClue);
    kbv.setCellType(5,3,CellType::CellClue);
    kbv.setClueRight(1,1,36);
    kbv.setClueDown(1,1,7);
    kbv.setClueRight(5,3,16);
    QCOMPARE(pKbm->getCellType(1,1), CellType::CellClue);
    QCOMPARE(pKbm->getCellType(5,3), CellType::CellClue);
    QCOMPARE(pKbm->getClueDown(1,0), CLOSED_CLUE);
    QCOMPARE(pKbm->getClueRight(0,1), CLOSED_CLUE);
    QCOMPARE(pKbm->getClueRight(1,1), 36);
    QCOMPARE(pKbm->getClueDown(1,1), 7);
    QCOMPARE(pKbm->getClueRight(5,3), 16);
    QCOMPARE(pKbm->getClueDown(5,3), CLOSED_CLUE);

    // change cell type back to answer
    kbv.setCellType(1,1,CellType::CellAnswer);
    QCOMPARE(pKbm->getClueDown(1,0), EMPTY_CLUE);
    QCOMPARE(pKbm->getClueRight(0,1), EMPTY_CLUE);
    QCOMPARE(pKbm->getCellType(1,1), CellType::CellAnswer);
    QCOMPARE(pKbm->getAnswer(1,1), EMPTY_ANSWER);

    // set answer
    kbv.setAnswer(1,1,8);
    kbv.setAnswer(6,3,4);
    QCOMPARE(pKbm->getAnswer(1,1), 8);
    QCOMPARE(pKbm->getAnswer(6,3), 4);
}

void EditorBoardTest::testCaseBoardDataMgrGet()
{
    /*
     * source board data
     */
    constexpr int cols = 10;
    constexpr int rows = 4;
    std::shared_ptr<BoardData> pBd{new BoardData{cols, rows}};

    // set cell type
    pBd->setCellType(1,1,CellType::CellClue);
    pBd->setCellType(5,3,CellType::CellClue);
    pBd->setClueRight(1,1,36);
    pBd->setClueDown(1,1,7);
    pBd->setClueRight(5,3,16);

    // set answer
    pBd->setAnswer(2,2,8);
    pBd->setAnswer(6,3,4);

    // update data of manager
    KkrBoardManager kbm;
    kbm.slRead(pBd);

    std::unique_ptr<BoardData> pBd2{kbm.getBoardData()};

    // size
    QCOMPARE(pBd2->getNumCols(), pBd->getNumCols());
    QCOMPARE(pBd2->getNumRows(), pBd->getNumRows());

    // contents
    bool allMatched = true;
    int badC = -1;
    int badR = -1;
    for(int c = 0; c < pBd->getNumCols(); ++c) {
        for(int r = 0; r < pBd->getNumRows(); ++r) {
            if(pBd2->getCellType(c,r) != pBd->getCellType(c,r)) {
                badC = c; badR = r;
                allMatched = false;
                goto endContentMatch;
            }
            if(pBd2->getCellType(c,r) == CellType::CellAnswer)
                allMatched = pBd2->getAnswer(c,r) == pBd->getAnswer(c,r);
            else
                allMatched = pBd2->getClueRight(c,r) == pBd->getClueRight(c,r)
                        && pBd2->getClueDown(c,r) == pBd->getClueDown(c,r);
            if(!allMatched) {
                badC = c; badR = r;
                goto endContentMatch;
            }
        }
    }
endContentMatch:
    QVERIFY2(allMatched, QString::asprintf("cell(%d,%d)", badC, badR).toLatin1().constData());
}

void EditorBoardTest::testCaseBoardDataMgrValid()
{
    constexpr int cols = 4;
    constexpr int rows = 5;
    std::shared_ptr<BoardData> pbd{new BoardData{cols, rows}};

    // set up data
    int c, r;

    // row 0
    c = 2; r = 0;
    pbd->setClueDown(c,r,30);
    c = 3;
    pbd->setClueDown(c,r,4);

    // row 1
    c = 1; r = 1;
    pbd->setCellType(c,r,CellType::CellClue);
    pbd->setClueRight(c,r,7);
    pbd->setClueDown(c,r,11);
    c = 2;
    pbd->setAnswer(c,r,6);
    c = 3;
    pbd->setAnswer(c,r,1);

    // row 2
    c = 0; r = 2;
    pbd->setClueRight(c,r,13);
    c = 1;
    pbd->setAnswer(c,r,2);
    c = 2;
    pbd->setAnswer(c,r,8);
    c = 3;
    pbd->setAnswer(c,r,3);

    // row 3
    c = 0; r = 3;
    pbd->setClueRight(c,r,8);
    c = 1;
    pbd->setAnswer(c,r,1);
    c = 2;
    pbd->setAnswer(c,r,7);
    c = 3;
    pbd->setCellType(c,r,CellType::CellClue);

    // row 4
    c = 0; r = 4;
    pbd->setClueRight(c,r,17);
    c = 1;
    pbd->setAnswer(c,r,8);
    c = 2;
    pbd->setAnswer(c,r,9);
    c = 3;
    pbd->setCellType(c,r,CellType::CellClue);

    KkrBoardManager kbm;
    kbm.slRead(pbd);

    QCOMPARE(kbm.isValid(), true);
}

void EditorBoardTest::testCaseBoardDataMgrInvalidNoClue()
{
    constexpr int cols = 4;
    constexpr int rows = 5;
    std::shared_ptr<BoardData> pbd{new BoardData{cols, rows}};

    // set up data
    int c, r;

    // row 0
    c = 2; r = 0;
    pbd->setClueDown(c,r,30);
    c = 3;
    pbd->setClueDown(c,r,4);

    // row 1
    c = 1; r = 1;
    pbd->setCellType(c,r,CellType::CellClue);
    pbd->setClueRight(c,r,7);
    pbd->setClueDown(c,r,11);
    c = 2;
    pbd->setAnswer(c,r,6);
    c = 3;
    pbd->setAnswer(c,r,1);

    // row 2
    c = 0; r = 2;
    pbd->setClueRight(c,r,13);
    c = 1;
    pbd->setAnswer(c,r,2);
    c = 2;
    pbd->setAnswer(c,r,8);
    c = 3;
    pbd->setAnswer(c,r,3);

    // row 3
    c = 0; r = 3;
    pbd->setClueRight(c,r,8);
    c = 1;
    pbd->setAnswer(c,r,1);
    c = 2;
    pbd->setAnswer(c,r,7);
    c = 3;
    pbd->setCellType(c,r,CellType::CellClue);

    // row 4
    c = 0; r = 4;
    //pbd->setClueRight(c,r,17);
    c = 1;
    pbd->setAnswer(c,r,8);
    c = 2;
    pbd->setAnswer(c,r,9);
    c = 3;
    pbd->setCellType(c,r,CellType::CellClue);

    KkrBoardManager kbm;
    kbm.slRead(pbd);

    QCOMPARE(kbm.isValid(), false);
}

void EditorBoardTest::testCaseBoardDataMgrInvalidNoAnswer()
{
    constexpr int cols = 4;
    constexpr int rows = 5;
    std::shared_ptr<BoardData> pbd{new BoardData{cols, rows}};

    // set up data
    int c, r;

    // row 0
    c = 2; r = 0;
    pbd->setClueDown(c,r,30);
    c = 3;
    pbd->setClueDown(c,r,4);

    // row 1
    c = 1; r = 1;
    pbd->setCellType(c,r,CellType::CellClue);
    pbd->setClueRight(c,r,7);
    pbd->setClueDown(c,r,11);
    c = 2;
    pbd->setAnswer(c,r,6);
    c = 3;
    pbd->setAnswer(c,r,1);

    // row 2
    c = 0; r = 2;
    pbd->setClueRight(c,r,13);
    c = 1;
    pbd->setAnswer(c,r,2);
    c = 2;
    pbd->setAnswer(c,r,8);
    c = 3;
    pbd->setAnswer(c,r,3);

    // row 3
    c = 0; r = 3;
    pbd->setClueRight(c,r,8);
    c = 1;
    //pbd->setAnswer(c,r,1);
    c = 2;
    pbd->setAnswer(c,r,7);
    c = 3;
    pbd->setCellType(c,r,CellType::CellClue);

    // row 4
    c = 0; r = 4;
    pbd->setClueRight(c,r,17);
    c = 1;
    pbd->setAnswer(c,r,8);
    c = 2;
    pbd->setAnswer(c,r,9);
    c = 3;
    pbd->setCellType(c,r,CellType::CellClue);

    KkrBoardManager kbm;
    kbm.slRead(pbd);

    QCOMPARE(kbm.isValid(), false);
}

void EditorBoardTest::testCaseBoardDataMgrInvalidWrongSum()
{
    constexpr int cols = 4;
    constexpr int rows = 5;
    std::shared_ptr<BoardData> pbd{new BoardData{cols, rows}};

    // set up data
    int c, r;

    // row 0
    c = 2; r = 0;
    pbd->setClueDown(c,r,30);
    c = 3;
    pbd->setClueDown(c,r,4);

    // row 1
    c = 1; r = 1;
    pbd->setCellType(c,r,CellType::CellClue);
    pbd->setClueRight(c,r,7);
    pbd->setClueDown(c,r,11);
    c = 2;
    pbd->setAnswer(c,r,6);
    c = 3;
    pbd->setAnswer(c,r,1);

    // row 2
    c = 0; r = 2;
    pbd->setClueRight(c,r,13);
    c = 1;
    pbd->setAnswer(c,r,2);
    c = 2;
    pbd->setAnswer(c,r,8);
    c = 3;
    pbd->setAnswer(c,r,3);

    // row 3
    c = 0; r = 3;
    pbd->setClueRight(c,r,8);
    c = 1;
    pbd->setAnswer(c,r,1);
    c = 2;
    pbd->setAnswer(c,r,7);
    c = 3;
    pbd->setCellType(c,r,CellType::CellClue);

    // row 4
    c = 0; r = 4;
    pbd->setClueRight(c,r,17);
    c = 1;
    //pbd->setAnswer(c,r,8);
    pbd->setAnswer(c,r,7);
    c = 2;
    pbd->setAnswer(c,r,9);
    c = 3;
    pbd->setCellType(c,r,CellType::CellClue);

    KkrBoardManager kbm;
    kbm.slRead(pbd);

    QCOMPARE(kbm.isValid(), false);
}

QTEST_APPLESS_MAIN(EditorBoardTest)

#include "tst_editorboardtest.moc"
