#ifndef EXTENSION_INFO_H
#define EXTENSION_INFO_H

#include <QString>

#include "core/logging/logging.h"
#include "core/extension/abstract_extension.h"

class ExtensionInfo {
public:
    ExtensionInfo() :
        m_isInitialized(false) {
    }
    
    ExtensionInfo(unsigned int majorVersion, unsigned int minorVersion, unsigned int patchVersion, 
                  const QString& author, const QString& name, const QString& title, const QString& description) {
        
        CHECK(!author.isEmpty()) << "Author name is required";
        CHECK(!name.isEmpty()) << "Extension name is required";
        CHECK(!description.isEmpty()) << "Description is required";
        /*CHECK(author.length() <= AbstractExtension::kExtensionMaxLengthAuthor) << "Author name is too long, max allowed: [" 
                                                                               << AbstractExtension::kExtensionMaxLengthAuthor << "]";
        CHECK(name.length() <= AbstractExtension::kExtensionMaxLengthName) << "Extension name is too long, max allowed: [" 
                                                                           << AbstractExtension::kExtensionMaxLengthName << "]";
        CHECK(title.length() <= AbstractExtension::kExtensionMaxLengthTitle) << "Title too long, max allowed: [" 
                                                                             << AbstractExtension::kExtensionMaxLengthTitle << "]";
        CHECK(description.length() <= AbstractExtension::kExtensionMaxLengthDescription) << "Description too long, max allowed: [" 
                                                                                         << AbstractExtension::kExtensionMaxLengthDescription << "]";
        CHECK(majorVersion <= 99) << "Invalid major version";
        CHECK(minorVersion <= 99) << "Invalid minor version";
        CHECK(patchVersion <= 99) << "Invalid patch version";*/
        m_majorVersion = majorVersion;
        m_minorVersion = minorVersion;
        m_patchVersion = patchVersion;
        m_author = author;
        m_name = name;
        m_title = title;
        m_description = description;
        m_isInitialized = true;
    }
    
    inline unsigned int majorVersion() const
    {
        return m_majorVersion;
    }
    
    inline unsigned int minorVersion() const
    {
        return m_minorVersion;
    }
    
    inline unsigned int patchVersion() const
    {
        return m_patchVersion;
    }
    
    inline QString author() const
    {
        return m_author;
    }
    
    inline QString name() const
    {
        return m_name;
    }
    
    inline QString title() const
    {
        return m_title;
    }
    
    inline QString description() const
    {
        return m_description;
    }
    
    inline bool isInitialized() const {
        return m_isInitialized;
    }
    
    
    inline QString versionString() const {
        return QString::number(m_majorVersion) + 
                "." + QString::number(m_minorVersion) + 
                "." + QString::number(m_patchVersion);
    }
    
    inline bool isCurrentVersion(const QString& versionStr) const {
        QStringList sep = versionStr.split(".");
        return sep[0].toInt() == m_majorVersion && 
                sep[1].toInt() == m_minorVersion && 
                sep[2].toInt() == m_patchVersion;
    }
    
private:
    unsigned int m_majorVersion;
    unsigned int m_minorVersion;
    unsigned int m_patchVersion;
    QString m_author;
    QString m_name;
    QString m_title;
    QString m_description;
    bool m_isInitialized;
};
#endif // EXTENSION_INFO_H
