#include "qibla_compass.h"
#define _USE_MATH_DEFINES
#include <cmath>
#ifdef _MSC_VER
#   include <float.h>
#   define __isNan(x) _isnan(x)
#else
#   define __isNan(x) isnan(x)
#endif // _MSC_VER
#include <QGraphicsLineItem>
#include "core/logging/logging.h"

QiblaCompass::QiblaCompass(double latitude, double longitude, QWidget *parent) :
<<<<<<< HEAD
    QGraphicsView(new QGraphicsScene(parent), parent),
    m_latitude(latitude),
    m_longitude(longitude)
=======
    m_latitude(latitude),
    m_longitude(longitude),
    QGraphicsView(new QGraphicsScene(parent), parent)
>>>>>>> 69926ae81324f2fdcfb34d17b4dc9ec0d6db8ec7
{
    _TRACE;
    updateCompass(latitude, longitude);
    resize(100);
}

QiblaCompass::~QiblaCompass()
{
    _TRACE;
    QGraphicsScene* s = scene();
    if (s != nullptr) {
        s->clear();
    }
    delete s;
    setScene(nullptr);
}

void QiblaCompass::updateCompass(double latitude, double longitude)
{
    _TRACE;
    scene()->clear();
    m_latitude = latitude;
    m_longitude = longitude;
    double a = angle();
    int length = (width() / 2) - 10;
    int xStart = width() / 2;
    int xEnd = 0;
    int yStart = height() / 2;
    int yEnd = 0;
    if (a > 0) {
        a = -a;
    }
    xEnd = xStart + cos(a) + length;
    yEnd = yStart + sin(a) + length;
    QGraphicsLineItem* line = scene()->addLine(xStart, yStart, xEnd, yEnd);
    Q_UNUSED(line);
    QGraphicsTextItem* n = scene()->addText("N");
    n->setY(1);
    n->setX(width() / 2);
    QGraphicsTextItem* e = scene()->addText("E");
    e->setY(width() / 2);
    e->setX(1);
    QGraphicsTextItem* s = scene()->addText("S");
    s->setY(height() - 1);
    s->setX(width() / 2);
    QGraphicsTextItem* w = scene()->addText("W");
    w->setY(height() / 2);
    w->setX(width() - 1);
}

double QiblaCompass::angle(void) const
{
    _TRACE;
    if (__isNan(m_latitude) || __isNan(m_longitude)) {
        return 0;
    }
    if ((m_latitude > 90.0) || (m_latitude < -90)) {
        return 0;
    }
    if ((m_longitude > 180.0) || (m_longitude < -180)) {
        return 0;
    }
    if ((abs(m_latitude - kLatitude) < abs(-0.01)) && abs(m_longitude - kLongitude) < abs(-0.01)) return 0; // We are in Kabba
    double phiB = kLatitude * M_PI / 180.0;
    double lambdaB = kLongitude * M_PI / 180.0;
    double phiA = m_latitude * M_PI / 180.0;
    double lambdaA = m_longitude * M_PI / 180.0;
    double psi = 180.0 / M_PI;
    psi *= atan2(sin(lambdaB - lambdaA), cos(phiA) * tan(phiB) - sin(phiA) * cos(lambdaB - lambdaA));
    return round(psi);
}

QString QiblaCompass::readDirection(void) const
{
    _TRACE;
    double a = angle();
    return QString::number((a > 0) ? a : -a) + " degrees east of North";
}

void QiblaCompass::resize(int sz)
{
    _TRACE;
    QGraphicsView::resize(sz, sz);
    updateCompass(m_latitude, m_longitude);
}
