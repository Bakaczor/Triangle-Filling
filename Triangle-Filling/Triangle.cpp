#include <QtConcurrent/QtConcurrent>
#include <QPainter>
#include <iterator>
#include <list>
#include <unordered_map>
#include <algorithm>

#include "Maths.h"
#include "Triangle.h"
#include "Edge.h"

Triangle::Triangle(QList<QPoint> positions, QSize size) : positions(positions), m_color(0, 0, 0),
    m_width(size.width()), m_height(size.height())
{
    coordinates = {
        QVector3D(positions.at(0).x() / m_width, positions.at(0).y() / m_height, 0),
        QVector3D(positions.at(1).x() / m_width, positions.at(1).y() / m_height, 0),
        QVector3D(positions.at(2).x() / m_width, positions.at(2).y() / m_height, 0)
    };
    normalVersors = QList<QVector3D>(3);
}

void Triangle::calculateNormalVersorsAndZ(const QList<ControlPoint>& controlPoints)
{
    QtConcurrent::map(coordinates, [this, &controlPoints](QVector3D& cor) {
        std::ptrdiff_t k = std::distance(&coordinates[0], &cor);
        float du = 0.0f;
        float dv = 0.0f;
        float Z = 0.0f;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                du += controlPoints.at(ij2k(i, j)).coordinate.z() * dB(i, cor.x()) * B(j, cor.y());
                dv += controlPoints.at(ij2k(i, j)).coordinate.z() * B(i, cor.x()) * dB(j, cor.y());
                Z += controlPoints.at(ij2k(i, j)).coordinate.z() * B(i, cor.x()) * B(j, cor.y());
            }
        }
        normalVersors[k] = QVector3D(-du, -dv, 1).normalized();
        coordinates[k].setZ(Z);
    });
}

void Triangle::paint(QSharedPointer<QImage> image) const
{
    QPainter painter(image.get());
    QPen pen(m_color);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawPolygon(positions);
}

void Triangle::fill(QSharedPointer<QImage> image, const Lambert& params) const
{
    // prepare edges
    QList<Edge> edges;
    // polygon has at least 2 non-horizontal edges
    edges.reserve(2);
    const int n = positions.count();
    int yend = positions.first().y();
    int ystart = positions.first().y();
    for (int i = 0; i < n; i++)
    {
        Edge e(positions.at(i), positions.at((i + 1) % n));
        if (!e.horizontal)
        {
            edges.append(e);
            if (e.ymin < ystart)
            {
                ystart = e.ymin;
            }
            if (e.ymax > yend)
            {
                yend = e.ymax;
            }
        }
    }

    // scan-line algorithm
    std::unordered_map<int, std::list<Edge>> ET;
    // fill buckets
    for (Edge& e : edges)
    {
        if (!ET.contains(e.ymin))
        {
            ET.insert(std::make_pair(e.ymin, std::list<Edge>{ e }));
        }
        else
        {
            ET[e.ymin].push_back(e);
        }
    }

    // main loop
    std::list<Edge> AET;
    while (ystart <= yend)
    {
        if (ystart == m_height) { break; }

        if (ET.contains(ystart))
        {
            std::list<Edge>& bucket = ET.at(ystart);
            AET.splice(begin(AET), bucket);
            AET.sort([](Edge& e1, Edge& e2) {
                return e1.x < e2.x;
            });
        }

        // paint segments
        auto itEnd = end(AET);
        auto it1 = begin(AET);
        while (true)
        {
            if (it1 == itEnd) { break; }
            auto it2 = it1;
            std::advance(it2, 1);
            if (it2 == itEnd) { break; }

            int xstart = it1->x;
            int xend = it2->x;

            while (xstart <= xend)
            {
                if (xstart == m_width) { break; }
                QPoint position(xstart, ystart);
                QPair<QVector3D, QVector3D> N_Z = approxN_Z(QVector2D(position.x() / m_width, position.y() / m_height));
                colorPixel(image, QPoint(xstart, ystart), N_Z.second, N_Z.first, params);
                xstart++;
            }
            std::advance(it1, 2);
        }

        ystart += 1;
        // remove edge if current y > ymax
        AET.remove_if([&ystart](const Edge& e) {
            return ystart > e.ymax;
        });
        // update x
        std::transform(begin(AET), end(AET), begin(AET), [](Edge& e) {
            e.x += e._1m;
            return e;
        });
    }
}

QPair<QVector3D, QVector3D> Triangle::approxN_Z(const QVector2D& coordinate) const
{
    const QVector2D vec0 = QVector2D(coordinates.at(0)) - coordinate;
    const QVector2D vec1 = QVector2D(coordinates.at(1)) - coordinate;
    const QVector2D vec2 = QVector2D(coordinates.at(2)) - coordinate;
    const float P0 = triangleArea(vec1.x(), vec1.y(), vec2.x(), vec2.y());
    const float P1 = triangleArea(vec0.x(), vec0.y(), vec2.x(), vec2.y());
    const float P2 = triangleArea(vec0.x(), vec0.y(), vec1.x(), vec1.y());

    const QVector3D N =  QVector3D((P0 * normalVersors.at(0) + P1 * normalVersors.at(1) + P2 * normalVersors.at(2)) / (P0 + P1 + P2));
    const float Z = (P0 * coordinates.at(0).z() + P1 * coordinates.at(1).z() + P2 * coordinates.at(2).z()) / (P0 + P1 + P2);
    return qMakePair(N, QVector3D(coordinate, Z));
}
