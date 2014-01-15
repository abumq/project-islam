#include "salah_clock.h"
#include <QTime>
SalahClock::SalahClock(QWidget* parent, SalahTimes::TimeType t, SalahTimes* times) : 
    Clock(parent),
    m_timeType(t),
    m_times(times)
{
    resize(200);
    if (t == SalahTimes::TimeType::Fajr) {
        setTitle("Fajr");
    } else if (t == SalahTimes::TimeType::Sunrise) {
        setTitle("Sunrise");
    } else if (t == SalahTimes::TimeType::Dhuhr) {
        setTitle("Dhuhr");
    } else if (t == SalahTimes::TimeType::Asr) {
        setTitle("Asr");
    } else if (t == SalahTimes::TimeType::Sunset) {
        setTitle("Sunset");
    } else if (t == SalahTimes::TimeType::Maghrib) {
        setTitle("Maghrib");
    } else  if (t == SalahTimes::TimeType::Isha) {
        setTitle("Isha");
    }
    setDisplayTextualTime(true);
    std::pair<int, int> tPair = m_times->readTimeHourMinutePair(t);
    setTime(tPair.first, tPair.second);
}

void SalahClock::paintEvent(QPaintEvent *e) 
{
    Clock::paintEvent(e);
    if (!selected() && isPrayerTime()) {
        emit prayerTime(true);
    } else if (selected() && !isPrayerTime()) {
        emit prayerTime(false);
    }
}

int SalahClock::minutesForValidity() 
{
    // TODO: Get these configurable i.e, 
    //         * Sunrise offset for Fajr validity
    //         * Sunset offset for Maghrib validity
    //         * Maghrib valid for
    if (m_timeType == SalahTimes::TimeType::Fajr) {
        std::pair<int, int> hmPair = m_times->readTimeHourMinutePair(SalahTimes::TimeType::Sunrise);
        int h = hmPair.first;
        int m = hmPair.second;
        return (QTime::currentTime().secsTo(QTime(h, m)) / 60) - 5; // Until (sunrise - 5 minutes)
    } else if (m_timeType == SalahTimes::TimeType::Dhuhr) {
        std::pair<int, int> hmPair = m_times->readTimeHourMinutePair(SalahTimes::TimeType::Asr);
        int h = hmPair.first;
        int m = hmPair.second;
        return (QTime::currentTime().secsTo(QTime(h, m)) / 60) - 5; // Until (asr - 5 minutes)
    } else if (m_timeType == SalahTimes::TimeType::Asr) {
        std::pair<int, int> hmPair = m_times->readTimeHourMinutePair(SalahTimes::TimeType::Sunset);
        int h = hmPair.first;
        int m = hmPair.second;
        return (QTime::currentTime().secsTo(QTime(h, m)) / 60) - 15; // Until (sunset - 15 minutes (zawal))
    } else if (m_timeType == SalahTimes::TimeType::Maghrib) {
        return 30; // Configurable
    } else  if (m_timeType == SalahTimes::TimeType::Isha) {
        return QTime::currentTime().secsTo(QTime(23, 59, 59)) / 60; // Until midnight
    }
    return 5; // Sunset and sunrise valid for 5 minutes
}

bool SalahClock::isPrayerTime() 
{
    if (m_live) {
        return false;
    }
    int currentPrayerValidFor = minutesForValidity() * 60; // seconds
    if (currentPrayerValidFor < 0) {
        return false;
    } else {
        int prayerValidSince = QTime::currentTime().secsTo(QTime(m_h, m_m));
        return prayerValidSince <= 0 && prayerValidSince <= currentPrayerValidFor;
    }
}
