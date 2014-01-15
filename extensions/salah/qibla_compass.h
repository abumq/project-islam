#ifndef QIBLA_COMPASS_H
#define QIBLA_COMPASS_H

#include <QGraphicsView>

class QiblaCompass : public QGraphicsView
{
    Q_OBJECT
public:
    static constexpr double kLatitude = 21.4224950;
    static constexpr double kLongitude = 39.8261650;
    
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
