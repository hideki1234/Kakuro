#ifndef KKREDITMAIN_H
#define KKREDITMAIN_H

#include <QMainWindow>
#include <QScrollArea>
#include "kkrworkboard.h"

class KkrEditMain : public QMainWindow
{
    Q_OBJECT

    // main board
    QScrollArea *m_pScrollBoard;
    KkrWorkBoard *m_pWorkBoard;

public:
    KkrEditMain(QWidget *parent = 0);
    ~KkrEditMain();
};

#endif // KKREDITMAIN_H
