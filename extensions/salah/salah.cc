#include "salah.h"
#include <QLabel>
#include <QTime>
#include "salah_times.h"
#include "settings_tab_widget_form.h"
#include "core/constants.h"
#include "core/controls/clock.h"

_INITIALIZE_EASYLOGGINGPP

const char* Salah::kAuthor       = "Project Islam Authors\n    Special thanks to:\n      - Mohammad Ebrahim\n      - Mohammadi Panah\n    Jazak Allah Khair";
const char* Salah::kName         = "Salah";
const char* Salah::kTitle        = "Ṣalāh";
const char* Salah::kDescription  = "Organize your ṣalāh (prayer) including athan.";

class SalahClock : public Clock {
public:
    SalahClock(QWidget* parent, SalahTimes::TimeType t, SalahTimes* times)
        : Clock(parent),
          m_timeType(t) 
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
        std::pair<int, int> tPair = times->readTimeHourMinutePair(t);
        setTime(tPair.first, tPair.second);
    }
    
    void paintEvent(QPaintEvent *e) 
    {
        Clock::paintEvent(e);
        if (isPrayerTime()) {
            select();
        } else {
            deselect();
        }
    }
    
    inline int validMinutes() 
    {
        // TODO: Get these from settings i.e, 
        //         * Sunrise offset for Fajr validity
        //         * Sunset offset for Maghrib validity
        //         * Maghrib valid for
        if (m_timeType == SalahTimes::TimeType::Fajr) {
            return 70; // Until (sunrise - 5 minutes)
        } else if (m_timeType == SalahTimes::TimeType::Dhuhr) {
            return 120; // Until (Asr)
        } else if (m_timeType == SalahTimes::TimeType::Asr) {
            return 60; // Until (Sunset - 15 minutes) - 15 minutes of zawal - Configurable
        } else if (m_timeType == SalahTimes::TimeType::Maghrib) {
            return 30; // Configurable
        } else  if (m_timeType == SalahTimes::TimeType::Isha) {
            return QTime::currentTime().secsTo(QTime(23, 59)) * 60; // Until midnight
        }
        return 5; // Sunset and sunrise valid for 5 minutes
    }

    inline bool isPrayerTime() 
    {
        if (m_live) {
            return false;
        }
        QTime cTime = QTime::currentTime();
        int s = cTime.secsTo(QTime(m_h, m_m));
        return s >= -(validMinutes() * 60) && s <= 0;
    }
private:
    SalahTimes::TimeType m_timeType;
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
    initializeMenu();
    initializeSettingsTabDialog();
    
    m_salahTimes = new SalahTimes(
                static_cast<SalahMethod::CalculationMethod>(setting(QString::fromStdString(SalahTimes::kCalculationMethodKey), QVariant(8)).toInt()),
                static_cast<SalahMethod::JuristicMethod>(setting(QString::fromStdString(SalahTimes::kJuristicMethodKey), QVariant(1)).toInt()),
                static_cast<SalahMethod::AdjustingMethod>(setting(QString::fromStdString(SalahTimes::kAdjustingMethodKey), QVariant(8)).toInt())
                );
    m_salahTimes->build(nativeSetting(SettingsLoader::kLatitudeKey, QVariant(kDefaultLatitude)).toDouble(),
                        nativeSetting(SettingsLoader::kLongitudeKey, QVariant(kDefaultLongitude)).toDouble());
    displayClocks();
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
                                                     std::bind(&ExtensionBase::saveSetting, this, _1, _2),
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
