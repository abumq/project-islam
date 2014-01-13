#include "salah_times.h"
#include "core/utils/datetime.h"

SalahTimes::SalahTimes(CalculationMethod calculationMethod,
        JuristicMethod juristicMethod,
        AdjustingMethod adjustingMethod) :
        m_calculationMethod(calculationMethod),
        m_juristicMethod(juristicMethod),
        m_adjustingMethod(adjustingMethod)
{
}

void SalahTimes::build(int year, int month, int day, double latitude, double longitude, double timezone)
{
    m_latitude = latitude;
    m_longitude = longitude;
    m_timezone = timezone;
    m_julianDate = datetime::georgianToJulianDate(year, month, day);
}
