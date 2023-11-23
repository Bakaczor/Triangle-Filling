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

QPoint rotate(const QVector4D& coordinate, const int& width, const int& heigth, const float& alfa, const float& beta)
{
    /*
    QMatrix4x4 translation1(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    translation1.translate(-width / 2, -heigth / 2, 0);

    QMatrix4x4 rotation(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    rotation.rotate(alfa, {1, 0, 0});
    rotation.rotate(beta, {0, 0, 1});

    QMatrix4x4 translation2(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    translation2.translate(width / 2, heigth / 2, 0);

    QMatrix4x4 M = translation1 * rotation * translation2;

    QVector4D out = M * coordinate;
    return QPoint(out.x(), out.y());
    */

    float x1 = coordinate.x() - width / 2;
    float y1 = coordinate.y() - heigth /2;

    float x2 = x1 * cos(alfa) - y1 * sin(alfa);
    float y2 = x1 * sin(alfa) + y1 * cos(alfa);

    float x3 = x2;
    float y3 = y2 * cos(beta) - coordinate.z() * sin(beta);

    return(QPoint(x3 + width / 2, y3 + heigth / 2));
}
