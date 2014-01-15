#ifndef QIBLA_COMPASS_H
#define QIBLA_COMPASS_H

#include <QGraphicsView>

class QiblaCompass : public QGraphicsView
{
    Q_OBJECT
public:
    static const double kLatitude;
    static const double kLongitude;
    
    QiblaCompass(double latitude, double longitude, QWidget *parent = 0);
    ~QiblaCompass();
    
    void updateCompass(double latitude, double longitude);
    double angle(void) const;
    QString readDirection(void) const;
    void resize(int);
private:
    double m_latitude;
    double m_longitude;
};

#endif // QIBLA_COMPASS_H
