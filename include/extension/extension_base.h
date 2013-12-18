#ifndef EXTENSION_BASE_H
#define EXTENSION_BASE_H

#include <QtPlugin>
#include <QObject>
#include <QMenu>

#include "logging/logging.h"
#include "utils/memory.h"
#include "settings_loader.h"
#include "extension/abstract_extension.h"
#include "extension/extension_info.h"

class ExtensionBase : public QObject
{
    // No Q_OBJECT here. Otherwise we get undefined AbstractExtension::staticMetaObject error
public:
    ExtensionBase() :
        m_extension(nullptr) {
        // We dont want to log to default log file made by extension' _ELPP_DEFAULT_LOG_FILE macro
        el::Loggers::reconfigureAllLoggers(el::ConfigurationType::ToFile, "false");
        // Do not call initialize() here since we dont want to run into FATAL error
        // of __not__ initializing extensionInfo
#ifdef _LOGGER
        el::Loggers::getLogger(_LOGGER);
#endif
#ifdef _PERFORMANCE_LOGGER
        el::Loggers::getLogger(_PERFORMANCE_LOGGER);
#endif
        el::Loggers::setDefaultConfigurations(LoggingConfigurer::baseConfiguration(), true);
    }
    
    virtual ~ExtensionBase() {
        // No need to delete m_extension as plugin loader
        // i.e, ExtensionLoader deletes this anyway in fact doing this
        // will cause segmentation fault
    }
    
    inline AbstractExtension *extension() {
        if (m_extension == nullptr) {
            m_extension = new AbstractExtension(0, &m_extensionInfo);
        }
        return m_extension;
    }
    
    inline QWidget* container() {
        return extension()->container();
    }
    
    inline data::DataHolder* data() {
        return extension()->dataHolder();
    }
    
    /// @brief Need to call this in extension and only proceed if this returns true
    /// Returns true if successfully initialized
    virtual bool initialize(int argc, const char** argv) {
        el::Helpers::setArgs(argc, argv);
        CHECK(m_extensionInfo.isInitialized()) << "Please initialize ExtensionInfo (using constructor) from constructor of your extension.";
        QObject::connect(extension(), SIGNAL(containerGeometryChanged(int, int)), this, SLOT(onContainerGeometryChanged(int, int)));
        QObject::connect(extension(), SIGNAL(activated()), this, SLOT(onActivated()));
        QObject::connect(extension(), SIGNAL(deactivated()), this, SLOT(onDeactivated()));
        return true;
    }
    
    inline void setExtensionInfo(const ExtensionInfo& extensionInfo) {
        m_extensionInfo = extensionInfo;
    }
    
    inline const ExtensionInfo& extensionInfo() const {
        return m_extensionInfo;
    }
    
    inline QMenu* extensionMenu() {
        return extension()->menu();
    }
    
    inline QString settingsKeyPrefix() {
        return "extension_setting__" + m_extensionInfo.name().replace(" ", "") 
                                                  + "__";
    }
    
    inline QVariant setting(const QString& key, const QVariant& defaultValue = QVariant()) {
        return extension()->settingsLoader()->get(settingsKeyPrefix() + key, defaultValue);
    }
    
    inline void saveSetting(const QString &key, const QVariant &value) {
        extension()->settingsLoader()->saveSettings(settingsKeyPrefix() + key, value);
    }
    
public:
    /// @brief Slot that is connected to signal that is emitted when container
    /// geometry changes. This is pretty much similar to QWidget::resizeEvent().
    virtual void onContainerGeometryChanged(int width, int height) = 0;
    
    /// @brief Slot that is connected to signal emmited when extension is activated
    virtual void onActivated() = 0;
    
    /// @brief Slot that is connected to signal emmited when extension is deactivated
    virtual void onDeactivated() = 0;
private:
    AbstractExtension* m_extension;
    ExtensionInfo m_extensionInfo;
};

Q_DECLARE_INTERFACE(ExtensionBase, "ProjectIslam.Api.ExtensionBase.v1.0")
    
#endif // EXTENSION_BASE_H
