#include "salah.h"
#include <QLabel>
#include "salah_times.h"
#include "settings_tab_widget_form.h"
#include "core/constants.h"

_INITIALIZE_EASYLOGGINGPP

const char* Salah::kAuthor       = "Project Islam Authors\n    Special thanks to:\n      - Mohammad Ebrahim\n      - Mohammadi Panah\n    Jazak Allah Khair";
const char* Salah::kName         = "Salah";
const char* Salah::kTitle        = "Ṣalāh";
const char* Salah::kDescription  = "Organize your ṣalāh (prayer) including athan.";

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
    QLabel* prayerTimes = new QLabel(container());
    
    QString fajr = QString::fromStdString(m_salahTimes->readTime(SalahTimes::TimeType::Fajr));
    QString dhuhr = QString::fromStdString(m_salahTimes->readTime(SalahTimes::TimeType::Dhuhr));
    QString asr = QString::fromStdString(m_salahTimes->readTime(SalahTimes::TimeType::Asr));
    QString maghrib = QString::fromStdString(m_salahTimes->readTime(SalahTimes::TimeType::Maghrib));
    QString isha = QString::fromStdString(m_salahTimes->readTime(SalahTimes::TimeType::Isha));
    
    prayerTimes->setText("Fajr: " + fajr + "\n"
                         +"Dhuhr: " +  dhuhr + "\n"
                         +"Asr: " +  asr + "\n"
                         +"Maghrib: " +  maghrib + "\n"
                         + "Isha: " + isha);
    return true;
}

void Salah::initializeMenu()
{
    _TRACE;
}

void Salah::initializeSettingsTabDialog() {
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
