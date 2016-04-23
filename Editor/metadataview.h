#ifndef METADATAVIEW_H
#define METADATAVIEW_H

#include <QWidget>
#include "metadatamanager.h"
#include <QBoxLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <utility>

class MetaDataView : public QWidget
{
    Q_OBJECT

    /*
     * dimension
     */
    static const int SPIN_WIDTH = 25;

    // model (owned by Main)
    MetaDataManager *m_pMetaData;

    /*
     * input controls
     */
    // author
    QLineEdit *m_pEditAuthor;

    // beginner target time
    QSpinBox *m_pEditBegMin;
    QSpinBox *m_pEditBegSec;

    // intermediate target time
    QSpinBox *m_pEditIntMin;
    QSpinBox *m_pEditIntSec;

    // advanced target time
    QSpinBox *m_pEditAdvMin;
    QSpinBox *m_pEditAdvSec;

    // expert target time
    QSpinBox *m_pEditExpMin;
    QSpinBox *m_pEditExpSec;

    /*
     * initializations
     */
    std::pair<QSpinBox *, QSpinBox *> makeTimeEdits(const QString &labelText, QBoxLayout *pBL);

public:
    explicit MetaDataView(MetaDataManager *metaData, QWidget *parent = 0);

signals:

public slots:
    // from MetaDataManager
    void slModelChanged();

    // from edit controls
    void slAuthorChanged(const QString &author);
    void slBeginnerChanged(int);
    void slIntermediateChanged(int);
    void slAdvancedChanged(int);
    void slExpertChanged(int);
};

#endif // METADATAVIEW_H
