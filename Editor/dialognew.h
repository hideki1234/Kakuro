#ifndef DIALOGNEW_H
#define DIALOGNEW_H

#include <QDialog>
#include <QSpinBox>

class DialogNew : public QDialog
{
    Q_OBJECT
    QSpinBox *m_pCols;
    QSpinBox *m_pRows;

    const static int MIN_CELLS = 2;
    const static int MAX_CELLS = 1000;

public:
    explicit DialogNew(QWidget *parent = nullptr);

    DialogNew(const DialogNew &) = delete;
    DialogNew &operator=(const DialogNew &) = delete;

    int getNumCols() const {return m_pCols->value();}
    int getNumRows() const {return m_pRows->value();}
};

#endif // DIALOGNEW_H
