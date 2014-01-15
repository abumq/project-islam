#ifndef SETTINGS_TAB_WIDGET_FORM_H
#define SETTINGS_TAB_WIDGET_FORM_H

#include <QWidget>
#include <functional>

namespace Ui {
class SettingsTabWidgetForm;
}

class SettingsTabWidgetForm : public QWidget
{
    Q_OBJECT
    
public:
    typedef std::function<QVariant(const QString&, const QVariant&)> LoadSettingFunc;
    
    SettingsTabWidgetForm(QWidget *parent, const LoadSettingFunc& loadFunc, 
        QMap<QString, QVariant> *settingsMap, const QString& settingKeyPrefix);
    ~SettingsTabWidgetForm();
    
private slots:
    void on_cboCalculationMethod_currentIndexChanged(int index);
    
    void on_cboJuristicMethod_currentIndexChanged(int index);
    
    void on_cboAdjustingMethod_currentIndexChanged(int index);
    
private:
    Ui::SettingsTabWidgetForm *ui;
    LoadSettingFunc m_loadFunc;
    QMap<QString, QVariant>* m_settingsMap;
    QString m_settingKeyPrefix;
    
    void addToQueue(const QString& key, const QVariant& val);
};

#endif // SETTINGS_TAB_WIDGET_FORM_H
