#ifndef SALAH_CLOCK_H
#define SALAH_CLOCK_H

#include "core/controls/clock.h"
#include "salah_times.h"

class QPaintEvent;
class SalahClock : public Clock {
    Q_OBJECT
public:
    SalahClock(QWidget* parent, SalahTimes::TimeType t, SalahTimes* times);
    bool isPrayerTime();
signals:
    void prayerTime(bool);
protected:
    virtual void paintEvent(QPaintEvent *e) ;
private:
    SalahTimes::TimeType m_timeType;
    SalahTimes* m_times;
    
    int minutesForValidity();
};

#endif // SALAH_CLOCK_H
