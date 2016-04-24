#ifndef KKRWORKBOARD_H
#define KKRWORKBOARD_H

#include <QWidget>
#include <QScrollArea>

class KkrBoardView : public QWidget
{
    Q_OBJECT
    QScrollArea *m_pScrollArea;

public:
    explicit KkrBoardView(QWidget *parent = 0);

    void setScrollArea(QScrollArea *pSA) {m_pScrollArea=pSA;}

signals:

public slots:
};

#endif // KKRWORKBOARD_H
