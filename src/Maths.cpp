#include <QMatrix4x4>

#include "Maths.h"

float B2(const int &i, const float &t)
{
    switch(i)
    {
    case 0: return (1 - t) * (1 - t);
    case 1: return 2 * t * (1 - t);
    case 2: return t * t;
    default: return 0;
    }
}

float B3(const int &i, const float &t)
{
    switch(i)
    {
        case 0: return (1 - t) * (1 - t) * (1 - t);
        case 1: return 3 * t * (1 - t) * (1 - t);
        case 2: return 3 * t * t * (1 - t);
        case 3: return t * t * t;
        default: return 0;
    }
}

float dB3(const int &i, const float &t)
{
    switch(i)
    {
        case 0: return - 3 * (1 - t) * (1 - t);
        case 1: return 3 * (1 - t) * (1 - 3 * t);
        case 2: return 3 * t * (2 - 3 * t);
        case 3: return 3 * t * t;
        default: return 0;
    }
}

QPoint rotate(const QVector4D& coordinate, const float& width, const float& heigth, const float& alfa, const float& beta)
{

    QMatrix4x4 M;
    M.translate(width / 2, heigth / 2, 0);
    M.rotate(beta, 1, 0, 0);
    M.rotate(alfa, 0, 0, 1);
    M.translate(-width / 2, -heigth / 2, 0);

    QVector4D out = M * coordinate;
    QPoint p(out.x(), out.y());

    /*
    float x1 = coordinate.x() - width / 2;
    float y1 = coordinate.y() - heigth /2;

    const float alfaR = qDegreesToRadians(alfa);
    float x2 = x1 * qCos(alfaR) - y1 * qSin(alfaR);
    float y2 = x1 * qSin(alfaR) + y1 * qCos(alfaR);

    const float betaR = qDegreesToRadians(beta);
    float x3 = x2;
    float y3 = y2 * qCos(betaR) - coordinate.z() * qSin(betaR);
    QPoint p(x3 + width / 2, y3 + heigth / 2);
    */

    wrapPoint(p, width, heigth);
    return p;
}

void wrapPoint(QPoint& p, const int& width, const int& height)
{
    p.rx() = p.x() < 0 ? 0 : p.x();
    p.rx() = p.x() >= width ? width - 1 : p.x();
    p.ry() = p.y() < 0 ? 0 : p.y();
    p.ry() = p.y() >= height ? height - 1 : p.y();
}
