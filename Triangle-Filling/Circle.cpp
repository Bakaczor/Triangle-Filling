#include "Circle.h"

Circle::Circle(uint numOfPoints) : m_i(0), m_numOfPoints(numOfPoints)
{
    const float R = 1 / static_cast<float>(3);
    const QVector2D mid(0.5, 0.5);
    const float d = 2 * M_PI / numOfPoints;
    QList<float> degrees(numOfPoints);
    degrees[0] = 0;
    for (uint i = 1; i < numOfPoints; i++)
    {
        degrees[i] = degrees[i - 1] + d;
    }
    m_points = QList<QVector2D>(numOfPoints);
    for (uint i = 0; i < numOfPoints; i++)
    {
        m_points[i] = R * QVector2D(qCos(degrees.at(i)), qSin(degrees.at(i))) + mid;
    }
}

QVector3D Circle::current() const
{
    return QVector3D(m_points.at(m_i), m_lightZ);
}

QVector3D Circle::next()
{
    m_i = (m_i + 1) % m_numOfPoints;
    return QVector3D(m_points.at(m_i), m_lightZ);
}
