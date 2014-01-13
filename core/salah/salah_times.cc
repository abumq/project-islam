
#include "core/salah/salah_times.h"
#include "core/utils/datetime.h"
#include "core/logging/logging.h"

SalahTimes::SalahTimes(CalculationMethod calculationMethod,
        JuristicMethod juristicMethod,
        AdjustingMethod adjustingMethod) :
        m_calculationMethod(calculationMethod),
        m_juristicMethod(juristicMethod),
        m_adjustingMethod(adjustingMethod)
{
    build(2014, 01, 13, -35.281983, 149.128718, datetime::getTimeZone(2014, 01, 13));
}

void SalahTimes::build(int year, int month, int day, double latitude, double longitude, double timezone)
{
    m_latitude = latitude;
    m_longitude = longitude;
    m_timezone = timezone;
    m_julianDate = datetime::georgianToJulianDate(year, month, day);
    
    // Default
    double defaultTimes[] = { 5, 6, 12, 13, 18, 18, 18 };
    int i = 0;
    int t = 1;
    do {
        m_times.insert(std::make_pair(static_cast<TimeType>(t), defaultTimes[i++] / 24.0));
        t = t << 1;
    } while (t <= static_cast<int>(kMaxTimeType));
    // assign times after computing
    
}
