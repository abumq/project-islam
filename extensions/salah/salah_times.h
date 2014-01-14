#ifndef SALAH_TIMES_H
#define SALAH_TIMES_H

#include <cmath>
#include <string>
#include <map>

class SalahTimes;

namespace {

    /* degree to radian */
    inline double deg2rad(double d) {
        return d * M_PI / 180.0;
    }
    
    /* radian to degree */
    inline double rad2deg(double r) {
        return r * 180.0 / M_PI;
    }
    
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
} // namespace

class SalahMethod {
public:
    enum class CalculationMethod {
        Jafari = 1,         // Ithna Ashari
        Karachi = 2,        // University of Islamic Sciences, Karachi
        ISNA = 4,           // Islamic Society of North America (ISNA)
        MWL = 8,            // Muslim World League (MWL)
        Makkah = 16,        // Umm al-Qura, Makkah
        Egypt = 32,         // Egyptian General Authority of Survey
        Custom = 64         // Custom Setting
    };
    
    static const int kCalculationMethodsCount = 7;
    static const int kMaxCalculationMethod = static_cast<int>(CalculationMethod::Egypt);
    
    enum class JuristicMethod {
        Shafii = 1,    // Shafii (standard)
        Hanafi = 2     // Hanafi
    };
    
    static const int kJuristicMethodsCount = 2;
    static const int kMaxJuristicMethod = static_cast<int>(JuristicMethod::Hanafi);
    
    enum class AdjustingMethod {
        None = 1,              // No adjustment
        MidNight = 2,          // middle of night
        OneSeventh = 4,        // 1/7th of night
        AngleBased = 8         // angle/60th of night
    };
    
    static const int kAdjustingMethodCount = 4;
    static const int kMaxAdjustingMethod = static_cast<int>(AdjustingMethod::AngleBased);
    
    SalahMethod() {}
    SalahMethod(double fajrAngle,
                bool maghribIsMinutes,
                double maghribValue,
                bool ishaIsMinutes,
                double ishaValue);
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
    static const std::string kCalculationMethodKey;
    static const std::string kJuristicMethodKey;
    static const std::string kAdjustingMethodKey;
    
    enum class TimeType {
        Fajr = 1,
        Sunrise = 2,
        Dhuhr = 4,
        Asr = 8,
        Sunset = 16,
        Maghrib = 32,
        Isha = 64
    };
    static const int kTimesCount = 7;
    static const int kMaxTimeType = static_cast<int>(TimeType::Isha);
    
    SalahTimes(SalahMethod::CalculationMethod calculationMethod = SalahMethod::CalculationMethod::MWL,
               SalahMethod::JuristicMethod juristicMethod = SalahMethod::JuristicMethod::Shafii,
               SalahMethod::AdjustingMethod adjustingMethod = SalahMethod::AdjustingMethod::MidNight);
    void build(double latitude, double longitude);
    void build(double latitude, double longitude, double timezone);
    void build(int year, int month, int day, double latitude, double longitude, double timezone);
    inline const std::map<TimeType, double>* times() const;
    std::string readTime(TimeType t);
    std::pair<int, int> readTimeHourMinutePair(TimeType t);
private:
    
    SalahMethod::CalculationMethod m_calculationMethod;
    SalahMethod::JuristicMethod m_juristicMethod;
    SalahMethod::AdjustingMethod m_adjustingMethod;
    double m_latitude;
    double m_longitude;
    double m_timezone;
    double m_julianDate;
    std::map<SalahMethod::CalculationMethod, SalahMethod> m_salahMethods;
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
};

#endif // SALAH_TIMES_H
