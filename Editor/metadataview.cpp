#include "metadataview.h"

MetaDataView::MetaDataView(MetaDataManager *metaData, QWidget *parent)
    : QWidget(parent)
    , m_pMetaData(metaData)
{
    setMinimumWidth(120);
}
