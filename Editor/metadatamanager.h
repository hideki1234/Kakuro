#ifndef METADATAMANAGER_H
#define METADATAMANAGER_H

#include <QObject>
#include <QString>
#include <memory>

class MetaData {
    QString m_author;
    int m_beginner;
    int m_intermed;
    int m_advanced;
    int m_expert;

public:
    MetaData(const QString &author, int begin, int inter, int advan, int expert);

    MetaData(const MetaData&) = delete;
    MetaData &operator=(const MetaData &r) = delete;

    const QString &getAuthor() const {return m_author;}
    int getBeginnerTime() const {return m_beginner;}
    int getIntermediateTime() const {return m_intermed;}
    int getAdvancedTime() const {return m_advanced;}
    int getExpertTime() const {return m_expert;}
};

class MetaDataManager : public QObject
{
    Q_OBJECT

    QString m_author;
    int m_beginner;
    int m_intermediate;
    int m_advanced;
    int m_expert;

    void setAuthor(const QString &newAuthor);
    void setBeginnerTime(int begin);
    void setIntermediateTime(int inter);
    void setAdvancedTime(int advan);
    void setExpertTime(int expert);

public:
    explicit MetaDataManager(QObject *parent = 0);

    MetaDataManager(const MetaDataManager&) = delete;
    MetaDataManager &operator=(const MetaDataManager&) = delete;

    const QString & getAuthor() const {return m_author;}
    int getBeginnerTime() const {return m_beginner;}
    int getIntermediateTime() const {return m_intermediate;}
    int getAdvancedTime() const {return m_advanced;}
    int getExpertTime() const {return m_expert;}

    bool isValid() const;

    std::unique_ptr<MetaData> getMetaData() const;

signals:
    void sigReset();

public slots:
    void slCreate();
    void slRead(std::shared_ptr<const MetaData> data);

friend class MetaView;
};

#endif // METADATAMANAGER_H
