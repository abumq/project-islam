#ifndef EXTENSION_INTERFACE_H
#define EXTENSION_INTERFACE_H

#include <QtPlugin>

class Extension;

class ExtensionInterface
{
public:
    virtual ~ExtensionInterface() {}
    virtual Extension *extension() = 0;
    virtual void initialize() = 0;

    virtual int majorVersion(void) const = 0;
    virtual int minorVersion(void) const = 0;
    virtual int patchVersion(void) const = 0;
};

Q_DECLARE_INTERFACE(ExtensionInterface, "ProjectIslam.ExtensionInterface.v1.0")

#endif // EXTENSION_INTERFACE_H
