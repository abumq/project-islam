#include "salah.h"
#include <QLabel>
#include <QTime>
#include <QTimer>
#include "core/logging/logging.h"
#include "salah_times.h"
#include "settings_tab_widget_form.h"
#include "core/constants.h"
#include "core/controls/clock.h"
#include "core/utils/notify.h"
#include "salah_clock.h"
#include "qibla_compass.h"

INITIALIZE_SHARED_LOGGING

const char* Salah::kAuthor       = "Project Islam Authors";
const char* Salah::kName         = "Salah";
const char* Salah::kTitle        = "Ṣalāh";
const char* Salah::kDescription  = "Organize your ṣalāh (prayer) including athan.";

Salah::Salah()
{
    memory::turnToNullPtr(m_salahTimes, m_settingsWidgetForm, m_qiblaCompass,
                          m_fajrClock, m_dhuhrClock, m_asrClock, m_maghribClock, m_ishaClock);
    Q_INIT_RESOURCE(icons_salah);
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
    memory::deleteAll(m_salahTimes, m_settingsWidgetForm,
                      m_fajrClock, m_dhuhrClock, m_asrClock, m_maghribClock, m_ishaClock);
    initializeMenu();
    initializeSettingsTabDialog();
    m_minutesToPrayerAboutToStartFajr = setting(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToStartFajrKey), QVariant(10)).toInt();
    m_minutesToPrayerAboutToStartDhuhr = setting(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToStartDhuhrKey), QVariant(10)).toInt();
    m_minutesToPrayerAboutToStartAsr = setting(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToStartAsrKey), QVariant(10)).toInt();
    m_minutesToPrayerAboutToStartMaghrib = setting(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToStartMaghribKey), QVariant(10)).toInt();
    m_minutesToPrayerAboutToStartIsha = setting(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToStartIshaKey), QVariant(10)).toInt();
    m_minutesToPrayerAboutToStartJumuah = setting(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToStartJumuahKey), QVariant(30)).toInt();
    m_minutesToPrayerAboutToOver = setting(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToOverKey), QVariant(30)).toInt();
    m_salahTimes = new SalahTimes(
                static_cast<SalahMethod::CalculationMethod>(setting(QString::fromStdString(SalahTimes::kCalculationMethodKey), QVariant(8)).toInt()),
                static_cast<SalahMethod::JuristicMethod>(setting(QString::fromStdString(SalahTimes::kJuristicMethodKey), QVariant(1)).toInt()),
                static_cast<SalahMethod::AdjustingMethod>(setting(QString::fromStdString(SalahTimes::kAdjustingMethodKey), QVariant(8)).toInt())
                );
    double lat = nativeSetting(SettingsLoader::kLatitudeKey, QVariant(kDefaultLatitude)).toDouble();
    double lng = nativeSetting(SettingsLoader::kLongitudeKey, QVariant(kDefaultLongitude)).toDouble();
    m_salahTimes->build(lat, lng);
    displayClocks();
    
    m_qiblaCompass = new QiblaCompass(lat, lng, container()); // FIXME: Qibla does not correctly work
    m_qiblaCompass->resize(256);
    // m_qiblaCompass->hide();
    
    QTimer* salahTimeChecker = new QTimer(this);
    connect(salahTimeChecker, SIGNAL(timeout()), this, SLOT(checkDayChange()));
    salahTimeChecker->start(1000); // Every second
    
    return true;
}

void Salah::initializeMenu()
{
    _TRACE;
}

void Salah::displayClocks()
{
    _TRACE;
    const int kApartThreshold = 5;
    
    m_fajrClock = new SalahClock(container(), SalahTimes::TimeType::Fajr, m_salahTimes, m_minutesToPrayerAboutToStartFajr, m_minutesToPrayerAboutToOver);
    m_fajrClock->move(kApartThreshold, kApartThreshold);
    QObject::connect(m_fajrClock, SIGNAL(prayerTime(bool)), this, SLOT(onPrayerTime(bool)));
    QObject::connect(m_fajrClock, SIGNAL(prayerTimeAboutToStart(int)), this, SLOT(onPrayerTimeAboutToStart(int)));
    QObject::connect(m_fajrClock, SIGNAL(prayerTimeAboutToOver(int)), this, SLOT(onPrayerTimeAboutToOver(int)));
    
    m_dhuhrClock = new SalahClock(container(), SalahTimes::TimeType::Dhuhr, m_salahTimes, 
                                  SalahClock::isJumuah() ? m_minutesToPrayerAboutToStartJumuah : m_minutesToPrayerAboutToStartDhuhr, m_minutesToPrayerAboutToOver);
    m_dhuhrClock->move(m_fajrClock->x() + m_fajrClock->width() + kApartThreshold, m_fajrClock->y());
    QObject::connect(m_dhuhrClock, SIGNAL(prayerTime(bool)), this, SLOT(onPrayerTime(bool)));
    QObject::connect(m_dhuhrClock, SIGNAL(prayerTimeAboutToStart(int)), this, SLOT(onPrayerTimeAboutToStart(int)));
    QObject::connect(m_dhuhrClock, SIGNAL(prayerTimeAboutToOver(int)), this, SLOT(onPrayerTimeAboutToOver(int)));
    
    m_asrClock = new SalahClock(container(), SalahTimes::TimeType::Asr, m_salahTimes, m_minutesToPrayerAboutToStartAsr, m_minutesToPrayerAboutToOver);
    m_asrClock->move(
                ((m_dhuhrClock->x() + m_dhuhrClock->width() + kApartThreshold) / 2) - (m_asrClock->width() / 2), 
                m_dhuhrClock->y() + m_dhuhrClock->height() + kApartThreshold);
    QObject::connect(m_asrClock, SIGNAL(prayerTime(bool)), this, SLOT(onPrayerTime(bool)));
    QObject::connect(m_asrClock, SIGNAL(prayerTimeAboutToStart(int)), this, SLOT(onPrayerTimeAboutToStart(int)));
    QObject::connect(m_asrClock, SIGNAL(prayerTimeAboutToOver(int)), this, SLOT(onPrayerTimeAboutToOver(int)));
    
    m_maghribClock = new SalahClock(container(), SalahTimes::TimeType::Maghrib, m_salahTimes, m_minutesToPrayerAboutToStartMaghrib, m_minutesToPrayerAboutToOver);
    m_maghribClock->move(kApartThreshold, m_asrClock->y() + m_asrClock->height() + kApartThreshold);
    QObject::connect(m_maghribClock, SIGNAL(prayerTime(bool)), this, SLOT(onPrayerTime(bool)));
    QObject::connect(m_maghribClock, SIGNAL(prayerTimeAboutToStart(int)), this, SLOT(onPrayerTimeAboutToStart(int)));
    QObject::connect(m_maghribClock, SIGNAL(prayerTimeAboutToOver(int)), this, SLOT(onPrayerTimeAboutToOver(int)));
    
    m_ishaClock = new SalahClock(container(),  SalahTimes::TimeType::Isha, m_salahTimes, m_minutesToPrayerAboutToStartIsha, m_minutesToPrayerAboutToOver);
    m_ishaClock->move(m_maghribClock->x() + m_maghribClock->width() + kApartThreshold, m_maghribClock->y());
    QObject::connect(m_ishaClock, SIGNAL(prayerTime(bool)), this, SLOT(onPrayerTime(bool)));
    QObject::connect(m_ishaClock, SIGNAL(prayerTimeAboutToStart(int)), this, SLOT(onPrayerTimeAboutToStart(int)));
    QObject::connect(m_ishaClock, SIGNAL(prayerTimeAboutToOver(int)), this, SLOT(onPrayerTimeAboutToOver(int)));
    
    Clock* liveClock = new Clock(container());
    liveClock->liveClock();
    liveClock->resize(300);
    liveClock->move(m_ishaClock->x() + m_ishaClock->width() + kApartThreshold, m_asrClock->y() - 50);
    liveClock->setTitle("Current");
    liveClock->setDisplayTextualTime(true);
}

void Salah::initializeSettingsTabDialog() 
{
    _TRACE;
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
    const int kPrayerTimeNotifySeconds = 30;
    const int kPrayerTimeOverNotifySeconds = 10;
    if (activated) {
        clock->select();
        notifyInfo("Prayer Time", "It's time for " + clock->title() + " prayer", kPrayerTimeNotifySeconds * 1000);
        LOG(INFO) << "Prayer time for [" << clock->title() << "]";
    } else {
        clock->deselect();
        notifyInfo("Prayer Time Over", "Time for " + clock->title() + " prayer is over", kPrayerTimeOverNotifySeconds * 1000);
        LOG(INFO) << "Prayer time over for [" << clock->title() << "]";
    }
}

void Salah::onPrayerTimeAboutToStart(int minutesLeft)
{    
    _TRACE;
    SalahClock* clock = qobject_cast<SalahClock*>(sender());
    clock->setColor(Qt::lightGray);
    LOG(INFO) << clock->title() << " prayer starting in " << minutesLeft << " minutes";
    if (minutesLeft <= 0) {
        return;
    }
    notifyInfo("Prayer About to Start", clock->title() + " prayer starting in " + QString::number(minutesLeft) + " minutes", 10000);
}

void Salah::onPrayerTimeAboutToOver(int minutesLeft)
{
    _TRACE;
    const int kPrayerTimeAboutToOverNotifySeconds = 20;
    SalahClock* clock = qobject_cast<SalahClock*>(sender());
    clock->setColor(Qt::red);
    LOG(INFO) << "Only " << minutesLeft << " minutes left for " << clock->title() << " prayer";
    if (minutesLeft <= 0) {
        return;
    }
    notifyWarn("Prayer Time", "Only " + QString::number(minutesLeft) + " minutes left for " + clock->title(), 
           kPrayerTimeAboutToOverNotifySeconds * 1000);
}

void Salah::checkDayChange()
{
    if (m_salahTimes != nullptr) {
        QDate currDate = QDate::currentDate();
        QDate salahDate = QDate(m_salahTimes->year(), m_salahTimes->month(), m_salahTimes->day());
        if (currDate != salahDate) {
            LOG(INFO) << "Refreshing salah times for new day";
            m_salahTimes->build();
            m_fajrClock->refresh();
            m_dhuhrClock->refresh();
            m_asrClock->refresh();
            m_maghribClock->refresh();
            m_ishaClock->refresh();
        }
    }
}
