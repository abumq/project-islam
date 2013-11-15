#include "ui/sidebar_widget.hh"
#include "ui_sidebar_widget.h"
#include <QRegularExpression>
#include "log/logging.hh"
namespace ui {
SidebarWidget::SidebarWidget(QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::SidebarWidget)
{
    ui->setupUi(this);
    
    ui->cboFeatures->addItem("Quran");
}

SidebarWidget::~SidebarWidget()
{
    delete ui;
}


void SidebarWidget::on_cboFeatures_currentIndexChanged(const QString& name)
{
    /*QList<QWidget*> features = parent()->findChildren<QWidget*>(QRegularExpression("feature[A-Z][a-z0-9]*"));
    LOG(INFO) << parent()->children();
    QWidget* widget = parent()->findChild<QWidget*>("feature" + name);*/
    //CHECK_NOTNULL(widget);
}


} // namespace ui
