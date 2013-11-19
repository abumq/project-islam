#include "welcome_page.h"
#include "ui_welcome_page.h"

WelcomePage::WelcomePage(QWidget *parent) :
    core::Extension("Welcome", parent),
    ui(new Ui::WelcomePage)
{
    ui->setupUi(this);
}

WelcomePage::~WelcomePage()
{
    delete ui;
}
