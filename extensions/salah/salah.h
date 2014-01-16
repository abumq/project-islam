#ifndef SALAH_H
#define SALAH_H

#include "core/extension/extension_base.h"

class SalahTimes;
class SettingsTabWidgetForm;
class QiblaCompass;

class Salah : public ExtensionBase
{
    Q_OBJECT
    Q_INTERFACES(ExtensionBase)
    Q_PLUGIN_METADATA(IID "ProjectIslam.Api.ExtensionBase.Salah.v0.1.0")
private:
    static const unsigned int kMajorVersion = 0;
    static const unsigned int kMinorVersion = 1;
    static const unsigned int kPatchVersion = 0;
    static const char* kAuthor;
    static const char* kName;
    static const char* kTitle;
    static const char* kDescription;
public:
    Salah();
    ~Salah();
    
    virtual bool initialize(int argc, const char **argv);
    void initializeSettingsTabDialog();
    void initializeMenu();
    void displayClocks();
    
public slots:

    // From interface
    virtual void onContainerGeometryChanged(int, int);
    virtual void onActivated();
    virtual void onDeactivated();
    void onPrayerTime(bool);
    void onPrayerTimeAboutToOver();
private:
    SalahTimes* m_salahTimes;
    SettingsTabWidgetForm* m_settingsWidgetForm;
    QiblaCompass* m_qiblaCompass;
    int m_minutesToPrayerAboutToOver;
};

#endif // SALAH_H
