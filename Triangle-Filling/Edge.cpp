#include "Edge.h"

Edge::Edge(const QPoint& p1, const QPoint& p2)
{
    int xend;
    if (p1.y() > p2.y())
    {
        ymax = p1.y();
        ymin = p2.y();
        xend = p1.x();
        x = p2.x();
    }
    else
    {
        ymax = p2.y();
        ymin = p1.y();
        xend = p2.x();
        x = p1.x();
    }
    const int dy = ymax - ymin;
    const int dx = xend - x;

    if (dy == 0)
    {
        horizontal = true;
        _1m = 0;
    }
    else
    {
        horizontal = false;
        _1m = dx / static_cast<float>(dy);
    }
}
