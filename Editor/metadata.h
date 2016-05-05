#ifndef METADATA_H
#define METADATA_H

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

#endif // METADATA_H
