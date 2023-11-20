#pragma once

#include <QVector3D>
#include <QtMath>

inline float dotProduct(const QVector3D& v1, const QVector3D& v2)
{
    return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

inline QVector3D crossProduct(const QVector3D& v1, const QVector3D& v2)
{
    return QVector3D(v1.y() * v2.z() - v1.z() * v2.y(), v1.z() * v2.x() - v1.x() * v2.z(), v1.x() * v2.y() - v1.y() * v2.x());
}

inline float parallelogramArea(const float& x1, const float& y1, const float& x2, const float& y2)
{
    return qFabs(x1 * y2 - y1 * x2);
}

inline int ij2k(const int& i, const int& j, const int& n = 4)
{
    return i * n + j;
}

/*!
 * \brief Calculates the Berenstein's basis polynomial of degree 2
 * \param i The index of the basis function
 * \param t The parameter of the basis function
 * \return The value of the B-spline basis function of degree 2
 */
float B2(const int &i, const float &t);
/*!
 * \brief Calculates the Berenstein's basis polynomial of degree 3
 * \param i The index of the basis function
 * \param t The parameter of the basis function
 * \return The value of the B-spline basis function of degree 3
 */
float B3(const int &i, const float &t);
/*!
 * \brief Calculates the derivative of the Berenstein's basis polynomial of degree 3
 * \param i The index of the basis function
 * \param t The parameter of the basis function
 * \return The value of the derivative of the B-spline basis function of degree 3
 */
float dB3(const int &i, const float &t);
