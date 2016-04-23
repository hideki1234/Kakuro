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

    // connect with MetaDataManager
    connect(m_pMetaData, &MetaDataManager::sigReset, this, &MetaDataView::slModelChanged);

    // connect with edit controls
    connect(m_pEditAuthor, &QLineEdit::textEdited, this, &MetaDataView::slAuthorChanged);
    connect(m_pEditBegMin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MetaDataView::slBeginnerChanged);
    connect(m_pEditBegSec, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MetaDataView::slBeginnerChanged);
    connect(m_pEditIntMin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MetaDataView::slIntermediateChanged);
    connect(m_pEditIntSec, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MetaDataView::slIntermediateChanged);
    connect(m_pEditAdvMin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MetaDataView::slAdvancedChanged);
    connect(m_pEditAdvSec, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MetaDataView::slAdvancedChanged);
    connect(m_pEditExpMin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MetaDataView::slExpertChanged);
    connect(m_pEditExpSec, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MetaDataView::slExpertChanged);
}

/*
 * slots
 */
void MetaDataView::slModelChanged()
{
    m_pEditAuthor->setText(m_pMetaData->getAuthor());

    auto tSeconds = m_pMetaData->getBeginnerTime();
    m_pEditBegMin->setValue(tSeconds/60);
    m_pEditBegSec->setValue(tSeconds%60);

    tSeconds = m_pMetaData->getIntermediateTime();
    m_pEditIntMin->setValue(tSeconds/60);
    m_pEditIntSec->setValue(tSeconds%60);

    tSeconds = m_pMetaData->getAdvancedTime();
    m_pEditAdvMin->setValue(tSeconds/60);
    m_pEditAdvSec->setValue(tSeconds%60);

    tSeconds = m_pMetaData->getExpertTime();
    m_pEditExpMin->setValue(tSeconds/60);
    m_pEditExpSec->setValue(tSeconds%60);
}

void MetaDataView::slAuthorChanged(const QString &author)
{
    m_pMetaData->setAuthor(author);
}

void MetaDataView::slBeginnerChanged(int)
{
    const int tSeconds = m_pEditBegMin->value()*60 + m_pEditBegSec->value();
    if(m_pMetaData->getBeginnerTime() != tSeconds)
        m_pMetaData->setBeginnerTime(tSeconds);
}

void MetaDataView::slIntermediateChanged(int)
{
    const int tSeconds = m_pEditIntMin->value()*60 + m_pEditIntSec->value();
    if(m_pMetaData->getIntermediateTime() != tSeconds)
        m_pMetaData->setIntermediateTime(tSeconds);
}

void MetaDataView::slAdvancedChanged(int)
{
    const int tSeconds = m_pEditAdvMin->value()*60 + m_pEditAdvSec->value();
    if(m_pMetaData->getAdvancedTime() != tSeconds)
        m_pMetaData->setAdvancedTime(tSeconds);
}

void MetaDataView::slExpertChanged(int)
{
    const int tSeconds = m_pEditExpMin->value()*60 + m_pEditExpSec->value();
    if(m_pMetaData->getExpertTime() != tSeconds)
        m_pMetaData->setExpertTime(tSeconds);
}
