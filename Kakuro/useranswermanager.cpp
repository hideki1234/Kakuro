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
    while(!m_undoStack.empty())
        m_undoStack.pop();

    emit newUserAnswer(m_pAnswer);
    emit undoable(false);
}

void UserAnswerManager::updateCellAnswer(CellData cellData)
{
    auto &cell = m_pAnswer->m_answers[m_pAnswer->getIndex(cellData.p.x(), cellData.p.y())];
    if(cell != cellData.answer) {
        const bool undoableChanged = m_undoStack.empty();

        // undo stack
        CellData undoData;
        undoData.p = cellData.p;
        if(cellData.answer == ANSWER_NODATA)
            undoData.answer = cell;
        else
            undoData.answer = ANSWER_NODATA;
        m_undoStack.push(undoData);

        // update answer
        cell = cellData.answer;

        // notify updates
        emit newCellAnswer(cellData.p);
        if(undoableChanged)
            emit undoable(true);
    }
}

void UserAnswerManager::deleteCellAnswer(QPoint p)
{
    CellData cellData;
    cellData.p = p; cellData.answer = ANSWER_NODATA;
    updateCellAnswer(cellData);
}

void UserAnswerManager::undo()
{
    if(m_undoStack.empty())
        return;

    auto &undoData = m_undoStack.top();
    auto &cell = m_pAnswer->m_answers[m_pAnswer->getIndex(undoData.p.x(), undoData.p.y())];
    cell = undoData.answer;
    m_undoStack.pop();

    emit newCellAnswer(undoData.p);
    if(m_undoStack.empty())
        undoable(false);
}

}   // namespace useranswer
