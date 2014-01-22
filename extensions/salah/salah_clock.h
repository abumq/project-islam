#ifndef SALAH_CLOCK_H
#define SALAH_CLOCK_H

#include "core/controls/clock.h"
#include "salah_times.h"

class QPaintEvent;
class SalahClock : public Clock {
    Q_OBJECT
public:
    SalahClock(QWidget* parent, SalahTimes::TimeType t, SalahTimes* times, int minutesPrayerAboutToStart = -1, 
               int minutesPrayerAboutToOver = -1);
    virtual ~SalahClock();
    bool isPrayerTimeAboutToStart(int* minutesLeft);
    bool isPrayerTime();
    bool isPrayerTimeAboutToOver(int* minutesLeft);
    void refresh();
public slots:
    virtual void emitRequiredSignals();
signals:
    void prayerTimeAboutToStart(int);
    void prayerTime(bool);
    void prayerTimeAboutToOver(int);
protected:
    virtual void paintEvent(QPaintEvent *e) ;
private:
    SalahTimes::TimeType m_timeType;
    SalahTimes* m_times;
    int m_minutesPrayerAboutToStart;
    int m_minutesPrayerAboutToOver;
    int m_prayerAboutToStartSignalEmitted;
    int m_prayerAboutToOverSignalEmitted;
    
    int minutesForValidity();
};

#endif // SALAH_CLOCK_H
