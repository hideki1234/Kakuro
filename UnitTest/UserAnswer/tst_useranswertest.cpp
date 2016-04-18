#include <QString>
#include <QtTest>
#include <QSignalSpy>
#include <memory>
#include "../../Kakuro/useranswermanager.h"

Q_DECLARE_METATYPE(useranswer::SharedAnswer)

namespace pd = problemdata;

class UserAnswerTest : public QObject
{
    Q_OBJECT

public:
    UserAnswerTest();

private Q_SLOTS:
    void testCaseInitialize();
    void testCaseCellUpdate();
    void testCaseSolved();
    void testCaseNotSolvedWithAnEmptyCell();
    void testCaseNotSolvedWithAWrongAnswer();
    void testCaseSigUndoable();
    void testCaseUndoneContent();
    void testCaseUndoCellSignal();
};

UserAnswerTest::UserAnswerTest()
{
}

void UserAnswerTest::testCaseInitialize()
{
    useranswer::UserAnswerManager target;
    qRegisterMetaType<useranswer::SharedAnswer>("SharedAnswer");
    QSignalSpy spy(&target, &useranswer::UserAnswerManager::newUserAnswer);

    const int numCols = 4;
    const int numRows = 10;
    const QString sSize = QString::asprintf("%d,%d", numCols, numRows);
    std::shared_ptr<pd::ProblemData> pProblem{pd::ProblemData::problemLoader(sSize)};
    target.updateProblem(pProblem);
    QCOMPARE(spy.count(), 1);

    useranswer::SharedAnswer pAns{qvariant_cast<useranswer::SharedAnswer>(spy.at(0).at(0))};
    QCOMPARE(pAns->getNumCols(), numCols);
    QCOMPARE(pAns->getNumRows(), numRows);
}

void UserAnswerTest::testCaseCellUpdate()
{
    useranswer::UserAnswerManager target;
    qRegisterMetaType<useranswer::SharedAnswer>("SharedAnswer");
    QSignalSpy spy0(&target, &useranswer::UserAnswerManager::newUserAnswer);
    QSignalSpy spy(&target, &useranswer::UserAnswerManager::newCellAnswer);

    const int numCols = 4;
    const int numRows = 10;
    const QString sSize = QString::asprintf("%d,%d", numCols, numRows);
    std::shared_ptr<pd::ProblemData> pProblem{pd::ProblemData::problemLoader(sSize)};
    target.updateProblem(pProblem);
    useranswer::SharedAnswer pAns{qvariant_cast<useranswer::SharedAnswer>(spy0.at(0).at(0))};

    QCOMPARE(pAns->getAnswer(1,1), useranswer::ANSWER_NODATA);
    QCOMPARE(pAns->getAnswer(1,2), useranswer::ANSWER_NODATA);
    QCOMPARE(pAns->getAnswer(2,1), useranswer::ANSWER_NODATA);
    QCOMPARE(pAns->getAnswer(2,2), useranswer::ANSWER_NODATA);

    useranswer::CellData cellData;
    cellData.p.setX(1); cellData.p.setY(2); cellData.answer = 3;
    target.updateCellAnswer(cellData);
    QCOMPARE(spy.count(), 1);

    QCOMPARE(pAns->getAnswer(1,1), useranswer::ANSWER_NODATA);
    QCOMPARE(pAns->getAnswer(1,2), 3);
    QCOMPARE(pAns->getAnswer(2,1), useranswer::ANSWER_NODATA);
    QCOMPARE(pAns->getAnswer(2,2), useranswer::ANSWER_NODATA);

    cellData.p.setX(1); cellData.p.setY(1); cellData.answer = 9;
    target.updateCellAnswer(cellData);
    QCOMPARE(spy.count(), 2);

    QCOMPARE(pAns->getAnswer(1,1), 9);
    QCOMPARE(pAns->getAnswer(1,2), 3);
    QCOMPARE(pAns->getAnswer(2,1), useranswer::ANSWER_NODATA);
    QCOMPARE(pAns->getAnswer(2,2), useranswer::ANSWER_NODATA);

    QPoint p{1,2};
    target.deleteCellAnswer(p);
    QCOMPARE(spy.count(), 3);

    QCOMPARE(pAns->getAnswer(1,1), 9);
    QCOMPARE(pAns->getAnswer(1,2), useranswer::ANSWER_NODATA);
    QCOMPARE(pAns->getAnswer(2,1), useranswer::ANSWER_NODATA);
    QCOMPARE(pAns->getAnswer(2,2), useranswer::ANSWER_NODATA);
}

void UserAnswerTest::testCaseSolved()
{
    useranswer::UserAnswerManager target;

    const int numCols = 4;
    const int numRows = 3;
    const QString sSize = QString::asprintf("%d,%d", numCols, numRows);
    std::shared_ptr<pd::ProblemData> pProblem{pd::ProblemData::problemLoader(sSize)};
    target.updateProblem(pProblem);

    useranswer::CellData cellData;
    cellData.p.setX(1); cellData.p.setY(0); cellData.answer = 2;
    target.updateCellAnswer(cellData);
    cellData.p.setX(3); cellData.p.setY(0); cellData.answer = 4;
    target.updateCellAnswer(cellData);
    cellData.p.setX(0); cellData.p.setY(1); cellData.answer = 2;
    target.updateCellAnswer(cellData);
    cellData.p.setX(2); cellData.p.setY(1); cellData.answer = 4;
    target.updateCellAnswer(cellData);
    cellData.p.setX(1); cellData.p.setY(2); cellData.answer = 4;
    target.updateCellAnswer(cellData);
    cellData.p.setX(3); cellData.p.setY(2); cellData.answer = 6;
    target.updateCellAnswer(cellData);

    QCOMPARE(target.isSolved(), true);
}

void UserAnswerTest::testCaseNotSolvedWithAnEmptyCell()
{
    useranswer::UserAnswerManager target;

    const int numCols = 4;
    const int numRows = 3;
    const QString sSize = QString::asprintf("%d,%d", numCols, numRows);
    std::shared_ptr<pd::ProblemData> pProblem{pd::ProblemData::problemLoader(sSize)};
    target.updateProblem(pProblem);

    useranswer::CellData cellData;
    cellData.p.setX(1); cellData.p.setY(0); cellData.answer = 2;
    target.updateCellAnswer(cellData);
    cellData.p.setX(3); cellData.p.setY(0); cellData.answer = 4;
    target.updateCellAnswer(cellData);
    cellData.p.setX(0); cellData.p.setY(1); cellData.answer = 2;
    target.updateCellAnswer(cellData);
    //cellData.p.setX(2); cellData.p.setY(1); cellData.answer = 4;
    //target.updateCellAnswer(cellData);
    cellData.p.setX(1); cellData.p.setY(2); cellData.answer = 4;
    target.updateCellAnswer(cellData);
    cellData.p.setX(3); cellData.p.setY(2); cellData.answer = 6;
    target.updateCellAnswer(cellData);

    QCOMPARE(target.isSolved(), false);
}

void UserAnswerTest::testCaseNotSolvedWithAWrongAnswer()
{
    useranswer::UserAnswerManager target;

    const int numCols = 4;
    const int numRows = 3;
    const QString sSize = QString::asprintf("%d,%d", numCols, numRows);
    std::shared_ptr<pd::ProblemData> pProblem{pd::ProblemData::problemLoader(sSize)};
    target.updateProblem(pProblem);

    useranswer::CellData cellData;
    cellData.p.setX(1); cellData.p.setY(0); cellData.answer = 2;
    target.updateCellAnswer(cellData);
    cellData.p.setX(3); cellData.p.setY(0); cellData.answer = 4;
    target.updateCellAnswer(cellData);
    cellData.p.setX(0); cellData.p.setY(1); cellData.answer = 3/*2*/;
    target.updateCellAnswer(cellData);
    cellData.p.setX(2); cellData.p.setY(1); cellData.answer = 4;
    target.updateCellAnswer(cellData);
    cellData.p.setX(1); cellData.p.setY(2); cellData.answer = 4;
    target.updateCellAnswer(cellData);
    cellData.p.setX(3); cellData.p.setY(2); cellData.answer = 6;
    target.updateCellAnswer(cellData);

    QCOMPARE(target.isSolved(), false);
}

void UserAnswerTest::testCaseSigUndoable()
{
    useranswer::UserAnswerManager target;
    QSignalSpy spy(&target, &useranswer::UserAnswerManager::undoable);

    const int numCols = 4;
    const int numRows = 10;
    const QString sSize = QString::asprintf("%d,%d", numCols, numRows);
    std::shared_ptr<pd::ProblemData> pProblem{pd::ProblemData::problemLoader(sSize)};
    target.updateProblem(pProblem);

    // undo with no data - nothing must happen
    target.undo();
    QCOMPARE(spy.count(), 1);

    // add first data
    useranswer::CellData cellData;
    cellData.p.setX(1); cellData.p.setY(1); cellData.answer = 2;
    target.updateCellAnswer(cellData);
    QCOMPARE(spy.count(), 2);

    // second data - no undoable signal
    cellData.p.setX(2); cellData.p.setY(2); cellData.answer = 2;
    target.updateCellAnswer(cellData);
    QCOMPARE(spy.count(), 2);

    // undo - no signal
    target.undo();
    QCOMPARE(spy.count(), 2);

    // undo - signal
    target.undo();
    QCOMPARE(spy.count(), 3);

    QVERIFY(spy.at(0).at(0).toBool() == false);
    QVERIFY(spy.at(1).at(0).toBool() == true);
    QVERIFY(spy.at(2).at(0).toBool() == false);
}

void UserAnswerTest::testCaseUndoneContent()
{
    useranswer::UserAnswerManager target;
    qRegisterMetaType<useranswer::SharedAnswer>("SharedAnswer");
    QSignalSpy spy(&target, &useranswer::UserAnswerManager::newUserAnswer);

    const int numCols = 4;
    const int numRows = 10;
    const QString sSize = QString::asprintf("%d,%d", numCols, numRows);
    std::shared_ptr<pd::ProblemData> pProblem{pd::ProblemData::problemLoader(sSize)};
    target.updateProblem(pProblem);
    useranswer::SharedAnswer pAns{qvariant_cast<useranswer::SharedAnswer>(spy.at(0).at(0))};

    useranswer::CellData cellData;
    int col, row, ans;

    col = 1; row = 1; ans = 2;
    cellData.p.setX(col); cellData.p.setY(row); cellData.answer = ans;
    target.updateCellAnswer(cellData);
    QCOMPARE(pAns->getAnswer(col,row), ans);
    target.undo();
    QCOMPARE(pAns->getAnswer(col,row), useranswer::ANSWER_NODATA);

    col = 2; row = 3; ans = 9;
    cellData.p.setX(col); cellData.p.setY(row); cellData.answer = ans;
    target.updateCellAnswer(cellData);
    target.deleteCellAnswer(cellData.p);
    QCOMPARE(pAns->getAnswer(col,row), useranswer::ANSWER_NODATA);
    target.undo();
    QCOMPARE(pAns->getAnswer(col,row), ans);
 }

void UserAnswerTest::testCaseUndoCellSignal()
{
    useranswer::UserAnswerManager target;
    QSignalSpy spy(&target, &useranswer::UserAnswerManager::newCellAnswer);

    const int numCols = 4;
    const int numRows = 10;
    const QString sSize = QString::asprintf("%d,%d", numCols, numRows);
    std::shared_ptr<pd::ProblemData> pProblem{pd::ProblemData::problemLoader(sSize)};
    target.updateProblem(pProblem);

    useranswer::CellData cellData;
    int col, row, ans;

    col = 1; row = 1; ans = 2;
    cellData.p.setX(col); cellData.p.setY(row); cellData.answer = ans;
    target.updateCellAnswer(cellData);
    target.undo();
    QCOMPARE(spy.count(), 2);
    QPoint pos{qvariant_cast<QPoint>(spy.at(1).at(0))};
    QCOMPARE(pos, QPoint(col, row));

    col = 2; row = 3; ans = 9;
    cellData.p.setX(col); cellData.p.setY(row); cellData.answer = ans;
    target.updateCellAnswer(cellData);
    target.deleteCellAnswer(cellData.p);
    target.undo();
    QCOMPARE(spy.count(), 5);
    pos = qvariant_cast<QPoint>(spy.at(4).at(0));
    QCOMPARE(pos, QPoint(col, row));
}

QTEST_APPLESS_MAIN(UserAnswerTest)

#include "tst_useranswertest.moc"
