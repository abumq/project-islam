#include "settings_tab_widget_form.h"
#include "ui_settings_tab_widget_form.h"

SettingsTabWidgetForm::SettingsTabWidgetForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsTabWidgetForm)
{
    ui->setupUi(this);
}

SettingsTabWidgetForm::~SettingsTabWidgetForm()
{
    delete ui;
}
