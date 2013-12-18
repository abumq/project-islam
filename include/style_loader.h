#ifndef STYLE_LOADER_H
#define STYLE_LOADER_H

#include <QString>

class StyleLoader
{
public:
    enum class StyleType {
        Menu, Extension, ExtensionBar, ExtensionItem
    };

    static const int kDefaultThemeRed = 72;
    static const int kDefaultThemeGreen = 72;
    static const int kDefaultThemeBlue = 72;
    
    static const QString kStylesDirectory;
    
    StyleLoader(int r = kDefaultThemeRed, int g = kDefaultThemeGreen, int b = kDefaultThemeBlue);
    
    void reset(int r, int g, int b);
    QString load(StyleType type) const;
    
    static QString rgba(int r, int g, int b, int a);
    static QString rgb(int r, int g, int b);
    
    int r() const { return m_r; }
    int g() const { return m_g; }
    int b() const { return m_b; }
    static QString defaultTheme();
private:
    int m_r, m_g, m_b;
    QString styleFilename(StyleType type) const;
};

#endif // STYLE_LOADER_H
