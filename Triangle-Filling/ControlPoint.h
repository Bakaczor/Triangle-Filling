#pragma once

#include <QSharedPointer>
#include <QVector3D>
#include <QPainter>
#include <QImage>
#include <QPoint>

class ControlPoint
{
    public:
        QPoint position;
        QVector3D coordinate;
        bool modifiable;
        /*!
        * \brief Constructs a ControlPoint object
        * \param position The position of the control point on an image
        * \param coordinate The 3D coordinate of the control point
        * \param modifiable A boolean indicating whether the control point is modifiable
        */
        ControlPoint(QPoint position, QVector3D coordinate, bool modifiable);
        /*!
        * \brief Paints the control point on an image
        * \param image A shared pointer to a QImage object
        */
        void paint(QSharedPointer<QImage> image) const;
        /*!
        * \brief Checks if the control point equals a given position
        * \param x The x-coordinate of the position
        * \param y The y-coordinate of the position
        * \return A boolean indicating whether the control point equals the given position
        */
        bool equals(const int& x, const int& y) const;
        /*!
        * \brief Selects the control point
        */
        void select();
        /*!
        * \brief Unselects the control point
        */
        void unselect();

    private:
        const static uint m_radius = 40;
        QColor m_color;
};
