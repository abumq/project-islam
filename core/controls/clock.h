#ifndef CLOCK_H
#define CLOCK_H

#include <QWidget>

class QPaintEvent;
class QTimer;

class Clock : public QWidget
{
    Q_OBJECT
public:
    static QColor kHourColor;
    static QColor kMinuteColor;
    static QColor kTextColor;
    
    explicit Clock(QWidget *parent = 0);
    ~Clock();
    void liveClock();
    void resize(int);
    void paintEvent(QPaintEvent *);
    void setTime(int h, int m, int s = 0);
    void setDisplayTextualTime(bool);
    void setTitle(const QString&);
    void select();
    void deselect();
    inline bool selected() { return m_selected; }
    inline int h() const { return m_h; }
    inline int m() const { return m_m; }
    inline int s() const { return m_s; }
protected:
    int m_h;
    int m_m;
    int m_s;
    bool m_live;
    bool m_selected;
    QTimer* m_liveTimer;
    bool m_displayTextualTime;
    QString m_title;
};

#endif // CLOCK_H
