#ifndef METADATAVIEW_H
#define METADATAVIEW_H

#include <QWidget>
#include "metadatamanager.h"

class MetaDataView : public QWidget
{
    Q_OBJECT

    MetaDataManager *m_pMetaData;

public:
    explicit MetaDataView(MetaDataManager *metaData, QWidget *parent = 0);

signals:

public slots:
};

#endif // METADATAVIEW_H
