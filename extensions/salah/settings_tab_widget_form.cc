#include "settings_tab_widget_form.h"
#include "ui_settings_tab_widget_form.h"
#include "salah_times.h"
#include "core/logging/logging.h"

SettingsTabWidgetForm::SettingsTabWidgetForm(QWidget *parent, const LoadSettingFunc& loadFunc,
                                             QMap<QString, QVariant> *settingsMap, const QString &settingKeyPrefix) :
    QWidget(parent),
    ui(new Ui::SettingsTabWidgetForm),
    m_loadFunc(loadFunc),
    m_settingsMap(settingsMap),
    m_settingKeyPrefix(settingKeyPrefix)
{
    _TRACE;
    ui->setupUi(this);
    
    // Calculation method
    ui->cboCalculationMethod->addItems(
                QStringList() << "Jafari" << "University of Islamic Sciences, Karachi" << "Islamic Society of North America (ISNA)"
                << "Muslim World League (MWL)" << "Umm al-Qura, Makkah" << "Egyptian General Authority of Survey"
                );
    QVariant qv_calcMethod = m_loadFunc(QString::fromStdString(SalahTimes::kCalculationMethodKey), QVariant(8)); // 8 = MWL
    int calcMethod = qv_calcMethod.toInt();
    int c = static_cast<int>(SalahMethod::CalculationMethod::Jafari);
    int i = 0;
    do {
        if (calcMethod == c && ui->cboCalculationMethod->count() > i) {
            ui->cboCalculationMethod->setCurrentIndex(i);
            break;
        }
        c = c << 1;
        ++i;
    } while (c <= SalahMethod::kMaxCalculationMethod);
    
    // Juristic method
    ui->cboJuristicMethod->addItems(
                QStringList() << "Standard (Imam Shafii, Hanbali and Maliki)" << "Imam Hanafi"
                );
    QVariant qv_juristicMethod = m_loadFunc(QString::fromStdString(SalahTimes::kJuristicMethodKey), QVariant(1)); // 1 = standard
    int juristicMethod = qv_juristicMethod.toInt();
    int j = static_cast<int>(SalahMethod::JuristicMethod::Shafii);
    i = 0;
    do {
        if (juristicMethod == j && ui->cboJuristicMethod->count() > i) {
            ui->cboJuristicMethod->setCurrentIndex(i);
            break;
        }
        j = j << 1;
        ++i;
    } while (j <= SalahMethod::kMaxJuristicMethod);
    
    // Adjusting method
    ui->cboAdjustingMethod->addItems(
                QStringList() << "No adjustment" << "Mid-night (Middle of night)" 
                << "One-seventh (1 / 7th of night)" << "Angle-based (angle / 60th of night)"
                );
    QVariant qt_adjustingMethod = m_loadFunc(QString::fromStdString(SalahTimes::kAdjustingMethodKey), QVariant(8)); // 8 = Angle-based
    int adjustingMethod = qt_adjustingMethod.toInt();
    int a = static_cast<int>(SalahMethod::AdjustingMethod::None);
    i = 0;
    do {
        if (adjustingMethod == a && ui->cboAdjustingMethod->count() > i) {
            ui->cboAdjustingMethod->setCurrentIndex(i);
            break;
        }
        a = a << 1;
        ++i;
    } while (a <= SalahMethod::kMaxAdjustingMethod);
    
    // Minutes warning
    QVariant qv_minutesStartFajrWarning = m_loadFunc(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToStartFajrKey), QVariant(10));
    ui->spnMinPrayerStartFajr->setValue(qv_minutesStartFajrWarning.toInt());
    QVariant qv_minutesStartDhuhrWarning = m_loadFunc(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToStartDhuhrKey), QVariant(10));
    ui->spnMinPrayerStartDhuhr->setValue(qv_minutesStartDhuhrWarning.toInt());
    QVariant qv_minutesStartAsrWarning = m_loadFunc(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToStartAsrKey), QVariant(10));
    ui->spnMinPrayerStartAsr->setValue(qv_minutesStartAsrWarning.toInt());
    QVariant qv_minutesStartMaghribWarning = m_loadFunc(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToStartMaghribKey), QVariant(10));
    ui->spnMinPrayerStartMaghrib->setValue(qv_minutesStartMaghribWarning.toInt());
    QVariant qv_minutesStartIshaWarning = m_loadFunc(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToStartIshaKey), QVariant(10));
    ui->spnMinPrayerStartIsha->setValue(qv_minutesStartIshaWarning.toInt());
    QVariant qv_minutesStartJumuahWarning = m_loadFunc(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToStartJumuahKey), QVariant(30));
    ui->spnMinPrayerStartJumuah->setValue(qv_minutesStartJumuahWarning.toInt());
    
    QVariant qv_minutesExpireWarning = m_loadFunc(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToOverKey), QVariant(30));
    ui->spnMinPrayerOver->setValue(qv_minutesExpireWarning.toInt());
}

SettingsTabWidgetForm::~SettingsTabWidgetForm()
{
    _TRACE;
    delete ui;
}

void SettingsTabWidgetForm::addToQueue(const QString &key, const QVariant &val)
{
    _TRACE;
    m_settingsMap->insert(m_settingKeyPrefix + key, val);
}

void SettingsTabWidgetForm::on_cboCalculationMethod_currentIndexChanged(int index)
{
    _TRACE;
    addToQueue(QString::fromStdString(SalahTimes::kCalculationMethodKey), 1 << index);
}

void SettingsTabWidgetForm::on_cboJuristicMethod_currentIndexChanged(int index)
{
    _TRACE;
    addToQueue(QString::fromStdString(SalahTimes::kJuristicMethodKey), 1 << index);
}

void SettingsTabWidgetForm::on_cboAdjustingMethod_currentIndexChanged(int index)
{   
    _TRACE;
    addToQueue(QString::fromStdString(SalahTimes::kAdjustingMethodKey), 1 << index);
}

void SettingsTabWidgetForm::on_spnMinPrayerOver_valueChanged(int val)
{
    _TRACE;
    addToQueue(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToOverKey), val);
}

void SettingsTabWidgetForm::on_spnMinPrayerStartFajr_valueChanged(int val)
{
    _TRACE;
    addToQueue(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToStartFajrKey), val);
}

void SettingsTabWidgetForm::on_spnMinPrayerStartDhuhr_valueChanged(int val)
{
    _TRACE;
    addToQueue(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToStartDhuhrKey), val);
}

void SettingsTabWidgetForm::on_spnMinPrayerStartAsr_valueChanged(int val)
{
    _TRACE;
    addToQueue(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToStartAsrKey), val);
}

void SettingsTabWidgetForm::on_spnMinPrayerStartMaghrib_valueChanged(int val)
{
    _TRACE;
    addToQueue(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToStartMaghribKey), val);
}

void SettingsTabWidgetForm::on_spnMinPrayerStartIsha_valueChanged(int val)
{
    _TRACE;
    addToQueue(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToStartIshaKey), val);
}

void SettingsTabWidgetForm::on_spnMinPrayerStartJumuah_valueChanged(int val)
{
    _TRACE;
    addToQueue(QString::fromStdString(SalahTimes::kMinutesToPrayerAboutToStartJumuahKey), val);
}
