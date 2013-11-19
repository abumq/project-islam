#ifndef WELCOME_PAGE_H
#define WELCOME_PAGE_H

#include "extension.h"

namespace Ui {
class WelcomePage;
}

class WelcomePage : public core::Extension
{
    Q_OBJECT
    
public:
    explicit WelcomePage(QWidget *parent = 0);
    ~WelcomePage();
    
private:
    Ui::WelcomePage *ui;
};

#endif // WELCOME_PAGE_H
