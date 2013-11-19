#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QResizeEvent>
#include <QDockWidget>
#include <QToolBar>
#include <QPushButton>

#include "sidebar.h"
#include "main_area.h"
#include "features/welcome_page.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_sidebar(nullptr),
    m_mainArea(nullptr),
    m_toolbar(nullptr)
{
    setup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    int newWidth = event->size().width();
    m_mainArea->setMinimumWidth(newWidth);
}

void MainWindow::setup(void)
{
    // UI
    ui->setupUi(this);
    
    // MainArea
    if (m_mainArea != nullptr) {
        delete m_mainArea;
    }
    m_mainArea = new QDockWidget("", this);
    m_mainArea->setFeatures(QDockWidget::NoDockWidgetFeatures);
    MainArea* area = new MainArea(this);
    m_mainArea->setWidget(area);
    m_mainArea->setMinimumWidth(400);
    
    // Sidebar
    if (m_sidebar != nullptr) {
        delete m_sidebar;
    }
    m_sidebar = new QDockWidget("", this);
    m_sidebar->setAllowedAreas(Qt::LeftDockWidgetArea);
    m_sidebar->setWidget(new Sidebar(m_sidebar, area));
    m_sidebar->setMinimumWidth(110);
    m_sidebar->setMaximumWidth(300);
    addDockWidget(Qt::LeftDockWidgetArea, m_sidebar, Qt::Horizontal);
    lockSidebar();
    
    addDockWidget(Qt::LeftDockWidgetArea, m_mainArea, Qt::Horizontal);
    
    // Toolbar
    m_toolbar = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea, m_toolbar);
    QPushButton* sidebarToggle = new QPushButton("X");
    sidebarToggle->setCheckable(true);
    sidebarToggle->setChecked(true);
    sidebarToggle->setToolTip("Unlock Sidebar");
    connect(sidebarToggle, SIGNAL(clicked(bool)), this, SLOT(toggleSidebar(bool)));
    m_toolbar->addWidget(sidebarToggle);
    
    
    static_cast<Sidebar*>(m_sidebar->widget())->extensions().at(0)->show();
}

void MainWindow::toggleSidebar(bool val)
{
    if (val) {
        lockSidebar();
        if (sender() != nullptr) {
            static_cast<QPushButton*>(sender())->setToolTip("Unlock Sidebar");
        }
    } else {
        unlockSidebar();
        if (sender() != nullptr) {
            static_cast<QPushButton*>(sender())->setToolTip("Lock Sidebar");
        }
    }
}

void MainWindow::lockSidebar(void) 
{
    if (m_sidebar == nullptr) {
        return;
    }
    m_sidebar->setFeatures(QDockWidget::NoDockWidgetFeatures);
    m_sidebar->setWindowTitle("");
}

void MainWindow::unlockSidebar(void) 
{
    if (m_sidebar == nullptr) {
        return;
    }
    m_sidebar->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    m_sidebar->setWindowTitle("");
}
