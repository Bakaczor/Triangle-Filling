#pragma once

#include <QtConcurrent/QtConcurrent>
#include <QList>

#include <unordered_map>
#include <algorithm>
#include <iterator>
#include <list>

#include "ControlPoint.h"
#include "Coloring.h"

class Triangle
{
    public:
        QList<QPoint> positions;
        QList<QVector3D> coordinates;
        QList<QVector3D> normalVersors;
        /*!
        * \brief Constructs a Triangle object
        * \param positions A list of QPoint objects representing the positions of the vertices of the triangle
        * \param size A QSize object representing the size of the image
        */
        Triangle(QList<QPoint> positions, QSize size);
        /*!
        * \brief Calculates the normal versors and Z values of the triangle
        * \param controlPoints A list of ControlPoint objects representing the control points of the triangle
        */

        // added bool for functional grid
        void calculateNormalVersorsAndZ(const QList<ControlPoint>& controlPoints, bool curveType);
         /*!
        * \brief Paints the outline of the triangle on an image
        * \param image A shared pointer to a QImage object representing the image
        */
        void paint(QSharedPointer<QImage> image, const float& alfa = 0, const float& beta = 0) const;
        /*!
        * \brief Fills the triangle with color
        * \param image A shared pointer to a QImage object representing the image
        * \param params A Lambert object representing the Lambertian reflection parameters
        */
        void fill(QSharedPointer<QImage> image, const Lambert& params) const;

    private:
        QColor m_color;
        float m_width;
        float m_height;
        /*!
        * \brief Approximates the normal versor and Z value at a given coordinate
        * \param coordinate A QVector2D object representing the coordinate
        * \return A QPair object containing the approximated normal versor and coordinnate with approximated Z value
        */
        QPair<QVector3D, QVector3D> approxN_Z(const QVector2D& coordinate) const;
};
