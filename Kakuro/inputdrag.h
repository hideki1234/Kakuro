#ifndef INPUTDRAG_H
#define INPUTDRAG_H

#include <QWidget>
#include "inputfactory.h"

class InputDrag : public QWidget
{
    Q_OBJECT

    KkrBoard *m_pParent;
    int m_orgVal;
    int m_ox, m_oy;
    int m_newVal;

public:
    explicit InputDrag(KkrBoard *parent = 0);

public:
    void paintEvent(QPaintEvent *) override;
    void changeEvent(QEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *e) override;

signals:
    void cellInput(int value);

public slots:
};

#endif // INPUTDRAG_H
