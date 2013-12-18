#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QResizeEvent>
#include <QLabel>
#include <QProcess>
#include <QSplashScreen>

#include "settings_dialog.h"
#include "utils/memory.h"
#include "constants.h"
#include "extension/abstract_extension.h"
#include "extension/extension_info.h"
#include "extension/extension_bar.h"
#include "extension/extension_item.h"
#include "extension/extension_loader.h"
#include "logging/logging.h"
MainWindow::MainWindow(QApplication* app, QSplashScreen *splashScreen) :
    QMainWindow(nullptr),
    ui(new Ui::MainWindow),
    m_app(app),
    m_splashScreen(splashScreen)
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
    memory::deleteAll(m_extensionBar, m_container);
    loadSettings();
    m_dataHolder.initialize(m_app, m_splashScreen);
    
    m_container = new QWidget(this);
    m_container->setObjectName("extensionContainer");
    m_container->setGeometry(ExtensionBar::kExtensionBarWidth, AbstractExtension::kExtensionTop, width(), height());
    
    m_extensionBar = new ExtensionBar(this, m_container);
    connect(m_extensionBar, SIGNAL(extensionChanged(AbstractExtension*)), this, SLOT(onExtensionChanged(AbstractExtension*)));
    addToolBar(Qt::LeftToolBarArea, m_extensionBar);
    
    ExtensionLoader extensionLoader(&m_dataHolder, &m_settingsLoader, m_app, ui->menuBar);
    extensionLoader.loadAll(m_app->applicationDirPath(), m_extensionBar, m_splashScreen);
    
    ExtensionItem* defaultExtension = m_extensionBar->defaultExtensionItem();
    if (defaultExtension != nullptr) {
        defaultExtension->showExtension(true);
    } else {
        LOG(ERROR) << "Default extension not found";
    }
    
#if !defined(DISABLE_AUTO_UPDATE)
    m_updateManager.initialize(m_app, m_extensionBar);
#endif // DISABLE_AUTO_UPDATE

    setWindowState(Qt::WindowMaximized);
    m_previousWindowsState = Qt::WindowMaximized;
    
    QObject::connect(ui->actionRestart, SIGNAL(triggered()), this, SLOT(restart()));
    reloadStyles();
}

void MainWindow::reloadStyles()
{
    TIMED_FUNC(timer);
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
