#include "sidebar.h"
#include "ui_sidebar.h"
#include "easylogging++.h"
#include "features/welcome_page.h"

Sidebar::Sidebar(QWidget* parent, QWidget* mainArea) :
        QWidget(parent),
        ui(new Ui::Sidebar)
{
    ui->setupUi(this);
    loadExtensions(mainArea);
    showExtensions();
}

Sidebar::~Sidebar()
{
    delete ui;
}

const QVector<core::Extension*>& Sidebar::extensions() const
{
    return m_extensions;
}

void Sidebar::addExtension(core::Extension* extension)
{
    if (m_extensions.contains(extension)) {
        LOG(WARNING) << "Ignoring extension [" << m_extensions << "]; already exist.";
        return;
    }
    m_extensions.push_back(extension);
    
}

void Sidebar::loadExtensions(QWidget* mainArea)
{
    WelcomePage* welcomePage = new WelcomePage(mainArea);
    welcomePage->hide();
    addExtension(welcomePage);
}

void Sidebar::showExtensions()
{
    for (QVector<core::Extension*>::const_iterator iter = m_extensions.begin(); iter != m_extensions.end(); ++iter) {
        QListWidgetItem* extensionWidgetItem = new QListWidgetItem((*iter)->name());
        ui->listFeatures->addItem(extensionWidgetItem);
    }
}
