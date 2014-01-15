#include "salah.h"
#include <QLabel>
#include <QTime>
#include "core/logging/logging.h"
#include "salah_times.h"
#include "settings_tab_widget_form.h"
#include "core/constants.h"
#include "core/controls/clock.h"
#include "core/utils/notify.h"
#include "salah_clock.h"
#include "qibla_compass.h"

_INITIALIZE_EASYLOGGINGPP

const char* Salah::kAuthor       = "Project Islam Authors\n    Special thanks to:\n      - Mohammad Ebrahim\n      - Mohammadi Panah\n    Jazak Allah Khair";
const char* Salah::kName         = "Salah";
const char* Salah::kTitle        = "Ṣalāh";
const char* Salah::kDescription  = "Organize your ṣalāh (prayer) including athan.";

Salah::Salah()
{
    memory::turnToNullPtr(m_salahTimes, m_settingsWidgetForm, m_qiblaCompass);
    setExtensionInfo(ExtensionInfo(kMajorVersion, kMinorVersion, kPatchVersion, 
                                   QString(kAuthor), QString(kName), 
                                   QString(kTitle), QString(kDescription)));
    Q_INIT_RESOURCE(icons);
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
    double lat = nativeSetting(SettingsLoader::kLatitudeKey, QVariant(kDefaultLatitude)).toDouble();
    double lng = nativeSetting(SettingsLoader::kLongitudeKey, QVariant(kDefaultLongitude)).toDouble();
    m_salahTimes->build(lat, lng);
    displayClocks();
    
    m_qiblaCompass = new QiblaCompass(lat, lng, container());
    m_qiblaCompass->resize(256);
    m_qiblaCompass->hide(); // FIXME: Qibla does not correctly work
    
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
    QObject::connect(fajrClock, SIGNAL(prayerTime(bool)), this, SLOT(onPrayerTime(bool)));
    SalahClock* dhuhrClock = new SalahClock(container(), 
                                            SalahTimes::TimeType::Dhuhr, m_salahTimes);
    dhuhrClock->move(fajrClock->x() + fajrClock->width() + kApartThreshold, fajrClock->y());
    QObject::connect(dhuhrClock, SIGNAL(prayerTime(bool)), this, SLOT(onPrayerTime(bool)));
    SalahClock* asrClock = new SalahClock(container(), 
                                          SalahTimes::TimeType::Asr, m_salahTimes);
    asrClock->move(
                ((dhuhrClock->x() + dhuhrClock->width() + kApartThreshold) / 2) - (asrClock->width() / 2), 
                dhuhrClock->y() + dhuhrClock->height() + kApartThreshold);
    QObject::connect(asrClock, SIGNAL(prayerTime(bool)), this, SLOT(onPrayerTime(bool)));
    SalahClock* maghribClock = new SalahClock(container(), 
                                              SalahTimes::TimeType::Maghrib, m_salahTimes);
    maghribClock->move(kApartThreshold, asrClock->y() + asrClock->height() + kApartThreshold);
    QObject::connect(maghribClock, SIGNAL(prayerTime(bool)), this, SLOT(onPrayerTime(bool)));
    SalahClock* ishaClock = new SalahClock(container(), 
                                           SalahTimes::TimeType::Isha, m_salahTimes);
    ishaClock->move(maghribClock->x() + maghribClock->width() + kApartThreshold, maghribClock->y());
    QObject::connect(ishaClock, SIGNAL(prayerTime(bool)), this, SLOT(onPrayerTime(bool)));
    
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
    if (m_qiblaCompass != nullptr) {
        m_qiblaCompass->move(w - m_qiblaCompass->width(), 0);
    }
}

void Salah::onActivated()
{
    _TRACE;
}

void Salah::onDeactivated()
{
    _TRACE;
}

void Salah::onPrayerTime(bool activated)
{
    _TRACE;
    SalahClock* clock = qobject_cast<SalahClock*>(sender());
    if (activated) {
        clock->select();
        notify("Prayer time", "It's time for " + clock->title().toStdString() + " prayer", 8000);
        LOG(INFO) << "Prayer time for [" << clock->title() << "]";
    } else {
        clock->deselect();
        notify("Prayer time", "Time for " + clock->title().toStdString() + " prayer is over", 8000);
        LOG(INFO) << "Prayer time over for [" << clock->title() << "]";
    }
}
