#include "kkrworkboard.h"
#include <QPainter>
#include <QKeyEvent>
#include <array>

// dimensions
static const int MARGIN = 10;
static const int FRAME_THICK = 5;
static const int BORDER_THICK = 1;
static const int CELL_WIDTH = 36;
static const int CLUE_WIDTH = CELL_WIDTH/2 - 4;

// font face
static const char FONT_ANS[] = "consolas";
static const char FONT_CLUE[] = "consolas";

KkrBoardView::KkrBoardView(KkrBoardManager *pBoardData, QWidget *parent)
    : QWidget(parent)
    , m_fontAns(FONT_ANS)
    , m_fontClue(FONT_CLUE)
    , m_pBoardData(pBoardData)
{
    m_fontAns.setPixelSize(CELL_WIDTH);
    m_fontClue.setPixelSize(CLUE_WIDTH);
    setMinimumHeight(300);
    setMinimumWidth(400);

    connect(pBoardData, &KkrBoardManager::sigReset, this, &KkrBoardView::slReset);
}

QRect KkrBoardView::getCellRect(int col, int row) const
{
    const int x = MARGIN + FRAME_THICK + col * (CELL_WIDTH + BORDER_THICK);
    const int y = MARGIN + FRAME_THICK + row * (CELL_WIDTH + BORDER_THICK);
    return QRect(x, y, CELL_WIDTH, CELL_WIDTH);
}

QRect KkrBoardView::getClueRectRight(const QRect &cellRect) const
{
    const QPoint topRight{cellRect.topRight()};
    const int x = topRight.x() - CLUE_WIDTH - 1;
    const int y = topRight.y() + 2;
    return QRect(x, y, CLUE_WIDTH, CLUE_WIDTH);
}

QRect KkrBoardView::getClueRectDown(const QRect &cellRect) const
{
    const QPoint bottomLeft{cellRect.bottomLeft()};
    const int x = bottomLeft.x() + 2;
    const int y = bottomLeft.y() - CLUE_WIDTH - 1;
    return QRect(x, y, CLUE_WIDTH, CLUE_WIDTH);
}

void KkrBoardView::drawCell(QPainter &p, int col, int row) const
{
    static const char * digits[] = {
        " ",  "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",
        "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
        "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
        "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
        "40", "41", "42", "43", "44", "45"
    };
    const QRect cellRect{getCellRect(col, row)};
    switch(m_pBoardData->getCellType(col, row)) {
    case CellType::CellAnswer:
    {
        if(m_curCol == col && m_curRow == row) {
            QBrush brCyan(Qt::cyan);
            p.fillRect(cellRect, brCyan);
        }
        const int ans = m_pBoardData->getAnswer(col, row);
        if(ans != EMPTY_ANSWER) {
            p.setFont(m_fontAns);
            p.drawText(cellRect, Qt::AlignCenter | Qt::AlignHCenter, digits[ans]);
        }
    }
        break;
    case CellType::CellClue:
    {
        const bool onCursor = m_curCol==col && m_curRow==row;
        QBrush brWhite(Qt::white);
        QBrush brBlack(Qt::black);
        QBrush brCyan(Qt::cyan);

        if(onCursor && m_curClue==CursorClue::None)
            p.setBrush(brCyan);
        else
            p.setBrush(brBlack);
        std::array<QPoint, 3> points;

        // upper right triangle
        points[0] = cellRect.topLeft(); points[0] += QPoint(2,1);
        points[1] = cellRect.topRight(); points[1] += QPoint(-1,1);
        points[2] = cellRect.bottomRight(); points[2] += QPoint(-1,-2);
        p.drawPolygon(points.data(), static_cast<int>(points.size()));

        // bottom left triangle
        points[0] = cellRect.topLeft(); points[0] += QPoint(1,2);
        points[1] = cellRect.bottomLeft(); points[1] += QPoint(1,-1);
        points[2] = cellRect.bottomRight(); points[2] += QPoint(-2,-1);
        p.drawPolygon(points.data(), static_cast<int>(points.size()));

        if(m_pBoardData->getClueRight(col, row) != CLOSED_CLUE) {
            const QRect clueRect{getClueRectRight(cellRect)};
            if(onCursor && m_curClue==CursorClue::Right)
                p.fillRect(clueRect, brCyan);
            else
                p.fillRect(clueRect, brWhite);
            p.setFont(m_fontClue);
            p.drawText(clueRect, Qt::AlignCenter | Qt::AlignHCenter,
                        digits[m_pBoardData->getClueRight(col, row)]);
        }

        if(m_pBoardData->getClueDown(col, row) != CLOSED_CLUE) {
            const QRect clueRect{getClueRectDown(cellRect)};
            if(onCursor && m_curClue==CursorClue::Down)
                p.fillRect(clueRect, brCyan);
            else
                p.fillRect(clueRect, brWhite);
            p.setFont(m_fontClue);
            p.drawText(clueRect, Qt::AlignCenter | Qt::AlignHCenter,
                       digits[m_pBoardData->getClueDown(col, row)]);
        }
    }
        break;
    default:
        Q_ASSERT(false);
    }
}

void KkrBoardView::showCell(int col, int row)
{
    if(m_pScrollArea != nullptr) {
        const QRect cellRect = getCellRect(col, row);
        // the top/right clue cell must be also in the visible area
        m_pScrollArea->ensureVisible(cellRect.x(), cellRect.y(),
                                     FRAME_THICK+CELL_WIDTH, FRAME_THICK+CELL_WIDTH);
    }
}

void KkrBoardView::resetCursor(int col, int row, CursorClue cl)
{
    Q_ASSERT(col >= 0 && col < m_pBoardData->getNumCols());
    Q_ASSERT(row >= 0 && row < m_pBoardData->getNumRows());

    // make sure the cursor comes in the visible area
    showCell(col, row);

    m_curCol = col; m_curRow = row;
    m_curClue = cl;
    update(); // can be optimized
}

/*
 * key input
 */
void KkrBoardView::keyCursor(QKeyEvent *e)
{
    int newCol;
    int newRow;
    const auto mod = e->modifiers();
    CursorClue cl = CursorClue::None;

    if(mod == Qt::NoModifier) {
        switch(e->key()) {
        case Qt::Key_Up:    case Qt::Key_K:
            // cusor move within cell
            if(m_pBoardData->getCellType(m_curCol, m_curRow) == CellType::CellClue) {
                if(m_curClue == CursorClue::Down
                        && m_pBoardData->getClueRight(m_curCol, m_curRow) != CLOSED_CLUE) {
                    cl = CursorClue::Right;
                    newCol = m_curCol;
                    newRow = m_curRow;
                    break;
                }
            }

            // move to next cell
            if(m_curRow < 1)
                return;
            newCol = m_curCol;
            newRow = m_curRow-1;

            if(m_pBoardData->getCellType(newCol, newRow) == CellType::CellClue) {
                if(m_pBoardData->getClueDown(newCol, newRow) != CLOSED_CLUE)
                    cl = CursorClue::Down;
                else if(m_pBoardData->getClueRight(newCol, newRow) != CLOSED_CLUE)
                    cl = CursorClue::Right;
            }
            break;

        case Qt::Key_Down:  case Qt::Key_J:
            // cusor move within cell
            if(m_pBoardData->getCellType(m_curCol, m_curRow) == CellType::CellClue) {
                if(m_curClue == CursorClue::Right
                        && m_pBoardData->getClueDown(m_curCol, m_curRow) != CLOSED_CLUE) {
                    cl = CursorClue::Down;
                    newCol = m_curCol;
                    newRow = m_curRow;
                    break;
                }
            }

            // move to next cell
            if(m_curRow >= m_pBoardData->getNumRows()-1)
                return;
            newCol = m_curCol;
            newRow = m_curRow+1;

            if(m_pBoardData->getCellType(newCol, newRow) == CellType::CellClue) {
                if(m_pBoardData->getClueRight(newCol, newRow) != CLOSED_CLUE)
                    cl = CursorClue::Right;
                else if(m_pBoardData->getClueDown(newCol, newRow) != CLOSED_CLUE)
                    cl = CursorClue::Down;
            }
            break;

        case Qt::Key_Left:  case Qt::Key_H:
            // cusor move within cell
            if(m_pBoardData->getCellType(m_curCol, m_curRow) == CellType::CellClue) {
                if(m_curClue == CursorClue::Right
                        && m_pBoardData->getClueDown(m_curCol, m_curRow) != CLOSED_CLUE) {
                    cl = CursorClue::Down;
                    newCol = m_curCol;
                    newRow = m_curRow;
                    break;
                }
            }

            // move to next cell
            if(m_curCol < 1)
                return;
            newCol = m_curCol-1;
            newRow = m_curRow;

            if(m_pBoardData->getCellType(newCol, newRow) == CellType::CellClue) {
                if(m_pBoardData->getClueRight(newCol, newRow) != CLOSED_CLUE)
                    cl = CursorClue::Right;
                else if(m_pBoardData->getClueDown(newCol, newRow) != CLOSED_CLUE)
                    cl = CursorClue::Down;
            }
            break;

        case Qt::Key_Right: case Qt::Key_L:
            // cusor move within cell
            if(m_pBoardData->getCellType(m_curCol, m_curRow) == CellType::CellClue) {
                if(m_curClue == CursorClue::Down
                        && m_pBoardData->getClueRight(m_curCol, m_curRow) != CLOSED_CLUE) {
                    cl = CursorClue::Right;
                    newCol = m_curCol;
                    newRow = m_curRow;
                    break;
                }
            }

            // move to next cell
            if(m_curCol >= m_pBoardData->getNumCols()-1)
                return;
            newCol = m_curCol+1;
            newRow = m_curRow;

            if(m_pBoardData->getCellType(newCol, newRow) == CellType::CellClue) {
                if(m_pBoardData->getClueDown(newCol, newRow) != CLOSED_CLUE)
                    cl = CursorClue::Down;
                else if(m_pBoardData->getClueRight(newCol, newRow) != CLOSED_CLUE)
                    cl = CursorClue::Right;
            }
            break;
        }
    }

    resetCursor(newCol, newRow, cl);
}

void KkrBoardView::resetClueCell(int col, int row)
{
    if(col < 0 || row < 0)
        return;
    if(m_pBoardData->getCellType(col, row) != CellType::CellClue)
        return;
    if(col == m_pBoardData->getNumCols()-1
            || m_pBoardData->getCellType(col+1, row) == CellType::CellClue)
        m_pBoardData->setClueRight(col, row, CLOSED_CLUE);
    if(row == m_pBoardData->getNumRows()-1
            || m_pBoardData->getCellType(col, row+1) == CellType::CellClue)
        m_pBoardData->setClueDown(col, row, CLOSED_CLUE);

}

void KkrBoardView::switchCellType(int col, int row)
{
    Q_ASSERT(0 <= col && col < m_pBoardData->getNumCols());
    Q_ASSERT(0 <= row && row < m_pBoardData->getNumRows());

    // topmost row and leftmost column are always clue
    if(col == 0 || row == 0)
        return;

    if(m_pBoardData->getCellType(col, row) == CellType::CellClue) {
        m_pBoardData->setCellType(col, row, CellType::CellAnswer);
        m_pBoardData->setAnswer(col, row, EMPTY_ANSWER);
    } else {
        m_pBoardData->setCellType(col, row, CellType::CellClue);
        m_pBoardData->setClueDown(col, row, EMPTY_CLUE);
        m_pBoardData->setClueRight(col, row, EMPTY_CLUE);
        resetClueCell(col, row);
        if(m_pBoardData->getClueDown(col, row) != CLOSED_CLUE)
            m_curClue = CursorClue::Down;
        else if(m_pBoardData->getClueRight(col, row) != CLOSED_CLUE)
            m_curClue = CursorClue::Right;
        else
            m_curClue = CursorClue::None;
    }

    resetClueCell(col-1, row);
    resetClueCell(col, row-1);

    update();
}

void KkrBoardView::keyValue(QKeyEvent *e)
{
    const auto ct = m_pBoardData->getCellType(m_curCol, m_curRow);
    if(ct == CellType::CellClue && m_curClue == CursorClue::None)
        return;

    int newValue;
    switch(e->key()) {
    case Qt::Key_Delete:
        newValue = 0;
        if(ct == CellType::CellClue) {
            if(m_curClue == CursorClue::Right)
                m_pBoardData->setClueRight(m_curCol, m_curRow, newValue);
            else
                m_pBoardData->setClueDown(m_curCol, m_curRow, newValue);
        } else {
            m_pBoardData->setAnswer(m_curCol, m_curRow, newValue);
        }
        break;

    case Qt::Key_0: case Qt::Key_1:
    case Qt::Key_2: case Qt::Key_3:
    case Qt::Key_4: case Qt::Key_5:
    case Qt::Key_6: case Qt::Key_7:
    case Qt::Key_8: case Qt::Key_9:
    {
        int kValue;
        switch(e->key()) {
        case Qt::Key_0: kValue = 0; break;
        case Qt::Key_1: kValue = 1; break;
        case Qt::Key_2: kValue = 2; break;
        case Qt::Key_3: kValue = 3; break;
        case Qt::Key_4: kValue = 4; break;
        case Qt::Key_5: kValue = 5; break;
        case Qt::Key_6: kValue = 6; break;
        case Qt::Key_7: kValue = 7; break;
        case Qt::Key_8: kValue = 8; break;
        case Qt::Key_9: kValue = 9; break;
        }

        if(ct == CellType::CellClue) {
            if(m_curClue == CursorClue::Right)
                newValue = m_pBoardData->getClueRight(m_curCol, m_curRow);
            else
                newValue = m_pBoardData->getClueDown(m_curCol, m_curRow);
            newValue *= 10;
            newValue += kValue;
            if(newValue > 99)
                newValue %= 100;
            if(newValue > 45)
                newValue %= 10;

            if(m_curClue == CursorClue::Right)
                m_pBoardData->setClueRight(m_curCol, m_curRow, newValue);
            else
                m_pBoardData->setClueDown(m_curCol, m_curRow, newValue);
        } else {
            m_pBoardData->setAnswer(m_curCol, m_curRow, kValue);
        }
    }
        break;
    }

    update();
}

/*
 * widget events
 */
void KkrBoardView::paintEvent(QPaintEvent * /*e*/)
{
    QPainter p{this};

    const auto C = m_pBoardData->getNumCols();
    if(C < 3)
        return;

    // frame
    p.fillRect(MARGIN, MARGIN, m_frame_width, FRAME_THICK, Qt::SolidPattern);
    p.fillRect(MARGIN, MARGIN+m_frame_height-FRAME_THICK, m_frame_width, FRAME_THICK, Qt::SolidPattern);
    p.fillRect(MARGIN, MARGIN, FRAME_THICK, m_frame_height, Qt::SolidPattern);
    p.fillRect(MARGIN+m_frame_width-FRAME_THICK, MARGIN, FRAME_THICK, m_frame_height, Qt::SolidPattern);

    // cell borders
    const int TL = MARGIN + FRAME_THICK;
    for(int yi = 1; yi < m_pBoardData->getNumRows(); ++yi) {
        const int y = TL - BORDER_THICK + yi * (BORDER_THICK+CELL_WIDTH);
        p.fillRect(TL, y, m_inner_width, BORDER_THICK, Qt::SolidPattern);
    }

    for(int xi = 1; xi < m_pBoardData->getNumCols(); ++xi) {
        const int x = TL - BORDER_THICK + xi * (BORDER_THICK+CELL_WIDTH);
        p.fillRect(x, TL, BORDER_THICK, m_inner_height, Qt::SolidPattern);
    }

    for(int c = 0; c < m_pBoardData->getNumCols(); ++c) {
        for(int r = 0; r < m_pBoardData->getNumRows(); ++r) {
            drawCell(p, c, r);
        }
    }
}

void KkrBoardView::keyReleaseEvent(QKeyEvent *e)
{
    switch(e->key()) {
    case Qt::Key_Up:    case Qt::Key_K:
    case Qt::Key_Down:  case Qt::Key_J:
    case Qt::Key_Left:  case Qt::Key_H:
    case Qt::Key_Right: case Qt::Key_L:
        keyCursor(e);
        break;

    case Qt::Key_S:
        if(e->modifiers() == Qt::NoModifier)
            switchCellType(m_curCol, m_curRow);
        break;

    case Qt::Key_0: case Qt::Key_1:
    case Qt::Key_2: case Qt::Key_3:
    case Qt::Key_4: case Qt::Key_5:
    case Qt::Key_6: case Qt::Key_7:
    case Qt::Key_8: case Qt::Key_9:
    case Qt::Key_Delete:
        keyValue(e);
        break;

    default:
        // just ignore other keys
        break;
    }
}

/*
 * slots
 */
void KkrBoardView::slReset()
{
    // sizes
    m_inner_width = (CELL_WIDTH + BORDER_THICK) * m_pBoardData->getNumCols() - BORDER_THICK;
    m_inner_height = (CELL_WIDTH + BORDER_THICK) * m_pBoardData->getNumRows() - BORDER_THICK;
    m_frame_width = 2*FRAME_THICK + m_inner_width;
    m_frame_height = 2*FRAME_THICK + m_inner_height;
    m_board_width = 2*MARGIN + m_frame_width;
    m_board_height = 2*MARGIN + m_frame_height;

    // initial cursor position
    m_curCol = m_curRow = 0;
    m_curClue = CursorClue::None;

    setFixedSize(m_board_width, m_board_height);

    update();
}
