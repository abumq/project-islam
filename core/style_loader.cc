#include "core/style_loader.h"

#include <QFile>

#include "core/logging.h"

const QString StyleLoader::kStylesDirectory = ":";

StyleLoader::StyleLoader(int r, int g, int b)
    : m_r(r), m_g(g), m_b(b)
{
}

void StyleLoader::reset(int r, int g, int b)
{
    m_r = r;
    m_g = g;
    m_b = b;
}

QString StyleLoader::load(StyleType type) const
{
    QFile file(styleFilename(type));
    file.open(QFile::ReadOnly);
    if (!file.isOpen()) {
        LOG(ERROR) << "Error opening file [" << file.fileName() << "]";
        return QString();
    }
    QString style = QString(file.readAll());
    
    const int kThresholdNormal = 50;
    int r2 = std::min(m_r + kThresholdNormal, 255);
    int g2 = std::min(m_g + kThresholdNormal, 255);
    int b2 = std::min(m_b + kThresholdNormal, 255);
    
    style.replace("[:color1]", rgba(m_r, m_g, m_b, 255));
    style.replace("[:color2]", rgba(r2, g2, b2, 255));
    style.replace("[:extensionContainerColor]", rgba(m_r, m_g, m_b, 40));
    style.replace("[:extensionTitleColor]", rgba(std::max(m_r - 20, 0), 
                                                 std::max(m_g - 20, 0), 
                                                 std::max(m_b - 20, 0), 255));
    style.replace("[:hoverColor1]", rgba(200, 200, 200, 50));
    style.replace("[:hoverColor2]", rgba(255, 255, 255, 50));
    style.replace("[:hoverBorder1]", rgba(r2, g2, b2, 255));
    style.replace("[:hoverBorder2]", rgba(m_r, m_g, m_b, 255));
    style.replace("[:menubarColor1]", rgba(m_r, m_g, m_b, 100));
    style.replace("[:menubarColor2]", rgba(m_r, m_g, m_b, 100));
    style.replace("[:menuBgColorPressed]", rgba(m_r, m_g, m_b, 255));
    style.replace("[:menuColorPressed]", rgba(255, 255, 255, 255));
    
    style.replace("[:elementColor1]", rgba(m_r, m_g, m_b, 50));
    style.replace("[:elementColor2]", rgba(r2, g2, b2, 100));
    style.replace("[:elementColor3]", rgba(m_r, m_g, m_b, 150));
    style.replace("[:elementColor4]", rgba(r2, g2, b2, 200));
    return style;
}

QString StyleLoader::rgba(int r, int g, int b, int a)
{
    return rgb(r, g, b) + ", " + QString::number(a);
}

QString StyleLoader::rgb(int r, int g, int b)
{
    return QString::number(r) + ", " + QString::number(g) + ", " + QString::number(b);
}

QString StyleLoader::defaultTheme()
{
    return QString::number(kDefaultThemeRed) + ", " + QString::number(kDefaultThemeGreen) + ", " + QString::number(kDefaultThemeBlue);
}

QString StyleLoader::styleFilename(StyleLoader::StyleType type) const
{
    QString result = kStylesDirectory;
    if (type == StyleType::Menu) {
        result.append("menu");
    } else if (type == StyleType::Extension) {
        result.append("extension");
    } else if (type == StyleType::ExtensionBar) {
        result.append("extension_bar");
    } else if (type == StyleType::ExtensionItem) {
        result.append("extension_item");
    }
    return result;
}
