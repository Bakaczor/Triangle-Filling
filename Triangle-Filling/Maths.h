#include <QtMath>
#include <QVector3D>

inline float dotProduct(const QVector3D& v1, const QVector3D& v2)
{
    return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

inline QVector3D crossProduct(const QVector3D& v1, const QVector3D& v2)
{
    return QVector3D(v1.y() * v2.z() - v1.z() * v2.y(), v1.z() * v2.x() - v1.x() * v2.z(), v1.x() * v2.y() - v1.y() * v2.x());
}

inline float triangleArea(const float& x1, const float& y1, const float& x2, const float& y2)
{
    return 0.5 * qFabs(x1 * y2 - y1 * x2);
}

inline int ij2k(const int& i, const int& j, const int& n = 4)
{
    return i * n + j;
}

float B(const int& i, const float& t);

float dB(const int& i, const float& t);
