#include "useranswermanager.h"
#include <utility>

namespace useranswer {

UserAnswerManager::UserAnswerManager()
{
}

UserAnswerManager::~UserAnswerManager()
{
}

/*
 * regular methods
 */
bool UserAnswerManager::isSolved() const
{
    for(int row = 0; row < m_pProblem->getNumRows(); ++row) {
        for(int col = 0; col < m_pProblem->getNumCols(); ++col) {
            if(m_pProblem->getCellType(col, row) == problemdata::CellType::CellAnswer
                    && m_pProblem->getAnswer(col, row) != m_pAnswer->getAnswer(col, row)) {
                return false;
            }
        }
    }

    return true;
}

/*
 * slots
 */
void UserAnswerManager::updateProblem(std::shared_ptr<problemdata::ProblemData> pNewData)
{
    const int cols = pNewData->getNumCols();
    const int rows = pNewData->getNumRows();
    m_pProblem = pNewData;
    m_pAnswer.reset(new UserAnswer(cols, rows));

    emit newUserAnswer(m_pAnswer);
}

void UserAnswerManager::updateCellAnswer(CellData cellData)
{
    m_pAnswer->m_answers[m_pAnswer->getIndex(cellData.p.x(), cellData.p.y())] = cellData.answer;
    emit newCellAnswer(cellData.p);
}

void UserAnswerManager::deleteCellAnswer(QPoint p)
{
    m_pAnswer->m_answers[m_pAnswer->getIndex(p.x(), p.y())] = ANSWER_NODATA;
    emit newCellAnswer(p);
}

}   // namespace useranswer
