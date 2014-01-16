#ifndef SALAH_CLOCK_H
#define SALAH_CLOCK_H

#include "core/controls/clock.h"
#include "salah_times.h"

class QPaintEvent;
class SalahClock : public Clock {
    Q_OBJECT
public:
    SalahClock(QWidget* parent, SalahTimes::TimeType t, SalahTimes* times, int minutesPrayerAboutToOver = -1);
    bool isPrayerTime();
    bool isPrayerTimeAboutToOver();
    void refresh();
    inline int minutesPrayerAboutToOver() { return m_minutesPrayerAboutToOver; }
signals:
    void prayerTime(bool);
    void prayerTimeAboutToOver();
protected:
    virtual void paintEvent(QPaintEvent *e) ;
private:
    SalahTimes::TimeType m_timeType;
    SalahTimes* m_times;
    int m_minutesPrayerAboutToOver;
    int m_prayerAboutToOverSignalEmitted;
    
    int minutesForValidity();
};

#endif // SALAH_CLOCK_H
