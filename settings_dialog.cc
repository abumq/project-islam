#include "settings_dialog.h"
#include "ui_settings_dialog.h"

#include "mainwindow.h"
#include "core/logging.h"
#include "core/style_loader.h"
#include "core/controls/colorbox.h"
#include "core/data/database_builder.h"
#include "core/data/database_manager.h"
#include "core/data/data_holder.h"

SettingsDialog::SettingsDialog(MainWindow* mainWindow, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    m_mainWindow(mainWindow),
    homeDirectoryChanged(false)
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
    settingsMap = ui->cboQuranTranslations->itemData(ui->cboQuranTranslations->currentIndex()).toMap();
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
    // TODO: Get it using defaultConfigurations() when it's available from Easylogging++
    el::base::TypedConfigurations* configurations = el::Loggers::getLogger("default")->typedConfigurations();
    
    std::string filename = configurations->filename(el::Level::Global);
    ui->txtLogFile->setText(QString(filename.c_str()));
    ui->chkLevelGlobal->setChecked(configurations->enabled(el::Level::Global));
    on_chkLevelGlobal_clicked(ui->chkLevelGlobal->isChecked());
    ui->chkLevelInfo->setChecked(configurations->enabled(el::Level::Info));
    ui->chkLevelWarning->setChecked(configurations->enabled(el::Level::Warning));
    ui->chkLevelError->setChecked(configurations->enabled(el::Level::Error));
    ui->chkLevelFatal->setChecked(configurations->enabled(el::Level::Fatal));
    ui->chkLevelDebug->setChecked(configurations->enabled(el::Level::Debug));
    ui->chkLevelVerbose->setChecked(configurations->enabled(el::Level::Verbose));
    
    ui->spnMaxLogFileSize->setValue(configurations->maxLogFileSize(el::Level::Global));
    
    // --------------------- Tab: Quran
    
    // Translations
    data::DatabaseManager dbManager;
    data::QueryResult result = dbManager.query(
                QString("SELECT * FROM sqlite_master WHERE type='table' AND name like 'Quran%Translation%' AND name != 'Quran__English_Transliteration';"));
    int selectedIndex = 0;
    int i = 0;
    for (QSqlRecord rec : result) {
        QString name = rec.value(1).toString();
        QString displayName = name;
        displayName = displayName.mid(QString("Quran__").length());
        displayName.replace("_", " ");
        int index = ui->cboQuranTranslations->findText(displayName);
        if (index != -1) {
            // No duplication!
            continue;
        }
        QMap<QString, QVariant> m;
        m.insert(SettingsLoader::kSettingKeyQuranTranslationFile, "project-islam.db");
        m.insert(SettingsLoader::kSettingKeyQuranTranslationTable, name);
        ui->cboQuranTranslations->addItem(displayName, m);
        if (SettingsLoader().get(SettingsLoader::kSettingKeyQuranTranslationTable, 
                                 QString(quran::Quran::kQuranDefaultTranslationDatabaseTable)) == name) {
            selectedIndex = i;
        }
        ++i;
    }
    // Now look at data/quran_translations folder for other translations
    QDir translationsDir((QStringList() << SettingsLoader().defaultHomeDir() << "data" << "translations").join(QDir::separator()));
    if (translationsDir.exists() && !translationsDir.entryList(QStringList() << "*.db").empty()) {
        QStringList translationsAvailable = translationsDir.entryList(QStringList() << "*.db");
        for (QString t : translationsAvailable) {
            QString dbfilename = "translations/" + t;
            data::DatabaseManager dbManager2("TranslationFinderData", dbfilename);
            result = dbManager2.query(
                        QString("SELECT * FROM sqlite_master WHERE type='table' AND name like 'Quran%Translation%' AND name != 'Quran__English_Transliteration';"));
            for (QSqlRecord rec : result) {
                QString name = rec.value(1).toString();
                QString displayName = name;
                displayName = displayName.mid(QString("Quran__").length());
                displayName.replace("_", " ");
                int index = ui->cboQuranTranslations->findText(displayName);
                if (index != -1) {
                    // No duplication!
                    continue;
                }
                QMap<QString, QVariant> m;
                m.insert(SettingsLoader::kSettingKeyQuranTranslationFile, dbfilename);
                m.insert(SettingsLoader::kSettingKeyQuranTranslationTable, name);
                ui->cboQuranTranslations->addItem(displayName, m);
                if (SettingsLoader().get(SettingsLoader::kSettingKeyQuranTranslationTable, 
                                         QString(quran::Quran::kQuranDefaultTranslationDatabaseTable)) == name &&
                        SettingsLoader().get(SettingsLoader::kSettingKeyQuranTranslationFile, 
                                             QString("project-islam.db")) == dbfilename) {
                    selectedIndex = i;
                }
                ++i;
            }
        }
    }
    ui->cboQuranTranslations->setCurrentIndex(selectedIndex);
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

void SettingsDialog::on_buildQuranEnglish_clicked()
{
    DATA_LOG(INFO) << "Building data: Quran English (Sahih International)";
    data::DatabaseBuilder::build(data::DatabaseBuilder::DataType::QuranSahihTranslation);
    m_mainWindow->dataHolder()->quranTranslation()->load(quran::Quran::TextType::Translation,
                                                         quran::Quran::kQuranTransliterationDatabaseTable);
}

void SettingsDialog::on_buildQuranArabic_clicked()
{
    DATA_LOG(INFO) << "Building data: Quran Arabic";
    data::DatabaseBuilder::build(data::DatabaseBuilder::DataType::QuranArabic);
    m_mainWindow->dataHolder()->quranArabic()->load(quran::Quran::TextType::Original,
                                                    quran::Quran::kQuranArabicDatabaseTable);
}

void SettingsDialog::on_buildQuranChapters_clicked()
{
    DATA_LOG(INFO) << "Building data: Quran Chapters";
    data::DatabaseBuilder::build(data::DatabaseBuilder::DataType::QuranChapters);
}

void SettingsDialog::on_buildDua_clicked()
{
    DATA_LOG(INFO) << "Building data: Dua";
    data::DatabaseBuilder::build(data::DatabaseBuilder::DataType::Dua);
}

void SettingsDialog::on_buildQuranTransliterationEnglish_clicked()
{
    DATA_LOG(INFO) << "Building data: Quran Tranliteration (English)";
    data::DatabaseBuilder::build(data::DatabaseBuilder::DataType::QuranTransliteration);
    m_mainWindow->dataHolder()->quranTransliteration()->load(quran::Quran::TextType::Transliteration, quran::Quran::kQuranTransliterationDatabaseTable);
}
