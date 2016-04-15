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

QTEST_APPLESS_MAIN(UserAnswerTest)

#include "tst_useranswertest.moc"
