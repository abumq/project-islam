#include "qibla_compass.h"
#define _USE_MATH_DEFINES
#include <cmath>
#ifdef _MSC_VER
#   include <float.h>
#   define __isNan(x) _isnan(x)
#else
#   define __isNan(x) isnan(x)
#endif // _MSC_VER
#ifndef M_PI
#   define M_PI		3.14159265358979323846	/* pi */
#endif // M_PI
#include <QGraphicsLineItem>
#include <QScrollBar>
#include "core/logging/logging.h"

const double QiblaCompass::kLatitude = 21.4224950;
const double QiblaCompass::kLongitude = 39.8261650;

QiblaCompass::QiblaCompass(double latitude, double longitude, QWidget *parent) :
    QGraphicsView(new QGraphicsScene(parent), parent),
    m_latitude(latitude),
    m_longitude(longitude)
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
    int length = (width() / 2);
    int xStart = 0;
    int xEnd = 0;
    int yStart = 0;
    int yEnd = 0;
    if (a > 0) {
        a = -a;
    }
    xEnd = xStart + cos(a) + length;
    yEnd = yStart + sin(a) + length;
    QGraphicsLineItem* line = scene()->addLine(xStart, yStart, xEnd, yEnd);
    line->setPen(QColor(Qt::red));
    QPixmap compass(":/icons/compass");
    QGraphicsPixmapItem* pixmapCompass = scene()->addPixmap(compass);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QString tooltip = readDirection() + " (Based on " + QString::number(m_latitude)
            + ", " + QString::number(m_longitude) + ")";
    pixmapCompass->setToolTip(tooltip);
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
    return psi;//round(psi);
}

QString QiblaCompass::readDirection(void) const
{
    _TRACE;
    double a = angle();
    return "<b>" + QString::number((a > 0) ? a : -a) 
            + " degrees " + ((a > 0) ? "east" : "west") + " of North</b>";
}

void QiblaCompass::resize(int sz)
{
    _TRACE;
    scene()->setSceneRect(0, 0, sz, sz);
    QGraphicsView::resize(sz, sz);
    updateCompass(m_latitude, m_longitude);
}
