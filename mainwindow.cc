#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QResizeEvent>
#include <QLabel>
#include <QProcess>

#include "core/logging.h"
#include "core/constants.h"
#include "core/extension/extension.h"
#include "core/extension/extension_bar.h"
#include "core/extension/extension_item.h"
#include "core/extension/extension_loader.h"

#include "settings_dialog.h"

MainWindow::MainWindow(QApplication* app) :
    QMainWindow(0),
    ui(new Ui::MainWindow),
    m_app(app),
    m_container(nullptr),
    m_extensionBar(nullptr)
{
    ui->setupUi(this);
    
    initialize();
}

MainWindow::~MainWindow()
{
    _TRACE;
    QMap<QString, QVariant> settingsMap;
    settingsMap.insert("theme", StyleLoader::rgb(m_styleLoader.r(), m_styleLoader.g(), m_styleLoader.b()));
    m_settingsLoader.saveSettings(&settingsMap);
    delete m_extensionBar;
    m_extensionBar = nullptr;
    delete m_container;
    m_container = nullptr;
    delete ui;
}

void MainWindow::initialize()
{
    if (m_extensionBar != nullptr) {
        delete m_extensionBar;
        m_extensionBar = nullptr;
    }
    if (m_container != nullptr) {
        delete m_container;
        m_container = nullptr;
    }
    loadSettings();
    
    m_container = new QWidget(this);
    m_container->setObjectName("extensionContainer");
    m_container->setGeometry(ExtensionBar::kExtensionBarWidth, Extension::kExtensionTop, width(), height());
    
    m_extensionBar = new ExtensionBar(this, m_container);
    connect(m_extensionBar, SIGNAL(extensionChanged(Extension*)), this, SLOT(onExtensionChanged(Extension*)));
    addToolBar(Qt::LeftToolBarArea, m_extensionBar);
    
    ExtensionLoader extensionLoader(&m_dataHolder);
    extensionLoader.loadAll(m_app->applicationDirPath(), m_extensionBar);
    
    ExtensionItem* defaultExtension = m_extensionBar->defaultExtensionItem();
    if (defaultExtension != nullptr) {
        defaultExtension->showExtension(true);
    } else {
        LOG(ERROR) << "Default extension not found";
    }
    
    setWindowState(Qt::WindowMaximized);
    m_previousWindowsState = Qt::WindowMaximized;
    
    reloadStyles();
}

void MainWindow::reloadStyles()
{
    TIMED_BLOCK(timer, "reloadStyles");
    m_extensionBar->setStyleSheet(m_styleLoader.load(StyleLoader::StyleType::ExtensionBar));
    m_container->setStyleSheet(m_styleLoader.load(StyleLoader::StyleType::Extension));
    setStyleSheet(m_styleLoader.load(StyleLoader::StyleType::Menu));
    for (ExtensionItem* item : *m_extensionBar->extensionItems()) {
        item->setStyleSheet(m_styleLoader.load(StyleLoader::StyleType::ExtensionItem));
    }
    for (Extension* extension : *m_extensionBar->extensions()) {
        extension->titleLabel()->setStyleSheet(m_styleLoader.load(StyleLoader::StyleType::Extension));
    }
}

void MainWindow::loadSettings()
{
    QString theme = m_settingsLoader.get("theme", StyleLoader::defaultTheme()).toString();
    QStringList rgbList = theme.split(',');
    int r = rgbList.at(0).trimmed().toInt();
    int g = rgbList.at(1).trimmed().toInt();
    int b = rgbList.at(2).trimmed().toInt();
    m_styleLoader = StyleLoader(r, g, b);
}

SettingsLoader *MainWindow::settingsLoader()
{
    return &m_settingsLoader;
}

StyleLoader *MainWindow::styleLoader()
{
    return &m_styleLoader;
}

data::DataHolder *MainWindow::dataHolder()
{
    return &m_dataHolder;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    m_container->resize(event->size().width(), event->size().height());
    Extension* currentExtension = m_extensionBar->currentExtension();
    if (currentExtension != nullptr) {
        currentExtension->resize(m_container->size());
    }
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit(0);
}

void MainWindow::onExtensionChanged(Extension *extension)
{
    if (extension != nullptr) {
        setWindowTitle(extension->title());
    }
    m_container->resize(width(), height());
    m_extensionBar->currentExtension()->resize(m_container->size());
}

void MainWindow::on_action_Settings_triggered()
{
    SettingsDialog settingsDialog(this, this);
    settingsDialog.exec();
    if (settingsDialog.homeDirectoryChanged) {
        LOG(INFO) << "Home directory has been updated - restarting";
        m_app->quit();
        QProcess::startDetached(m_app->arguments()[0], m_app->arguments());
    } else {
        reloadStyles();
    }
}

void MainWindow::on_actionFull_Screen_triggered(bool checked)
{
    if (checked) {
        m_previousWindowsState = windowState();
        setWindowState(Qt::WindowFullScreen);
    } else {
        setWindowState(m_previousWindowsState);
    }
}
