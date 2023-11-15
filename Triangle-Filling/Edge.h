#include <QPoint>

struct Edge
{
    int ymax;
    float x;
    float _1m;

    int ymin;
    bool horizontal;

    Edge(const QPoint& p1, const QPoint& p2);
};
