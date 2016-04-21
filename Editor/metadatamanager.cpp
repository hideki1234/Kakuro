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

void MetaDataManager::setAuthor(const QString &newAuthor)
{
    m_author = newAuthor;
}

void MetaDataManager::setBeginnerTime(int begin)
{
    m_beginner = begin;
}

void MetaDataManager::setIntermediateTime(int inter)
{
    m_intermediate = inter;
}

void MetaDataManager::setAdvancedTime(int advan)
{
    m_advanced = advan;
}

void MetaDataManager::setExpertTime(int expert)
{
    m_expert = expert;
}

bool MetaDataManager::isValid() const
{
    if(m_beginner < m_intermediate)
        return false;
    if(m_beginner == m_intermediate && m_intermediate != 0)
        return false;

    if(m_intermediate < m_advanced)
        return false;
    if(m_intermediate == m_advanced && m_advanced != 0)
        return false;

    if(m_advanced < m_expert)
        return false;
    if(m_advanced == m_expert && m_expert != 0)
        return false;
    if(m_expert < 0)
        return false;

    return true;
}

std::unique_ptr<MetaData> MetaDataManager::getMetaData() const
{
    MetaData * pMd = (isValid()
                      ? new MetaData{m_author, m_beginner, m_intermediate, m_advanced, m_expert}
                      : nullptr);
    return std::unique_ptr<MetaData>(pMd);
}

void MetaDataManager::slCreate()
{
    m_author = "";
    m_beginner = m_intermediate = m_advanced = m_expert = 0;

    emit sigReset();
}

void MetaDataManager::slRead(std::shared_ptr<const MetaData> data)
{
    m_author = data->getAuthor();
    m_beginner = data->getBeginnerTime();
    m_intermediate = data->getIntermediateTime();
    m_advanced = data->getAdvancedTime();
    m_expert = data->getExpertTime();

    emit sigReset();
}
