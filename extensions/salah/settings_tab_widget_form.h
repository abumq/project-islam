#ifndef SETTINGS_TAB_WIDGET_FORM_H
#define SETTINGS_TAB_WIDGET_FORM_H

#include <QWidget>

namespace Ui {
class SettingsTabWidgetForm;
}

class SettingsTabWidgetForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit SettingsTabWidgetForm(QWidget *parent = 0);
    ~SettingsTabWidgetForm();
    
private:
    Ui::SettingsTabWidgetForm *ui;
};

#endif // SETTINGS_TAB_WIDGET_FORM_H
