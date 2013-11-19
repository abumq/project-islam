#ifndef MAIN_AREA_H
#define MAIN_AREA_H

#include <QWidget>

namespace Ui {
class MainArea;
}

class MainArea : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainArea(QWidget *parent = 0);
    ~MainArea();
    
//private:
    Ui::MainArea *ui;
};

#endif // MAIN_AREA_H
