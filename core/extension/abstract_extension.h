#ifndef ABSTRACT_EXTENSION_H
#define ABSTRACT_EXTENSION_H

#include <QtPlugin>
#include "core/logging.h"
#include "core/extension/extension.h"

class AbstractExtension
{
public:
    AbstractExtension() :
            m_extension(nullptr) {
        el::Loggers::reconfigureAllLoggers(el::ConfigurationType::ToFile, "false");
        initialize(false, nullptr);
    }
    
    virtual ~AbstractExtension() {
        // No need to delete m_extension as plugin loader
        // i.e, ExtensionLoader deletes this anyway in fact doing this
        // will cause segmentation fault
    }
    
    Extension *extension() {
        if (m_extension == nullptr) {
            m_extension = new Extension(0, name(), title(), description());
        }
        return m_extension;
    }

    /// @brief Returns true if successfully initialized
    virtual bool initialize(bool initFromLoader, const el::Configurations* confFromLoader) {
        if (!initFromLoader) return false;
    
        // We dont expect confFromLoader being null from loader
        el::Loggers::setDefaultConfigurations(*confFromLoader, true);
        return true;
    }

    virtual int majorVersion(void) const = 0;
    virtual int minorVersion(void) const = 0;
    virtual int patchVersion(void) const = 0;
    virtual const char* author(void) const = 0;
    
    virtual QString name() const = 0;
    virtual QString title() const = 0;
    virtual QString description() const = 0;
    
public slots:

    /// @brief This needs to connect to Extension::containerGeometryChanged()
    virtual void onContainerGeometryChanged() = 0;
private:
    Extension* m_extension;
};

Q_DECLARE_INTERFACE(AbstractExtension, "ProjectIslam.AbstractExtension.v1.0")

#endif // ABSTRACT_EXTENSION_H
