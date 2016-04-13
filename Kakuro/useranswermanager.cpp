#include "useranswermanager.h"
#include <utility>

namespace useranswer {

UserAnswerManager::UserAnswerManager()
{
}

UserAnswerManager::~UserAnswerManager()
{
}

void UserAnswerManager::updateData(std::shared_ptr<problemdata::ProblemData> pNewData)
{
    const int cols = pNewData->getNumCols();
    const int rows = pNewData->getNumRows();
    m_answer.reset(new UserAnswer(cols, rows));

    emit newUserAnswer(m_answer);
}

void UserAnswerManager::updateCellAnswer(CellData cellData)
{
    m_answer->m_answers[m_answer->getIndex(cellData.p.x(), cellData.p.y())] = cellData.answer;
    emit updateCell(cellData.p);
}

void UserAnswerManager::deleteCellAnswer(QPoint p)
{
    m_answer->m_answers[m_answer->getIndex(p.x(), p.y())] = ANSWER_NODATA;
    emit updateCell(p);
}

}   // namespace useranswer
