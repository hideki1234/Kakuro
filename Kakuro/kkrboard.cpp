#include "kkrboard.h"
#include <QPainter>
#include <QPalette>
#include <QDebug>
#include <QFont>
#include <array>

KkrBoard::KkrBoard(QWidget *parent)
    : QWidget(parent)
    , m_fontAns("consolas")
    , m_fontClue("consolas")
{
    m_fontAns.setPixelSize(CELL_WIDTH);
    m_fontClue.setPixelSize(CLUE_WIDTH);
}

void KkrBoard::updateData(std::shared_ptr<problemdata::ProblemData> pNewData)
{
    m_pData = pNewData;

    // calculate sizes
    m_inner_width = (CELL_WIDTH + BORDER_THICK) * m_pData->getNumCols() - BORDER_THICK;
    m_inner_height = (CELL_WIDTH + BORDER_THICK) * m_pData->getNumRows() - BORDER_THICK;
    m_frame_width = 2*FRAME_THICK + m_inner_width;
    m_frame_height = 2*FRAME_THICK + m_inner_height;
    m_board_width = 2*MARGIN + m_frame_width;
    m_board_height = 2*MARGIN + m_frame_height;

    setFixedSize(m_board_width, m_board_height);

    update();
}

QRect KkrBoard::getCellRect(int col, int row) const
{
    const int x = MARGIN + FRAME_THICK + col * (CELL_WIDTH + BORDER_THICK);
    const int y = MARGIN + FRAME_THICK + row * (CELL_WIDTH + BORDER_THICK);
    return QRect(x, y, CELL_WIDTH, CELL_WIDTH);
}

QRect KkrBoard::getClueRectRight(const QRect &cellRect) const
{
    const QPoint topRight{cellRect.topRight()};
    const int x = topRight.x() - CLUE_WIDTH - 1;
    const int y = topRight.y() + 2;
    return QRect(x, y, CLUE_WIDTH, CLUE_WIDTH);
}

QRect KkrBoard::getClueRectDown(const QRect &cellRect) const
{
    const QPoint bottomLeft{cellRect.bottomLeft()};
    const int x = bottomLeft.x() + 2;
    const int y = bottomLeft.y() - CLUE_WIDTH - 1;
    return QRect(x, y, CLUE_WIDTH, CLUE_WIDTH);
}

void KkrBoard::drawCell(QPainter &p, int col, int row, bool bDrawValue) const
{
    static const char * digits[] = {
        "0",  "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",
        "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
        "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
        "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
        "40", "41", "42", "43", "44", "45"
    };
    const QRect cellRect{getCellRect(col, row)};
    switch(m_pData->getCellType(col, row)) {
    case pd::CellType::CellAnswer:
        if(!bDrawValue)
            break;
        p.setFont(m_fontAns);
        p.drawText(cellRect, Qt::AlignCenter | Qt::AlignHCenter, digits[m_pData->getAnswer(col, row)]);
        break;
    case pd::CellType::CellClue:
    {
        p.setBrush(Qt::SolidPattern);
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
        if(m_pData->getClueRight(col, row) != pd::CLOSED_CLUE) {
            const QRect clueRect{getClueRectRight(cellRect)};
            p.fillRect(clueRect, brWhite);
            if(bDrawValue) {
                 p.setFont(m_fontClue);
                 p.drawText(clueRect, Qt::AlignCenter | Qt::AlignHCenter,
                            digits[m_pData->getClueRight(col, row)]);
            }
        }

        if(m_pData->getClueDown(col, row) != pd::CLOSED_CLUE) {
            const QRect clueRect{getClueRectDown(cellRect)};
            p.fillRect(clueRect, brWhite);
            if(bDrawValue) {
                 p.setFont(m_fontClue);
                 p.drawText(clueRect, Qt::AlignCenter | Qt::AlignHCenter,
                            digits[m_pData->getClueDown(col, row)]);
            }
        }
    }
        break;
    default:
        Q_ASSERT(false);
    }
}

void KkrBoard::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    // frame
    p.fillRect(MARGIN, MARGIN, m_frame_width, FRAME_THICK, Qt::SolidPattern);
    p.fillRect(MARGIN, MARGIN+m_frame_height-FRAME_THICK, m_frame_width, FRAME_THICK, Qt::SolidPattern);
    p.fillRect(MARGIN, MARGIN, FRAME_THICK, m_frame_height, Qt::SolidPattern);
    p.fillRect(MARGIN+m_frame_width-FRAME_THICK, MARGIN, FRAME_THICK, m_frame_height, Qt::SolidPattern);

    // cell borders
    const int TL = MARGIN + FRAME_THICK;
    for(int yi = 1; yi < m_pData->getNumRows(); ++yi) {
        const int y = TL - BORDER_THICK + yi * (BORDER_THICK+CELL_WIDTH);
        p.fillRect(TL, y, m_inner_width, BORDER_THICK, Qt::SolidPattern);
    }

    for(int xi = 1; xi < m_pData->getNumCols(); ++xi) {
        const int x = TL - BORDER_THICK + xi * (BORDER_THICK+CELL_WIDTH);
        p.fillRect(x, TL, BORDER_THICK, m_inner_height, Qt::SolidPattern);
    }

    // cell contents
    for(int y = 0; y < m_pData->getNumRows(); ++y) {
        for(int x = 0; x < m_pData->getNumCols(); ++x) {
            drawCell(p, x, y, true);
        }
    }
}
