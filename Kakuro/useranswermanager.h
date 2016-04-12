#ifndef USERANSWERMANAGER_H
#define USERANSWERMANAGER_H

#include <QObject>
#include <QPoint>
#include <memory>
#include "useranswer.h"
#include "problemdata.h"

namespace pd = problemdata;

namespace useranswer {

using SharedAnswer = std::shared_ptr<UserAnswer>;

struct CellData {
    QPoint p;
    int answer;
};
static const int ANSWER_NODATA = 0;

class UserAnswerManager : public QObject
{
    Q_OBJECT

public:
    UserAnswerManager();

    UserAnswerManager(const UserAnswerManager&) = delete;
    UserAnswerManager &operator=(const UserAnswerManager&) = delete;

signals:
    void newUserAnswer(SharedAnswer pNewAns);
    void updateCell(QPoint cellPos);

public slots:
    void updateData(std::shared_ptr<pd::ProblemData> pNewData);
    void updateCellAnswer(CellData cellData);
    void deleteCellAnswer(QPoint p);
};

}   // namespace useranswer

#endif // USERANSWERMANAGER_H
