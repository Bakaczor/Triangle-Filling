#include "Maths.h"
#include "Triangle.h"
#include "Edge.h"
#include <math.h>

Triangle::Triangle(QList<QPoint> positions, QSize size) : positions(positions), m_color(0, 0, 255),
    m_width(size.width()), m_height(size.height())
{
    coordinates = {
        QVector3D(positions.at(0).x() / m_width, positions.at(0).y() / m_height, 0),
        QVector3D(positions.at(1).x() / m_width, positions.at(1).y() / m_height, 0),
        QVector3D(positions.at(2).x() / m_width, positions.at(2).y() / m_height, 0)
    };
    normalVersors = QList<QVector3D>(3);
}

void Triangle::calculateNormalVersorsAndZ(const QList<ControlPoint>& controlPoints, bool curveType)
{
    if (curveType)
    {
        QtConcurrent::map(coordinates, [this, &controlPoints](QVector3D& cor) {
            std::ptrdiff_t k = std::distance(&coordinates[0], &cor);
            double du = 0.0f;
            double dv = 0.0f;
            double Z = 0.0f;
            const int n = 3;
            for (int i = 0; i <= n; i++)
            {
                for (int j = 0; j <= n; j++)
                {
                    const double& zij = controlPoints.at(ij2k(i, j)).coordinate.z();
                    if (i < n)
                    {
                        du += (controlPoints.at(ij2k(i + 1, j)).coordinate.z() - zij) * B2(i, cor.x()) * B3(j, cor.y());
                    }
                    if (j < n)
                    {
                        dv += (controlPoints.at(ij2k(i, j + 1)).coordinate.z() - zij) * B3(i, cor.x()) * B2(j, cor.y());
                    }
                    Z += zij * B3(i, cor.x()) * B3(j, cor.y());
                }
            }
            du *= n;
            dv *= n;
            normalVersors[k] = QVector3D(-du, -dv, 1).normalized();
            coordinates[k].setZ(Z);
        });
    }
    else
    {
        // new functionality
        QtConcurrent::map(coordinates, [this](QVector3D& cor) {
            std::ptrdiff_t k = std::distance(&coordinates[0], &cor);
            const double pi_2 = M_PI / 2;
            double d = pi_2 * cos(pi_2 * (cor.x() + cor.y()));
            double Z = sin(pi_2 * (cor.x() + cor.y()));
            normalVersors[k] = QVector3D(-d, -d, 1).normalized();
            coordinates[k].setZ(Z);
        });
    }
}

void Triangle::paint(QSharedPointer<QImage> image, const float& alfa, const float& beta) const
{
    QPainter painter(image.get());
    QPen pen(m_color);
    pen.setWidth(1);
    painter.setPen(pen);

    if (alfa ==0 && beta == 0)
    {
        painter.drawPolygon(positions);
    }
    else
    {
        // new functionality
        QList<QPoint> rotated;
        for (const QPoint& p : positions)
        {
            QPoint rt = rotate(QVector4D(p.x(), p.y(), 300, 1), m_width, m_height, alfa, beta);

            if (rt.x() > 0 && rt.y() > 0 && rt.x() < m_width && rt.y() < m_height)
            {
                rotated.append(rt);
            }
        }
        painter.drawPolygon(rotated);
    }
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

    // ----- scan-line algorithm -----
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
                QPair<QVector3D, QVector3D> N_Z = approxN_Z(QVector2D(static_cast<float>(xstart) / m_width, static_cast<float>(ystart) / m_height));
                if (*params.alfa == 0 && *params.beta == 0)
                {
                    colorPixel(image, QPoint(xstart, ystart), N_Z.second, N_Z.first, params);
                }
                else
                {
                    // new functionality
                    QPoint rotated = rotate(QVector4D(xstart, ystart, 300, 1), m_width, m_height, *params.alfa, *params.beta);
                    if (rotated.x() > 0 && rotated.y() > 0 && rotated.x() < m_width && rotated.y() < m_height)
                    {
                        colorPixel(image, rotated, N_Z.second, N_Z.first, params);
                    }
                }
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
    const float P0 = parallelogramArea(vec1.x(), vec1.y(), vec2.x(), vec2.y());
    const float P1 = parallelogramArea(vec0.x(), vec0.y(), vec2.x(), vec2.y());
    const float P2 = parallelogramArea(vec0.x(), vec0.y(), vec1.x(), vec1.y());

    const float P = P0 + P1 + P2;
    const QVector3D N = (P0 * normalVersors.at(0) + P1 * normalVersors.at(1) + P2 * normalVersors.at(2)) / P;
    const float Z = (P0 * coordinates.at(0).z() + P1 * coordinates.at(1).z() + P2 * coordinates.at(2).z()) / P;
    return qMakePair(N.normalized(), QVector3D(coordinate, Z));
}
