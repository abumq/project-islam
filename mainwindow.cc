#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QResizeEvent>
#include <QLabel>
#include <QProcess>

#include "settings_dialog.h"
#include "core/logging.h"
#include "core/memory.h"
#include "core/constants.h"
#include "core/extension/abstract_extension.h"
#include "core/extension/extension_info.h"
#include "core/extension/extension_bar.h"
#include "core/extension/extension_item.h"
#include "core/extension/extension_loader.h"

MainWindow::MainWindow(QApplication* app) :
    QMainWindow(nullptr),
    ui(new Ui::MainWindow),
    m_app(app)
{
    memory::turnToNullPtr(m_container, m_extensionBar);
    ui->setupUi(this);
    
    initialize();
}

MainWindow::~MainWindow()
{
    _TRACE;
    memory::deleteAll(m_extensionBar, m_container, ui);
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
    m_container->setGeometry(ExtensionBar::kExtensionBarWidth, AbstractExtension::kExtensionTop, width(), height());
    
    m_extensionBar = new ExtensionBar(this, m_container);
    connect(m_extensionBar, SIGNAL(extensionChanged(AbstractExtension*)), this, SLOT(onExtensionChanged(AbstractExtension*)));
    addToolBar(Qt::LeftToolBarArea, m_extensionBar);
    
    ExtensionLoader extensionLoader(&m_dataHolder, &m_settingsLoader, ui->menuBar);
    extensionLoader.loadAll(m_app->applicationDirPath(), m_extensionBar);
    
    ExtensionItem* defaultExtension = m_extensionBar->defaultExtensionItem();
    if (defaultExtension != nullptr) {
        defaultExtension->showExtension(true);
    } else {
        LOG(ERROR) << "Default extension not found";
    }
    
    setWindowState(Qt::WindowMaximized);
    m_previousWindowsState = Qt::WindowMaximized;
    
    QObject::connect(ui->actionRestart, SIGNAL(triggered()), this, SLOT(restart()));
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
    for (AbstractExtension* extension : *m_extensionBar->extensions()) {
        extension->titleLabel()->setStyleSheet(m_styleLoader.load(StyleLoader::StyleType::Extension));
    }
}

void MainWindow::loadSettings()
{
    QString theme = m_settingsLoader.get(SettingsLoader::kSettingKeyTheme, 
                                         StyleLoader::defaultTheme()).toString();
    QStringList rgbList = theme.split(',');
    int r = rgbList.at(0).trimmed().toInt();
    int g = rgbList.at(1).trimmed().toInt();
    int b = rgbList.at(2).trimmed().toInt();
    m_styleLoader = StyleLoader(r, g, b);
    
    // TODO: Update log settings also update LoggingConfigurer::baseConfiguration() so that when
    // in future LoggingConfigurer::configureLoggers() is used we default it to this new log file and other configurations
    
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
    AbstractExtension* currentExtension = m_extensionBar->currentExtension();
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

void MainWindow::onExtensionChanged(AbstractExtension *extension)
{
    if (extension != nullptr) {
        setWindowTitle(extension->info()->title());
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
        restart();
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

void MainWindow::restart()
{
    m_app->quit();
    QProcess::startDetached(m_app->arguments()[0], m_app->arguments());
}
