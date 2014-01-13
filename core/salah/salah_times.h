#ifndef SALAH_TIMES_H
#define SALAH_TIMES_H

#include <cmath>
#include <map>
#include "core/utils/misc.h"

class SalahTimes;

class SalahMethod {
public:
    SalahMethod(double fajrAngle = 16.0,
                bool maghribIsMinutes = false,
                double maghribValue = 4.0,
                bool ishaIsMinutes = false,
                double ishaValue = 14.0);
private:
    double m_fajrAngle;
    bool m_maghribIsMinutes;
    double m_maghribValue;
    bool m_ishaIsMinutes;
    double m_ishaValue;
    
    friend class SalahTimes;
};

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
    inline const std::map<TimeType, double>* times() const;
    std::string readTime(TimeType t);
private:
    const int kTimesCount = static_cast<int>(bits::countBits(
                                                 static_cast<long>(TimeType::Fajr) + static_cast<long>(TimeType::Sunrise)
                                                 + static_cast<long>(TimeType::Dhuhr) + static_cast<long>(TimeType::Asr)
                                                 + static_cast<long>(TimeType::Sunset) + static_cast<long>(TimeType::Maghrib)
                                                 + static_cast<long>(TimeType::Isha)));
    const int kMaxTimeType = static_cast<int>(TimeType::Isha);
    
    const int kCalculationMethodsCount = static_cast<int>(bits::countBits(
                                                              static_cast<long>(CalculationMethod::Jafari) + static_cast<long>(CalculationMethod::Karachi)
                                                              + static_cast<long>(CalculationMethod::ISNA) + static_cast<long>(CalculationMethod::MWL)
                                                              + static_cast<long>(CalculationMethod::Makkah) + static_cast<long>(CalculationMethod::Egypt)
                                                              + static_cast<long>(CalculationMethod::Custom)));
    
    CalculationMethod m_calculationMethod;
    JuristicMethod m_juristicMethod;
    AdjustingMethod m_adjustingMethod;
    double m_latitude;
    double m_longitude;
    double m_timezone;
    double m_julianDate;
    std::map<CalculationMethod, SalahMethod> m_salahMethods;
    std::map<TimeType, double> m_times;
    
    double equationOfTime(double julianDate);
    double sunDeclination(double julianDate);
    double computeMidDay(double currTime);
    double computeAsr(double currTime);
    void adjustTimes();
    void adjustHighLatTimes();
    double nightPortion(double angle);
    std::pair<double, double> sunPosition(double julianDate);
    double computePrayerTime(double angle, double currTime);
    
    inline double dsin(double d) {
        return sin(deg2rad(d));
    }
    
    /* degree cos */
    inline double dcos(double d) {
        return cos(deg2rad(d));
    }
    
    /* degree tan */
    inline double dtan(double d) {
        return tan(deg2rad(d));
    }
    
    /* degree arcsin */
    inline double darcsin(double x) {
        return rad2deg(asin(x));
    }
    
    /* degree arccos */
    inline double darccos(double x) {
        return rad2deg(acos(x));
    }
    
    /* degree arctan */
    inline double darctan(double x) {
        return rad2deg(atan(x));
    }
    
    /* degree arctan2 */
    inline double darctan2(double y, double x) {
        return rad2deg(atan2(y, x));
    }
    
    /* degree arccot */
    inline double darccot(double x) {
        return rad2deg(atan(1.0 / x));
    }
    
    /* degree to radian */
    inline double deg2rad(double d) {
        return d * M_PI / 180.0;
    }
    
    /* radian to degree */
    inline double rad2deg(double r) {
        return r * 180.0 / M_PI;
    }
    
    /* range reduce angle in degrees. */
    inline double fixAngle(double a) {
        a = a - 360.0 * floor(a / 360.0);
        a = a < 0.0 ? a + 360.0 : a;
        return a;
    }
    
    /* range reduce hours to 0..23 */
    inline double fixHour(double a) {
        a = a - 24.0 * floor(a / 24.0);
        a = a < 0.0 ? a + 24.0 : a;
        return a;
    }
};

#endif // SALAH_TIMES_H
