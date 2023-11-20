#pragma once

#include <QVector3D>
#include <QtMath>
#include <QList>

#include "math.h"

/*!
 * \brief The Circle class
 * This class represents a circle with a certain number of points. It provides methods to get the current and next points on the circle.
 */
class Circle
{
    public:
    /*!
    * \brief Constructs a Circle object
    * \param numOfPoints The number of points on the circle
    */
    Circle(uint numOfPoints = 120);
    /*!
    * \brief Returns the current point on the circle
    * \return A QVector3D object representing the current point on the circle
    */
    QVector3D current() const;
    /*!
    * \brief Returns the next point on the circle
    * \return A QVector3D object representing the next point on the circle
    */
    QVector3D next();

    private:
        const float m_lightZ = 1.5f;
        uint m_i;
        uint m_numOfPoints;
        QList<QVector2D> m_points;
};
