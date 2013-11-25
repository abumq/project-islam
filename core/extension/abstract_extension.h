#ifndef ABSTRACT_EXTENSION_H
#define ABSTRACT_EXTENSION_H

#include <QtPlugin>
#include <QObject>
#include "core/logging.h"
#include "core/extension/extension.h"
#include "core/extension/extension_info.h"

class AbstractExtension : public QObject
{
public:
    AbstractExtension() :
        m_extension(nullptr) {
        el::Loggers::reconfigureAllLoggers(el::ConfigurationType::ToFile, "false");
        
        // Do not call initialize here since we dont want to run into FATAL error
        // of not initializing extensionInfo
    }
    
    virtual ~AbstractExtension() {
        // No need to delete m_extension as plugin loader
        // i.e, ExtensionLoader deletes this anyway in fact doing this
        // will cause segmentation fault
    }
    
    Extension *extension() {
        if (m_extension == nullptr) {
            m_extension = new Extension(0, &m_extensionInfo);
        }
        return m_extension;
    }
    
    /// @brief Returns true if successfully initialized
    virtual bool initialize(const el::Configurations* confFromLoader) {
        CHECK (m_extensionInfo.isInitialized()) << "Please initialize ExtensionInfo (using constructor) from constructor of your extension.";
        if (confFromLoader == nullptr) return false;
        
        // We dont expect confFromLoader being null from loader
        el::Loggers::setDefaultConfigurations(*confFromLoader, true);
        QObject::connect(extension(), SIGNAL(containerGeometryChanged()), this, SLOT(onContainerGeometryChanged()));
        return true;
    }
    
    void setExtensionInfo(const ExtensionInfo& extensionInfo) {
        m_extensionInfo = extensionInfo;
    }
    
    const ExtensionInfo& extensionInfo() const {
        return m_extensionInfo;
    }

public slots:
    
    /// @brief This needs to connect to Extension::containerGeometryChanged()
    virtual void onContainerGeometryChanged() = 0;
private:
    Extension* m_extension;
    ExtensionInfo m_extensionInfo;
};

Q_DECLARE_INTERFACE(AbstractExtension, "ProjectIslam.AbstractExtension.v1.0")

#endif // ABSTRACT_EXTENSION_H
