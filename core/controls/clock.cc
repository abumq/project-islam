#include "clock.h"
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QTextItem>
#include "core/settings_loader.h"

QColor Clock::kHourColor = QColor(0, 0, 0, 150);
QColor Clock::kMinuteColor = QColor(0, 0, 0, 100);
QColor Clock::kTextColor = QColor(0, 0, 0);

Clock::Clock(QWidget *parent) :
    QWidget(parent)
{
    m_liveTimer = new QTimer(this);
    connect(m_liveTimer, SIGNAL(timeout()), this, SLOT(update()));
    // Update clock every second
    m_liveTimer->start(1000);
    
    setObjectName("Clock");
    QString themeColor = SettingsLoader::getInstance()->get(SettingsLoader::kSettingKeyTheme, QVariant("0, 0, 0")).toString();
    QStringList colors = themeColor.split(",");
    int r = 0;
    int g = 0;
    int b = 0;
    if (colors.size() >= 3) {
        r = colors.at(0).trimmed().toInt();
        g = colors.at(1).trimmed().toInt();
        b = colors.at(1).trimmed().toInt();
    }
    QColor c = QColor(r, g, b);
    kHourColor = c;
    kMinuteColor = c;
    kTextColor = c;
    kHourColor.setAlpha(150);
    kMinuteColor.setAlpha(100);
    kTextColor.setAlpha(255);
    deselect();
}

Clock::~Clock()
{
    delete m_liveTimer;
    m_liveTimer = nullptr;
}

void Clock::liveClock()
{
    if (!m_live) {
        m_live = true;
    }
}

void Clock::resize(int sz)
{
    QWidget::resize(sz, sz);
}

void Clock::paintEvent(QPaintEvent *)
{
    static const QPoint hourHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -40)
    };
    static const QPoint minuteHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -70)
    };
    
    int side = qMin(width(), height());
    QTime time;
    if (m_live) {
        time = QTime::currentTime();
    } else {
        time = QTime(m_h, m_m, m_s);
    }
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(kHourColor);
    
    painter.save();
    painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    painter.drawConvexPolygon(hourHand, 3);
    painter.restore();
    
    painter.setPen(kHourColor);
    
    for (int i = 0; i < 12; ++i) {
        painter.drawLine(88, 0, 96, 0);
        painter.rotate(30.0);
    }
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(kMinuteColor);
    
    painter.save();
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();
    
    painter.setPen(kMinuteColor);
    
    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
            painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);
    }
    
    painter.setPen(kTextColor);
    if (m_displayTextualTime) {
        painter.setFont(QFont("Arial", 15));
        QString tH = QString::number(time.hour());
        if (tH.length() == 1) {
            tH = "0" + tH;
        }
        QString tM = QString::number(time.minute());
        if (tM.length() == 1) {
            tM = "0" + tM;
        }
        QString tS = QString::number(time.second());
        if (tS.length() == 1) {
            tS = "0" + tS;
        }
        QString t = tH + ":" + tM + (time.second() == 0 ? "" : ":" + tS);
        painter.drawText(time.second() == 0 ? -20 : -30, -20, t);
    }
    if (!m_title.isEmpty()) {
        int offset = -(m_title.length() * 5);
        painter.setFont(QFont("Arial", 15, QFont::Bold));
        painter.drawText(offset, -50, m_title);
    }
}

void Clock::setTime(int h, int m, int s)
{
    m_h = h;
    m_m = m;
    m_s = s;
    m_live = false;
    QString tH = QString::number(h);
    if (tH.length() == 1) {
        tH = "0" + tH;
    }
    QString tM = QString::number(m);
    if (tM.length() == 1) {
        tM = "0" + tM;
    }
    QString tS = (s == 0 ? "" : (":" + QString::number(s)));
    if (tS.length() == 1) {
        tS = "0" + tS;
    }
    QString t = tH + ":" + tM + tS;
    setToolTip(m_title.isEmpty() ? "" : ("<b>" + m_title + "</b><br/>") + t);
}

void Clock::setDisplayTextualTime(bool displayTextualTime)
{
    m_displayTextualTime = displayTextualTime;
}

void Clock::setTitle(const QString& title)
{
    m_title = title;
}

void Clock::select()
{
    QPalette p(palette());
    p.setColor(QPalette::Background, Qt::yellow);
    setAutoFillBackground(true);
    setPalette(p);
    m_selected = true;
}

void Clock::deselect()
{
    QPalette p(palette());
    p.setColor(QPalette::Background, Qt::color0);
    setAutoFillBackground(false);
    setPalette(p);
    m_selected = false;
}
