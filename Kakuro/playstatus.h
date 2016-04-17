#ifndef PLAYSTATUS_H
#define PLAYSTATUS_H

#include <QObject>
#include <QElapsedTimer>
#include <memory>
#include "problemdata.h"

namespace playstatus {

enum class Status {
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
    bool m_solved;

    void done();

public:
    PlayStatus();

    qint64 getElapsedTime() const;
    bool isSolved() const {return m_solved;}
    Status status() const {return m_status;}

    // delete unnecessary methods
    PlayStatus(const PlayStatus&) = delete;
    PlayStatus &operator=(const PlayStatus&) = delete;

public slots:
    void updateProblem(std::shared_ptr<problemdata::ProblemData> pNewData);
    void playPressed();
    void solved();
    void giveup();

signals:
    void statusChanged(Status newStatus);
};

}   // namespace playstatus

#endif // PLAYSTATUS_H
