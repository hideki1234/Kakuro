#ifndef PLAYSTATUS_H
#define PLAYSTATUS_H

#include <QObject>
#include <QElapsedTimer>
#include <memory>
#include "problemdata.h"

namespace playstatus {

enum Status {
    NODATA,
    READY,
    INPLAY,
    PAUSED,
    DONE
};

class PlayStatus : public QObject
{
    Q_OBJECT

    Status m_status;
    QElapsedTimer m_time;
    qint64 m_timeOffset;

public:
    PlayStatus();

    qint64 getElapsedTime() const;

    // delete unnecessary methods
    PlayStatus(const PlayStatus&) = delete;
    PlayStatus &operator=(const PlayStatus&) = delete;

public slots:
    void updateData(std::shared_ptr<problemdata::ProblemData> pNewData);
    void playPressed();
    void solved();

signals:
    void statusChanged(Status newStatus);
};

}   // namespace playstatus

#endif // PLAYSTATUS_H
