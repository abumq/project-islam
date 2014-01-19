#include "salah_clock.h"
#include <QTime>
SalahClock::SalahClock(QWidget* parent, SalahTimes::TimeType t, SalahTimes* times, int minutesPrayerAboutToStart, int minutesPrayerAboutToOver) : 
    Clock(parent),
    m_timeType(t),
    m_times(times),
    m_prayerAboutToStartSignalEmitted(false),
    m_minutesPrayerAboutToStart(minutesPrayerAboutToStart),
    m_minutesPrayerAboutToOver(minutesPrayerAboutToOver)
{
    resize(200);
    setDisplayTextualTime(true);
    refresh();
}

void SalahClock::paintEvent(QPaintEvent *e) 
{
    Clock::paintEvent(e);
    // TODO: Ability to configurable prayerAboutToStart() reminder for each prayer
    //       and separately for Jumuah as well
    int minutesLeftStart;
    if (!m_prayerAboutToStartSignalEmitted && isPrayerTimeAboutToStart(&minutesLeftStart)) {
        emit prayerTimeAboutToStart(minutesLeftStart);
        m_prayerAboutToStartSignalEmitted = true;
    }
    if (!selected() && isPrayerTime()) {
        // reset it for next day
        m_prayerAboutToStartSignalEmitted = false;
        m_prayerAboutToOverSignalEmitted = false;
        emit prayerTime(true);
    } else if (selected() && !isPrayerTime()) {
        // reset it for next day
        m_prayerAboutToStartSignalEmitted = false;
        m_prayerAboutToOverSignalEmitted = false;
        emit prayerTime(false);
    }
    int minutesLeftEnd;
    if (!m_prayerAboutToOverSignalEmitted && isPrayerTimeAboutToOver(&minutesLeftEnd)) {
        emit prayerTimeAboutToOver(minutesLeftEnd);
        m_prayerAboutToOverSignalEmitted = true;
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
        return (QTime::currentTime().secsTo(QTime(h, m)) / 60) - 25; // Until (sunset - 25 minutes (zawal))
    } else if (m_timeType == SalahTimes::TimeType::Maghrib) {
        return (QTime::currentTime().secsTo(QTime(m_h, m_m)) / 60) + 30;
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

bool SalahClock::isPrayerTimeAboutToStart(int *minutesLeft)
{
    if (m_live || m_minutesPrayerAboutToStart <= 0) {
        return false;
    }
    *minutesLeft = QTime::currentTime().secsTo(QTime(m_h, m_m)) / 60;
    if (*minutesLeft <= 0) {
        return false; // already passed prayer
    }
    return !isPrayerTime() && (*minutesLeft <= m_minutesPrayerAboutToStart);
}

bool SalahClock::isPrayerTimeAboutToOver(int* minutesLeft)
{
    if (m_live || m_minutesPrayerAboutToOver <= 0) {
        return false;
    }
    *minutesLeft = minutesForValidity();
    return isPrayerTime() && (*minutesLeft <= m_minutesPrayerAboutToOver);
}

void SalahClock::refresh()
{
    if (m_timeType == SalahTimes::TimeType::Fajr) {
        setTitle("Fajr");
    } else if (m_timeType == SalahTimes::TimeType::Sunrise) {
        setTitle("Sunrise");
    } else if (m_timeType == SalahTimes::TimeType::Dhuhr) {
        setTitle(QDate::currentDate().dayOfWeek() == 5 ? "Jumu'ah" : "Dhuhr");
    } else if (m_timeType == SalahTimes::TimeType::Asr) {
        setTitle("Asr");
    } else if (m_timeType == SalahTimes::TimeType::Sunset) {
        setTitle("Sunset");
    } else if (m_timeType == SalahTimes::TimeType::Maghrib) {
        setTitle("Maghrib");
    } else  if (m_timeType == SalahTimes::TimeType::Isha) {
        setTitle("Isha");
    }
    std::pair<int, int> tPair = m_times->readTimeHourMinutePair(m_timeType);
    setTime(tPair.first, tPair.second);
}
