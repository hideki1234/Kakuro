#include "metadatamanager.h"

MetaData::MetaData(const QString &author, int begin, int inter, int advan, int expert)
    : m_author(author)
    , m_beginner(begin)
    , m_intermed(inter)
    , m_advanced(advan)
    , m_expert(expert)
{
}

MetaDataManager::MetaDataManager(QObject *parent)
    : QObject(parent)
{
}
