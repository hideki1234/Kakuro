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

KkrBoardManager::KkrBoardManager(QObject *parent)
    : QObject(parent)
    , m_cols(0)
    , m_rows(0)
{
}

void KkrBoardManager::setCellType(int col, int row, CellType ct)
{
    Q_ASSERT(0 <= col && col < m_cols);
    Q_ASSERT(0 <= row && row < m_rows);

    const int i = c2i(col,row);
    if(m_data[i].ctype != ct) {
        m_data[i].ctype = ct;
        if(ct == CellType::CellAnswer)
            m_data[i].answer = NO_ANSWER;
        else {
            m_data[i].clueRight = m_data[i].clueDown = NO_CLUE;
        }
    }
}

void KkrBoardManager::setClueRight(int col, int row, int clue)
{
    Q_ASSERT(0 <= col && col < m_cols);
    Q_ASSERT(0 <= row && row < m_rows);

    const int i = c2i(col,row);
    Q_ASSERT(m_data[i].ctype == CellType::CellClue);
    m_data[i].clueRight = clue;
}

void KkrBoardManager::setClueDown(int col, int row, int clue)
{
    Q_ASSERT(0 <= col && col < m_cols);
    Q_ASSERT(0 <= row && row < m_rows);

    const int i = c2i(col,row);
    Q_ASSERT(m_data[i].ctype == CellType::CellClue);
    m_data[i].clueDown = clue;
}

void KkrBoardManager::setAnswer(int col, int row, int ans)
{
    Q_ASSERT(0 <= col && col < m_cols);
    Q_ASSERT(0 <= row && row < m_rows);

    const int i = c2i(col,row);
    Q_ASSERT(m_data[i].ctype == CellType::CellAnswer);
    m_data[i].answer = ans;
}

CellType KkrBoardManager::getCellType(int col, int row) const
{
    Q_ASSERT(0 <= col && col < m_cols);
    Q_ASSERT(0 <= row && row < m_rows);

    return m_data[c2i(col,row)].ctype;
}

int KkrBoardManager::getClueRight(int col, int row) const
{
    Q_ASSERT(0 <= col && col < m_cols);
    Q_ASSERT(0 <= row && row < m_rows);

    const int i = c2i(col,row);
    Q_ASSERT(m_data[i].ctype == CellType::CellClue);
    return m_data[i].clueRight;
}

int KkrBoardManager::getClueDown(int col, int row) const
{
    Q_ASSERT(0 <= col && col < m_cols);
    Q_ASSERT(0 <= row && row < m_rows);

    const int i = c2i(col,row);
    Q_ASSERT(m_data[i].ctype == CellType::CellClue);
    return m_data[i].clueDown;
}

int KkrBoardManager::getAnswer(int col, int row) const
{
    Q_ASSERT(0 <= col && col < m_cols);
    Q_ASSERT(0 <= row && row < m_rows);

    const int i = c2i(col,row);
    Q_ASSERT(m_data[i].ctype == CellType::CellAnswer);
    return m_data[i].answer;
}

bool KkrBoardManager::isValid() const
{
    // topmost row; must be always clue
    for(int c = 0; c < m_cols; ++c) {
        const int i = c2i(c,0);
        if(m_data[i].ctype != CellType::CellClue)
            return false;
    }

    // leftmost column; must be always clue
    for(int r = 1; r < m_rows; ++r) {
        const int i = c2i(0,r);
        if(m_data[i].ctype != CellType::CellClue)
            return false;
    }

    // make sure all cells have a value
    for(int c = 0; c < m_cols; ++c) {
        for(int r = 0; r < m_rows; ++r) {
            const int i = c2i(c,r);
            if(m_data[i].ctype == CellType::CellAnswer) {
                if(m_data[i].answer < 1 || m_data[i].answer > 9)
                    return false;
            } else {
                const int cr = m_data[i].clueRight;
                if(cr == NO_CLUE) {
                    if(c != m_cols-1 && m_data[c2i(c+1,r)].ctype != CellType::CellClue)
                        return false;
                } else if(cr < 1 || cr > 45)
                    return false;

                const int cd = m_data[i].clueDown;
                if(cd == NO_CLUE) {
                    if(r != m_rows-1 && m_data[c2i(c,r+1)].ctype != CellType::CellClue)
                        return false;
                } else if(cd < 1 || cd > 45)
                    return false;
            }
        }
    }

    // make sure clue and sum of answers match
    // right
    for(int r = 0; r < m_rows; ++r) {
        int sum = 0;
        int clue = 0;
        for(int c = 0; c < m_cols; ++c) {
            const int i = c2i(c,r);
            if(m_data[i].ctype == CellType::CellClue) {
                if(sum != clue)
                    return false;
                sum = 0;
                clue = m_data[i].clueRight;
            } else
                sum += m_data[i].answer;
        }
        if(sum != clue)
            return false;
    }
    // down
    for(int c = 0; c < m_cols; ++c) {
        int sum = 0;
        int clue = 0;
        for(int r = 0; r < m_rows; ++r) {
            const int i = c2i(c,r);
            if(m_data[i].ctype == CellType::CellClue) {
                if(sum != clue)
                    return false;
                sum = 0;
                clue = m_data[i].clueDown;
            } else
                sum += m_data[i].answer;
        }
        if(sum != clue)
            return false;
    }
    // TODO - need to implement more checks
    return true;
}

std::unique_ptr<BoardData> KkrBoardManager::getBoardData() const
{
    std::unique_ptr<BoardData> pBdr{new BoardData{m_cols, m_rows}};
    std::copy(begin(m_data), end(m_data), begin(pBdr->m_data));

    return pBdr;
}

/*
 * slots
 */
void KkrBoardManager::slCreate(int cols, int rows)
{
    m_cols = cols; m_rows = rows;
    m_data.resize(m_cols*m_rows);

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

    emit sigReset();
}

void KkrBoardManager::slRead(std::shared_ptr<const BoardData> data)
{
    m_cols = data->getNumCols();
    m_rows = data->getNumRows();
    m_data.resize(data->m_data.size());
    std::copy(begin(data->m_data), end(data->m_data), begin(m_data));

    emit sigReset();
}
