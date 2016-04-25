#include "kkrboardmanager.h"
#include <QtGlobal>

BoardData::BoardData(int cols, int rows)
    : m_cols(cols)
    , m_rows(rows)
    , m_data(cols*rows)
{
    // topmost row
    for(int c = 0; c < m_cols; ++c) {
        const int i = c2i(c, 0);
        m_data[i].ctype = CellType::CellClue;
        m_data[i].clueRight = m_data[i].clueDown = NO_CLUE;
    }

    // leftmost column
    for(int r = 1; r < m_rows; ++r) {
        const int i = c2i(0, r);
        m_data[i].ctype = CellType::CellClue;
        m_data[i].clueRight = m_data[i].clueDown = NO_CLUE;
    }

    // everything else
    for(int c = 1; c < m_cols; ++c) {
        for(int r = 1; r < m_rows; ++r) {
            const int i = c2i(c, r);
            m_data[i].ctype = CellType::CellAnswer;
            m_data[i].answer = NO_ANSWER;
        }
    }
}

BoardData::BoardData(const BoardData &rhs)
    : m_cols(rhs.m_cols)
    , m_rows(rhs.m_rows)
    , m_data(rhs.m_data)
{
}

BoardData::~BoardData()
{
}

CellType BoardData::getCellType(int col, int row) const
{
    Q_ASSERT(0 <= col && col < m_cols);
    Q_ASSERT(0 <= row && row < m_rows);
    return m_data[c2i(col,row)].ctype;
}

void BoardData::setCellType(int col, int row, CellType ct)
{
    // topmost row and leftmost column cannot be changed
    Q_ASSERT(1 <= col && col < m_cols);
    Q_ASSERT(1 <= row && row < m_rows);

    const int i = c2i(col,row);
    if(m_data[i].ctype != ct) {
        m_data[i].ctype = ct;
        switch(ct) {
        case CellType::CellAnswer:
            m_data[i].answer = NO_ANSWER;
            break;
        default:
            m_data[i].clueRight = m_data[i].clueDown = NO_CLUE;
            break;
        }
    }
}

int BoardData::getClueRight(int col, int row) const
{
    Q_ASSERT(0 <= col && col < m_cols);
    Q_ASSERT(0 <= row && row < m_rows);

    const int i = c2i(col,row);
    Q_ASSERT(m_data[i].ctype == CellType::CellClue);
    return m_data[i].clueRight;
}

void BoardData::setClueRight(int col, int row, int clue)
{
    Q_ASSERT(0 <= col && col < m_cols);
    Q_ASSERT(0 <= row && row < m_rows);
    Q_ASSERT(0 <= clue && clue <= 45);

    const int i = c2i(col,row);
    Q_ASSERT(m_data[i].ctype == CellType::CellClue);
    m_data[i].clueRight = clue;
}

int BoardData::getClueDown(int col, int row) const
{
    Q_ASSERT(0 <= col && col < m_cols);
    Q_ASSERT(0 <= row && row < m_rows);

    const int i = c2i(col,row);
    Q_ASSERT(m_data[i].ctype == CellType::CellClue);
    return m_data[i].clueDown;
}

void BoardData::setClueDown(int col, int row, int clue)
{
    Q_ASSERT(0 <= col && col < m_cols);
    Q_ASSERT(0 <= row && row < m_rows);
    Q_ASSERT(0 <= clue && clue <= 45);

    const int i = c2i(col,row);
    Q_ASSERT(m_data[i].ctype == CellType::CellClue);
    m_data[i].clueDown = clue;
}

int BoardData::getAnswer(int col, int row) const
{
    Q_ASSERT(0 <= col && col < m_cols);
    Q_ASSERT(0 <= row && row < m_rows);

    const int i = c2i(col,row);
    Q_ASSERT(m_data[i].ctype == CellType::CellAnswer);
    return m_data[i].answer;
}

void BoardData::setAnswer(int col, int row, int ans)
{
    Q_ASSERT(0 <= col && col < m_cols);
    Q_ASSERT(0 <= row && row < m_rows);
    Q_ASSERT(0 <= ans && ans <= 9);

    const int i = c2i(col,row);
    Q_ASSERT(m_data[i].ctype == CellType::CellAnswer);
    m_data[i].answer = ans;
}

KkrBoardManager::KkrBoardManager(QObject *parent) : QObject(parent)
{

}

void KkrBoardManager::slCreate(int cols, int rows)
{
    // TODO
}
