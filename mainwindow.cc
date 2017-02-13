#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QResizeEvent>
#include <QLabel>
#include <QProcess>
#include <QSplashScreen>

#include "settings_dialog.h"
#include "core/utils/memory.h"
#include "core/constants.h"
#include "core/extension/abstract_extension.h"
#include "core/extension/extension_info.h"
#include "core/extension/extension_bar.h"
#include "core/extension/extension_item.h"
#include "core/extension/extension_loader.h"
#include "core/logging/logging.h"
#include "core/utils/version.h"

MainWindow::MainWindow(QSplashScreen *splashScreen) :
    QMainWindow(nullptr),
    ui(new Ui::MainWindow),
    m_splashScreen(splashScreen)
{
    memory::turnToNullPtr(m_container, m_extensionBar, m_trayIcon);
    ui->setupUi(this);
    m_settingsDialog = new SettingsDialog(this, this);
    initialize();
}

MainWindow::~MainWindow()
{
    _TRACE;
    memory::deleteAll(m_extensionBar, m_container, m_trayIcon, ui);
}

void MainWindow::initialize()
{
    memory::deleteAll(m_extensionBar, m_container);
    if (m_splashScreen != nullptr) {
        m_splashScreen->showMessage("Checking updates from local filesystem..."
                                    , Qt::AlignHCenter | Qt::AlignBottom);
    }
    
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        m_trayIcon = new QSystemTrayIcon(QIcon(":/img/project-tray"), this);
        m_trayIcon->setContextMenu(ui->menu_Application);
        m_trayIcon->show(); // FIXME: This does not work on some desktop managers!
        qApp->setQuitOnLastWindowClosed(false);
        connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    } else {
        DLOG(INFO) << "System tray not available";
    }
    m_updateManager.updateFiles();
    if (!m_updateManager.performedUpdate()) {
        loadSettings();
        m_dataHolder.initialize(m_splashScreen);
        
        m_container = new QWidget(this);
        m_container->setObjectName("extensionContainer");
        m_container->setGeometry(ExtensionBar::kExtensionBarWidth, AbstractExtension::kExtensionTop, width(), height());
        
        m_extensionBar = new ExtensionBar(this, m_container);
        connect(m_extensionBar, SIGNAL(extensionChanged(AbstractExtension*)), this, SLOT(onExtensionChanged(AbstractExtension*)));
        addToolBar(Qt::LeftToolBarArea, m_extensionBar);
        
        ExtensionLoader extensionLoader(&m_dataHolder, ui->menuBar, m_settingsDialog, m_trayIcon);
        extensionLoader.loadAll(m_extensionBar, m_splashScreen, el::Helpers::storage());
        initializeDefaultExtension();
#if !defined(DISABLE_AUTO_UPDATE)
        m_updateManager.initialize(m_extensionBar);
#endif // DISABLE_AUTO_UPDATE
        
        setWindowState(Qt::WindowMaximized);
        m_previousWindowsState = Qt::WindowMaximized;
        
        QObject::connect(ui->actionRestart, SIGNAL(triggered()), this, SLOT(restart()));
        reloadStyles();
    }
    if (m_splashScreen != nullptr) {
        m_splashScreen->finish(this);
    }
}

void MainWindow::reloadStyles()
{
    m_extensionBar->setStyleSheet(m_styleLoader.load(StyleLoader::StyleType::ExtensionBar));
    m_container->setStyleSheet(m_styleLoader.load(StyleLoader::StyleType::Extension));
    setStyleSheet(m_styleLoader.load(StyleLoader::StyleType::Menu));
    for (AbstractExtension* extension : *m_extensionBar->extensions()) {
        extension->titleLabel()->setStyleSheet(m_styleLoader.load(StyleLoader::StyleType::Extension));
        extension->extensionItem()->setStyleSheet(m_styleLoader.load(StyleLoader::StyleType::ExtensionItem));
    }
}

void MainWindow::loadSettings()
{
    QString theme = SettingsLoader::getInstance()->get(SettingsLoader::kSettingKeyTheme, 
                                                       StyleLoader::defaultTheme()).toString();
    QStringList rgbList = theme.split(',');
    int r = rgbList.at(0).trimmed().toInt();
    int g = rgbList.at(1).trimmed().toInt();
    int b = rgbList.at(2).trimmed().toInt();
    m_styleLoader = StyleLoader(r, g, b);
    
    // TODO: Update log settings also update LoggingConfigurer::baseConfiguration() so that when
    // in future LoggingConfigurer::configureLoggers() is used we default it to this new log file and other configurations
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
        if (m_trayIcon != nullptr) {
            m_trayIcon->setToolTip(windowTitle() + " - " + qApp->applicationName());
        }
    }
    m_container->resize(width(), height());
    if (m_extensionBar->currentExtension() != nullptr) {
        m_extensionBar->currentExtension()->resize(m_container->size());
    }
}

void MainWindow::on_action_Settings_triggered()
{
    m_settingsDialog->exec();
    if (m_settingsDialog->homeDirectoryChanged) {
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
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About", 
                             QString(qApp->applicationDisplayName() + " v") +
                             version::versionString());
    
}

void MainWindow::on_actionExtension_Bar_triggered(bool checked)
{
    // FIXME: m_container->setGeometry does not properly work, until it works properly
    //        we will leave this menu item disabled
    m_extensionBar->setVisible(checked);
    m_container->setGeometry(checked ? ExtensionBar::kExtensionBarWidth : 0, 
                             AbstractExtension::kExtensionTop, width(), height());
}

bool MainWindow::applicationUpdated() const
{
    return m_updateManager.performedUpdate();
}

void MainWindow::on_actionAbout_Extensions_triggered()
{
    QString aboutExtensionsStr;
    for (AbstractExtension* extension : *m_extensionBar->extensions()) {
        aboutExtensionsStr.append(
                    "** " + extension->info()->name() 
                    + " v" + extension->info()->versionString() + " by "
                    + extension->info()->author() + "\n\n"
                    );
    }
    QMessageBox::information(this, "About Extensions", aboutExtensionsStr);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_trayIcon != nullptr && m_trayIcon->isVisible()) {
        QMessageBox::information(this, windowTitle(),
                                 tr("The program will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::MiddleClick:
        if (!isVisible()) {
            showNormal();
        } else {
            hide();
        }
        break;
    default:
        break;
    }
}

void MainWindow::initializeDefaultExtension()
{
    const QString kExtensionFromArgParamName = "--extension=";
    QString extensionFromParam;
    for (QString arg : qApp->arguments()) {
        if (arg.startsWith(kExtensionFromArgParamName) && arg.length() > kExtensionFromArgParamName.length()) {
            extensionFromParam = arg.mid(kExtensionFromArgParamName.length());
            break;
        }
    }
    ExtensionItem* defaultExtension = nullptr;
    AbstractExtension* abstractExtensionFromParam = nullptr;
    if (!extensionFromParam.isEmpty()
        && (abstractExtensionFromParam = m_extensionBar->hasExtension(extensionFromParam)) != nullptr) {
        defaultExtension = abstractExtensionFromParam->extensionItem();
        LOG(INFO) << "Activating extension [" << extensionFromParam << "]";
    } else {
        defaultExtension = m_extensionBar->defaultExtensionItem();
    }
    if (defaultExtension != nullptr) {
        defaultExtension->showExtension(true);
    } else {
        LOG(ERROR) << "Default extension not found";
    }
}
