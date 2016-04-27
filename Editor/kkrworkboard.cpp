#include "kkrworkboard.h"
#include <QPainter>

KkrBoardView::KkrBoardView(KkrBoardManager *pBoardData, QWidget *parent)
    : QWidget(parent)
    , m_pBoardData(pBoardData)
{
    setMinimumHeight(200);
    setMinimumWidth(400);
}

void KkrBoardView::paintEvent(QPaintEvent * /*e*/)
{
    QPainter pt{this};

    // TODO
    const auto H = height();
    pt.drawText(0, H/2, QString::asprintf("%d x %d", m_pBoardData->getNumCols(), m_pBoardData->getNumRows()));

    const auto C = m_pBoardData->getNumCols();
    if(C < 3)
        return;
}
