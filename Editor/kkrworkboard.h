#ifndef KKRWORKBOARD_H
#define KKRWORKBOARD_H

#include <QWidget>
#include <QScrollArea>
#include "kkrboardmanager.h"

class KkrBoardView : public QWidget
{
    Q_OBJECT
    QScrollArea *m_pScrollArea;
    KkrBoardManager *m_pBoardData;

public:
    explicit KkrBoardView(KkrBoardManager *m_pBoardData, QWidget *parent = 0);

    KkrBoardView(const KkrBoardView &) = delete;
    KkrBoardView &operator =(const KkrBoardView &) = delete;

    void setScrollArea(QScrollArea *pSA) {m_pScrollArea=pSA;}

    bool isEdited() const {/*TODO*/return false;}

protected:
    void paintEvent(QPaintEvent *e) override;

signals:

public slots:
};

#endif // KKRWORKBOARD_H
