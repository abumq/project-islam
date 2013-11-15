#include "ui/mainwindow.hh"
#include "ui_mainwindow.h"
#include <QDockWidget>
#include "ui/sidebar_widget.hh"
#include "quran/quran_widget.hh"

namespace ui {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    sidebar(nullptr)
{
    ui->setupUi(this);
    m_features.insert("featureQuran", new quran::ui::QuranWidget(this));
    
    buildDockedWidgets();
}

MainWindow::~MainWindow(void)
{
    delete ui;
}

void MainWindow::buildDockedWidgets()
{
    sidebar = new QDockWidget("Sidebar", this);
    sidebar->resize(300, sidebar->height());
    sidebar->setAllowedAreas(Qt::LeftDockWidgetArea);
    sidebar->setWidget(new ui::SidebarWidget());
    addDockWidget(Qt::LeftDockWidgetArea, sidebar);
    lockSidebar();
    
    
    for (QMap<QString, QWidget*>::Iterator it = m_features.begin(); it != m_features.end(); ++it) {
        //QDockWidget* featureWidget = new QDockWidget(it.key(), this);
        QWidget* featureWidget = it.value();
        featureWidget->setObjectName(it.key());
        featureWidget->setWindowTitle(it.key().mid(QString("feature").size()));
        //featureWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        //featureWidget->setWidget(it.value());
        ui->horizontalLayout->addWidget(featureWidget);
        //addDockWidget(Qt::RightDockWidgetArea, featureWidget);
        //featureWidget->hide();
    }
}

void MainWindow::lockSidebar(void) {
    if (sidebar == nullptr) return;
    sidebar->setFeatures(QDockWidget::NoDockWidgetFeatures);
    sidebar->setWindowTitle("");
}

void MainWindow::unlockSidebar(void) {
    if (sidebar == nullptr) return;
    sidebar->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    sidebar->setWindowTitle("Sidebar");
}
}
