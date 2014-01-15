#include "salah.h"
#include <QLabel>
#include <QTime>
#include "core/logging/logging.h"
#include "salah_times.h"
#include "settings_tab_widget_form.h"
#include "core/constants.h"
#include "core/controls/clock.h"
#include "core/utils/notify.h"
#include "qibla_compass.h"

_INITIALIZE_EASYLOGGINGPP

const char* Salah::kAuthor       = "Project Islam Authors\n    Special thanks to:\n      - Mohammad Ebrahim\n      - Mohammadi Panah\n    Jazak Allah Khair";
const char* Salah::kName         = "Salah";
const char* Salah::kTitle        = "Ṣalāh";
const char* Salah::kDescription  = "Organize your ṣalāh (prayer) including athan.";

class SalahClock : public Clock {
public:
    SalahClock(QWidget* parent, SalahTimes::TimeType t, SalahTimes* times)
        : Clock(parent),
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
    
    void paintEvent(QPaintEvent *e) 
    {
        Clock::paintEvent(e);
        if (!selected() && isPrayerTime()) {
            select();
            notify("Prayer time", "It's time for " + m_title.toStdString() + " prayer");
            LOG(INFO) << "Prayer time for [" << m_title << "]";
        } else if (selected() && !isPrayerTime()) {
            deselect();
            notify("Prayer time", "Time for " + m_title.toStdString() + " prayer is over");
            LOG(INFO) << "Prayer time over for [" << m_title << "]";
        }
    }
    
    int minutesForValidity() 
    {
        // TODO: Get these from settings i.e, 
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
    
    inline bool isPrayerTime() 
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
private:
    SalahTimes::TimeType m_timeType;
    SalahTimes* m_times;
};

Salah::Salah()
{
    memory::turnToNullPtr(m_salahTimes, m_settingsWidgetForm);
    setExtensionInfo(ExtensionInfo(kMajorVersion, kMinorVersion, kPatchVersion, 
                                   QString(kAuthor), QString(kName), 
                                   QString(kTitle), QString(kDescription)));
}

Salah::~Salah()
{
    memory::deleteAll(m_salahTimes); 
    // Do not delete m_settingsWidgetForm since parent (settingsTabWidget()) deletes it
}

bool Salah::initialize(int argc, const char** argv)
{
    if (!ExtensionBase::initialize(argc, argv)) {
        // Do not proceed
        return false;
    }
    // Do not trace location before calling parent's initialize
    _TRACE;
    memory::deleteAll(m_salahTimes, m_settingsWidgetForm);
    notify("Prayer time", "test");
    initializeMenu();
    initializeSettingsTabDialog();
    
    m_salahTimes = new SalahTimes(
                static_cast<SalahMethod::CalculationMethod>(setting(QString::fromStdString(SalahTimes::kCalculationMethodKey), QVariant(8)).toInt()),
                static_cast<SalahMethod::JuristicMethod>(setting(QString::fromStdString(SalahTimes::kJuristicMethodKey), QVariant(1)).toInt()),
                static_cast<SalahMethod::AdjustingMethod>(setting(QString::fromStdString(SalahTimes::kAdjustingMethodKey), QVariant(8)).toInt())
                );
    double lat = nativeSetting(SettingsLoader::kLatitudeKey, QVariant(kDefaultLatitude)).toDouble();
    double lng = nativeSetting(SettingsLoader::kLongitudeKey, QVariant(kDefaultLongitude)).toDouble();
    m_salahTimes->build(lat, lng);
    displayClocks();
    
    QiblaCompass* qiblaCompass = new QiblaCompass(lat, lng, container());
    qiblaCompass->resize(200);
    qiblaCompass->hide();
    
    return true;
}

void Salah::initializeMenu()
{
    _TRACE;
}

void Salah::displayClocks()
{
    const int kApartThreshold = 5;
    
    SalahClock* fajrClock = new SalahClock(container(), 
                                           SalahTimes::TimeType::Fajr, m_salahTimes);
    fajrClock->move(kApartThreshold, kApartThreshold);
    SalahClock* dhuhrClock = new SalahClock(container(), 
                                            SalahTimes::TimeType::Dhuhr, m_salahTimes);
    dhuhrClock->move(fajrClock->x() + fajrClock->width() + kApartThreshold, fajrClock->y());
    SalahClock* asrClock = new SalahClock(container(), 
                                          SalahTimes::TimeType::Asr, m_salahTimes);
    asrClock->move(
                ((dhuhrClock->x() + dhuhrClock->width() + kApartThreshold) / 2) - (asrClock->width() / 2), 
                dhuhrClock->y() + dhuhrClock->height() + kApartThreshold);
    SalahClock* maghribClock = new SalahClock(container(), 
                                              SalahTimes::TimeType::Maghrib, m_salahTimes);
    maghribClock->move(kApartThreshold, asrClock->y() + asrClock->height() + kApartThreshold);
    SalahClock* ishaClock = new SalahClock(container(), 
                                           SalahTimes::TimeType::Isha, m_salahTimes);
    ishaClock->move(maghribClock->x() + maghribClock->width() + kApartThreshold, maghribClock->y());
    
    Clock* liveClock = new Clock(container());
    liveClock->liveClock();
    liveClock->resize(300);
    liveClock->move(ishaClock->x() + ishaClock->width() + kApartThreshold, asrClock->y() - 50);
    liveClock->setTitle("Current");
    liveClock->setDisplayTextualTime(true);
}

void Salah::initializeSettingsTabDialog() 
{
    using namespace std::placeholders;
    m_settingsWidgetForm = new SettingsTabWidgetForm(settingsTabWidget(),
                                                     std::bind(&ExtensionBase::setting, this, _1, _2),
                                                     extension()->settingsMap(),
                                                     settingsKeyPrefix());
    
}

void Salah::onContainerGeometryChanged(int w, int h)
{
}

void Salah::onActivated()
{
    _TRACE;
}

void Salah::onDeactivated()
{
    _TRACE;
}
