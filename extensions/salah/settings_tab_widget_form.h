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
    typedef std::function<void(const QString&, const QVariant&)> SaveSettingFunc;
    typedef std::function<QVariant(const QString&, const QVariant&)> LoadSettingFunc;
    
    SettingsTabWidgetForm(QWidget *parent, const SaveSettingFunc& saveFunc, const LoadSettingFunc& loadFunc);
    ~SettingsTabWidgetForm();
    
private:
    Ui::SettingsTabWidgetForm *ui;
    SaveSettingFunc m_saveFunc;
    LoadSettingFunc m_loadFunc;
};

#endif // SETTINGS_TAB_WIDGET_FORM_H
