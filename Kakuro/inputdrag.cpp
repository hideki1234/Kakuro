#include "inputdrag.h"
#include "cosmetic.h"
#include <QPainter>
#include <QtGlobal>
#include <QDebug>
#include <QPalette>
#include <QFont>
#include <QPoint>

InputDrag::InputDrag(KkrBoard *parent)
    : QWidget(parent)
    , m_pParent(parent)
{
    QFont f(FONT_ANS);
    f.setPixelSize(CELL_WIDTH);
    setFont(f);

    QPalette back(palette());
    back.setColor(QPalette::Background, Qt::cyan);
    setPalette(back);
    setAutoFillBackground(true);

    setEnabled(false);
    setVisible(false);
    setFixedHeight(CELL_WIDTH);
    setFixedWidth(CELL_WIDTH);

    connect(this, &InputDrag::cellInput, m_pParent, &KkrBoard::cellInput);
}

void InputDrag::paintEvent(QPaintEvent *)
{
    static const char *digits[] = {
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
    };
    QPainter p(this);

    if(m_newVal != 0)
        p.drawText(rect(), Qt::AlignCenter | Qt::AlignHCenter, digits[m_newVal]);
}

void InputDrag::changeEvent(QEvent *e)
{
    if(e->type() == QEvent::EnabledChange) {
        Q_ASSERT(m_pParent != nullptr);
        if(isEnabled()) {
            grabMouse();
            m_newVal = m_orgVal = m_pParent->getCurrentCellValue();
            QPoint mousePos{mapFromGlobal(QCursor::pos())};
            m_ox = mousePos.x(); m_oy = mousePos.y();
            qDebug() << "InputDrag enabled:" << m_orgVal;
            update();
        }
    }
}

void InputDrag::mouseReleaseEvent(QMouseEvent *)
{
    releaseMouse();
    setVisible(false);
    setEnabled(false);

    emit cellInput(m_newVal);
}

void InputDrag::mouseMoveEvent(QMouseEvent *e)
{
    const int dx = e->x() - m_ox;
    const int dy = e->y() - m_oy;
    int dist = std::sqrt(dx*dx + dy*dy);
    dist /= 15;
    const int newVal = (dist + m_orgVal) % 10;
    if(newVal != m_newVal) {
        m_newVal = newVal;
        qDebug() << m_newVal;
        update();
    }
}
