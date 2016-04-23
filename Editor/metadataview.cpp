#include "metadataview.h"
#include <QLabel>
#include <QIntValidator>

std::pair<QSpinBox *, QSpinBox *>
MetaDataView::makeTimeEdits(const QString &labelText, QBoxLayout *pBL)
{
    // input line layout
    auto pBLLine = new QBoxLayout{QBoxLayout::LeftToRight};

    // level label
    auto pLbl = new QLabel{labelText, this};
    pLbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pBLLine->addWidget(pLbl);

    pBLLine->addStretch();

    // minute
    QFontMetrics metrics(font());

    auto pEditMin = new QSpinBox{this};
    pEditMin->setMinimum(0);
    pEditMin->setMaximum(9999);
    pEditMin->setFixedWidth(metrics.width("8888") + SPIN_WIDTH);
    pEditMin->setAlignment(Qt::AlignRight);
    pBLLine->addWidget(pEditMin);

    pLbl = new QLabel{":"};
    pBLLine->addWidget(pLbl);

    // second
    auto pEditSec = new QSpinBox{this};
    pEditSec->setMinimum(0);
    pEditSec->setMaximum(59);
    pEditSec->setFixedWidth(metrics.width("88") + SPIN_WIDTH);
    pEditSec->setAlignment(Qt::AlignHCenter);
    pBLLine->addWidget(pEditSec);

    pBL->addLayout(pBLLine);

    return std::make_pair(pEditMin, pEditSec);
}

MetaDataView::MetaDataView(MetaDataManager *metaData, QWidget *parent)
    : QWidget(parent)
    , m_pMetaData(metaData)
{
    // backgournd color
    QPalette palBack(palette());
    palBack.setColor(QPalette::Background, Qt::white);
    setPalette(palBack);
    setAutoFillBackground(true);

    QBoxLayout *pBLTop = new QBoxLayout{QBoxLayout::TopToBottom, this};

    // Author
    QLabel *pLbl;
    pLbl = new QLabel{tr("Author:"), this};
    pLbl->setAlignment(Qt::AlignLeft);
    pLbl->setStyleSheet("font-weight:bold");
    pBLTop->addWidget(pLbl);

    m_pEditAuthor = new QLineEdit{this};
    m_pEditAuthor->setPlaceholderText(tr("Enter author's name"));
    m_pEditAuthor->setMaxLength(255);
    pBLTop->addWidget(m_pEditAuthor);

    pBLTop->addSpacing(20);

    // target times
    pLbl = new QLabel{tr("Target Times"), this};
    pLbl->setAlignment(Qt::AlignLeft);
    pLbl->setStyleSheet("font-weight:bold");
    pBLTop->addWidget(pLbl);

    // beginner
    auto timeEdits = makeTimeEdits(tr("Beginner:"), pBLTop);
    m_pEditBegMin = timeEdits.first;
    m_pEditBegSec = timeEdits.second;

    // intermediate
    timeEdits = makeTimeEdits(tr("Intermediate:"), pBLTop);
    m_pEditIntMin = timeEdits.first;
    m_pEditIntSec = timeEdits.second;

    // advanced
    timeEdits = makeTimeEdits(tr("Advanced:"), pBLTop);
    m_pEditAdvMin = timeEdits.first;
    m_pEditAdvSec = timeEdits.second;

    // expert
    timeEdits = makeTimeEdits(tr("Expert:"), pBLTop);
    m_pEditExpMin = timeEdits.first;
    m_pEditExpSec = timeEdits.second;

    // stretch
    pBLTop->addStretch();

    setLayout(pBLTop);
    setMinimumWidth(120);
}
