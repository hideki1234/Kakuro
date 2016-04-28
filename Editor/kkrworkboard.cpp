#include "kkrworkboard.h"
#include <QPainter>

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
        QBrush brFG{ m_curCol==col && m_curRow==row ? Qt::cyan : Qt::black };
        p.setBrush(brFG);
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

        QBrush brWhite(Qt::white);
        if(m_pBoardData->getClueRight(col, row) != CLOSED_CLUE) {
            const QRect clueRect{getClueRectRight(cellRect)};
            p.fillRect(clueRect, brWhite);
            p.setFont(m_fontClue);
            p.drawText(clueRect, Qt::AlignCenter | Qt::AlignHCenter,
                        digits[m_pBoardData->getClueRight(col, row)]);
        }

        if(m_pBoardData->getClueDown(col, row) != CLOSED_CLUE) {
            const QRect clueRect{getClueRectDown(cellRect)};
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
    m_curDown = true;

    setFixedSize(m_board_width, m_board_height);

    update();
}
