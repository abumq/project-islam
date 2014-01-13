#include "salah.h"
#include "salah_times.h"
#include <QLabel>

_INITIALIZE_EASYLOGGINGPP

const char* Salah::kAuthor       = "Project Islam Authors";
const char* Salah::kName         = "Salah";
const char* Salah::kTitle        = "Ṣalāh";
const char* Salah::kDescription  = "Organize your ṣalāh (prayer) including athan.";

Salah::Salah()
{
    memory::turnToNullPtr(m_salahTimes);
    setExtensionInfo(ExtensionInfo(kMajorVersion, kMinorVersion, kPatchVersion, 
                                   QString(kAuthor), QString(kName), 
                                   QString(kTitle), QString(kDescription)));
}

Salah::~Salah()
{
    memory::deleteAll(m_salahTimes);
}

bool Salah::initialize(int argc, const char** argv)
{
    if (!ExtensionBase::initialize(argc, argv)) {
        // Do not proceed
        return false;
    }
    
    // Do not trace location before calling parent's initialize
    _TRACE;
    initializeMenu();
    
    QLabel* test = new QLabel(settingsTabWidget());
    test->setText("this is test label on settings tab!");
    
    m_salahTimes = new SalahTimes();
    
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
