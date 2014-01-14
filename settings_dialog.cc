#include "settings_dialog.h"
#include "ui_settings_dialog.h"

#include "core/logging/logging.h"
#include "mainwindow.h"
#include "core/style_loader.h"
#include "core/controls/colorbox.h"
#include "core/data/database_manager.h"
#include "core/data/data_holder.h"
#include "core/constants.h"

SettingsDialog::SettingsDialog(MainWindow* mainWindow, QWidget* parent) :
    QDialog(parent),
    homeDirectoryChanged(false),
    ui(new Ui::SettingsDialog),
    m_mainWindow(mainWindow)
{
    ui->setupUi(this);
    loadSettingsInUi();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

QTabWidget *SettingsDialog::settingsTabWidget()
{
    return ui->tabWidget;
}

QMap<QString, QVariant> *SettingsDialog::settingsMap()
{
    return &m_settingsMap;
}

void SettingsDialog::accept()
{
    LOG(INFO) << "Updating settings...";
    
    homeDirectoryChanged = SettingsLoader::getInstance()->defaultHomeDir() != ui->txtHomeDir->text();
    SettingsLoader::getInstance()->updateDefaultHomeDir(ui->txtHomeDir->text());
    m_mainWindow->styleLoader()->reset(m_colorBox->color().red(), m_colorBox->color().green(), m_colorBox->color().blue());
    
    m_settingsMap.insert(SettingsLoader::kSettingKeyQuranTable, ui->cboQuranOriginals->itemData(ui->cboQuranOriginals->currentIndex()));
    m_settingsMap.insert(SettingsLoader::kSettingKeyQuranTransliterationTable, ui->cboQuranTransliterations->itemData(ui->cboQuranTransliterations->currentIndex()));
    m_settingsMap.insert(SettingsLoader::kSettingKeyQuranTranslationTable, ui->cboQuranTranslations->itemData(ui->cboQuranTranslations->currentIndex()));
    m_settingsMap.insert(SettingsLoader::kSettingKeyQuranTafseerTable, ui->cboQuranTafseers->itemData(ui->cboQuranTafseers->currentIndex()));
    m_settingsMap.insert(SettingsLoader::kSettingKeyTheme, StyleLoader::rgb(m_mainWindow->styleLoader()->r(), 
                                                                          m_mainWindow->styleLoader()->g(), m_mainWindow->styleLoader()->b()));
    m_settingsMap.insert(SettingsLoader::kLatitudeKey, ui->spnLatitude->value());
    m_settingsMap.insert(SettingsLoader::kLongitudeKey, ui->spnLongitude->value());
    SettingsLoader::getInstance()->saveSettings(&m_settingsMap);
    m_settingsMap.clear();
    close();
}

void SettingsDialog::reject()
{
    m_settingsMap.clear();
    QDialog::reject();
}

int SettingsDialog::exec()
{
    loadSettingsInUi();
    return QDialog::exec();
}

void SettingsDialog::loadSettingsInUi()
{
    ui->tabWidget->setCurrentIndex(0);
    m_colorBox = new ColorBox(QColor::fromRgb(StyleLoader::kDefaultThemeRed, 
                                              StyleLoader::kDefaultThemeGreen, StyleLoader::kDefaultThemeBlue), ui->groupTheme);
    m_colorBox->move(0, 25);
    // -------------------- Tab: Environment ------------------------
    m_colorBox->setColor(QColor::fromRgb(m_mainWindow->styleLoader()->r(), m_mainWindow->styleLoader()->g(), m_mainWindow->styleLoader()->b()));
    ui->txtHomeDir->setText(SettingsLoader::getInstance()->defaultHomeDir());

    // ---------------------- Tab: Location
    ui->spnLatitude->setValue(SettingsLoader::getInstance()->get(SettingsLoader::kLatitudeKey, QVariant(kDefaultLatitude)).toDouble());
    ui->spnLongitude->setValue(SettingsLoader::getInstance()->get(SettingsLoader::kLongitudeKey, QVariant(kDefaultLongitude)).toDouble());
    
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
        if (SettingsLoader::getInstance()->get(SettingsLoader::kSettingKeyQuranTable, 
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
        if (SettingsLoader::getInstance()->get(SettingsLoader::kSettingKeyQuranTranslationTable, 
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
        if (SettingsLoader::getInstance()->get(SettingsLoader::kSettingKeyQuranTransliterationTable, 
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
        if (SettingsLoader::getInstance()->get(SettingsLoader::kSettingKeyQuranTafseerTable, 
                                 QString(quran::Quran::kQuranDefaultTafseerDatabaseTable)) == nameTafseerTable) {
            selectedIndex = i;
        }
        ++i;
    }
    ui->cboQuranTafseers->setCurrentIndex(selectedIndex);
}

void SettingsDialog::on_rdoManualDayLightSavings_toggled(bool checked)
{
    ui->cboDayLightSavings->setEnabled(checked);
    ui->chkDayLightSaving->setEnabled(checked);
}
