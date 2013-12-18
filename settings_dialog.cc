#include "settings_dialog.h"
#include "ui_settings_dialog.h"

#include "logging/logging.h"
#include "mainwindow.h"
#include "style_loader.h"
#include "controls/colorbox.h"
#include "data/database_manager.h"
#include "data/data_holder.h"

SettingsDialog::SettingsDialog(MainWindow* mainWindow, QWidget* parent) :
    QDialog(parent),
    homeDirectoryChanged(false),
    ui(new Ui::SettingsDialog),
    m_mainWindow(mainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    m_colorBox = new ColorBox(QColor::fromRgb(StyleLoader::kDefaultThemeRed, 
                                              StyleLoader::kDefaultThemeGreen, StyleLoader::kDefaultThemeBlue), ui->groupTheme);
    m_colorBox->move(0, 25);
    loadSettingsInUi();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::accept()
{
    LOG(INFO) << "Updating settings...";
    el::Configurations configurations = LoggingConfigurer::baseConfiguration();
    configurations.set(el::Level::Global, el::ConfigurationType::Filename, ui->txtLogFile->text().toStdString());
    configurations.set(el::Level::Global, el::ConfigurationType::Enabled, ui->chkLevelGlobal->isChecked() ? "true" : "false");
    if (!ui->chkLevelGlobal->isChecked()) {
        configurations.set(el::Level::Error, el::ConfigurationType::Enabled, ui->chkLevelError->isChecked() ? "true" : "false");
        configurations.set(el::Level::Info, el::ConfigurationType::Enabled, ui->chkLevelInfo->isChecked() ? "true" : "false");
        configurations.set(el::Level::Warning, el::ConfigurationType::Enabled, ui->chkLevelWarning->isChecked() ? "true" : "false");
        configurations.set(el::Level::Fatal, el::ConfigurationType::Enabled, ui->chkLevelFatal->isChecked() ? "true" : "false");
        configurations.set(el::Level::Debug, el::ConfigurationType::Enabled, ui->chkLevelDebug->isChecked() ? "true" : "false");
        configurations.set(el::Level::Verbose, el::ConfigurationType::Enabled, ui->chkLevelVerbose->isChecked() ? "true" : "false");
    }
    configurations.set(el::Level::Global, el::ConfigurationType::MaxLogFileSize, QString::number(ui->spnMaxLogFileSize->value()).toStdString());
    el::Loggers::setDefaultConfigurations(configurations, true);
    
    SettingsLoader s;
    homeDirectoryChanged = s.defaultHomeDir() != ui->txtHomeDir->text();
    s.updateDefaultHomeDir(ui->txtHomeDir->text());
    m_mainWindow->styleLoader()->reset(m_colorBox->color().red(), m_colorBox->color().green(), m_colorBox->color().blue());
    
    QMap<QString, QVariant> settingsMap;
    settingsMap.insert(SettingsLoader::kSettingKeyQuranTable, ui->cboQuranOriginals->itemData(ui->cboQuranOriginals->currentIndex()));
    settingsMap.insert(SettingsLoader::kSettingKeyQuranTransliterationTable, ui->cboQuranTransliterations->itemData(ui->cboQuranTransliterations->currentIndex()));
    settingsMap.insert(SettingsLoader::kSettingKeyQuranTranslationTable, ui->cboQuranTranslations->itemData(ui->cboQuranTranslations->currentIndex()));
    settingsMap.insert(SettingsLoader::kSettingKeyQuranTafseerTable, ui->cboQuranTafseers->itemData(ui->cboQuranTafseers->currentIndex()));
    settingsMap.insert(SettingsLoader::kSettingKeyTheme, StyleLoader::rgb(m_mainWindow->styleLoader()->r(), 
                                                                          m_mainWindow->styleLoader()->g(), m_mainWindow->styleLoader()->b()));
    s.saveSettings(&settingsMap);
    close();
}

void SettingsDialog::loadSettingsInUi()
{
    
    // -------------------- Tab: Environment ------------------------
    m_colorBox->setColor(QColor::fromRgb(m_mainWindow->styleLoader()->r(), m_mainWindow->styleLoader()->g(), m_mainWindow->styleLoader()->b()));
    ui->txtHomeDir->setText(SettingsLoader().defaultHomeDir());
    
    // -------------------- Tab: Logging ------------------------
    
    // All loggers
    el::base::TypedConfigurations configurations = 
            el::Loggers::defaultTypedConfigurations();
    
    std::string filename = configurations.filename(el::Level::Global);
    ui->txtLogFile->setText(QString(filename.c_str()));
    ui->chkLevelGlobal->setChecked(configurations.enabled(el::Level::Global));
    on_chkLevelGlobal_clicked(ui->chkLevelGlobal->isChecked());
    ui->chkLevelInfo->setChecked(configurations.enabled(el::Level::Info));
    ui->chkLevelWarning->setChecked(configurations.enabled(el::Level::Warning));
    ui->chkLevelError->setChecked(configurations.enabled(el::Level::Error));
    ui->chkLevelFatal->setChecked(configurations.enabled(el::Level::Fatal));
    ui->chkLevelDebug->setChecked(configurations.enabled(el::Level::Debug));
    ui->chkLevelVerbose->setChecked(configurations.enabled(el::Level::Verbose));
    
    ui->spnMaxLogFileSize->setValue(static_cast<int>(configurations.maxLogFileSize(el::Level::Global)));
    // --------------------- Tab: Quran
    // Originals
    data::DatabaseManager dbManager;
    data::QueryResult result = dbManager.query(
                QString("SELECT * FROM sqlite_master WHERE type='table' AND name like 'Quran%Arabic%Original%';"));
    int selectedIndex = 0;
    int i = 0;
    for (QSqlRecord rec : result) {
        QString nameTable = rec.value(1).toString();
        QString displayName = nameTable;
        displayName = displayName.mid(QString("Quran__Arabic_Original_").length());
        displayName.replace("_", " ");
        int index = ui->cboQuranOriginals->findText(displayName);
        if (index != -1) {
            // No duplication!
            continue;
        }
        ui->cboQuranOriginals->addItem(displayName, nameTable);
        if (SettingsLoader().get(SettingsLoader::kSettingKeyQuranTable, 
                                 QString(quran::Quran::kQuranDefaultArabicDatabaseTable)) == nameTable) {
            selectedIndex = i;
        }
        ++i;
    }
    ui->cboQuranOriginals->setCurrentIndex(selectedIndex);
    // Translations
    result = dbManager.query(
                QString("SELECT * FROM sqlite_master WHERE type='table' AND name like 'Quran%Translation%';"));
    selectedIndex = 0;
    i = 0;
    for (QSqlRecord rec : result) {
        QString nameTranslationTable = rec.value(1).toString();
        QString displayName = nameTranslationTable;
        displayName = displayName.mid(QString("Quran__").length());
        displayName.replace("_", " ");
        int index = ui->cboQuranTranslations->findText(displayName);
        if (index != -1) {
            // No duplication!
            continue;
        }
        ui->cboQuranTranslations->addItem(displayName, nameTranslationTable);
        if (SettingsLoader().get(SettingsLoader::kSettingKeyQuranTranslationTable, 
                                 QString(quran::Quran::kQuranDefaultTranslationDatabaseTable)) == nameTranslationTable) {
            selectedIndex = i;
        }
        ++i;
    }
    ui->cboQuranTranslations->setCurrentIndex(selectedIndex);
    // Transliteration
    result = dbManager.query(
                QString("SELECT * FROM sqlite_master WHERE type='table' AND name like 'Quran%Transliteration%';"));
    selectedIndex = 0;
    i = 0;
    for (QSqlRecord rec : result) {
        QString nameTransliterationTable = rec.value(1).toString();
        QString displayName = nameTransliterationTable;
        displayName = displayName.mid(QString("Quran__").length());
        displayName.replace("_", " ");
        int index = ui->cboQuranTranslations->findText(displayName);
        if (index != -1) {
            // No duplication!
            continue;
        }
        ui->cboQuranTransliterations->addItem(displayName, nameTransliterationTable);
        if (SettingsLoader().get(SettingsLoader::kSettingKeyQuranTransliterationTable, 
                                 QString(quran::Quran::kQuranDefaultTransliterationDatabaseTable)) == nameTransliterationTable) {
            selectedIndex = i;
        }
        ++i;
    }
    ui->cboQuranTransliterations->setCurrentIndex(selectedIndex);
    // Tafseer
    result = dbManager.query(
                QString("SELECT * FROM sqlite_master WHERE type='table' AND name like 'Quran%Tafseer%';"));
    selectedIndex = 0;
    i = 0;
    for (QSqlRecord rec : result) {
        QString nameTafseerTable = rec.value(1).toString();
        QString displayName = nameTafseerTable;
        displayName = displayName.mid(QString("Quran__").length());
        displayName.replace("_", " ");
        int index = ui->cboQuranTafseers->findText(displayName);
        if (index != -1) {
            // No duplication!
            continue;
        }
        ui->cboQuranTafseers->addItem(displayName, nameTafseerTable);
        if (SettingsLoader().get(SettingsLoader::kSettingKeyQuranTafseerTable, 
                                 QString(quran::Quran::kQuranDefaultTafseerDatabaseTable)) == nameTafseerTable) {
            selectedIndex = i;
        }
        ++i;
    }
    ui->cboQuranTafseers->setCurrentIndex(selectedIndex);
}

void SettingsDialog::on_chkLevelGlobal_clicked(bool checked)
{
    ui->chkLevelInfo->setEnabled(!checked);
    ui->chkLevelWarning->setEnabled(!checked);
    ui->chkLevelError->setEnabled(!checked);
    ui->chkLevelFatal->setEnabled(!checked);
    ui->chkLevelDebug->setEnabled(!checked);
    ui->chkLevelVerbose->setEnabled(!checked);
}
