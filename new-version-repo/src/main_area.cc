#include "main_area.h"
#include "ui_main_area.h"

MainArea::MainArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainArea)
{
    ui->setupUi(this);
}

MainArea::~MainArea()
{
    delete ui;
}
