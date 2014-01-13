
#include "salah_times.h"
#include "core/utils/datetime.h"
#include "core/logging/logging.h"

SalahMethod::SalahMethod(double fajrAngle, bool maghribIsMinutes, 
                         double maghribValue, bool ishaIsMinutes, double ishaValue) :
    m_fajrAngle(fajrAngle), m_maghribIsMinutes(maghribIsMinutes),
    m_maghribValue(maghribValue), m_ishaIsMinutes(ishaIsMinutes),
    m_ishaValue(ishaValue)
{
}


SalahTimes::SalahTimes(CalculationMethod calculationMethod,
                       JuristicMethod juristicMethod,
                       AdjustingMethod adjustingMethod) :
    m_calculationMethod(calculationMethod),
    m_juristicMethod(juristicMethod),
    m_adjustingMethod(adjustingMethod)
{
    m_salahMethods.insert(std::make_pair(CalculationMethod::Jafari, SalahMethod(16.0, false, 4.0, false, 14.0)));
    m_salahMethods.insert(std::make_pair(CalculationMethod::Karachi, SalahMethod(18.0, true,  0.0, false, 18.0)));
    m_salahMethods.insert(std::make_pair(CalculationMethod::ISNA, SalahMethod(15.0, true,  0.0, false, 15.0)));
    m_salahMethods.insert(std::make_pair(CalculationMethod::MWL, SalahMethod(18.0, true,  0.0, false, 17.0)));
    m_salahMethods.insert(std::make_pair(CalculationMethod::Makkah, SalahMethod(19.0, true,  0.0, true,  90.0)));
    m_salahMethods.insert(std::make_pair(CalculationMethod::Egypt, SalahMethod(19.5, true,  0.0, false, 17.5)));
    m_salahMethods.insert(std::make_pair(CalculationMethod::Custom, SalahMethod(18.0, true,  0.0, false, 17.0)));
    time_t date = time(NULL);
    tm* t = localtime(&date);
    build(1900 + t->tm_year, t->tm_mon + 1, t->tm_mday, -35.281983, 149.128718, 
          datetime::getTimeZone(1900 + t->tm_year, t->tm_mon + 1, t->tm_mday));
}

void SalahTimes::build(int year, int month, int day, double latitude, double longitude, double timezone)
{
    LOG(INFO) << day << "/" << month << "/" << year << " @ " << latitude << ":" << longitude << "; timezone: " << timezone;
    
    m_latitude = latitude;
    m_longitude = longitude;
    m_timezone = timezone;
    m_julianDate = datetime::georgianToJulianDate(year, month, day);
    
    // Default values
    double defaultTimes[] = { 5, 6, 12, 13, 18, 18, 18 };
    int i = 0;
    int t = 1;
    do {
        m_times.insert(std::make_pair(static_cast<TimeType>(t), defaultTimes[i++] / 24.0));
        t = t << 1;
    } while (t <= static_cast<int>(kMaxTimeType));
    // Assign times after computing
    m_times[TimeType::Fajr] = computePrayerTime(180.0 - m_salahMethods.at(m_calculationMethod).m_fajrAngle, m_times[TimeType::Fajr]);
    m_times[TimeType::Sunrise] = computePrayerTime(180.0 - 0.833, m_times[TimeType::Sunrise]);
    m_times[TimeType::Dhuhr] = computeMidDay(m_times[TimeType::Dhuhr]);
    m_times[TimeType::Asr] = computeAsr(m_times[TimeType::Asr]);
    m_times[TimeType::Sunset] = computePrayerTime(0.833, m_times[TimeType::Sunset]);
    m_times[TimeType::Maghrib] = computePrayerTime(m_salahMethods.at(m_calculationMethod).m_maghribValue, m_times[TimeType::Maghrib]);
    m_times[TimeType::Isha] = computePrayerTime(m_salahMethods.at(m_calculationMethod).m_ishaValue, m_times[TimeType::Isha]);
    adjustTimes();
}

const std::map<SalahTimes::TimeType, double>*SalahTimes::times() const
{
    return &m_times;
}

std::string SalahTimes::readTime(SalahTimes::TimeType t)
{
    double _time = m_times[t];
    if (isnan(_time)) {
        return std::string();
    }
    int hours, minutes;
    _time = fixHour(_time + 0.5 / 60);
    hours = floor(_time);
    minutes = floor((_time - hours) * 60);
    std::stringstream ss;
    ss << hours << ":" << minutes;
    return ss.str();
}

double SalahTimes::equationOfTime(double julianDate)
{
    return sunPosition(julianDate).second;
}

double SalahTimes::sunDeclination(double julianDate)
{
    return sunPosition(julianDate).first;
}

double SalahTimes::computeMidDay(double currTime)
{
    double t = equationOfTime(m_julianDate + currTime);
    double z = fixHour(12 - t);
    return z;
}

double SalahTimes::computeAsr(double currTime)
{
    double d = sunDeclination(m_julianDate + currTime);
    double g = -darccot(static_cast<int>(m_juristicMethod) + dtan(fabs(m_latitude - d)));
    return computePrayerTime(g, currTime);
}

void SalahTimes::adjustTimes()
{
    int t = 1;
    do {
        m_times[static_cast<TimeType>(t)] += m_timezone - m_longitude / 15.0;
        t = t << 1;
    } while (t <= static_cast<int>(kMaxTimeType));
    const double kDhuharMinutesOffset = 0.0;
    m_times[TimeType::Dhuhr] += kDhuharMinutesOffset / 60.0;
    if (m_salahMethods[m_calculationMethod].m_maghribIsMinutes) {
        m_times[TimeType::Maghrib] = m_times[TimeType::Sunset] 
                + m_salahMethods[m_calculationMethod].m_maghribValue / 60.0;
    }
    if (m_salahMethods[m_calculationMethod].m_ishaIsMinutes) {
        m_times[TimeType::Isha] = m_times[TimeType::Maghrib] 
                + m_salahMethods[m_calculationMethod].m_ishaValue / 60.0;
    }
    if (m_adjustingMethod != AdjustingMethod::None) {
        adjustHighLatTimes();
    }
}

void SalahTimes::adjustHighLatTimes()
{
    auto timeDiff = [&](double t1, double t2) {
        return fixHour(t2 - t1);
    };
    
    double nightTime = timeDiff(m_times[TimeType::Sunset], m_times[TimeType::Sunrise]);
    
    // Fajr
    double fajrDiff = nightPortion(m_salahMethods[m_calculationMethod].m_fajrAngle) * nightTime;
    if (isnan(m_times[TimeType::Fajr]) || timeDiff(m_times[TimeType::Fajr], m_times[TimeType::Sunrise]) > fajrDiff) {
        m_times[TimeType::Fajr] = m_times[TimeType::Sunrise] - fajrDiff;
    }
    
    // Isha
    double ishaAngle = m_salahMethods[m_calculationMethod].m_ishaIsMinutes ? 18.0 : m_salahMethods[m_calculationMethod].m_ishaValue;
    double ishaDiff = nightPortion(ishaAngle) * nightTime;
    if (isnan(m_times[TimeType::Isha]) || timeDiff(m_times[TimeType::Sunset], m_times[TimeType::Isha]) > ishaDiff) {
        m_times[TimeType::Isha] = m_times[TimeType::Sunset] + ishaDiff;
    }
    
    // Maghrib
    double maghribAngle = m_salahMethods[m_calculationMethod].m_maghribIsMinutes ? 4.0 : m_salahMethods[m_calculationMethod].m_maghribValue;
    double maghribDiff = nightPortion(maghribAngle) * nightTime;
    if (isnan(m_times[TimeType::Maghrib]) || timeDiff(m_times[TimeType::Sunset], m_times[TimeType::Maghrib]) > maghribDiff) {
        m_times[TimeType::Maghrib] = m_times[TimeType::Sunset] + maghribDiff;
    }
}

double SalahTimes::nightPortion(double angle)
{
    if (m_adjustingMethod == AdjustingMethod::AngleBased) {
        return angle / 60.0;
    }
    if (m_adjustingMethod == AdjustingMethod::MidNight) {
        return 1.0 / 2.0;
    }
    if (m_adjustingMethod == AdjustingMethod::OneSeventh) {
        return 1.0 / 7.0;
    }
    return 0; // Should never reach here
}

std::pair<double, double> SalahTimes::sunPosition(double julianDate)
{
    double d = julianDate - 2451545.0;
    double g = fixAngle(357.529 + 0.98560028 * d);
    double q = fixAngle(280.459 + 0.98564736 * d);
    double l = fixAngle(q + 1.915 * dsin(g) + 0.020 * dsin(2 * g));
    double e = 23.439 - 0.00000036 * d;
    
    double dd = darcsin(dsin(e) * dsin(l));
    double ra = darctan2(dcos(e) * dsin(l), dcos(l)) / 15.0;
    ra = fixHour(ra);
    double eq_t = q / 15.0 - ra;
    
    return std::pair<double, double>(dd, eq_t);
}

double SalahTimes::computePrayerTime(double angle, double currTime)
{
    double d = sunDeclination(m_julianDate + currTime);
    double z = computeMidDay(currTime);
    double v = 1.0 / 15.0 * darccos(
                (-dsin(angle) - dsin(d) * dsin(m_latitude)) / (dcos(d) * dcos(m_latitude)));
    return z + (angle > 90.0 ? -v : v);
}
