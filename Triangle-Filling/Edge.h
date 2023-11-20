#pragma once

#include <QPoint>

/*!
 * \brief The Edge struct
 * This struct represents a polygon's edge in a graphical application.
 */
struct Edge
{
    int ymax;
    float x;
    float _1m;
    int ymin;
    bool horizontal;
    /*!
    * \brief Constructs an Edge object
    * \param p1 The first point of the edge
    * \param p2 The second point of the edge
    */
    Edge(const QPoint& p1, const QPoint& p2);
};
