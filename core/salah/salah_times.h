#ifndef SALAH_TIMES_H
#define SALAH_TIMES_H

#include <map>
#include "core/utils/misc.h"

class SalahTimes
{
public:
    enum class CalculationMethod {
        Jafari = 1,         // Ithna Ashari
        Karachi = 2,        // University of Islamic Sciences, Karachi
        ISNA = 4,           // Islamic Society of North America (ISNA)
        MWL = 8,            // Muslim World League (MWL)
        Makkah = 16,        // Umm al-Qura, Makkah
        Egypt = 32,         // Egyptian General Authority of Survey
        Custom = 64,        // Custom Setting
    };
    
    enum class JuristicMethod {
        Shafii = 1,    // Shafii (standard)
        Hanafi = 2,    // Hanafi
    };
    
    enum class AdjustingMethod {
        None = 0,              // No adjustment
        MidNight = 1,          // middle of night
        OneSeventh = 2,        // 1/7th of night
        AngleBased = 4,        // angle/60th of night
    };
    
    enum class TimeType {
        Fajr = 1,
        Sunrise = 2,
        Dhuhr = 4,
        Asr = 8,
        Sunset = 16,
        Maghrib = 32,
        Isha = 64
    };
    
    SalahTimes(CalculationMethod calculationMethod = CalculationMethod::Jafari,
        JuristicMethod juristicMethod = JuristicMethod::Shafii,
        AdjustingMethod adjustingMethod = AdjustingMethod::MidNight);
    void build(int year, int month, int day, double latitude, double longitude, double timezone);
        
private:
    const int kTimesCount = static_cast<int>(bits::countBits(
        static_cast<long>(TimeType::Fajr) + static_cast<long>(TimeType::Sunrise)
         + static_cast<long>(TimeType::Dhuhr) + static_cast<long>(TimeType::Asr)
          + static_cast<long>(TimeType::Sunset) + static_cast<long>(TimeType::Maghrib)
           + static_cast<long>(TimeType::Isha)));
    const int kMaxTimeType = static_cast<int>(TimeType::Isha);
    
    CalculationMethod m_calculationMethod;
    JuristicMethod m_juristicMethod;
    AdjustingMethod m_adjustingMethod;
    double m_latitude;
    double m_longitude;
    double m_timezone;
    double m_julianDate;
    std::map<TimeType, double> m_times;
};

#endif // SALAH_TIMES_H
