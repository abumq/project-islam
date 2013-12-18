#ifndef COLORBOX_H
#define COLORBOX_H

#include <QComboBox>
#include <QVector>

class ColorBox : public QComboBox
{
	Q_OBJECT

public:
	ColorBox(const QColor& defaultColor, QWidget *parent = 0);
	
	void setColor(const QColor& c);
	
	QColor color() const;
protected:
	void init();
	void initColorVector();
	QColor m_defaultColor;
	QVector<QColor> m_colors;
};

#endif
