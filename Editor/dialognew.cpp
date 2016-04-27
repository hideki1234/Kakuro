#include "dialognew.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

DialogNew::DialogNew(QWidget *parent)
    : QDialog(parent)
    , m_pCols(nullptr)
    , m_pRows(nullptr)
{
    m_pCols = new QSpinBox;
    m_pRows = new QSpinBox;
    m_pCols->setMinimum(MIN_CELLS);
    m_pCols->setMaximum(MAX_CELLS);
    m_pRows->setMinimum(MIN_CELLS);
    m_pRows->setMaximum(MAX_CELLS);

    // label
    QVBoxLayout *pVbl = new QVBoxLayout;
    QLabel *pLbl = new QLabel(tr("New size:"));
    pLbl->setAlignment(Qt::AlignLeft);
    pVbl->addWidget(pLbl);

    // input fields
    QHBoxLayout *pHbl = new QHBoxLayout;
    pLbl = new QLabel(" X ");
    pHbl->addStretch();
    pHbl->addWidget(m_pCols);
    pHbl->addWidget(pLbl);
    pHbl->addWidget(m_pRows);
    pVbl->addLayout(pHbl);

    // Blank
    pVbl->addSpacing(20);
    pVbl->addStretch();

    // buttons
    QHBoxLayout *pHbl2 = new QHBoxLayout;
    QPushButton *pBtnCreate = new QPushButton{tr("Create")};
    QPushButton *pBtnCancel = new QPushButton{tr("Cancel")};
    pHbl2->addStretch();
    pHbl2->addWidget(pBtnCreate);
    pHbl2->addWidget(pBtnCancel);
    pVbl->addLayout(pHbl2);

    connect(pBtnCreate, &QPushButton::clicked, this, &QDialog::accept);
    connect(pBtnCancel, &QPushButton::clicked, this, &QDialog::reject);

    setLayout(pVbl);
    setWindowTitle(tr("Kakuro New"));
}
