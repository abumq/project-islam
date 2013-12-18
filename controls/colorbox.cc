#include "controls/colorbox.h"

#include <algorithm>

#include <QPixmap>
#include <QPainter>

#include "logging/logging.h"

ColorBox::ColorBox(const QColor& defaultColor, QWidget *parent) : 
    QComboBox(parent), 
    m_defaultColor(defaultColor)
{
    setEditable(false);
    initColorVector();
    init();
    setColor(m_defaultColor);
}

void ColorBox::init()
{
    QPixmap icon = QPixmap(28, 16);
    QRect r = QRect(0, 0, 27, 15);
    
    
    QPainter p;
    p.begin(&icon);
    
    int i = 0;
    icon.fill ( m_colors[i] );
    addItem(icon, tr( "Default" ) );
    
    p.setBrush(QBrush(m_colors[++i]));
    p.drawRect(r);
    addItem(icon, tr( "Dark Gray" ) );
    
    p.setBrush(QBrush(m_colors[++i]));
    p.drawRect(r);
    addItem(icon, tr( "Gray" ) );
    
    p.setBrush(QBrush(m_colors[++i]));
    p.drawRect(r);
    addItem(icon, tr( "Light Gray" ) );
    
    p.setBrush(QBrush(m_colors[++i]));
    p.drawRect(r);
    addItem(icon, tr( "Dark Yellow" ) );
    
    p.setBrush(QBrush(m_colors[++i]));
    p.drawRect(r);
    addItem(icon, tr( "Navy" ) );
    
    p.setBrush(QBrush(m_colors[++i]));
    p.drawRect(r);
    addItem(icon, tr( "Purple" ) );
    
    p.setBrush(QBrush(m_colors[++i]));
    p.drawRect(r);
    addItem(icon, tr( "Maroon" ) );
    
    p.setBrush(QBrush(m_colors[++i]));
    p.drawRect(r);
    addItem(icon, tr( "Olive" ) );
    
    p.setBrush(QBrush(m_colors[++i]));
    p.drawRect(r);
    addItem(icon, tr( "Dark Cyan" ) );
    
    p.setBrush(QBrush(m_colors[++i]));
    p.drawRect(r);
    addItem(icon, tr( "Orange" ) );
    
    p.setBrush(QBrush(m_colors[++i]));
    p.drawRect(r);
    addItem(icon, tr( "Violet" ) );
    
    p.setBrush(QBrush(m_colors[++i]));
    p.drawRect(r);
    addItem(icon, tr( "Pink" ) );
    p.end();
}

void ColorBox::initColorVector()
{
    m_colors.push_back(m_defaultColor);
    m_colors.push_back(QColor(Qt::darkGray));
    m_colors.push_back(QColor(Qt::gray));
    m_colors.push_back(QColor(Qt::lightGray));
    m_colors.push_back(QColor(Qt::darkYellow));
    m_colors.push_back(QColor(Qt::darkBlue));
    m_colors.push_back(QColor(Qt::darkMagenta));
    m_colors.push_back(QColor(Qt::darkRed));
    m_colors.push_back(QColor(Qt::darkGreen));
    m_colors.push_back(QColor(Qt::darkCyan));
    m_colors.push_back(QColor("#FF8000"));
    m_colors.push_back(QColor("#8000FF"));
    m_colors.push_back(QColor("#FF0080"));
}

void ColorBox::setColor(const QColor& c)
{
    CHECK(count() > 0) << "ColorBox not initialized";
    for (int i = 0; i < m_colors.size(); ++i) {
        if (m_colors.at(i) == c) {
            setCurrentIndex(i);
            return;
        }
    }
    // Not found
    if (c == m_defaultColor) {
        setCurrentIndex(0);
    } else {
        setColor(m_defaultColor);
    }
}

QColor ColorBox::color() const
{
    int i = currentIndex();
    if (i >= 0) {
        return m_colors.at(i);
    }
    else {
        return m_defaultColor;
    }
}
