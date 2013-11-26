#ifndef EXTENSION_INFO_H
#define EXTENSION_INFO_H

#include <QString>
#include "core/logging.h"
#include "core/extension/abstract_extension.h"
class ExtensionInfo {
public:
    ExtensionInfo() :
        m_isInitialized(false) {
    }

    ExtensionInfo(unsigned int majorVersion, unsigned int minorVersion, const QString& author,
        const QString& name, const QString& title, const QString& description) {

        CHECK(!author.isEmpty()) << "Author name is required";
        CHECK(!name.isEmpty()) << "Extension name is required";
        CHECK(!description.isEmpty()) << "Description is required";
        CHECK(author.length() <= AbstractExtension::kExtensionMaxLengthAuthor) << "Author name is too long, max allowed: [" 
                                                                        << AbstractExtension::kExtensionMaxLengthAuthor << "]";
        CHECK(name.length() <= AbstractExtension::kExtensionMaxLengthName) << "Extension name is too long, max allowed: [" 
                                                                        << AbstractExtension::kExtensionMaxLengthName << "]";
        CHECK(title.length() <= AbstractExtension::kExtensionMaxLengthTitle) << "Title too long, max allowed: [" 
                                                                        << AbstractExtension::kExtensionMaxLengthTitle << "]";
        CHECK(description.length() <= AbstractExtension::kExtensionMaxLengthDescription) << "Description too long, max allowed: [" 
                                                                        << AbstractExtension::kExtensionMaxLengthDescription << "]";
        CHECK(majorVersion <= 99) << "Invalid major version";
        CHECK(minorVersion <= 99) << "Invalid minor version";
        m_majorVersion = majorVersion;
        m_minorVersion = minorVersion;
        m_author = author;
        m_name = name;
        m_title = title;
        m_description = description;
        m_isInitialized = true;
    }

    unsigned int majorVersion() const
    {
        return m_majorVersion;
    }
    
    unsigned int minorVersion() const
    {
        return m_minorVersion;
    }
    
    QString author() const
    {
        return m_author;
    }
    
    QString name() const
    {
        return m_name;
    }
    
    QString title() const
    {
        return m_title;
    }
    
    QString description() const
    {
        return m_description;
    }
    
    bool isInitialized() const {
        return m_isInitialized;
    }
private:
    unsigned int m_majorVersion;
    unsigned int m_minorVersion;
    QString m_author;
    QString m_name;
    QString m_title;
    QString m_description;
    bool m_isInitialized;
};
#endif // EXTENSION_INFO_H
